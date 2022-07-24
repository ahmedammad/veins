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

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer11ad.h"
#include "veins/modules/phy/DeciderResult80211.h"
#include "veins/base/phyLayer/PhyToMacControlInfo.h"

using namespace veins;

Define_Module(veins::DemoBaseApplLayer11ad);

void DemoBaseApplLayer11ad::initialize(int stage)
{
    BaseApplLayer::initialize(stage);
    
    if (stage == 0) {

        // initialize pointers to other modules
        if (FindModule<TraCIMobility*>::findSubModule(getParentModule())) {
            mobility = TraCIMobilityAccess().get(getParentModule());
            traci = mobility->getCommandInterface();
            traciVehicle = mobility->getVehicleCommandInterface();
        }
        else {
            traci = nullptr;
            mobility = nullptr;
            traciVehicle = nullptr;
        }

        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        mac = FindModule<DemoBaseApplLayerToMac1609_4Interface*>::findSubModule(getParentModule());
        ASSERT(mac);

        // read parameters
        headerLength = par("headerLength");
        sendBeacons = par("sendBeacons").boolValue();
        beaconLengthBits = par("beaconLengthBits");
        beaconUserPriority = par("beaconUserPriority");
        beaconInterval = par("beaconInterval");
        sectorSweepInterval = par("sectorSweepInterval");
        dataLengthBits = par("dataLengthBits");
        dataUserPriority = par("dataUserPriority");

        wsaInterval = par("wsaInterval").doubleValue();
        currentOfferedServiceId = -1;

        // isParked = false;

        findHost()->subscribe(BaseMobility::mobilityStateChangedSignal, this);
        findHost()->subscribe(TraCIMobility::parkingStateChangedSignal, this);

        sendBeaconEvt = new cMessage("beacon evt", SEND_BEACON_EVT);
        sendWSAEvt = new cMessage("wsa evt", SEND_WSA_EVT);
        calcStatsEvt = new cMessage("stats evt", Calc_Stats_EVT);
        sectorSweepEvt = new cMessage("sector sweep evt", SECTOR_SWEEP_EVT);
        responderSectorSweepEvt = new cMessage("responder sector sweep evt", RESPONDER_SECTOR_SWEEP_EVT);

        generatedBSMs = 0;
        generatedWSAs = 0;
        generatedWSMs = 0;
        receivedBSMs = 0;
        notBSMs = 0;
        receivedWSAs = 0;
        receivedWSMs = 0;

        numPack = 0;
        packets.setName("receivedPackets");
        this->lastUpdate = 0;
        numBits = 0;
        dataRate.setName("dataRateMbps");
        latency.setName("latency");
        lastIdleTime = 0;
        lastBusyTime = 0;
        channelUtiliz.setName("channelUtilization");

        simtime_t firstBeacon = simTime();
        simtime_t randomOffset = dblrand() * beaconInterval;
        firstBeacon = simTime() + randomOffset;

        if (sendBeacons) {
            scheduleAt(firstBeacon, sectorSweepEvt);
//            scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
            scheduleAt(firstBeacon + 0.01, sendBeaconEvt);
//            scheduleAt(simTime()+1, calcStatsEvt);
        }
        scheduleAt(simTime()+1, calcStatsEvt);
        
    }
    // else if (stage == 1) {

    //     // store MAC address for quick access
    //     myId = mac->getMACAddress();

    //     simtime_t firstBeacon = simTime();

    //     if (par("avoidBeaconSynchronization").boolValue() == true) {

    //         simtime_t randomOffset = dblrand() * beaconInterval;
    //         firstBeacon = simTime() + randomOffset;

    //         if (sendBeacons) {
    //             scheduleAt(firstBeacon, sendBeaconEvt);
    //         }
    //     }
    // }
}

