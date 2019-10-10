/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/* Generated by Together */

#ifndef FAIRDOUBLEHIT_H
#define FAIRDOUBLEHIT_H


#include "FairMultiLinkedData.h"
#include <TVector3.h>

class FairDoubleHit : public FairMultiLinkedData
{
  protected:
    Int_t  fRefIndex;
    Int_t fDetectorID;
    TVector3 fPosition_in;
    TVector3 fPositionError_in;
    TVector3 fPosition_out;
    TVector3 fPositionError_out;

  public:

    FairDoubleHit();
    virtual ~FairDoubleHit();

    // methods

    virtual void Print(const Option_t* opt = 0) const =0;

    // set methods
    virtual void SetDetectorID(Int_t chamb)=0;
    virtual void SetPos_in      (TVector3 xyz)=0;
    virtual void SetDPos_in      (TVector3 xyz)=0;
    virtual void SetPos_out      (TVector3 xyz)=0;
    virtual void SetDPos_out      (TVector3 xyz)=0;
    virtual void SetRefIndex      (Int_t index)=0;

    // get methods
    virtual Int_t GetDetectorID()=0;
    virtual Int_t GetRefIndex()=0;

    virtual TVector3 GetPos_in()=0;
    virtual TVector3 GetDPos_in()=0;
    virtual TVector3 GetPos_out()=0;
    virtual TVector3 GetDPos_out()=0;

    virtual Double_t x_in() =0;
    virtual Double_t y_in() =0;
    virtual Double_t z_in() =0;
    virtual Double_t dx_in()=0;
    virtual Double_t dy_in()=0;
    virtual Double_t dz_in()=0;

    virtual Double_t x_out() =0;
    virtual Double_t y_out() =0;
    virtual Double_t z_out() =0;
    virtual Double_t dx_out()=0;
    virtual Double_t dy_out()=0;
    virtual Double_t dz_out()=0;

    ClassDef(FairDoubleHit,1) //FAIRDoubleHit
};
#endif //FAIRDOUBLEHIT_H




