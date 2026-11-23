#include "hit.h"
#include "TRandom.h"

hit::hit(){fX=0; fY=0;fTrackIndex=-1;}

hit::hit(double x, double y, int trueTrackIndex)
{this->Set(x,y,trueTrackIndex);}

double hit::GetX() const {return fX;}

double hit::GetY() const {return fY;}

double hit::GetTrackIndex() const {return fTrackIndex;}

void hit::Set(double x, double y, int trueTrackIndex)
{
    fX=x; 
    fY=gRandom->Gaus(y,fgkYRes);
    fTrackIndex=trueTrackIndex;
}

