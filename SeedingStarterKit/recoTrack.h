#ifndef RECOTRACK_H
#define RECOTRACK_H

#include <iostream>
#include <vector>


class recoTrack{
    public:
        recoTrack();
        ~recoTrack();
        void AddHitLink(unsigned int index);
        double GetA() const;
        double GetB() const;
        double GetdA() const;
        double GetdB() const;
        double GetchiSeed() const;
        unsigned int GetNHits() const;
        std::vector<unsigned int> GetHits() const;
        void PrintLabels() const;
        void SetA(double A);
        void SetB(double B);
        void SetdA(double dA);
        void SetdB(double dB);
        void SetchiSeed(double chiSeed);
        bool isGoodSeed();
    private:
        double fA,fB,fdA,fdB; //the reco track parameters
        double  fchiSeed;//the Chi**2/NDF of the seed
        std::vector<unsigned int> fHits; // stores the labels of the true track that generated the hit;
};

#endif
