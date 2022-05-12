//
// Copyright (C) 2008 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
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

package org.car2x.veins.nodes;

import org.car2x.veins.base.modules.*;
import org.car2x.veins.modules.nic.Nic80211p;

module Car
{
    parameters:
        string applType; //type of the application layer
        string nicType = default("Nic80211p"); // type of network interface card
        string veinsmobilityType = default("org.car2x.veins.modules.mobility.traci.TraCIMobility"); //type of the mobility module
    gates:
        input veinsradioInA; // gate for sendDirect
        input veinsradioInB;
        input veinsradioInC;
    submodules:
        appl: <applType> like org.car2x.veins.base.modules.IBaseApplLayer {
            parameters:
                @display("p=60,50");
        }

		nicA: <nicType> like org.car2x.veins.modules.nic.INic80211p {
             parameters:
                 @display("p=60,166");
         }

        nicB: <nicType> like org.car2x.veins.modules.nic.INic80211p {
             parameters:
                 @display("p=150,250");
         }

        nicC: <nicType> like org.car2x.veins.modules.nic.INic80211p {
             parameters:
                 @display("p=250,250");
         }

        veinsmobility: <veinsmobilityType> like org.car2x.veins.base.modules.IMobility {
            parameters:
                @display("p=130,172;i=block/cogwheel");
        }

        splitter: org.car2x.veins.modules.application.ieee80211p.Splitter11ad {
             @display("p=150,100");
        }

    connections allowunconnected:
//        nicA.upperLayerOut --> appl.lowerLayerIn++;
//        nicA.upperLayerIn <-- appl.lowerLayerOut++;
//        nicA.upperControlOut --> appl.lowerControlIn++;
//        nicA.upperControlIn <-- appl.lowerControlOut++;

     	appl.lowerLayerOut --> splitter.upperLayerIn;
     	appl.lowerLayerIn <-- splitter.upperLayerOut;

     	splitter.lowerLayerOut_nicA --> nicA.upperLayerIn;
     	splitter.lowerLayerIn_nicA <-- nicA.upperLayerOut;

     	splitter.lowerLayerOut_nicB --> nicB.upperLayerIn;
     	splitter.lowerLayerIn_nicB <-- nicB.upperLayerOut;

     	splitter.lowerLayerOut_nicC --> nicC.upperLayerIn;
     	splitter.lowerLayerIn_nicC <-- nicC.upperLayerOut;

		veinsradioInA --> nicA.radioIn;
        veinsradioInB --> nicB.radioIn;
        veinsradioInC --> nicC.radioIn;
}
