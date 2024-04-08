#include <iostream>
#include <cmath>

class QAMModulator {
public:
    enum class ModulationType {
        QPSK,
        QAM16,
        QAM64
    };

    QAMModulator(ModulationType modulation_type) : modulation_type(modulation_type) {}

    void modulate(int *in_phase, int *quadrature, int bit) {
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

        modulation_internal(quadrature, in_phase, mask, bit, max_bits);
    }

    void print_map() {
        int arr[15][15] = {0};

        int max_bit = get_max_bit();

        for (int bit = 0; bit < max_bit; bit++) {
            int in_phase = 0;
            int quadrature = 0;

            modulate(&in_phase, &quadrature, bit);

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

private:
    ModulationType modulation_type;

    void modulation_internal(int* quadrature, int* in_phase, int mask, int bit, int max_bits) {
        for (int co = max_bits; co >= 0; co--) {
            *in_phase += powneg1(max_bits % 2) * powneg1(co % 2) * powneg1(bit & mask) * sgn(*in_phase - sgn(*in_phase) * pow2(co + 2)) * pow2(co);
            mask >>= 1;
        }

        for (int co = max_bits; co >= 0; co--) {
            *quadrature += powneg1(max_bits % 2) * powneg1(co % 2) * powneg1(bit & mask) * sgn(*quadrature - sgn(*quadrature) * pow2(co + 2)) * pow2(co);
            mask >>= 1;
        }
    }

    int powneg1(int pow) {
        return (pow == 0) ? 1 : -1;
    }

    int pow2(int pow) {
        return (pow < 0) ? 1 : (1 << pow);
    }

    int sgn(int val) {
        return (val < 0) ? -1 : 1;
    }

    int get_max_bit() {
        switch (modulation_type) {
            case ModulationType::QPSK:
                return 4;
            case ModulationType::QAM16:
                return 16;
            case ModulationType::QAM64:
                return 64;
            default:
                return 0;
        }
    }

    void bin_print(int value) {
        for (int i = 5; i >= 0; --i) {
            std::cout << ((value >> i) & 1);
        }
    }
};

class QAMDemodulator {
public:
    enum class ModulationType {
        QPSK,
        QAM16,
        QAM64
    };

    QAMDemodulator(ModulationType modulation_type) : modulation_type(modulation_type) {}

    int demodulate(double in_phase, double quadrature) {
        int max_bits = get_max_bits();

        int inted_in_phase   = nearest_odd(in_phase);
        int inted_quadrature = nearest_odd(quadrature);

        std::cout<< "quadrature = " << quadrature << std::endl;
        std::cout<< "in_phase = " << in_phase << std::endl;

        std::cout<< "inted_quadrature = " << inted_quadrature << std::endl;
        std::cout<< "inted_in_phase = " << inted_in_phase << std::endl;

        inted_in_phase   -= pow2(max_bits + 1);
        inted_quadrature -= pow2(max_bits + 1);

        return demodulation(inted_in_phase, inted_quadrature, max_bits);
    }

private:
    ModulationType modulation_type;

    int pow2(int pow) {
        return 1 << pow;
    }

    int get_max_bits() {
        switch (modulation_type) {
            case ModulationType::QPSK:
                return 0;
            case ModulationType::QAM16:
                return 1;
            case ModulationType::QAM64:
                return 2;
            default:
                return 0;
        }
    }

    int demodulation(int in_phase, int quadrature, int max_bits) {
        int bit = 0;
        for (int bit_idx = max_bits; bit_idx >= 0; bit_idx--) {
            if (std::abs(in_phase) < pow2(bit_idx + 1)) {
                bit |= 1 << bit_idx + max_bits + 1;
            }
            if (std::abs(quadrature) < pow2(bit_idx + 1)) {
                bit |= 1 << bit_idx;
            }
            in_phase = adjust_value(in_phase, pow2(bit_idx + 1));
            quadrature = adjust_value(quadrature, pow2(bit_idx + 1));
        }
        return bit;
    }

    int adjust_value(int val, int step) {
        return (val >= 0) ? val - step : val + step;
    }

    int sgn(double val) {
        return (val < 0) ? -1 : 1;
    }

    int nearest_odd(double val) {
        int nearest_int = static_cast<int>(val);
        return (nearest_int % 2) ? nearest_int :  nearest_int + sgn(val);
    }

};

int main() {
    QAMModulator qam64Modulator(QAMModulator::ModulationType::QAM64);
    QAMModulator qam16Modulator(QAMModulator::ModulationType::QAM16);
    QAMModulator qpskModulator(QAMModulator::ModulationType::QPSK);

    QAMDemodulator qpsk_demodulator(QAMDemodulator::ModulationType::QPSK);
    QAMDemodulator qam16_demodulator(QAMDemodulator::ModulationType::QAM16);
    QAMDemodulator qam64_demodulator(QAMDemodulator::ModulationType::QAM64);

    // Примеры значений in_phase и quadrature для тестирования
    int in_phase_values[] = {-1, 1, -1, 1};
    int quadrature_values[] = {-1, 1, 1, -1};

    qpskModulator.print_map();
    std::cout << "#######################################" <<  std::endl;
/*
    // Тестирование для каждого типа модуляции
    std::cout << "QPSK Demodulation:" << std::endl;
    for (int i = 0; i < sizeof(in_phase_values) / sizeof(in_phase_values[0]); ++i) {
        int bit = qpsk_demodulator.demodulate(in_phase_values[i], quadrature_values[i]);
        std::cout << "InPhase: " << in_phase_values[i] << ", Quadrature: " << quadrature_values[i] << ", Bit: " << bit << std::endl;
    }*/

    qam16Modulator.print_map();
    std::cout << "#######################################" <<  std::endl;

    /*std::cout << "\nQAM16 Demodulation:" << std::endl;
    for (int i = 0; i < sizeof(in_phase_values) / sizeof(in_phase_values[0]); ++i) {
        int bit = qam16_demodulator.demodulate(in_phase_values[i], quadrature_values[i]);
        std::cout << "InPhase: " << in_phase_values[i] << ", Quadrature: " << quadrature_values[i] << ", Bit: " << bit << std::endl;
    }*/
    
    qam64Modulator.print_map();
    std::cout << "#######################################" <<  std::endl;

    /*std::cout << "\nQAM64 Demodulation:" << std::endl;
    for (int i = 0; i < sizeof(in_phase_values) / sizeof(in_phase_values[0]); ++i) {
        int bit = qam64_demodulator.demodulate(in_phase_values[i], quadrature_values[i]);
        std::cout << "InPhase: " << in_phase_values[i] << ", Quadrature: " << quadrature_values[i] << ", Bit: " << bit << std::endl;
    }*/

    int bit = 0;

    bit = qam64_demodulator.demodulate(-8 + 0.5, -8 + 0.5);
    std::cout<< "bit = " << bit << std::endl;

}