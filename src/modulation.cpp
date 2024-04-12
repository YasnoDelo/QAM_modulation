#include "../include/QAMModulator.h"
#include "../include/help.h"
#include <iostream>
#include <cmath>
#include <random>

QAMModulator::QAMModulator(ModulationType modulation_type) : modulation_type(modulation_type) {}

void QAMModulator::modulate(Signal& signal, int bit) {
    int mask = 0;
    int max_bits = 0;

    switch (modulation_type) {
        case ModulationType::QPSK:
            max_bits = 0;
            mask = 0b10;
            break;
        case ModulationType::QAM16:
            max_bits = 1;
            mask = 0b1000;
            break;
        case ModulationType::QAM64:
            max_bits = 2;
            mask = 0b100000;
            break;
        default:
            std::cout << "Invalid modulation type!" << std::endl;
            return;
    }

    modulation_internal(signal.quadrature, signal.in_phase, mask, bit, max_bits);
}

void QAMModulator::print_map(){
    int arr[15][15] = {0};
    Signal signal;
    int max_bit = get_max_bit();

    int in_phase = 0;
    int quadrature = 0;

    for (int bit = 0; bit < max_bit; bit++) {
        signal.in_phase   = 0;
        signal.quadrature = 0;

        in_phase   = 0;
        quadrature = 0;

        modulate(signal, bit);

        in_phase   = signal.in_phase;
        quadrature = signal.quadrature;

        in_phase += 7;
        quadrature += 7;

        arr[quadrature][in_phase] = bit;
    }

    for (int row = 14; row >= 0; row--) {
        if (row % 2) {
            for (int col = 0; col < 15; col++) {
                std::cout << "--------";
            }
            std::cout << std::endl;
        } else {
            for (int col = 0; col < 15; col++) {
                if (col % 2) {
                    std::cout << "  |  ";
                } else {
                    std::cout << "  ";
                    bin_print(arr[row][col]);
                    std::cout << "  ";
                }
            }
            std::cout << std::endl;
        }
    }
}


TestInfo QAMModulator::rand_bits_modulate(int*& bit_arr, Signal*& signal_arr) {
    TestInfo test_info;

    int max_bit           = get_max_bit();
    test_info.amount      = get_amount();
    test_info.bit_in_word = 2*get_max_shift();
    check(max_bit != 0);
    check(test_info.amount != 0);
    check(test_info.bit_in_word != 0);

    bit_arr    =    (int*)calloc(test_info.amount, sizeof(int));
    hard_check(bit_arr != 0);

    signal_arr = (Signal*)calloc(test_info.amount, sizeof(Signal));
    hard_check(signal_arr != 0);

    for (int co = 0; co < test_info.amount; co++)
    {
        bit_arr[co] = random_bit(0, max_bit);
    }

    for (int co = 0; co < test_info.amount; co++)
    {
        modulate(signal_arr[co], bit_arr[co]);
    }

    return test_info;
}


void QAMModulator::modulation_internal(double& quadrature, double& in_phase, int mask, int bit, int max_bits) {
    int inted_in_phase = 0;
    int inted_quadrature = 0;

    for (int co = max_bits; co >= 0; co--) {
        inted_in_phase += powneg1(max_bits % 2) * powneg1(co % 2) * powneg1(bit & mask) * sgn(inted_in_phase - sgn(inted_in_phase) * pow2(co + 2)) * pow2(co);
        mask >>= 1;
    }

    for (int co = max_bits; co >= 0; co--) {
        inted_quadrature += powneg1(max_bits % 2) * powneg1(co % 2) * powneg1(bit & mask) * sgn(inted_quadrature - sgn(inted_quadrature) * pow2(co + 2)) * pow2(co);
        mask >>= 1;
    }

    quadrature = inted_quadrature;
    in_phase = inted_in_phase;
}

int QAMModulator::powneg1(int pow) {
    return (pow == 0) ? 1 : -1;
}

int QAMModulator::pow2(int pow) {
    return (pow < 0) ? 1 : (1 << pow);
}

int QAMModulator::sgn(int val) {
    return (val < 0) ? -1 : 1;
}

int QAMModulator::get_amount() {
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

int QAMModulator::get_max_bit() {
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

int QAMModulator::get_max_shift() {
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

void QAMModulator::bin_print(int value) {
    for (int i = 5; i >= 0; --i) {
        std::cout << ((value >> i) & 1);
    }
}

int QAMModulator::random_bit(int min, int max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max - 1);

    return dis(gen);
}
