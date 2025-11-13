#pragma once
#include <systemc.h>

struct Bus {
    sc_signal<sc_uint<16>> A, B;
    sc_signal<sc_uint<8>> A8, B8;
    sc_signal<int> ignoreA, ignoreB;
    sc_signal<sc_uint<32>> result;
    sc_signal<bool> compute;
};