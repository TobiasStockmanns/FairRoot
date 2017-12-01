/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----              FairTimeLabelDraw header file                       -----
// -----          Created 26/03/09  by T. Stockmanns                   -----
// -------------------------------------------------------------------------


/** FairTimeLabelDraw
 * @author T. Stockmanns
 * @since 26.3.2009
 *   Base class to display 3D Points in Eve as a BoxSet
 *   One has to overwrite the method GetVector which takes a TObject and writes out a TVector3 which is then taken as an input
 *   to place the points.
 *   If one wants to change the color of the points one has to overwrite the method GetValue. This method takes a TObject and
 *   an integer and translates this into an integer as input for the EveBoxSet method DigitValue
 **
 **/

#ifndef FairTimeLabelDraw_H
#define FairTimeLabelDraw_H

#include "FairTask.h"                   // for FairTask, InitStatus
#include "TEveText.h"
#include "Rtypes.h"                     // for Double_t, Int_t, Bool_t, etc

class TObject;
class FairEventManager;
class FairRootManager;

class FairTimeLabelDraw : public FairTask
{

  public:

    /** Default constructor **/
    FairTimeLabelDraw();


    /** Standard constructor
    *@param name        Name of task
    *@param iVerbose    Verbosity level
    **/
    FairTimeLabelDraw(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~FairTimeLabelDraw();

    /** Executed task **/
    virtual void Exec(Option_t* option);

    void Reset();

  protected:

    virtual InitStatus Init();
    /** Action after each event**/
    virtual void Finish(){};

    FairEventManager* fEventManager;   //!
    FairRootManager* fManager;
    TEveText* fText;


  private:

    ClassDef(FairTimeLabelDraw,1);

};


#endif
