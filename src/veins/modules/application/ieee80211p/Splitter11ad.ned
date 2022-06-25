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

package org.car2x.veins.modules.application.ieee80211p;
//@namespace(veins);
simple Splitter11ad
{
    parameters:
        @class(veins::Splitter11ad);
    gates:
        output upperLayerOut;             // output gate towards the application
        input upperLayerIn;               // input gate from the application

        output lowerLayerOut_nicA;        // output gate towards the 802.11p NIC
        input lowerLayerIn_nicA;          // input gate from the 802.11p NIC

        output lowerLayerOut_nicB;        // output gate towards the 802.11p NIC
        input lowerLayerIn_nicB;          // input gate from the 802.11p NIC

        output lowerLayerOut_nicC;        // output gate towards the 802.11p NIC
        input lowerLayerIn_nicC;          // input gate from the 802.11p NIC
        
        output lowerLayerOut_nicD;        // output gate towards the 802.11p NIC
        input lowerLayerIn_nicD;          // input gate from the 802.11p NIC
        
        output lowerLayerOut_nicE;        // output gate towards the 802.11p NIC
        input lowerLayerIn_nicE;          // input gate from the 802.11p NIC
}
