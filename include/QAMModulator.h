#ifndef QAM_MODULATOR_H
#define QAM_MODULATOR_H

typedef struct signal
{
    double in_phase   = 0;
    double quadrature = 0;
} Signal;

typedef struct test_info
{
    int amount      = 0;
    int bit_in_word = 0;
} TestInfo;

class QAMModulator {
public:
    enum class ModulationType {
        QPSK,
        QAM16,
        QAM64
    };

    QAMModulator(ModulationType modulation_type);
    void modulate(Signal& signal, int bit);
    void print_map();
    TestInfo rand_bits_modulate(int*& bit_arr, Signal*& signal_arr);

private:
    ModulationType modulation_type;
    void modulation_internal(double& quadrature, double& in_phase, int mask, int bit, int max_bits);
    int powneg1(int pow);
    int pow2(int pow);
    int sgn(int val);
    int get_amount();
    int get_max_bit();
    int get_max_shift();
    void bin_print(int value);
    int random_bit(int min, int max);
};

#endif // QAM_MODULATOR_H
