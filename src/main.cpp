#include "../include/QAMModulator.h"
#include "../include/QAMDemodulator.h"
#include "../include/QAMChannel.h"
#include "../include/PythonFileCreator.h"
#include "../include/help.h"
#include "../include/words.h"

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

    full_py_end(file_ctor, filename);

    system("python3 output.py");

    return 0;
}
