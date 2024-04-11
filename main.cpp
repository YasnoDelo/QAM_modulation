#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iomanip>

#define check(condition)                                                                                                                 \
    if (!(condition))                                                                                                                                \
    {                                                                                                                                                \
        printf("\033[91mОшибка в условии %s в строке %d в файле %s в функции %s \n\033[39m", #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__ );  \
    }

#define hard_check(condition)                                                                                                                        \
    if (!(condition))                                                                                                                                \
    {                                                                                                                                                \
        printf("\033[91mОшибка в условии %s в строке %d в файле %s в функции %s \n\033[39m", #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__ ); \
        abort();                                                                                                                                     \
    }

#define RED(b) "\033[91m" b "\033[39m"
#define GREEN(b) "\033[32m" b "\033[39m"

#define  QPSK_amount 60;
#define QAM16_amount 30;
#define QAM64_amount 20;

#define  QPSK_max  4;
#define QAM16_max 16;
#define QAM64_max 64;

std::string after_qpsk = R"(])
missed_qam16    = np.array([)";

std::string after_qam16 = R"(])
missed_qam64    = np.array([)";

std::string after_qam64 = R"(])
diviation = np.array([)";

typedef struct signal
{
    double in_phase   = 0;
    double quadrature = 0;
} Signal;

class QAMModulator {
public:
    enum class ModulationType {
        QPSK,
        QAM16,
        QAM64
    };

    QAMModulator(ModulationType modulation_type) : modulation_type(modulation_type) {}

    void modulate(Signal& signal, int bit) {
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

    void print_map() {
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

    int rand_bits_modulate(int*& bit_arr, Signal*& signal_arr) {
        int max_bit = 0;
        int amount  = 0;

        switch (modulation_type) {
            case ModulationType::QPSK:
                max_bit = QPSK_max;
                amount  = QPSK_amount;
                break;
            case ModulationType::QAM16:
                max_bit = QAM16_max;
                amount  = QAM16_amount;
                break;
            case ModulationType::QAM64:
                max_bit = QAM64_max;
                amount  = QAM64_amount;
                break;
            default:
                return 0;
        }

        bit_arr    =    (int*)calloc(amount, sizeof(int));
        hard_check(bit_arr != 0);

        signal_arr = (Signal*)calloc(amount, sizeof(Signal));
        hard_check(signal_arr != 0);

        for (int co = 0; co < amount; co++)
        {
            bit_arr[co] = random_bit(0, max_bit);
        }

        for (int co = 0; co < amount; co++)
        {
            modulate(signal_arr[co], bit_arr[co]);
        }

        return amount;
    }


private:
    ModulationType modulation_type;

    void modulation_internal(double& quadrature, double& in_phase, int mask, int bit, int max_bits) {
        int inted_in_phase   = 0;
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
        in_phase   = inted_in_phase;
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

    int get_amount() {
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

    int get_max_bit() {
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

    void bin_print(int value) {
        for (int i = 5; i >= 0; --i) {
            std::cout << ((value >> i) & 1);
        }
    }

    int random_bit(int min, int max){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max - 1);

        return dis(gen);
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
        int max_shift = get_max_shift();

        int inted_in_phase   = nearest_odd(in_phase);
        int inted_quadrature = nearest_odd(quadrature);

        inted_in_phase   -= pow2(max_shift);
        inted_quadrature -= pow2(max_shift);

        return demodulation(inted_in_phase, inted_quadrature, max_shift);
    }

    int* demodulate_arr(Signal* signal_arr) {
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

    int bit_check(int* origin_arr, int* check_arr, int amount)
    {
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

private:
    ModulationType modulation_type;

    int pow2(int pow) {
        return 1 << pow;
    }

    int get_amount() {
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

    int get_max_shift() {
        switch (modulation_type) {
            case ModulationType::QPSK:
                return 1;
            case ModulationType::QAM16:
                return 2;
            case ModulationType::QAM64:
                return 3;
            default:
                return 0;
        }
    }

    int get_max_bit() {
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

    int demodulation(int in_phase, int quadrature, int max_shift) {
        
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

class QAMChannel {
public:
    QAMChannel() = default;

    void add_noise(Signal& signal, double noise_stddeviation) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> d(0, noise_stddeviation);

        double noise_in_phase = d(gen);
        double noise_quadrature = d(gen);

        signal.in_phase += noise_in_phase;
        signal.quadrature += noise_quadrature;
    }

    void add_noise_arr(Signal* signal_arr, double noise_stddeviation, int amount) {
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

private:
    double noise_stddeviation;
};

class PythonFileCreator {
private:

public:
    PythonFileCreator() = default;

    std::string double_array_to_string(double* arr, int array_size) {
        std::stringstream ss;
        for (int i = 0; i < array_size; ++i) {
            if(i == array_size -1){
                ss << arr[i];
                break;
            }
            ss << arr[i] << ", ";
        }
        return ss.str();
    }

    void add_lines_to_python_file(const std::string& filename, const std::string& string) {
        std::ofstream outfile(filename, std::ios::app);
        if (outfile.is_open()) {
            outfile << string;
            outfile.close();
            std::cout << "Lines added to Python file '" << filename << "' successfully.\n";
        } else {
            std::cerr << "Unable to open Python file.\n";
        }
    }

    void clear_file_content(const std::string& filename) {
        std::ofstream outfile(filename, std::ios::trunc);
        if (outfile.is_open()) {
            outfile.close();
            std::cout << "Content of file '" << filename << "' cleared successfully.\n";
        } else {
            std::cerr << "Unable to clear content of file.\n";
        }
    }
};

void test_modulation(QAMModulator& modulator, QAMDemodulator& demodulator, const std::string& modulation_name, PythonFileCreator& file_ctor, QAMChannel& channel, const std::string& filename) {
    std::cout << modulation_name << ":" << std::endl;

    int amount = 0;
    int misses = 0;
    int max_co = 10000;
    int points = 90;

    double sum = 0.0;

    int* arr_bit         = nullptr;
    int* demodulated_arr = nullptr;
    Signal* signal_arr   = nullptr;

    double* dev = (double*)calloc(points, sizeof(double));
    double* ave = (double*)calloc(points, sizeof(double));
    hard_check(ave != nullptr);
    hard_check(dev != nullptr);

    for (int co = 1; co < points; co++) {
        dev[co] = 0.05*co;
    }

    for(int out_co = 0; out_co < points; out_co++) {
        sum = 0.0;

        for (int in_co = 0; in_co < max_co; in_co++) {
            amount = modulator.rand_bits_modulate(arr_bit, signal_arr);
            check(arr_bit    != nullptr);
            check(signal_arr != nullptr);
            check(amount     != 0);

            channel.add_noise_arr(signal_arr, dev[out_co], amount);

            demodulated_arr = demodulator.demodulate_arr(signal_arr);
            misses =  demodulator.bit_check(arr_bit, demodulated_arr, amount);
            sum += misses;
        }

        ave[out_co] = sum/((double)max_co);
        
        std::cout.setf(std::ios::fixed);
        std::cout.precision(2); //2 - число символов после точки
        std::cout << "dev = " << dev[out_co] << "   ave = " << ave[out_co] << std::endl;
    }

    std::string modulation_arr = file_ctor.double_array_to_string(ave, points);
    file_ctor.add_lines_to_python_file(filename, modulation_arr);

    free(demodulated_arr);
    free(signal_arr);
    free(arr_bit);
    free(dev);
    free(ave);
}

void full_py_beg(PythonFileCreator& file_ctor, const std::string filename) {
    std::string intro = R"(import numpy as np
import matplotlib.pyplot as plt

missed_qpsk     = np.array([)";

    
    file_ctor.clear_file_content(filename);
    file_ctor.add_lines_to_python_file(filename, intro);
}

void full_py_end(PythonFileCreator& file_ctor, const std::string filename){
    std::string end = R"(])

print(diviation)
print(missed_qpsk)
print(missed_qam16)
print(missed_qam64)

koefs_qpsk  = linearize(diviation, missed_qpsk)
koefs_qam16 = linearize(diviation, missed_qam16)
koefs_qam64 = linearize(diviation, missed_qam64)

plt.ylabel(r"missed bits") # подписи к осям
plt.xlabel(r"diviation")
plt.title(r'Зависимость количесва утеряных бит от дисперсии шума') # заголовок
plt.grid(True) # сетка

plt.plot(diviation, missed_qpsk, '+', label='QPSK', color="red")
plt.plot(diviation, missed_qam16, 'o', label='QAM16', color="green")
plt.plot(diviation, missed_qam64, 'x', label='QAM64', color="blue")

plt.legend()
plt.show()
    )";

    file_ctor.add_lines_to_python_file(filename, end);

}

int main() {    
    const std::string filename = "output.py";
    PythonFileCreator file_ctor;
    QAMChannel channel;

    full_py_beg(file_ctor, filename);

    QAMModulator modulator_4(QAMModulator::ModulationType::QPSK);
    QAMDemodulator demodulator_4(QAMDemodulator::ModulationType::QPSK);
    test_modulation(modulator_4, demodulator_4, "QPSK", file_ctor, channel, filename);
    file_ctor.add_lines_to_python_file(filename, after_qpsk);

    QAMModulator modulator_16(QAMModulator::ModulationType::QAM16);
    QAMDemodulator demodulator_16(QAMDemodulator::ModulationType::QAM16);
    test_modulation(modulator_16, demodulator_16, "QAM16", file_ctor, channel, filename);
    file_ctor.add_lines_to_python_file(filename, after_qam16);

    QAMModulator modulator_64(QAMModulator::ModulationType::QAM64);
    QAMDemodulator demodulator_64(QAMDemodulator::ModulationType::QAM64);
    test_modulation(modulator_64, demodulator_64, "QAM64", file_ctor, channel, filename);
    file_ctor.add_lines_to_python_file(filename, after_qam64);

    full_py_end(file_ctor, filename);

    return 0;

}