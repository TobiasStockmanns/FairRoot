/*
 * FairRecoTracksDraw.cxx
 *
 *  Created on: 27 sie 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FairRecoTracksDraw.h"

#include <TEveManager.h>
#include <TEveSelection.h>

FairRecoTracksDraw::FairRecoTracksDraw() :fEventManager(nullptr),fVisualizer(nullptr){
}

void FairRecoTracksDraw::Exec(Option_t *option) {
    fVisualizer->Repaint();
}

InitStatus FairRecoTracksDraw::Init() {
    fEventManager = FairEventManager::Instance();
    fVisualizer = new FairEveRecoTracksExample();
    fVisualizer->SetRnrChildren(1);
    gEve->AddElement(fVisualizer, FairEventManager::Instance());
    gEve->GetSelection()->SetPickToSelect(TEveSelection::kPS_PableCompound);
    gEve->GetHighlight()->SetPickToSelect(TEveSelection::kPS_PableCompound);
    gEve->Redraw3D(kFALSE);
    return fVisualizer->Init();
}

FairRecoTracksDraw::~FairRecoTracksDraw() {

}
