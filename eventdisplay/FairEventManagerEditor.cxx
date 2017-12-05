/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "FairEventManagerEditor.h"

#include "FairEventManager.h"           // for FairEventManager
#include "FairRootManager.h"            // for FairRootManager
#include "FairRunAna.h"                 // for FairRunAna

#include "TChain.h"                     // for TChain
#include "TEveGValuators.h"             // for TEveGValuator
#include "TFile.h"                      // for TFile
#include "TGButton.h"                   // for TGCheckButton, TGTextButton
#include "TGLabel.h"                    // for TGLabel
#include "TGLayout.h"                   // for TGLayoutHints, etc
#include "TGNumberEntry.h"              // for TGNumberEntry, etc
#include "TGeoManager.h"                // for TGeoManager, gGeoManager
#include "TString.h"                    // for TString
#include "TEveManager.h"

#include <stddef.h>                     // for NULL

class TGWindow;
class TObject;

#define MAXE 5000

//______________________________________________________________________________
// FairEventManagerEditor
//
// Specialization of TGedEditor for proper update propagation to
// TEveManager.

ClassImp(FairEventManagerEditor)


//______________________________________________________________________________
FairEventManagerEditor::FairEventManagerEditor(const TGWindow* p, Int_t width, Int_t height,
    UInt_t options, Pixel_t back)
  :TGedFrame(p, width, height, options | kVerticalFrame, back),
   fObject(0),
   fManager(FairEventManager::Instance()),
   fCurrentEvent(0),
   fCurrentPDG(0),
   fVizPri(0),
   fMinEnergy(0),
   fMaxEnergy(0),
   fEventTime(NULL)
{
  Init();
}

void FairEventManagerEditor::Init()
{

  CreateSubFrameInfo();
  CreateSubFrameAnimation();

}

