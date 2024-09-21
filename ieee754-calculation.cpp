#include <stdint.h>

#include <bitset>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

#define NUM_TESTS 10
#define MAX_VALUE 100
#define MIN_VALUE -100
uint8_t const table_width[] = {12, 12, 35, 12};

// IEEE 754 single-precision float constants
uint8_t const width = 32U;
uint8_t const exp_width = 8U;
uint8_t const mantissa_width = width - exp_width - 1;
uint8_t const bias = 127U;

/*
 * *** STUDENTS SHOULD WRITE CODE FOR THIS FUNCTION ***
 * Students should create or add any data structures needed.
 * Students should create or add any functions or classes they may need.
 */
float ieee_754(uint32_t const data)
{
    float value;

    const int width_exponent = 23;
    // Bias 1/2 x 2^8 -1 = 127
    const int bias_constant = 127;

    // Extract components from given data, total 32 bits
    // Bit 31 (1)
    uint32_t sign = (data >> width - 1) & 1;
    // Bits 30-23 (8)
    uint32_t exponent = (data >> width_exponent) & 0xFF;
    // Bits 22-0 (23)
    uint32_t mantissa = data & 0x007FFFFF;

    // Apply the bias
    int32_t actual_exponent = static_cast<int32_t>(exponent) - bias_constant;

    // Construct the value
    uint32_t biased_exponent = actual_exponent + bias_constant;
    uint32_t ieee_bits = (sign << width - 1) | (biased_exponent << width_exponent) | mantissa;

    // * dereferences the pointer, essentially copying the bit pattern into value
    value = *reinterpret_cast<float *>(&ieee_bits);

    return value;
}

/*
 * *** STUDENTS SHOULD NOT NEED TO CHANGE THE CODE BELOW. IT IS A CUSTOM TEST HARNESS. ***
 */

void header()
{
    cout << left << setw(table_width[0]) << setfill(' ') << "pass/fail";
    cout << left << setw(table_width[1]) << setfill(' ') << "value";
    cout << left << setw(table_width[2]) << setfill(' ') << "bits";
    cout << left << setw(table_width[3]) << setfill(' ') << "IEEE-754" << endl;

    cout << left << setw(table_width[0]) << setfill(' ') << "--------";
    cout << left << setw(table_width[1]) << setfill(' ') << "--------";
    cout << left << setw(table_width[2]) << setfill(' ') << "--------";
    cout << left << setw(table_width[3]) << setfill(' ') << "--------" << endl;
}

void print_row(bool const test_success, float const rand_val, uint32_t const val_int, float const ieee_754_value)
{
    // print results
    string const pass_fail = test_success ? "PASS" : "FAIL";
    cout << left << setw(table_width[0]) << setfill(' ') << pass_fail;
    cout << left << setw(table_width[1]) << setfill(' ') << rand_val;
    cout << left << setw(table_width[2]) << setfill(' ') << bitset<width>(val_int);
    cout << left << setw(table_width[3]) << setfill(' ') << ieee_754_value << endl;
}

template <typename T>
T rand_min_max(T const min, T const max)
{
    T const rand_val =
        min + static_cast<double>(static_cast<double>(rand())) / (static_cast<double>(RAND_MAX / (max - min)));
    return rand_val;
}

bool test()
{
    // the union
    union float_uint
    {
        float val_float;
        uint32_t val_int;
    } data;

    // print header
    header();

    // seed the random number generator
    srand(time(NULL));

    bool success = true;
    uint16_t pass = 0;
    for (size_t i = 0; i < NUM_TESTS; i++)
    {
        // random value
        float const rand_val = rand_min_max<float>(MIN_VALUE, MAX_VALUE);

        data.val_float = rand_val;

        // calculate using ieee_754 function
        float ieee_754_value = ieee_754(data.val_int);

        // test the results
        float const epsilon = std::numeric_limits<float>::epsilon();
        bool test_success = (abs(ieee_754_value - rand_val) < epsilon);
        if (test_success)
        {
            pass += 1;
        }

        // print row
        print_row(test_success, rand_val, data.val_int, ieee_754_value);
    }

    // summarize results
    cout << "-------------------------------------------" << endl;
    if (pass == NUM_TESTS)
    {
        cout << "SUCCESS ";
    }
    else
    {
        cout << "FAILURE ";
    }
    cout << pass << "/" << NUM_TESTS << " passed" << endl;
    cout << "-------------------------------------------" << endl;

    return success;
}

int main()
{
    if (!test())
    {
        return -1;
    }
    return 0;
}