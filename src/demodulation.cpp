#include "../include/QAMDemodulator.h"
#include "../include/help.h"
#include <iostream>
#include <cmath>

QAMDemodulator::QAMDemodulator(ModulationType modulation_type) : modulation_type(modulation_type) {}

int QAMDemodulator::demodulate(double in_phase, double quadrature) {
    int max_shift = get_max_shift();

    int inted_in_phase = nearest_odd(in_phase);
    int inted_quadrature = nearest_odd(quadrature);

    inted_in_phase -= pow2(max_shift);
    inted_quadrature -= pow2(max_shift);

    return demodulation(inted_in_phase, inted_quadrature, max_shift);
}

int* QAMDemodulator::demodulate_arr(Signal* signal_arr) {
    hard_check(signal_arr != 0);

    int max_shift = get_max_shift();
    int amount    = get_amount(); 

    int inted_in_phase   = 0;
    int inted_quadrature = 0;

    int* demodulated_arr = (int*)calloc(amount, sizeof(int));
    hard_check(demodulated_arr != 0);

    for (int co = 0; co < amount; co++)
    {
        inted_in_phase   = nearest_odd(signal_arr[co].in_phase);
        inted_quadrature = nearest_odd(signal_arr[co].quadrature);

        inted_in_phase   -= pow2(max_shift);
        inted_quadrature -= pow2(max_shift);

        demodulated_arr[co] = demodulation(inted_in_phase, inted_quadrature, max_shift);
    }

    return demodulated_arr;
}

int QAMDemodulator::bit_check(int* origin_arr, int* check_arr, int amount)     {
    hard_check(origin_arr != 0);
    hard_check(check_arr  != 0);

    int max_bit  = 2 * get_max_shift(); 
    int mistakes = 0;

    for (int co = 0; co < amount; co++)
    {
        for (int i = max_bit - 1; i >= 0; i--) {
            mistakes += ((origin_arr[co] >> i) & 1) ^ ((check_arr[co] >> i) & 1);
        }
    }

    return mistakes;
}

int QAMDemodulator::pow2(int pow) {
    return 1 << pow;
}

int QAMDemodulator::get_amount() {
    switch (modulation_type) {
        case ModulationType::QPSK:
            return QPSK_amount;
        case ModulationType::QAM16:
            return QAM16_amount;
        case ModulationType::QAM64:
            return QAM64_amount;
        default:
            return 0;
    }
}

int QAMDemodulator::get_max_shift() {
    switch (modulation_type) {
        case ModulationType::QPSK:
            return QPSK_shft;
        case ModulationType::QAM16:
            return QAM16_shft;
        case ModulationType::QAM64:
            return QAM64_shft;
        default:
            return 0;
    }
}

int QAMDemodulator::get_max_bit() {
    switch (modulation_type) {
        case ModulationType::QPSK:
            return QPSK_max;
        case ModulationType::QAM16:
            return QAM16_max;
        case ModulationType::QAM64:
            return QAM64_max;
        default:
            return 0;
    }
}

int QAMDemodulator::demodulation(int in_phase, int quadrature, int max_shift) {
    int bit = 0;

    for (int bit_idx = max_shift; bit_idx > 0; bit_idx--) {
        if (std::abs(in_phase) < pow2(bit_idx)) {
            bit |= 1 << bit_idx + max_shift - 1;
        }
        if (std::abs(quadrature) < pow2(bit_idx)) {
            bit |= 1 << bit_idx - 1;
        }
        in_phase   = adjust_value(in_phase,   pow2(bit_idx));
        quadrature = adjust_value(quadrature, pow2(bit_idx));
    }
    return bit;
}

int QAMDemodulator::adjust_value(int val, int step) {
    return (val >= 0) ? val - step : val + step;
}

int QAMDemodulator::sgn(double val) {
    return (val < 0) ? -1 : 1;
}

int QAMDemodulator::nearest_odd(double val) {
    int nearest_int = static_cast<int>(val);
    return (nearest_int % 2) ? nearest_int : nearest_int + sgn(val);
}
