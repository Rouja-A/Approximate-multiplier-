#ifndef BUS_H
#define BUS_H

#include <systemc.h>

class Bus {
public:
    sc_in<sc_logic> clk, rst, clr, sel, acc_clear;
    sc_in<sc_logic> load, en;
    sc_in<sc_uint<4>> posA, posB;
    sc_in<sc_uint<16>> add_a, inA, inB, acc_in, extractedA16;
    sc_in<sc_uint<32>> add_b;
    sc_out<sc_uint<32>> result, acc_out;
    sc_out<sc_uint<4>> count, ignored_bits;
    sc_out<sc_uint<16>> sum, outW;
    sc_out<sc_uint<8>> extractedB;

    sc_out<sc_uint<8>> regA_out;
    sc_in<sc_uint<8>> extractedA;
    sc_signal<sc_uint<16>> shiftA_out, inA_ext, inB_ext;
    sc_signal<sc_uint<32>> padded_result;
    sc_signal<sc_uint<4>> ignoredA, ignoredB, totalIgnored;

    Bus() {
        reset_outputs();
    }

    void reset_outputs() {
        if (result.get_interface()) result.write(0);
        if (acc_out.get_interface()) acc_out.write(0);
        if (count.get_interface()) count.write(0);
        if (ignored_bits.get_interface()) ignored_bits.write(0);
        if (sum.get_interface()) sum.write(0);
        if (outW.get_interface()) outW.write(0);
        if (extractedB.get_interface()) extractedB.write(0);
        if (regA_out.get_interface()) regA_out.write(0);

        shiftA_out = 0;
        inA_ext = 0;
        inB_ext = 0;
        padded_result = 0;
        ignoredA = 0;
        ignoredB = 0;
        totalIgnored = 0;
    }

    sc_uint<32> read_result() {
        return result.read();
    }

    void bind_dummy_outputs() {
    static sc_signal<sc_uint<32>> dummy32;
    static sc_signal<sc_uint<16>> dummy16;
    static sc_signal<sc_uint<8>> dummy8;
    static sc_signal<sc_uint<4>> dummy4;

    if (!result.get_interface()) result(dummy32);
    if (!acc_out.get_interface()) acc_out(dummy32);
    if (!count.get_interface()) count(dummy4);
    if (!ignored_bits.get_interface()) ignored_bits(dummy4);
    if (!sum.get_interface()) sum(dummy16);
    if (!outW.get_interface()) outW(dummy16);
    if (!extractedB.get_interface()) extractedB(dummy8);
    if (!regA_out.get_interface()) regA_out(dummy8);
}

};

#endif