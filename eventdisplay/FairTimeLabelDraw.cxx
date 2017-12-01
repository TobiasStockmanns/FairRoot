/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                        FairTimeLabelDraw source file                  -----
// -----                  Created 03/01/08  by M. Al-Turany            -----
// -------------------------------------------------------------------------
#include "FairTimeLabelDraw.h"

#include "FairBoxSet.h"                 // for FairBoxSet
#include "FairEventManager.h"           // for FairEventManager
#include "FairRootManager.h"            // for FairRootManager
#include "FairRunAna.h"                 // for FairRunAna
#include "FairTSBufferFunctional.h"     // for StopTime
#include "FairTimeStamp.h"              // for FairTimeStamp

#include <iosfwd>                       // for ostream
#include "TClonesArray.h"               // for TClonesArray
#include "TEveBoxSet.h"
#include "TEveManager.h"                // for TEveManager, gEve
#include "TVector3.h"                   // for TVector3

#include <stddef.h>                     // for NULL
#include <iostream>                     // for operator<<, basic_ostream, etc

using std::cout;
using std::endl;

// -----   Default constructor   -------------------------------------------
FairTimeLabelDraw::FairTimeLabelDraw()
  : FairTask("FairTimeLabelDraw",0),
    fEventManager(NULL),
    fManager(NULL),
	fText(NULL)
{
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
FairTimeLabelDraw::FairTimeLabelDraw(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose),
    fEventManager(NULL),
    fManager(NULL),
	fText(NULL)
{
}
// -------------------------------------------------------------------------
InitStatus FairTimeLabelDraw::Init()
{

  fManager = FairRootManager::Instance();
  fEventManager =FairEventManager::Instance();
  fText = new TEveText("Test");
  gEve->AddElement(fText);
  gEve->FullRedraw3D(kTRUE);

  return kSUCCESS;
}
// -------------------------------------------------------------------------
void FairTimeLabelDraw::Exec(Option_t* /*option*/)
{
	TString textToDisplay;
	textToDisplay += fEventManager->GetT0Time();
	textToDisplay += " ns";
	fText->SetText(textToDisplay.Data());
    gEve->Redraw3D(kFALSE);
}

// -----   Destructor   ----------------------------------------------------
FairTimeLabelDraw::~FairTimeLabelDraw()
{
}


ClassImp(FairTimeLabelDraw)