void FairEventManagerEditor::CreateSubFrameInfo()
{

	TChain* chain =FairRootManager::Instance()->GetInChain();
	  Int_t Entries= chain->GetEntriesFast();

	  MakeTitle("FairEventManager  Editor");
	  TGVerticalFrame*      infoFrame= CreateEditorTabSubFrame("Info");
	  TGCompositeFrame* title1 = new TGCompositeFrame(infoFrame, 250, 10,
	      kVerticalFrame | kLHintsExpandX |
	      kFixedWidth    | kOwnBackground);

	  TString Infile= "Input file : ";
	//  TFile* file =FairRunAna::Instance()->GetInputFile();
	  TFile* file =FairRootManager::Instance()->GetInChain()->GetFile();
	  Infile+=file->GetName();
	  TGLabel* TFName=new TGLabel(title1, Infile.Data());
	  title1->AddFrame(TFName);

	  UInt_t RunId= FairRunAna::Instance()->getRunId();
	  TString run= "Run Id : ";
	  run += RunId;
	  TGLabel* TRunId=new TGLabel(title1, run.Data());
	  title1->AddFrame( TRunId);

	  TString nevent= "No of events : ";
	  nevent +=Entries ;
	  TGLabel* TEvent=new TGLabel(title1, nevent.Data());
	  title1->AddFrame(TEvent);

	  Int_t nodes= gGeoManager->GetNNodes();
	  TString NNodes= "No. of Nodes : ";
	  NNodes += nodes;
	  TGLabel* NoNode=new TGLabel(title1, NNodes.Data());
	  title1->AddFrame( NoNode);

	  TGVerticalFrame* vFTime = new TGVerticalFrame(title1, 250, 100);
	  fTimebased = new TGCheckButton(title1, "Timebased");
	  title1->AddFrame(fTimebased, new TGLayoutHints(kLHintsLeft, 3, 1, 1, 0));

	  if (FairRootManager::Instance()->IsTimeStampMode() == kTRUE){
		  fTimebased->SetState(kButtonDown);
	  }

	  TGVerticalFrame* f = new TGVerticalFrame(vFTime);
	  TGLabel* l = new TGLabel(f, "Current Event:");
	  f->AddFrame(l, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 1, 2, 1, 1));
	  fCurrentEvent = new TGNumberEntry(f, 0., 6, -1,
	                                    TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
	                                    TGNumberFormat::kNELLimitMinMax, 0, Entries);
	  f->AddFrame(fCurrentEvent, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
	  fCurrentEvent->Connect("ValueSet(Long_t)","FairEventManagerEditor", this, "SelectEvent()");
	  vFTime->AddFrame(f);

	  title1->AddFrame(vFTime);

	  if (FairRootManager::Instance()->IsTimeStampMode() == kTRUE){
		  TGVerticalFrame* f0 = new TGVerticalFrame(title1);
		  TGLabel* l1 = new TGLabel(f0, "Current T0 (step/width):");
		  f0->AddFrame(l1, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 1, 2, 1, 1));
		  TGHorizontalFrame* hf0 = new TGHorizontalFrame(f0);
		  fCurrentTime = new TGNumberEntry(hf0, 0., 6, -1,
				  TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
				  TGNumberFormat::kNELNoLimits, 0, Entries);
		  hf0->AddFrame(fCurrentTime, new TGLayoutHints(kLHintsLeft,1,2,1,1));
		  fCurrentTime->Connect("ValueSet(Long_t)","FairEventManagerEditor",this, "SetTime()");
		  fTimeStep = new TGNumberEntry(hf0, 0., 6, -1,
				  TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
				  TGNumberFormat::kNELNoLimits, 0, Entries);
		  hf0->AddFrame(fTimeStep, new TGLayoutHints(kLHintsRight | kLHintsExpandX,1,2,1,1));
		  TGHorizontalFrame* f3 = new TGHorizontalFrame(f0);
			  TGLabel* T0TimeLabel = new TGLabel(f3, "T0 Time: ");
			  fT0Time = new TGLabel(f3,"");
			  f3->AddFrame(T0TimeLabel);
			  f3->AddFrame(fT0Time);
		  f0->AddFrame(hf0);
		  f0->AddFrame(f3);
		  title1->AddFrame(f0);
	  }

	  TGHorizontalFrame* f2 = new TGHorizontalFrame(title1);
	  TGLabel* EventTimeLabel = new TGLabel(f2, "Event Time: ");
	  fEventTime = new TGLabel(f2,"");
	  f2->AddFrame(EventTimeLabel);
	  f2->AddFrame(fEventTime);
	  title1->AddFrame(f2);

	  fVizPri = new TGCheckButton(title1, "Primary Only");
	  AddFrame(fVizPri, new TGLayoutHints(kLHintsTop, 3, 1, 1, 0));
	  fVizPri->Connect("Toggled(Bool_t)", "FairEventManagerEditor", this, "DoVizPri()");
	  title1->AddFrame(fVizPri);

	  fSavePicture = new TGCheckButton(title1, "Save Picture");
	  AddFrame(fSavePicture, new TGLayoutHints(kLHintsTop, 3,1,1,0));
	  fSavePicture->Connect("Toggled(Bool_t)", "FairEventManagerEditor", this, "DoSavePicture()");
	  title1->AddFrame(fSavePicture);

	  TGHorizontalFrame* f1 = new TGHorizontalFrame(title1);
	  TGLabel* L1 = new TGLabel(f1, "Select PDG :");
	  f1->AddFrame(L1, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 1, 2, 1, 1));
	  fCurrentPDG = new TGNumberEntry(f1, 0., 12, -1,
	                                  TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber,
	                                  TGNumberFormat::kNELNoLimits, 0, 1);
	  f1->AddFrame(fCurrentPDG, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
	  fCurrentPDG->Connect("ValueSet(Long_t)","FairEventManagerEditor", this, "SelectPDG()");
	  title1->AddFrame(f1);


	  fMinEnergy = new TEveGValuator(title1, "Min Energy:", 90, 0);
	  fMinEnergy->SetNELength(5);
	  fMinEnergy->SetLabelWidth(80);
	  fMinEnergy->Build();
	  fMinEnergy->SetLimits(0, MAXE, 2501, TGNumberFormat::kNESRealOne);
	  fMinEnergy->SetToolTip("Minimum energy of displayed track.");
	  fMinEnergy->Connect("ValueSet(Double_t)", "FairEventManagerEditor",this, "MinEnergy()");
	  title1->AddFrame(fMinEnergy, new TGLayoutHints(kLHintsTop, 1, 1, 1, 0));


	  fMaxEnergy = new TEveGValuator(title1, "Max Energy:", 90, 0);
	  fMaxEnergy->SetNELength(5);
	  fMaxEnergy->SetLabelWidth(80);
	  fMaxEnergy->Build();
	  fMaxEnergy->SetLimits(0, MAXE, 2501, TGNumberFormat::kNESRealOne);
	  fMaxEnergy->SetToolTip("Maximum energy of displayed track.");
	  fMaxEnergy->SetValue(MAXE);
	  fMaxEnergy->Connect("ValueSet(Double_t)", "FairEventManagerEditor",this, "MaxEnergy()");
	  title1->AddFrame(fMaxEnergy, new TGLayoutHints(kLHintsTop, 1, 1, 1, 0));

	  TGTextButton* fUpdate = new TGTextButton(title1, "Update");
	  title1->AddFrame(fUpdate, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 5,5,1,1));
	  fUpdate->Connect("Clicked()", "FairEventManagerEditor", this, "SelectEvent()");

	  infoFrame->AddFrame(title1, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));

	  fManager->SetMaxEnergy(MAXE);
}

