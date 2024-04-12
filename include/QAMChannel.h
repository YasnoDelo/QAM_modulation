#ifndef QAM_CHANNEL_H
#define QAM_CHANNEL_H

#include "../include/QAMModulator.h"
#include <random>

class QAMChannel {
public:
    QAMChannel();
    void add_noise(Signal& signal, double noise_stddeviation);
    void add_noise_arr(Signal* signal_arr, double noise_stddeviation, int amount);

private:
    double noise_stddeviation;
};

#endif // QAM_CHANNEL_H
