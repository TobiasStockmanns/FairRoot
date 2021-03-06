/**
 * FairMQMerger.cxx
 *
 * @since 2012-12-06
 * @author D. Klein, A. Rybalchenko
 */

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "FairMQLogger.h"
#include "FairMQMerger.h"
#include "FairMQPoller.h"


FairMQMerger::FairMQMerger()
{
}

FairMQMerger::~FairMQMerger()
{
}

void FairMQMerger::Run()
{
  LOG(INFO) << ">>>>>>> Run <<<<<<<";

  boost::thread rateLogger(boost::bind(&FairMQDevice::LogSocketRates, this));

  FairMQPoller* poller = fTransportFactory->CreatePoller(*fPayloadInputs);

  bool received = false;

  while ( fState == RUNNING ) {
    FairMQMessage* msg = fTransportFactory->CreateMessage();

    poller->Poll(100);

    for(int i = 0; i < fNumInputs; i++) {
      if (poller->CheckInput(i)){
        received = fPayloadInputs->at(i)->Receive(msg);
      }
      if (received) {
        fPayloadOutputs->at(0)->Send(msg);
        received = false;
      }
    }

    delete msg;
  }

  delete poller;

  rateLogger.interrupt();
  rateLogger.join();
}

