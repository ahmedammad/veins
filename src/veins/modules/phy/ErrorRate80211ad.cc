/*
 * Copyright (c) 2010 The Boeing Company
 * Copyright (c) 2014 Michele Segata <segata@ccs-labs.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Gary Pei <guangyu.pei@boeing.com>
 */

#include "veins/veins.h"

#include "veins/modules/phy/ErrorRate80211ad.h"

using veins::ErrorRate80211ad;

ErrorRate80211ad::ErrorRate80211ad()
{
}
signed int ErrorRate80211ad::getSqpskBer(double snr, uint32_t rate)
{
    if (rate == 1) {
        if (snr > 1 && snr <= 2) { return -1; }
        if (snr > 2 && snr <= 6) { return -2; }
        if (snr > 6 && snr <= 7) { return -5; }
    }
    if (rate == 5) {
        if (snr > 1 && snr <= 2) { return -1; }
        if (snr > 2 && snr <= 6) { return -2; }
        if (snr > 6 && snr <= 7) { return -7; }
    }
    return 0.0;
}
signed int ErrorRate80211ad::getQpskBer(double snr, uint32_t rate)
{
    if (rate == 1) {
        if (snr >= 4 && snr <= 6.5) { return -1; }
        if (snr > 6.5 && snr <= 7) { return -2; }
        if (snr > 7 && snr <= 7.2) { return -3; }
        if (snr > 7.2 && snr <= 7.4) { return -4; }
        if (snr > 7.4 && snr <= 7.7) { return -5; }
        if (snr > 7.7 && snr <= 8) { return -6; }
    }
    if (rate == 3) {
        if (snr >= 6 && snr <= 8.2) { return -1; }
        if (snr > 8.2 && snr <= 9.2) { return -2; }
        if (snr > 9.2 && snr <= 9.7) { return -3; }
        if (snr > 9.7 && snr <= 10.2) { return -4; }
        if (snr > 10.2 && snr <= 10.5) { return -5; }
        if (snr > 10.5 && snr <= 11) { return -6; }
    }
    if (rate == 5) {
        if (snr >= 4 && snr <= 7.5) { return -1; }
        if (snr > 7.5 && snr <= 8.3) { return -2; }
        if (snr > 8.3 && snr <= 9) { return -3; }
        if (snr > 9 && snr <= 9.3) { return -4; }
        if (snr > 9.3 && snr <= 9.8) { return -5; }
        if (snr > 9.8 && snr <= 10) { return -6; }
    }
    return 0.0;
}
signed int ErrorRate80211ad::get16QamBer(double snr, uint32_t rate)
{
    if (rate == 1) {
        if (snr == 10) { return -1; }
        if (snr > 10 && snr <= 12) { return -2; }
        if (snr > 12 && snr <= 12.5) { return -3; }
        if (snr > 12.5 && snr <= 13.1) { return -4; }
        if (snr > 13.1 && snr <= 13.7) { return -5; }
        if (snr > 13.7 && snr <= 14) { return -6; }
    }
    if (rate == 3) {
        if (snr >= 11 && snr <=12) { return -1; }
        if (snr > 12 && snr <= 13) { return -2; }
        if (snr > 13 && snr <= 14) { return -3; }
        if (snr > 14 && snr <= 14.4) { return -4; }
        if (snr > 14.4 && snr <= 14.7) { return -5; }
        if (snr > 14.7 && snr <= 15) { return -6; }
    }
    if (rate == 5) {
        if (snr >= 13 && snr<=14) { return -1; }
        if (snr > 14 && snr <= 15) { return -2; }
        if (snr > 15 && snr <= 16) { return -3; }
        if (snr > 16 && snr <= 17.2) { return -4; }
        if (snr > 17.2 && snr <= 17.4) { return -5; }
        if (snr > 17.4 && snr <= 18) { return -6; }
    }
    if (rate == 13) {
        if (snr >= 10 && snr<=14) { return -1; }
        if (snr > 14 && snr <= 16) { return -2; }
        if (snr > 16 && snr <= 17) { return -3; }
        if (snr > 17 && snr <= 18) { return -4; }
        if (snr > 18 && snr <= 18.6) { return -5; }
        if (snr > 18.6 && snr <= 19) { return -6; }
    }
    return 0.0;
}
signed int ErrorRate80211ad::get64QamBer(double snr, uint32_t rate)
{
    if (rate == 3) {
        if (snr >= 17 && snr<=20) { return -1; }
        if (snr > 20 && snr <= 21.5) { return -2; }
        if (snr > 21.5 && snr <= 23) { return -3; }
        if (snr > 23 && snr <= 25) { return -4; }
        if (snr > 25 && snr <= 25.5) { return -5; }
        if (snr > 25.5 && snr <= 26) { return -6; }
    }
    if (rate == 5) {
        if (snr >= 16 && snr <=18) { return -1; }
        if (snr > 18 && snr <= 20) { return -2; }
        if (snr > 20 && snr <= 20.4) { return -3; }
        if (snr > 20.4 && snr <= 20.8) { return -4; }
        if (snr > 20.8 && snr <= 21.5) { return -5; }
        if (snr > 21.5 && snr <= 22) { return -6; }
    }
    if (rate == 13) {
        if (snr >= 18 && snr <=20) { return -1; }
        if (snr > 20 && snr <= 23) { return -2; }
        if (snr > 23 && snr <= 24) { return -3; }
        if (snr > 24 && snr <= 26) { return -4; }
        if (snr > 26 && snr <= 27.3) { return -5; }
        if (snr > 27.3 && snr <= 28) { return -6; }
    }
    return 0.0;
}

