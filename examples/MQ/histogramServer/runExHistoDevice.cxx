/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "FairMQExHistoDevice.h"
#include "runFairMQDevice.h"

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& /*options*/) {}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/)
{
    FairMQExHistoDevice* histoDevice = new FairMQExHistoDevice();

    return histoDevice;
}
