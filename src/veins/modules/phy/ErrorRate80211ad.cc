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
double ErrorRate80211ad::getSqpskBer(double snr, uint32_t rate)
{
    double z = std::sqrt(snr / 2.0);
    double ber = 0.5 * erfc(z);
    return ber;
}
double ErrorRate80211ad::getQpskBer(double snr, uint32_t rate)
{
    if (rate == 1) {
        if (snr == 7) { return -2; }
        if (snr > 7 && snr <= 7.2) { return -3; }
        if (snr > 7.2 && snr <= 7.4) { return -4; }
        if (snr > 7.4 && snr <= 7.7) { return -5; }
        if (snr > 7.7 && snr <= 8) { return -6; }
    }
    if (rate == 3) {
        if (snr == 9) { return -2; }
        if (snr > 9 && snr <= 9.3) { return -3; }
        if (snr > 9.3 && snr <= 10) { return -4; }
        if (snr > 10 && snr <= 10.5) { return -5; }
        if (snr > 10.5 && snr <= 11) { return -6; }
    }
    if (rate == 5) {
        if (snr == 8) { return -2; }
        if (snr > 8 && snr <= 9) { return -3; }
        if (snr > 9 && snr <= 9.3) { return -4; }
        if (snr > 9.3 && snr <= 9.8) { return -5; }
        if (snr > 9.8 && snr <= 10) { return -6; }
    }
    return 0.0;
}
double ErrorRate80211ad::get16QamBer(double snr, uint32_t rate)
{
    double z = std::sqrt(snr / (5.0 * 2.0));
    double ber = 0.75 * 0.5 * erfc(z);
    return ber;
}
double ErrorRate80211ad::get64QamBer(double snr, uint32_t rate)
{
    double z = std::sqrt(snr / (21.0 * 2.0));
    double ber = 7.0 / 12.0 * 0.5 * erfc(z);
    return ber;
}

double ErrorRate80211ad::getSqpskSuccessProb(double snr, uint32_t nbits, uint32_t rate)
{
    double ber = getSqpskBer(snr, rate);
    if (ber == 0.0) {
        return 1.0;
    }

    return 0;
}
double ErrorRate80211ad::getQpskSuccessProb(double snr, uint32_t nbits, uint32_t rate)
{
    double ber = getQpskBer(snr, rate);
    if (ber == 0.0) {
        return 1.0;
    }

    return 0;
}
double ErrorRate80211ad::getQam16SuccessProb(double snr, uint32_t nbits, uint32_t rate)
{
    double ber = get16QamBer(snr, rate);
    if (ber == 0.0) {
        return 1.0;
    }

    return 0;
}
double ErrorRate80211ad::getQam64SuccessProb(double snr, uint32_t nbits, uint32_t rate)
{
    double ber = get64QamBer(snr, rate);
    if (ber == 0.0) {
        return 1.0;
    }

    return 0;
}
double ErrorRate80211ad::getChunkSuccessRate(unsigned int datarate, enum Bandwidth bw, double snr_mW, uint32_t nbits)
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