void DemoBaseApplLayer11ad::populateWSM(BaseFrame1609_4* wsm, LAddress::L2Type rcvId, int serial)
{
    wsm->setRecipientAddress(rcvId);
    wsm->setBitLength(headerLength);

    if (DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(wsm)) {
        bsm->setSenderPos(curPosition);
        bsm->setSenderSpeed(curSpeed);
        bsm->setPsid(getNode()->getId());
        bsm->setChannelNumber(static_cast<int>(Channel::ch2));
        bsm->addBitLength(2097144);     // max allowed 262143 octets = 2097144 bits
        // bsm->setSendSector("NIC_A");
        wsm->setUserPriority(beaconUserPriority);
    }
    else if (DemoServiceAdvertisment* wsa = dynamic_cast<DemoServiceAdvertisment*>(wsm)) {
        wsa->setChannelNumber(static_cast<int>(Channel::ch2));
        wsa->setTargetChannel(static_cast<int>(currentServiceChannel));
        wsa->setPsid(currentOfferedServiceId);
        wsa->setServiceDescription(currentServiceDescription.c_str());
    }
    else {
        wsm->setChannelNumber(static_cast<int>(Channel::ch2));
        wsm->addBitLength(dataLengthBits);
        wsm->setUserPriority(dataUserPriority);
    }
}

void DemoBaseApplLayer11ad::populateSSW(BaseFrame1609_4* wsm, LAddress::L2Type rcvId, int serial)
{
    wsm->setRecipientAddress(rcvId);
    wsm->setBitLength(headerLength);

    if (DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(wsm)) {
        bsm->setSenderPos(curPosition);
        bsm->setSenderSpeed(curSpeed);
        bsm->setChannelNumber(static_cast<int>(Channel::ch2));
        bsm->addBitLength(10000);
        bsm->setTimestamp(simTime());
        bsm->setPsid(getNode()->getId());
        bsm->setUserPriority(7);
        bsm->setIsSectorSweep(true);
    }
}

void DemoBaseApplLayer11ad::handleLowerMsg(cMessage* msg)
{
    BaseFrame1609_4* wsm = dynamic_cast<BaseFrame1609_4*>(msg);
    ASSERT(wsm);

    if (DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(wsm)) {
        receivedBSMs++;
        // std::cerr << bsm->getPsid() <<" appbsm received " << simTime() << std::endl;
        //  std::cerr << bsm->getTimestamp() <<" time "<< std::endl;
        latency.record(simTime() - bsm->getTimestamp());

        DeciderResult80211* macRes = check_and_cast<DeciderResult80211*>(PhyToMacControlInfo::getDeciderResult(msg));
        DeciderResult80211* res = new DeciderResult80211(*macRes);
//        std::cerr << " app SinrMin mW "<< res->getSnr() << std::endl;
//        std::cerr <<" app getSendSector "<< bsm->getSendSector() << std::endl;
//        std::cerr <<" app getReceiveSector "<< bsm->getReceiveSector() << std::endl;
        int senderId = bsm->getPsid();
//        std::cerr << " sender node id " << senderId << std::endl;

        if (getNode()->getId() != senderId) { // check if it is from my own antenna sectors

            if(bsm->isSectorSweep()) {
                if (sectorInfo.find(senderId) == sectorInfo.end()) { // node does not exist so save it
    //                std::cerr << " tuple not exist " << senderId << std::endl;
                    sectorInfo[senderId] = std::make_tuple(bsm->getSendSector(), bsm->getReceiveSector(), res->getSnr());
                }
                else {
                    auto& tup = sectorInfo[senderId];
    //                std::cerr << " tuple exist " << senderId << std::endl;
                    if (std::get<2>(tup) < res->getSnr()) {

                        std::get<0>(tup) = bsm->getSendSector();
                        std::get<1>(tup) = bsm->getReceiveSector();
                        std::get<2>(tup) = res->getSnr();
                    }
                }

    //            if(strcmp(bsm->getSendSector(), "nicC") == 0) {  //if last NIC/sector is sending message then send back responder SSW on so far best sector
                if(bsm->getResponderSweep()) {
                    receiverId = senderId;
                    scheduleAt(simTime(), responderSectorSweepEvt);

                }
//                auto tup = sectorInfo[senderId];
//                std::cerr << " sender node id " << senderId << std::endl;
//                std::cerr << " tuple sendsector " << std::get<0>(tup) << " receivesector " << std::get<1>(tup) << " snir " << std::get<2>(tup) << std::endl;
            }
            else {
                // onBSM(bsm);
                auto tup = sectorInfo[senderId];
                if(wsm->getReceiveSector() == std::get<1>(tup)) {   // only consider the packets received on my best sector/NIC

                    numPack++;
                    numBits += wsm->getBitLength();
//                    simtime_t interval = simTime() - this->lastUpdate;
//                    if(interval >= 1) {
//                        uint64_t divby = 1048576;
//                        double mbps = FWMath::div(numBits, divby) ;// numBits / divby;
//                        std::cerr << " bytes " << wsm->getBitLength() <<" datarate " << mbps << " Mbits/s" << std::endl;
//                        numBits = 0;
//                        dataRate.record(mbps);
//                        packets.record(numPack);
//                        this->lastUpdate = simTime();
//                        numPack = 0;
                        // EV_INFO << "interval is " << interval << endl;
//                    }
                }
            }
        }

    }
    else {
        notBSMs++;
    }

    delete (msg);
}

