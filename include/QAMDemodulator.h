#ifndef QAM_DEMODULATOR_H
#define QAM_DEMODULATOR_H

#include "../include/QAMModulator.h"

class QAMDemodulator {
public:
    enum class ModulationType {
        QPSK,
        QAM16,
        QAM64
    };

    QAMDemodulator(ModulationType modulation_type);
    int demodulate(double in_phase, double quadrature);
    int* demodulate_arr(Signal* signal_arr);
    int bit_check(int* origin_arr, int* check_arr, int amount);

private:
    ModulationType modulation_type;
    int pow2(int pow);
    int get_amount();
    int get_max_shift();
    int get_max_bit();
    int demodulation(int in_phase, int quadrature, int max_shift);
    int adjust_value(int val, int step);
    int sgn(double val);
    int nearest_odd(double val);
};

#endif // QAM_DEMODULATOR_H
