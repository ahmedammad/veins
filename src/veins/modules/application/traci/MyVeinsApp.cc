//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
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

#include "veins/modules/application/traci/MyVeinsApp.h"
//#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

Define_Module(veins::MyVeinsApp);

void MyVeinsApp::initialize(int stage)
{
    DemoBaseApplLayer11ad::initialize(stage);

    if (stage == 0) {
//        traciVehicle->setSpeedMode(0x1f);
//        traciVehicle->setSpeed(0);
        traciVehicle->setSpeed(10);
        // Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;
//        numPack = 0;
//        packets.setName("receivedPackets");
//        this->lastUpdate = 0;
    }
//    else if (stage == 1) {
//        // Initializing members that require initialized other modules goes here
//    }
//    std::cerr << "slot" << SimTime().setRaw(5000000UL).dbl() << std::endl;
//    std::cerr << "sifs " << SimTime().setRaw(3000000UL).dbl() << std::endl;
//    exit (-1);
}

void MyVeinsApp::finish()
{
    DemoBaseApplLayer11ad::finish();
    // statistics recording goes here
//    recordScalar("#numPack", numPack);
//    recordScalar("#numSent", numSent);
}

void MyVeinsApp::onBSM(DemoSafetyMessage* bsm)
{
//    numPack++;

//    packets.record(wsm->getBitLength());
//    simtime_t interval = simTime() - this->lastUpdate;
//    if(interval >= 1) {
//        packets.record(numPack);
//        this->lastUpdate = simTime();
//        numPack = 0;
//        EV_INFO << "interval is " << interval << endl;
//    }
/*    TraCIDemo11pMessage* wsm = new TraCIDemo11pMessage();
    populateWSM(wsm);
    wsm->setDemoData(mobility->getRoadId().c_str());
    wsm->addBitLength(100000);
    sendDown(wsm);
*/
//    packets.record(nextPos.x);
    // Your application has received a beacon message from another car or RSU
    // code for handling the message goes here
//    bool hasStopped = false;
//    if(hasStopped == false) {
//        traciVehicle->setSpeedMode(0x1f);
//        traciVehicle->setSpeed(0);
//        hasStopped = true;
//    }
//    else {
//        traciVehicle->setSpeedMode(0x1f);
//        traciVehicle->setSpeed(20);
//        hasStopped = false;
//    }
}

void MyVeinsApp::onWSM(BaseFrame1609_4* wsm)
{
    // Your application has received a data message from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::onWSA(DemoServiceAdvertisment* wsa)
{
    // Your application has received a service advertisement from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::handleSelfMsg(cMessage* msg)
{
    DemoBaseApplLayer11ad::handleSelfMsg(msg);
    // this method is for self messages (mostly timers)
    // it is important to call the DemoBaseApplLayer function for BSM and WSM transmission
}

//void MyVeinsApp::handlePositionUpdate(cObject* obj)
//{
//    DemoBaseApplLayer11ad::handlePositionUpdate(obj);
//    // the vehicle has moved. Code that reacts to new positions goes here.
//    // member variables such as currentPosition and currentSpeed are updated in the parent class
//}
