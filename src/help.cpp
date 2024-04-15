#include "../include/help.h"

std::string missed_text = R"(missed_)";

std::string SNR_text = R"(SNR_)";

std::string np_arr_beg = R"( = np.array([)";

std::string np_arr_end = R"(])
)";

void full_py_beg(PythonFileCreator& file_ctor, const std::string filename) {
    std::string intro = R"(import numpy as np
import matplotlib.pyplot as plt

)";

    
    file_ctor.clear_file_content(filename);
    file_ctor.add_lines_to_python_file(filename, intro);
}

void full_py_end(PythonFileCreator& file_ctor, const std::string filename){
    std::string end = R"(

print(SNR_QPSK)
print(missed_QPSK)
print(SNR_QAM16)
print(missed_QAM16)
print(SNR_QAM64)
print(missed_QAM64)

plt.ylabel(r"BER, штуки") # подписи к осям
plt.xlabel(r"SNR, Дб")
plt.title(r'Зависимость bit error ratio от signal/noise ratio') # заголовок
plt.grid(True) # сетка

plt.plot(SNR_QPSK, missed_QPSK, '+', label='QPSK', color="red")
plt.plot(SNR_QAM16, missed_QAM16, 'o', label='QAM16', color="green")
plt.plot(SNR_QAM64, missed_QAM64, 'x', label='QAM64', color="blue")

plt.legend()
plt.show()
    )";

    file_ctor.add_lines_to_python_file(filename, end);
}

double aver_sq_find(Signal* signal_arr, int amount){
    double sum_sq = 0.0;

    for (int co = 0; co < amount; co++)
    {
        sum_sq += signal_arr[co].quadrature * signal_arr[co].quadrature;
    }

    sum_sq = sum_sq / ((double)amount);

    std::cout << "sum_sq = " << sum_sq <<std::endl;

    return sum_sq;
}

void test_modulation(QAMModulator& modulator, QAMDemodulator& demodulator, const std::string& modulation_name, PythonFileCreator& file_ctor, QAMChannel& channel, const std::string& filename) {
    std::cout << modulation_name << ":" << std::endl;

    TestInfo test_info;

    double sum = 0.0;
    double aver_quadr = 0.0;

    static int status = 0;

    int found = 0;

    int misses = 0;
    int max_co = ACCUR;
    int points = POINTS;

    int* arr_bit         = nullptr;
    int* demodulated_arr = nullptr;
    Signal* signal_arr   = nullptr;

    double* dev = (double*)calloc(points, sizeof(double));  //array of deviation
    double* ave = (double*)calloc(points, sizeof(double));  //array of average amount of missed 
    double* SNR = (double*)calloc(points, sizeof(double));  //array of SNR for every deviation & modulation
    hard_check(ave != nullptr);
    hard_check(dev != nullptr);

    for (int co = 0; co < points; co++) {
        dev[co] = 0.05*co + 0.05;
    }

    for(int out_co = 0; out_co < points; out_co++) {
        sum = 0.0;

        for (int in_co = 0; in_co < max_co; in_co++) {
            test_info = modulator.rand_bits_modulate(arr_bit, signal_arr);
            hard_check(arr_bit    != nullptr);
            hard_check(signal_arr != nullptr);

            check(test_info.amount      != 0);
            check(test_info.bit_in_word != 0);

            if(!found){
                aver_quadr = aver_sq_find(signal_arr, test_info.amount);
                found++;   
            }
            

            channel.add_noise_arr(signal_arr, dev[out_co], test_info.amount);

            demodulated_arr = demodulator.demodulate_arr(signal_arr);
            misses          = demodulator.bit_check(arr_bit, demodulated_arr, test_info.amount);

            sum += misses;
        }

        ave[out_co] = sum/((double)max_co * (double)test_info.amount * (double)test_info.bit_in_word);
        SNR[out_co] = 20 * log((((double)test_info.amount) * aver_quadr) / (dev[out_co] * dev[out_co]));

        std::cout.setf(std::ios::fixed);
        std::cout.precision(2); //2 - число символов после точки
        std::cout << "dev = " << dev[out_co] << "   ave = " << ave[out_co] << std::endl;
    }



    std::string SNR_arr = file_ctor.double_array_to_string(SNR, points);
    
    file_ctor.add_lines_to_python_file(filename, SNR_text);
    file_ctor.add_lines_to_python_file(filename, modulation_name);
    file_ctor.add_lines_to_python_file(filename, np_arr_beg);
    file_ctor.add_lines_to_python_file(filename, SNR_arr);
    file_ctor.add_lines_to_python_file(filename, np_arr_end);
    

    std::string ave_arr = file_ctor.double_array_to_string(ave, points);
    
    file_ctor.add_lines_to_python_file(filename, missed_text);
    file_ctor.add_lines_to_python_file(filename, modulation_name);
    file_ctor.add_lines_to_python_file(filename, np_arr_beg);
    file_ctor.add_lines_to_python_file(filename, ave_arr);
    file_ctor.add_lines_to_python_file(filename, np_arr_end);

    free(demodulated_arr);
    free(signal_arr);
    free(arr_bit);
    free(dev);
    free(ave);
    free(SNR);
}
