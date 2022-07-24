//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/ieee80211p/Splitter11ad.h"
#include "veins/modules/mac/ieee80211p/Mac1609_4.h"

using namespace veins;

Define_Module(veins::Splitter11ad);

void Splitter11ad::initialize()
{
    // From upper layers --> lower layers
    upperLayerIn = findGate("upperLayerIn");
    lowerLayerOut_nicA = findGate("lowerLayerOut_nicA");
    lowerLayerOut_nicB = findGate("lowerLayerOut_nicB");
    lowerLayerOut_nicC = findGate("lowerLayerOut_nicC");
    lowerLayerOut_nicD = findGate("lowerLayerOut_nicD");
    lowerLayerOut_nicE = findGate("lowerLayerOut_nicE");

    // From lower layers --> upper layers
    upperLayerOut = findGate("upperLayerOut");
    lowerLayerIn_nicA = findGate("lowerLayerIn_nicA");
    lowerLayerIn_nicB = findGate("lowerLayerIn_nicB");
    lowerLayerIn_nicC = findGate("lowerLayerIn_nicC");
    lowerLayerIn_nicD = findGate("lowerLayerIn_nicD");
    lowerLayerIn_nicE = findGate("lowerLayerIn_nicE");

    calcStatsEvt = new cMessage("stats evt", Calc_Stats_EVT);
    channelUtiliz.setName("channelUtilizationSpliter");
    scheduleAt(simTime()+1, calcStatsEvt);

}

void Splitter11ad::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
//        error("Self-message arrived!");
//        delete msg;
//        msg = NULL;
    }
    else {
        int arrivalGate = msg->getArrivalGateId();
        if (arrivalGate == upperLayerIn) {
            handleUpperMessage(msg);
        }
         // The arrival gate is not from the application, it'a from lower layers
        else {
            handleLowerMessage(msg);
        }
     }
}

void Splitter11ad::handleSelfMsg(cMessage* msg)
{
//    std::cerr <<" splitter self message " << msg->getKind()<< std::endl;
    switch (msg->getKind()) {
        case Calc_Stats_EVT: {
/*            mac = FindModule<DemoBaseApplLayerToMac1609_4Interface*>::findSubModule(getParentModule());
            std::pair<simtime_t, simtime_t> time = mac->getBusyIdleTime();
            simtime_t busy = time.first - lastBusyTime;
            simtime_t idle = time.second - lastIdleTime;
            double chUtilization = (busy.dbl() / (busy + idle).dbl() * 100);
            channelUtiliz.record(chUtilization);
            // std::cerr <<" chUtilization " << chUtilization << " %" << std::endl;
            lastBusyTime = time.first;
            lastIdleTime = time.second;*/
            // std::cerr <<" calc busytime " << busy << std::endl;
            // std::cerr <<" calc idletime " << idle << std::endl;
        //    std::cerr <<" splitter name " << getParentModule()->getName() << std::endl;

            std::size_t count = nicArr.size();

            for(size_t i = 0; i < count; ++i) {
                std::string nic = nicArr[i];
                // std::cerr <<" arr nic " << nic << std::endl;
                cModule *mod = getParentModule()->getSubmodule(nic.c_str())->getSubmodule("mac1609_4");
                mac = check_and_cast<Mac1609_4*>(mod);

                std::pair<simtime_t, simtime_t> time =  mac->getBusyIdleTime();
                std::pair<simtime_t, simtime_t>& lastBusyIdleTime = chBusyIdleInfo[nic];

                simtime_t busy = time.first - lastBusyIdleTime.first;
                simtime_t idle = time.second - lastBusyIdleTime.second;
                double chUtilization = (busy.dbl()/(busy+idle).dbl()*100);

                lastBusyIdleTime.first = time.first;
                lastBusyIdleTime.second = time.second;

                //  std::cerr << mac->getParentModule()->getParentModule()->getName() << mac->getParentModule()->getName() <<" splitter case " << lastBusyIdleTime.first << " , " << chUtilization << std::endl;
            }

            scheduleAt(simTime() + 1, calcStatsEvt);
            break;
        }
        default: {
            if (msg) EV_WARN << "Splitter: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
            break;
        }
    }
}

void Splitter11ad::handleUpperMessage(cMessage* msg)
{
    std::string networkType;
    std::string msgName = msg->getName();

    if (DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(msg)) {
          
        networkType = bsm->getSendSector();
        // std::cerr << networkType <<" networkType "<< std::endl;
        if (networkType == "nicA") {
            send(bsm, lowerLayerOut_nicA);
        }
        else if (networkType == "nicB"){
            send(bsm, lowerLayerOut_nicB);
        }
        else if (networkType == "nicC"){
            send(bsm, lowerLayerOut_nicC);
        }
        else if (networkType == "nicD"){
            send(bsm, lowerLayerOut_nicD);
        }
        else if (networkType == "nicE"){
            send(bsm, lowerLayerOut_nicE);
        }
        else {
            delete(bsm);
            // error("Unknown heterogeneous message received from upper layer!");
        }
    }
    // else if (DataMessage* dataMsg = dynamic_cast<DataMessage*>(msg)) {
    //     networkType = dataMsg->getAccessTechnology();
    //     if (networkType == NIC_A) {
    //         dataMsg->setChannelNumber(static_cast<int>(Channel::sch1));
    //         send(dataMsg, lowerLayerOut_nicA);
    //     }
    //     else if (networkType == NIC_B){
    //         dataMsg->setChannelNumber(static_cast<int>(Channel::cch));
    //         send(dataMsg, lowerLayerOut_nicB);
    //     }
    //     else if (networkType == NIC_C){
    //         dataMsg->setChannelNumber(static_cast<int>(Channel::sch4));
    //         send(dataMsg, lowerLayerOut_nicC);
    //     }
    //     else {
    //         error("Unknown data message received from upper layer!");
    //     }
    // }
    else {
        // error("Unknown message received from upper layer!");
    }
}

void Splitter11ad::handleLowerMessage(cMessage* msg)
{
    int lowerGate = msg->getArrivalGateId();

    if (lowerGate == lowerLayerIn_nicA){}
    else if (lowerGate == lowerLayerIn_nicB){}
    else if (lowerGate == lowerLayerIn_nicC){}
    else {
        // error("Unknown message from lower layers received!");
    }

    send(msg, upperLayerOut);
}

void Splitter11ad::finish()
{

}

Splitter11ad::~Splitter11ad()
{

}