double ErrorRate80211ad::getSqpskSuccessProb(double snr, uint32_t nbits, uint32_t rate)
{
    double ber = getSqpskBer(snr, rate);
    if (ber == 0.0) {
        ber = 1.0;
    }
    return getSuccessRate(ber, nbits);
}
double ErrorRate80211ad::getQpskSuccessProb(double snr, uint32_t nbits, uint32_t rate)
{
    double ber = getQpskBer(snr, rate);
    if (ber == 0.0) {
        ber = 1.0;
    }
    return getSuccessRate(ber, nbits);
}
double ErrorRate80211ad::getQam16SuccessProb(double snr, uint32_t nbits, uint32_t rate)
{
    double ber = get16QamBer(snr, rate);
    if (ber == 0.0) {
        ber = 1.0;
    }
    return getSuccessRate(ber, nbits);
}
double ErrorRate80211ad::getQam64SuccessProb(double snr, uint32_t nbits, uint32_t rate)
{
    EV_INFO << "in qam64 nbitss, snr " << nbits << " ," << snr << endl;
    double ber = get64QamBer(snr, rate);
    if (ber == 0.0) {
        ber = 1.0;
    }
    return getSuccessRate(ber, nbits);
}
double ErrorRate80211ad::getSuccessRate(signed int ber, uint32_t nbits)
{
    double succProb = 1 - std::pow(10, ber);
    EV_INFO << "ber, succProb "<<  ber << " , " << succProb << endl;
    succProb = std::pow(succProb , nbits);

    return succProb;
}
double ErrorRate80211ad::getChunkSuccessRate(uint64_t datarate, enum Bandwidth bw, double snr_mW, uint32_t nbits)
{
    // get mcs from datarate and bw
    MCS mcs = getMCS(datarate, bw);
    // compute success probabilty depending on mcs
    switch (mcs) {
    case MCS::ofdm_sqpsk_r_1_2:
        return getSqpskSuccessProb(snr_mW, nbits, 1);
        break;
    case MCS::ofdm_sqpsk_r_5_8:
        return getSqpskSuccessProb(snr_mW, nbits, 5);
        break;
    case MCS::ofdm_qpsk_r_1_2:
        return getQpskSuccessProb(snr_mW, nbits, 1);
        break;
    case MCS::ofdm_qpsk_r_3_4:
        return getQpskSuccessProb(snr_mW, nbits, 3);
        break;
    case MCS::ofdm_qpsk_r_5_8:
        return getQpskSuccessProb(snr_mW, nbits, 5);
        break;
    case MCS::ofdm_qam16_r_1_2:
        return getQam16SuccessProb(snr_mW, nbits, 1);
        break;
    case MCS::ofdm_qam16_r_3_4:
        return getQam16SuccessProb(snr_mW, nbits, 3);
        break;
    case MCS::ofdm_qam16_r_5_8:
        return getQam16SuccessProb(snr_mW, nbits, 5);
        break;
    case MCS::ofdm_qam16_r_13_16:
        return getQam16SuccessProb(snr_mW, nbits, 13);
        break;
    case MCS::ofdm_qam64_r_3_4:
        return getQam64SuccessProb(snr_mW, nbits, 3);
        break;
    case MCS::ofdm_qam64_r_5_8:
        return getQam64SuccessProb(snr_mW, nbits, 5);
        break;
    case MCS::ofdm_qam64_r_13_16:
        return getQam64SuccessProb(snr_mW, nbits, 13);
        break;
    default:
        ASSERT2(false, "Invalid MCS chosen");
        break;
    }

    return 0;
}
