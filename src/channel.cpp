#include "../include/QAMChannel.h"
#include "../include/help.h"
#include "../include/QAMModulator.h"

QAMChannel::QAMChannel() = default;

void QAMChannel::add_noise(Signal& signal, double noise_stddeviation) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, noise_stddeviation);

    double noise_in_phase = d(gen);
    double noise_quadrature = d(gen);

    signal.in_phase += noise_in_phase;
    signal.quadrature += noise_quadrature;
}

void QAMChannel::add_noise_arr(Signal* signal_arr, double noise_stddeviation, int amount) {
    check(signal_arr != 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, noise_stddeviation);

    double noise_in_phase = d(gen);
    double noise_quadrature = d(gen);

    for (int co = 0; co < amount; co++)
    {
        signal_arr[co].in_phase   += noise_in_phase;
        signal_arr[co].quadrature += noise_quadrature;
    }
}