void DemoBaseApplLayer11ad::handleSelfMsg(cMessage* msg)
{
    switch (msg->getKind()) {
    case SEND_BEACON_EVT: {
        if (!doSectorSweep) {
            DemoSafetyMessage* bsm = new DemoSafetyMessage();
            populateWSM(bsm);
            bsm->setTimestamp(simTime());
            // bsm->setPsid(123);
            // std::cerr << bsm->getPsid() << " bsm app " << simTime() << std::endl;

//            std::cerr <<  getNode()->getId() << " no sector sweep, size" << sectorInfo.size() << std::endl;
            // auto tup = sectorInfo[21];
            for (auto& p : sectorInfo)
            {
                // std::int const& key = p.first;
//                std::cerr << getNode()->getId() << " key is " << p.first << std::endl;
                auto& tup2 = sectorInfo[p.first];
//                std::cerr << " tup is " << std::get<0>(tup2) << " tup is " << std::get<1>(tup2) << " tup is " << std::get<2>(tup2)<< std::endl;

                bsm->setSendSector(std::get<1>(tup2).c_str());
                if(!bsm->isScheduled())
                 {sendDown(bsm);}
            }
            // std::cerr << " no sector sweep, size" << sectorInfo.size() << " sendsector " << std::get<0>(tup) << " receivesector " << std::get<1>(tup) << " snir " << std::get<2>(tup) << std::endl;
            // bsm->setSendSector(std::get<0>(tup).c_str());

//            sendDown(bsm);
            if(sectorInfo.size() <1) {
                delete (bsm);
            }
//            scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
        }
        // else {
        scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
        // }
        break;
    }
    case SECTOR_SWEEP_EVT: {
        DemoSafetyMessage* bsm = new DemoSafetyMessage();
        populateSSW(bsm);
        doSectorSweep = true;

        if (sectorNumb == 1) {
            bsm->setSendSector("nicA");
            sectorNumb++;
        }
        else if (sectorNumb == 2) {
            bsm->setSendSector("nicB");
            sectorNumb++;
        }
        else if (sectorNumb == 3) {
            bsm->setSendSector("nicC");
            sectorNumb++;
        }
        else if (sectorNumb == 4) {
            bsm->setSendSector("nicD");
            sectorNumb++;
        }
        else if (sectorNumb == 5) {
            bsm->setSendSector("nicE");
            bsm->setResponderSweep(true);
            sectorNumb = 1;
            doSectorSweep = false;
        }

        sendDown(bsm);
        scheduleAt(simTime() + (sectorNumb == 1 ? sectorSweepInterval : 0.000001), sectorSweepEvt);

        break;
    }
    case RESPONDER_SECTOR_SWEEP_EVT: {
        auto& sender = sectorInfo[receiverId];
        DemoSafetyMessage* responderBsm = new DemoSafetyMessage();
        populateSSW(responderBsm);
        responderBsm->setSendSector(std::get<1>(sender).c_str());   // send at node's best receive sector
//        std::cerr << " *-*-*-**-*-*-**-*-*-*-**-* sending ack" << receiverId << " " << simTime() << std::endl;
        sendDown(responderBsm);

        break;
    }
    case SEND_WSA_EVT: {
        DemoServiceAdvertisment* wsa = new DemoServiceAdvertisment();
        // populateWSM(wsa);
        sendDown(wsa);
        scheduleAt(simTime() + wsaInterval, sendWSAEvt);
        break;
    }
    case Calc_Stats_EVT: {
        std::pair<simtime_t, simtime_t> time = mac->getBusyIdleTime();
        simtime_t busy = time.first - lastBusyTime;
        simtime_t idle = time.second - lastIdleTime;
        double chUtilization = (busy.dbl()/(busy+idle).dbl()*100);
        channelUtiliz.record(chUtilization);
        // std::cerr <<" chUtilization " << chUtilization << " %" << std::endl;
        lastBusyTime = time.first;
        lastIdleTime = time.second;
        // std::cerr <<" calc busytime " << busy << std::endl;
        // std::cerr <<" calc idletime " << idle << std::endl;
        uint64_t divby = 1048576;
        double mbps = FWMath::div(numBits, divby) ;// numBits / divby;
        std::cerr <<" datarate " << mbps << " Mbits/s" << std::endl;
        numBits = 0;
        dataRate.record(mbps);
        packets.record(numPack);
        numPack = 0;
        scheduleAt(simTime() + 1, calcStatsEvt);
        break;
    }
    default: {
        if (msg) EV_WARN << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
        break;
    }
    }
}

