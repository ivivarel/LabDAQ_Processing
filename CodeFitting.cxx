#include <iostream>

#include <TRandom3.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1F.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TMath.h>
#include <TString.h>

TRandom3 randGen;

class straightLine
{
 public: 
        straightLine(double m, double b) : m(m), b(b) {}

        double eval(double x) const { return m*x + b; }
    private: 
        double m, b;

};

class hit
{
 public: 
        hit(double x, double y) : x(x), y(y) {}

        double getX() const { return x; }
        double getY() const { return y; }

    private: 
        double x, y;

};

class dummyDetector
{


    // mode = 0: gaussian smearing of the y coordinate
    // mode = 1: gaussian smearing of the y coordinate with an extra 1 mm noise
    // mode = 2: bias in the y coordinate of 0.2 mm * x  
 public: 
        dummyDetector(double hitPrecision = 1, unsigned int mode = 0) {m_hitPrecision = hitPrecision; m_mode = mode;}
            double getHitPrecision() const { return m_hitPrecision; }
        hit generateHit(double x, double y) 
        {
            double x_hit = x;
            double y_hit = 0; 

            if (m_mode == 0) {
                y_hit = randGen.Gaus(y,m_hitPrecision);
            } else if (m_mode == 1) {
                y_hit = randGen.Gaus(y,m_hitPrecision) + randGen.Gaus(0,1);
            } else if (m_mode == 2) {
                y_hit = randGen.Gaus(y + 0.2*x,m_hitPrecision);
            }

            return hit(x_hit, y_hit);
        }

    private: 
        unsigned int m_mode;
      double m_hitPrecision;
};

int CodeFitting(unsigned int mode = 0) 
{
    // This piece of code generates a number of randomly generated triplets of points and then fits them with straight lines 
    // It further produces residual plots of teh fits

    unsigned int nPoints = 100000;

    double true_m = 1.;
    double true_b = 1./3.;
    straightLine line(true_m, true_b); 
    dummyDetector detector(1, mode); // build a detector with precision 1 mm and specified mode

    std::vector<double> xvec = {-1,0,1};

    TFile f("fit.root", "RECREATE");

    TH1F h_b("h_b", "b distribution", 100, -2, 2);
    h_b.SetXTitle("Intercept b");
    h_b.SetYTitle("Entries");
    TH1F h_m("h_m", "m distribution", 100, -1, 3);
    h_m.SetXTitle("Angular coefficient m");
    h_m.SetYTitle("Entries");
    TH1F h_residuals("h_residuals", "residuals distribution", 100, -5, 5);
    h_residuals.SetXTitle("hit residual: (m - b - mx)/#sigma");
    h_residuals.SetYTitle("Entries");
    TH1F h_e_b("h_e_b", "b error distribution", 100, -1, 1);
    h_e_b.SetXTitle("b error");
    h_e_b.SetYTitle("Entries");
    TH1F h_e_m("h_e_m", "m error distribution", 100, 0, 2);
    h_e_m.SetXTitle("m error");
    h_e_m.SetYTitle("Entries");
    TH1F h_par_residual_b("h_par_residual_b", "b parameter residuals", 100, -5, 5);
    h_par_residual_b.SetXTitle("b parameter residuals");
    h_par_residual_b.SetYTitle("Entries");
    TH1F h_par_residual_m("h_par_residual_m", "m parameter residuals", 100, -5, 5);
    h_par_residual_m.SetXTitle("m parameter residuals");
    h_par_residual_m.SetYTitle("Entries");
    TH1F h_chi2("h_chi2", "chi2 distribution", 100, 0, 10);
    h_chi2.SetXTitle("#chi^{2}");


    TGraphErrors * ev[6];

    for (unsigned int i = 0; i < nPoints; ++i){
        // generate hits and fit them
        std::vector<hit> l_hits;
        
        for (double x : xvec) {
            double y = line.eval(x);
            l_hits.push_back(detector.generateHit(x,y));
        }
        
        // Now create the TGraph and fit
        std::vector<double> yvec;
        std::vector<double> sigma_xvec;
        std::vector<double> sigma_yvec;

        for (hit h : l_hits) {
            yvec.push_back(h.getY());
            sigma_xvec.push_back(0);
            sigma_yvec.push_back(detector.getHitPrecision());   
        }

        if (i < 6) {
            ev[i] = new TGraphErrors(xvec.size(), &xvec[0], &yvec[0], &sigma_xvec[0], &sigma_yvec[0]);
            TString grName = "event_";
            grName += i;
            ev[i]->SetName(grName);
            ev[i]->SetMarkerStyle(20);
            ev[i]->SetMarkerSize(1);
            ev[i]->SetMarkerColor(kBlue);
            ev[i]->Fit("pol1");
            ev[i]->Write();
        }

        // Create the matrices to do the track analysis

        TMatrixD F(3,2);
        TMatrixD V(3,3);

        F.Zero();
        V.Zero();

        F(0,0) = 1;  
        F(1,0) = 1;  
        F(2,0) = 1;  

        unsigned int j = 0;
        for (double x : xvec) {
            F(j,1) = x;
            V(j,j) = sigma_yvec[j]*sigma_yvec[j];
            j++;            
        }

        // Compute the transposed of F

        TMatrixD FT(TMatrixD::kTransposed, F);

        // Compute the inverse of V;

        TMatrixD Vinv = V;
        Vinv.Invert();

        // Compute the Covariance matrix of the parameters
        TMatrixD C = FT * Vinv * F;
        C.Invert();

        // Finally, the vector of the measurements
        TMatrixD m(3,1);
        for (unsigned int i = 0; i < yvec.size(); ++i) {
            m(i,0) = yvec[i];
        }

       

        // Now compute the parameters of the fit 

        TMatrixD lambda = C * FT * Vinv * m;

        h_b.Fill(lambda(0,0));
        h_m.Fill(lambda(1,0));

        // Compute the errors on the parameters

        double e_b = sqrt(C(0,0));
        double e_m = sqrt(C(1,1));

        // Evaluate the chi2

        TMatrixD chi2M = (m - F * lambda).T() * Vinv * (m - F * lambda);

        h_chi2.Fill(chi2M(0,0));

        h_e_b.Fill(e_b);
        h_e_m.Fill(e_m);

        h_par_residual_b.Fill((lambda(0,0) - true_b)/e_b);
        h_par_residual_m.Fill((lambda(1,0) - true_m)/e_m);

        for (hit h : l_hits) {
            double residual = h.getY() - (lambda(1,0)*h.getX() + lambda(0,0));
            h_residuals.Fill(residual);
        }

    }
    
    f.Write();
    f.Close();

    return 0;

}