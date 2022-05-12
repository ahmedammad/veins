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

using namespace veins;

Define_Module(veins::Splitter11ad);

void Splitter11ad::initialize()
{
    // From upper layers --> lower layers
    upperLayerIn = findGate("upperLayerIn");
    lowerLayerOut_nicA = findGate("lowerLayerOut_nicA");
    lowerLayerOut_nicB = findGate("lowerLayerOut_nicB");
    lowerLayerOut_nicC = findGate("lowerLayerOut_nicC");

    // From lower layers --> upper layers
    upperLayerOut = findGate("upperLayerOut");
    lowerLayerIn_nicA = findGate("lowerLayerIn_nicA");
    lowerLayerIn_nicB = findGate("lowerLayerIn_nicB");
    lowerLayerIn_nicC = findGate("lowerLayerIn_nicC");

}

void Splitter11ad::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage()) {
//        error("Self-message arrived!");
        delete msg;
        msg = NULL;
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

void Splitter11ad::handleUpperMessage(cMessage* msg)
{
    std::string networkType;
    std::string msgName = msg->getName();

    if (DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(msg)) {
          
        networkType = bsm->getAccessTechnology();
        // std::cerr << networkType <<" networkType "<< std::endl;
        if (networkType == "NIC_A") {
            // hetMsg->setChannelNumber(static_cast<int>(Channel::sch1));
            send(bsm, lowerLayerOut_nicA);
        }
        else if (networkType == "NIC_B"){
            // hetMsg->setChannelNumber(static_cast<int>(Channel::cch));
            send(bsm, lowerLayerOut_nicB);
        }
        else if (networkType == "NIC_C"){
            // hetMsg->setChannelNumber(static_cast<int>(Channel::sch4));
            send(bsm, lowerLayerOut_nicC);
        }
        else {
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
