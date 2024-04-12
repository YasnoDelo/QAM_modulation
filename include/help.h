#ifndef Help
#define Help

#include <iostream>
#include "../include/PythonFileCreator.h"
#include "../include/QAMChannel.h"
#include "../include/QAMDemodulator.h"
#include "../include/QAMModulator.h"

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

#define  QPSK_shft 1;
#define QAM16_shft 2;
#define QAM64_shft 3;

#define ACCUR 10000;    //Than more this number - than mor will be value on the graph
#define POINTS 90;      //Amount of points on the graph

void full_py_beg(PythonFileCreator& file_ctor, const std::string filename);

void full_py_end(PythonFileCreator& file_ctor, const std::string filename);

void test_modulation(QAMModulator& modulator, QAMDemodulator& demodulator, const std::string& modulation_name, PythonFileCreator& file_ctor, QAMChannel& channel, const std::string& filename);

#endif