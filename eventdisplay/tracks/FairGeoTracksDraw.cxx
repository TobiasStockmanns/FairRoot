/*
 * FairGeoTracksDraw.cxx
 *
 *  Created on: 23 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FairGeoTracksDraw.h"

#include <TEveManager.h>
#include <TEveSelection.h>



FairGeoTracksDraw::FairGeoTracksDraw() :fEventManager(nullptr),fVisualizer(nullptr){

}

void FairGeoTracksDraw::Exec(Option_t *option) {
    fVisualizer->Repaint();
}

InitStatus FairGeoTracksDraw::Init() {
    fEventManager = FairEventManager::Instance();
    fVisualizer = new FairEveGeoTracks();
    fVisualizer->SetRnrChildren(1);
    gEve->AddElement(fVisualizer, FairEventManager::Instance());
    gEve->GetSelection()->SetPickToSelect(TEveSelection::kPS_PableCompound);
    gEve->GetHighlight()->SetPickToSelect(TEveSelection::kPS_PableCompound);
    gEve->Redraw3D(kFALSE);
    return fVisualizer->Init();
}

FairGeoTracksDraw::~FairGeoTracksDraw() {
}