void FairEventManagerEditor::CreateSubFrameAnimation()
{
	TGVerticalFrame*      animationFrame= CreateEditorTabSubFrame("Animation");
	TGCompositeFrame* title1 = new TGCompositeFrame(animationFrame, 250, 10,
		      kVerticalFrame | kLHintsExpandX |
		      kFixedWidth    | kOwnBackground);

	TGVerticalFrame* vf1 = new TGVerticalFrame(title1);
	TGLabel* labelAnimation = new TGLabel(vf1, "Start, Stop, Step");
	vf1->AddFrame(labelAnimation, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));

	TGHorizontalFrame* hfRange = new TGHorizontalFrame(vf1);
	fStart = new TGNumberEntry(hfRange, 0., 6, -1,
					  TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
					  TGNumberFormat::kNELNoLimits, 0, 999999);
	fStop = new TGNumberEntry(hfRange, 0., 6, -1,
					  TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
					  TGNumberFormat::kNELNoLimits, 0, 999999);
	fStep = new TGNumberEntry(hfRange, 0., 6, -1,
					  TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative,
					  TGNumberFormat::kNELNoLimits, 0, 999999);
	hfRange->AddFrame(fStart,  new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));
	hfRange->AddFrame(fStop, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));
	hfRange->AddFrame(fStep, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));

	TGTextButton* btnAnimate = new TGTextButton(hfRange, "Animate");
		  hfRange->AddFrame(btnAnimate, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 5,5,1,1));
		  btnAnimate->Connect("Clicked()", "FairEventManagerEditor", this, "DoAnimation()");

	vf1->AddFrame(hfRange, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));
	title1->AddFrame(vf1, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));
	animationFrame->AddFrame(title1, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));

}

//______________________________________________________________________________
void FairEventManagerEditor::MaxEnergy()
{
  fManager->SetMaxEnergy(fMaxEnergy->GetValue());

}
//______________________________________________________________________________
void FairEventManagerEditor::MinEnergy()
{
  fManager->SetMinEnergy(fMinEnergy->GetValue());

}

//______________________________________________________________________________
void FairEventManagerEditor::DoVizPri()
{
  if (fVizPri->IsOn()) { fManager->SetPriOnly(kTRUE); }
  else { fManager->SetPriOnly(kFALSE); }
}
//______________________________________________________________________________
void FairEventManagerEditor::DoSavePicture()
{
  if (fSavePicture->IsOn()) { fManager->SetCreatePicture(kTRUE); }
  else { fManager->SetCreatePicture(kFALSE); }
}

void FairEventManagerEditor::DoAnimation()
{
	int start = fStart->GetIntNumber();
	int stop = fStop->GetIntNumber();
	int step = fStep->GetIntNumber();

	fTimeStep->SetIntNumber(step);

	for (int i = start; i < stop; i++){
		fCurrentTime->SetIntNumber(i);
		SetTime();

		gEve->DoRedraw3D();
	}
}

//______________________________________________________________________________
void FairEventManagerEditor::SelectPDG()
{
  fManager->SelectPDG(fCurrentPDG->GetIntNumber());
}

//______________________________________________________________________________
void FairEventManagerEditor::SelectEvent()
{
  fManager->GotoEvent(fCurrentEvent->GetIntNumber());

  TString time;
// time+=(FairRootManager::Instance()->GetEventTime());
  time.Form("%.2f", FairRootManager::Instance()->GetEventTime());
  time += " ns";
  fEventTime->SetText(time.Data());

  fMinEnergy->SetLimits(fManager->GetEvtMinEnergy(), fManager->GetEvtMaxEnergy(), 100);
  fMinEnergy->SetValue(fManager->GetEvtMinEnergy());
  fMaxEnergy->SetLimits(fManager->GetEvtMinEnergy(), fManager->GetEvtMaxEnergy(), 100);
  fMaxEnergy->SetValue( fManager->GetEvtMaxEnergy());
  Update();

}
//______________________________________________________________________________
void FairEventManagerEditor::SetTime()
{
	fManager->SetT0Time(fCurrentTime->GetNumber() * fTimeStep->GetNumber());
	std::cout << "FairEventManagerEditor::SetTime() called: " << fCurrentTime->GetNumber() << " " << fTimeStep->GetNumber() << std::endl;
	TString time;
	// time+=(FairRootManager::Instance()->GetEventTime());
	time.Form("%.2f", fManager->GetT0Time());
	time += " ns";
	fT0Time->SetText(time.Data());
	std::cout << "FairEventManagerEditor::SetTime() " << fManager->GetT0Time() << std::endl;
	std::cout << "EventTime text: " << time.Data() << std::endl;
	fManager->GotoEvent(fCurrentEvent->GetIntNumber());
}
//______________________________________________________________________________
void FairEventManagerEditor::SetModel( TObject* obj)
{
  fObject     = obj;

}
//______________________________________________________________________________



