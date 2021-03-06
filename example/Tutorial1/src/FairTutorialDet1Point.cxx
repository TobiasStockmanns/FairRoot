#include "FairTutorialDet1Point.h"

#include "Riosfwd.h"                    // for ostream

#include <iostream>                     // for operator<<, basic_ostream, etc

using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
FairTutorialDet1Point::FairTutorialDet1Point()
  : FairMCPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
FairTutorialDet1Point::FairTutorialDet1Point(Int_t trackID, Int_t detID,
    TVector3 pos, TVector3 mom,
    Double_t tof, Double_t length,
    Double_t eLoss)
  : FairMCPoint(trackID, detID, pos, mom, tof, length, eLoss)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
FairTutorialDet1Point::~FairTutorialDet1Point() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void FairTutorialDet1Point::Print(const Option_t* opt) const
{
  cout << "-I- FairTutorialDet1Point: TutorialDet point for track " << fTrackID
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength
       << " cm,  Energy loss " << fELoss*1.0e06 << " keV" << endl;
}
// -------------------------------------------------------------------------

ClassImp(FairTutorialDet1Point)

