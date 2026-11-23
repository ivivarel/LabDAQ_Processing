#include "recoTrack.h"

recoTrack::~recoTrack(){}

recoTrack::recoTrack(){}

void recoTrack::AddHitLink(unsigned int index){fHits.push_back(index);}

void recoTrack::PrintLabels() const { 
    for( auto i : fHits)
    {
        cout << "track label = [ "<<i << "] is:" <<fHits[i]<<endl;
    }
}

double recoTrack::GetA() const {return fA;}

double recoTrack::GetB() const {return fB;}

double recoTrack::GetdA() const {return fdA;}

double recoTrack::GetdB() const {return fdB;}

double recoTrack::GetchiSeed() const {return fchiSeed;}

unsigned int recoTrack::GetNHits() const {return fHits.size();}

std::vector<unsigned int> recoTrack::GetHits() const {return fHits;}

void recoTrack::SetA( double A){fA=A;}

void recoTrack::SetB( double B){fB=B;}

void recoTrack::SetdA( double dA){fdA=dA;}

void recoTrack::SetdB( double dB){fdB=dB;}

void recoTrack::SetchiSeed( double chiSeed){fchiSeed=chiSeed;}

bool recoTrack::isGoodSeed(){
    // if all the link refer to teh same gen track, then the seed is good. Otherwise is bad.
    if (this->GetNHits() == 0) return false;
    for (unsigned int iHit = 1; iHit < this->GetNHits(); ++iHit){
        if (fHits[iHit] != fHits[0]) return false;
    }
    return true;
}
