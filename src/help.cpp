#include "../include/help.h"

std::string after_dev = R"(])
missed_qpsk  = np.array([)";

void full_py_beg(PythonFileCreator& file_ctor, const std::string filename) {
    std::string intro = R"(import numpy as np
import matplotlib.pyplot as plt

diviation   = np.array([)";

    
    file_ctor.clear_file_content(filename);
    file_ctor.add_lines_to_python_file(filename, intro);
}

void full_py_end(PythonFileCreator& file_ctor, const std::string filename){
    std::string end = R"(])

print(diviation)
print(missed_qpsk)
print(missed_qam16)
print(missed_qam64)

plt.ylabel(r"probability of missing on 1 bit") # подписи к осям
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

void test_modulation(QAMModulator& modulator, QAMDemodulator& demodulator, const std::string& modulation_name, PythonFileCreator& file_ctor, QAMChannel& channel, const std::string& filename) {
    std::cout << modulation_name << ":" << std::endl;

    TestInfo test_info;

    double sum = 0.0;

    static int status = 0;

    int misses = 0;
    int max_co = ACCUR;
    int points = POINTS;

    int* arr_bit         = nullptr;
    int* demodulated_arr = nullptr;
    Signal* signal_arr   = nullptr;

    double* dev = (double*)calloc(points, sizeof(double));  //array of deviation
    double* ave = (double*)calloc(points, sizeof(double));  //array of average value of misses
    hard_check(ave != nullptr);
    hard_check(dev != nullptr);

    for (int co = 1; co < points; co++) {
        dev[co] = 0.05*co;
    }

    if (!status)
    {
        std::string dev_arr = file_ctor.double_array_to_string(dev, points);
        file_ctor.add_lines_to_python_file(filename, dev_arr);
        file_ctor.add_lines_to_python_file(filename, after_dev);
        status++;
    }

    for(int out_co = 0; out_co < points; out_co++) {
        sum = 0.0;

        for (int in_co = 0; in_co < max_co; in_co++) {
            test_info = modulator.rand_bits_modulate(arr_bit, signal_arr);
            hard_check(arr_bit    != nullptr);
            hard_check(signal_arr != nullptr);

            check(test_info.amount      != 0);
            check(test_info.bit_in_word != 0);


            channel.add_noise_arr(signal_arr, dev[out_co], test_info.amount);

            demodulated_arr = demodulator.demodulate_arr(signal_arr);
            misses          =  demodulator.bit_check(arr_bit, demodulated_arr, test_info.amount);

            sum += misses;
        }

        ave[out_co] = sum/((double)max_co * (double)test_info.amount * (double)test_info.bit_in_word);
        
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