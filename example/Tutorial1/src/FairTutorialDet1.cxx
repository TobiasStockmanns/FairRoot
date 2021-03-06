#include "FairTutorialDet1.h"

#include "FairDetectorList.h"           // for DetectorId::kTutDet
#include "FairGeoInterface.h"           // for FairGeoInterface
#include "FairGeoLoader.h"              // for FairGeoLoader
#include "FairGeoNode.h"                // for FairGeoNode
#include "FairGeoVolume.h"              // for FairGeoVolume
#include "FairRootManager.h"            // for FairRootManager
#include "FairRun.h"                    // for FairRun
#include "FairRuntimeDb.h"              // for FairRuntimeDb
#include "FairStack.h"                  // for FairStack
#include "FairTutorialDet1Geo.h"        // for FairTutorialDet1Geo
#include "FairTutorialDet1GeoPar.h"     // for FairTutorialDet1GeoPar
#include "FairTutorialDet1Point.h"      // for FairTutorialDet1Point
#include "FairVolume.h"                 // for FairVolume

#include "Riosfwd.h"                    // for ostream
#include "TClonesArray.h"               // for TClonesArray
#include "TList.h"                      // for TListIter, TList (ptr only)
#include "TObjArray.h"                  // for TObjArray
#include "TString.h"                    // for TString
#include "TVirtualMC.h"                 // for TVirtualMC, gMC
#include "TVirtualMCStack.h"            // for TVirtualMCStack

#include <stddef.h>                     // for NULL
#include <iostream>                     // for cout, endl, operator<<, etc

using std::cout;
using std::endl;

FairTutorialDet1::FairTutorialDet1()
  : FairDetector("TutorialDet", kTRUE, kTutDet),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fFairTutorialDet1PointCollection(new TClonesArray("FairTutorialDet1Point"))
{
}

FairTutorialDet1::FairTutorialDet1(const char* name, Bool_t active)
  : FairDetector(name, active, kTutDet),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fFairTutorialDet1PointCollection(new TClonesArray("FairTutorialDet1Point"))
{
}

FairTutorialDet1::~FairTutorialDet1()
{
  if (fFairTutorialDet1PointCollection) {
    fFairTutorialDet1PointCollection->Delete();
    delete fFairTutorialDet1PointCollection;
  }
}

void FairTutorialDet1::Initialize()
{
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  FairTutorialDet1GeoPar* par=(FairTutorialDet1GeoPar*)(rtdb->getContainer("FairTutorialDet1GeoPar"));
}

Bool_t  FairTutorialDet1::ProcessHits(FairVolume* vol)
{
  /** This method is called from the MC stepping */

  cout << "In FairTutorialDet1::ProcessHits" << endl;
  //Set parameters at entrance of volume. Reset ELoss.
  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPos);
    gMC->TrackMomentum(fMom);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Create FairTutorialDet1Point at exit of active volume
  if ( gMC->IsTrackExiting()    ||
       gMC->IsTrackStop()       ||
       gMC->IsTrackDisappeared()   ) {
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fVolumeID = vol->getMCid();
    if (fELoss == 0. ) { return kFALSE; }
    AddHit(fTrackID, fVolumeID, TVector3(fPos.X(),  fPos.Y(),  fPos.Z()),
           TVector3(fMom.Px(), fMom.Py(), fMom.Pz()), fTime, fLength,
           fELoss);

    // Increment number of tutorial det points in TParticle
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kTutDet);
  }

  return kTRUE;
}

void FairTutorialDet1::EndOfEvent()
{

  fFairTutorialDet1PointCollection->Clear();

}



void FairTutorialDet1::Register()
{

  /** This will create a branch in the output tree called
      FairTutorialDet1Point, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("TutorialDetPoint", "TutorialDet",
                                        fFairTutorialDet1PointCollection, kTRUE);

}


TClonesArray* FairTutorialDet1::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fFairTutorialDet1PointCollection; }
  else { return NULL; }
}

void FairTutorialDet1::Reset()
{
  fFairTutorialDet1PointCollection->Clear();
}

void FairTutorialDet1::ConstructGeometry()
{
  /** If you are using the standard ASCII input for the geometry
      just copy this and use it for your detector, otherwise you can
      implement here you own way of constructing the geometry. */

  FairGeoLoader*    geoLoad = FairGeoLoader::Instance();
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  FairTutorialDet1Geo*  Geo  = new FairTutorialDet1Geo();
  Geo->setGeomFile(GetGeometryFileName());
  geoFace->addGeoModule(Geo);

  Bool_t rc = geoFace->readSet(Geo);
  if (rc) { Geo->create(geoLoad->getGeoBuilder()); }
  TList* volList = Geo->getListOfVolumes();

  // store geo parameter
  FairRun* fRun = FairRun::Instance();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  FairTutorialDet1GeoPar* par=(FairTutorialDet1GeoPar*)(rtdb->getContainer("FairTutorialDet1GeoPar"));
  TObjArray* fSensNodes = par->GetGeoSensitiveNodes();
  TObjArray* fPassNodes = par->GetGeoPassiveNodes();

  TListIter iter(volList);
  FairGeoNode* node   = NULL;
  FairGeoVolume* aVol=NULL;

  while( (node = (FairGeoNode*)iter.Next()) ) {
    aVol = dynamic_cast<FairGeoVolume*> ( node );
    if ( node->isSensitive()  ) {
      fSensNodes->AddLast( aVol );
    } else {
      fPassNodes->AddLast( aVol );
    }
  }
  par->setChanged();
  par->setInputVersion(fRun->GetRunId(),1);

  ProcessNodes ( volList );
}

FairTutorialDet1Point* FairTutorialDet1::AddHit(Int_t trackID, Int_t detID,
    TVector3 pos, TVector3 mom,
    Double_t time, Double_t length,
    Double_t eLoss)
{
  TClonesArray& clref = *fFairTutorialDet1PointCollection;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) FairTutorialDet1Point(trackID, detID, pos, mom,
         time, length, eLoss);
}

ClassImp(FairTutorialDet1)