void DemoBaseApplLayer11ad::finish()
{
    recordScalar("generatedWSMs", generatedWSMs);
    recordScalar("receivedWSMs", receivedWSMs);

    recordScalar("generatedBSMs", generatedBSMs);
    recordScalar("receivedBSMs", receivedBSMs);
recordScalar("notBSMs", notBSMs);
    recordScalar("generatedWSAs", generatedWSAs);
    recordScalar("receivedWSAs", receivedWSAs);
}

DemoBaseApplLayer11ad::~DemoBaseApplLayer11ad()
{
    cancelAndDelete(sendBeaconEvt);
    cancelAndDelete(sendWSAEvt);
    cancelAndDelete(calcStatsEvt);
    cancelAndDelete(sectorSweepEvt);
    cancelAndDelete(responderSectorSweepEvt);
    findHost()->unsubscribe(BaseMobility::mobilityStateChangedSignal, this);
}

void DemoBaseApplLayer11ad::sendDown(cMessage* msg)
{
    checkAndTrackPacket(msg);
    // std::cerr << " index " << getIndex() << std::endl;
    // std::cerr << " gatesize " << gateSize("out") << std::endl;
    BaseApplLayer::sendDown(msg);
}

// void DemoBaseApplLayer11ad::sendDelayedDown(cMessage* msg, simtime_t delay)
// {
//     checkAndTrackPacket(msg);
//     BaseApplLayer::sendDelayedDown(msg, delay);
// }

void DemoBaseApplLayer11ad::checkAndTrackPacket(cMessage* msg)
{
    if (dynamic_cast<DemoSafetyMessage*>(msg)) {
        EV_TRACE << "sending down a BSM" << std::endl;
        generatedBSMs++;
    }
    else if (dynamic_cast<DemoServiceAdvertisment*>(msg)) {
        EV_TRACE << "sending down a WSA" << std::endl;
        generatedWSAs++;
    }
    else if (dynamic_cast<BaseFrame1609_4*>(msg)) {
        EV_TRACE << "sending down a wsm" << std::endl;
        generatedWSMs++;
    }
}
