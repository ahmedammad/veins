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
#pragma once

#include <stdint.h>
#include <cmath>
#include "veins/modules/utility/ConstsPhy.h"

namespace veins {

/**
 * Sample the error rate for different modulations and coding schemes.
 * Taken from the IEEE papers.
 */
class VEINS_API ErrorRate80211ad {
public:
    ErrorRate80211ad();

    static double getChunkSuccessRate(unsigned int datarate, enum Bandwidth bw, double snr_mW, uint32_t nbits);

private:
    /**
     * Return BER of SQPSK at the given SNR.
     *
     * \param snr snr value
     * \return BER of SQPSK at the given SNR
     */
    static double getSqpskBer(double snr, uint32_t rate);
    /**
     * Return BER of QPSK at the given SNR.
     *
     * \param snr snr value
     * \return BER of QPSK at the given SNR
     */
    static double getQpskBer(double snr, uint32_t rate);
    /**
     * Return BER of QAM16 at the given SNR.
     *
     * \param snr snr value
     * \return BER of QAM16 at the given SNR
     */
    static double get16QamBer(double snr, uint32_t rate);
    /**
     * Return BER of QAM64 at the given SNR.
     *
     * \param snr snr value
     * \return BER of QAM64 at the given SNR
     */
    static double get64QamBer(double snr, uint32_t rate);
    /**
     * Return Success Probability of SQPSK at the given SNR after applying BER sampling.
     *
     * \param snr snr value
     * \param nbits the number of bits in the chunk
     * \param rate
     * \return Success Probabilty of SQPSK at the given SNR after applying BER sampling.
     */
    static double getSqpskSuccessProb(double snr, uint32_t nbits, uint32_t rate);
    /**
     * Return Success Probability of QPSK at the given SNR after applying BER sampling.
     *
     * \param snr snr value
     * \param nbits the number of bits in the chunk
     * \param rate
     * \return Success Probabilty of QPSK at the given SNR after applying BER sampling.
     */
    static double getQpskSuccessProb(double snr, uint32_t nbits, uint32_t rate);
    /**
     * Return Success Probability of QAM16 at the given SNR after applying BER sampling.
     *
     * \param snr snr value
     * \param nbits the number of bits in the chunk
     * \param rate
     * \return Success Probabilty of QAM16 at the given SNR after applying BER sampling.
     */
    static double getQam16SuccessProb(double snr, uint32_t nbits, uint32_t rate);
    /**
     * Return Success Probability of QAM64 at the given SNR after applying BER sampling.
     *
     * \param snr snr value
     * \param nbits the number of bits in the chunk
     * \param rate
     * \return Success Probabilty of QAM64 at the given SNR after applying BER sampling.
     */
    static double getQam64SuccessProb(double snr, uint32_t nbits, uint32_t rate);
};

} // namespace veins
