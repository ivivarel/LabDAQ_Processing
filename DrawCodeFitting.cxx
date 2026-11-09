#include <TFile.h>
#include <TCanvas.h>


void DrawCodeFitting()
{
  TFile f("fit.root");
  for (unsigned int i = 0; i < 6; ++i){
    TCanvas c;
    TString graphName = "event_";
    graphName += i;
    TGraphErrors * mygraph = (TGraphErrors *) f.Get(graphName);
    TH2F h_axes("h_axes","",100,-2,2,100,-3.5,3.5);
    h_axes.SetXTitle("x [mm]");
    h_axes.SetYTitle("y [mm]");
    h_axes.Draw();
    mygraph->Fit("pol1");
    mygraph->Draw("P");
    TString graphFileName = "graph_";
    graphFileName += i;
    graphFileName += ".pdf";
    c.Print(graphFileName);
  }

  TCanvas c;
  gStyle->SetOptFit();

  TList * l_list = f.GetListOfKeys();

  for (TObject* obj : *l_list) {
    TString name = obj->GetName();
    
    if (name.BeginsWith("h_")) {
      c.SetLogy(0);
      c.SetLogx(0);
      TH1F * h = (TH1F *) f.Get(name);
      h->SetXTitle(name);
      if (name != "h_chi2"){
	h->Fit("gaus");
      } else {
	h->Scale(1./(h->GetBinWidth(1)*h->Integral()));
      }
      h->SetMaximum(1.4* h->GetMaximum()); 
      h->Draw();
      if (name == "h_chi2"){
	 TF1 *f = new TF1("f",
			  "ROOT::Math::chisquared_pdf(x, 1)",
			  0.001, 10.);
	 f->SetLineColor(2);
	 f->Draw("same");
	 c.SetLogy();
      }
   
      c.Print(name + ".pdf");
    }

  }

}
  

