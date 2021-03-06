#ifndef FAIRDETECTOR_H
#define FAIRDETECTOR_H

#include "FairModule.h"                 // for FairModule

#include "Rtypes.h"                     // for Int_t, Bool_t, etc

class FairLogger;
class FairVolume;
class TClonesArray;

/**
 * Base class for constructing detecors
 * @author M. Al-Turany, Denis Bertini
 * @version 0.1
 * @since 12.01.04
 */
class FairDetector : public FairModule
{

  public:

    /**
      constructor
      Name :  Detector Name
      Active: kTRUE for active detectors  (ProcessHits() will be called)
              kFALSE for inactive detectors
    */
    FairDetector(const char* Name, Bool_t Active, Int_t DetId=0);
    /**
      default constructor
    */
    FairDetector();
    /**
      destructor
    */
    virtual ~FairDetector();
    /**
      Initialization of the detector is done here
    */
    virtual void   Initialize();
    /**
      this method is called for each step during simulation (see FairMCApplication::Stepping())
    */
    virtual Bool_t ProcessHits( FairVolume* v=0)=0;
    /**
      this is called at the end of an event
    */
    virtual void   EndOfEvent() {
      ;
    }
    /**
      Registers the produced collections in FAIRRootManager.
    */
    virtual void   Register()=0;

    /** Gets the produced collections **/
    virtual TClonesArray* GetCollection(Int_t iColl) const = 0;

    /**
      has to be called after each event to reset the containers
    */
    virtual void   Reset()=0;

    virtual void   CopyClones( TClonesArray* cl1,  TClonesArray* cl2 , Int_t offset) {
      ;
    }

    virtual void   FinishPrimary() {
      ;
    }
    virtual void   FinishRun() {
      ;
    }
    virtual void   BeginPrimary() {
      ;
    }
    virtual void   PostTrack() {
      ;
    }
    virtual void   PreTrack() {
      ;
    }
    virtual void   BeginEvent() {
      ;
    }
    virtual void   FinishEvent() {
      ;
    }
    virtual void   SetSpecialPhysicsCuts() {
      ;
    }
    void SaveGeoParams();
    Int_t  GetDetId() {
      return fDetId;
    }

  private:
    FairDetector(const FairDetector& M);
    FairDetector& operator= (const FairDetector&) {
      return *this;
    }


  protected:

    Int_t fDetId; // Detector Id has to be set from ctr.
    FairLogger* fLogger;  //! /// FairLogger

    ClassDef(FairDetector,1)

};
#endif //FAIRDETECTOR_H
