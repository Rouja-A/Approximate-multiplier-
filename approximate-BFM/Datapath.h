#ifndef DATAPATH_H
#define DATAPATH_H

#include <systemc.h>
#include "Adder.h"
#include "Counter.h"
#include "Mux.h"
#include "Shifter.h"
#include "ShiftRegRight.h"
#include "VBExtractor.h"
#include "Register.h"
#include "Bus.h"

SC_MODULE(Datapath) {
    sc_in<sc_logic> clk, rst, clr;
    sc_in<sc_logic> load, en;
    sc_in<sc_logic> start;
    sc_in<sc_uint<16>> inA, inB;
    
    sc_out<sc_logic> b0;
    sc_out<sc_uint<32>> result;
    sc_out<sc_uint<4>> count;

    sc_signal<sc_uint<16>> s_acc_in, s_shiftA_out;
    sc_signal<sc_uint<32>> s_acc_out;
    sc_signal<sc_logic> s_sel;
    sc_signal<sc_uint<4>> s_posA, s_posB, s_ignoredA, s_ignoredB;
    sc_signal<sc_uint<4>> totalIgnored;
    sc_signal<sc_uint<8>> regA_out, regB_out, extractedA, extractedB;
    sc_signal<sc_uint<16>> inA_ext, inB_ext, sum;
    sc_signal<sc_uint<32>> padded_result;

    Bus bus_regA, bus_regB, bus_acc, bus_adder, bus_mux;
    Bus bus_shiftA, bus_counter, bus_leadA, bus_leadB;
    Bus bus_extractA, bus_extractB;

    Register<8>* regA;
    Register<16>* acc;
    ShiftRegisterRight<8>* regB;
    ShifterLeft<16>* shiftA;
    Adder<16>* adder;
    Mux2to1<16>* muxAcc;
    Counter4* counter;
    ValuableBitsExtractor* extractA;
    ValuableBitsExtractor* extractB;

    void extendInA() {
        sc_uint<16> extended = 0;
        extended.range(7, 0) = extractedA.read();
        inA_ext.write(extended);
    }

    void extendInB() {
        sc_uint<16> extended = 0;
        extended.range(7, 0) = extractedB.read();
        inB_ext.write(extended);
    }

    void extractB0() {
        b0.write((regB_out.read()[0]) ? SC_LOGIC_1 : SC_LOGIC_0);
    }

    void assign_result() {
        result.write(padded_result.read());
    }

    void padZeros() {
        sc_uint<32> padded = s_acc_out.read();
        padded = padded << (totalIgnored.read() + 1);
        padded_result.write(padded);
    }

    void computeTotalIgnored() {
        totalIgnored.write(s_ignoredA.read() + s_ignoredB.read());
    }


    SC_CTOR(Datapath) {
        bus_regA.clk(clk); bus_regA.rst(rst); bus_regA.load(load);
        bus_regA.en(en); bus_regA.acc_clear(clr);
        bus_regA.extractedA(extractedA); bus_regA.regA_out(regA_out);

        bus_regB.clk(clk); bus_regB.rst(rst); bus_regB.load(load);
        bus_regB.en(en); bus_regB.acc_clear(clr);
        bus_regB.extractedA(extractedB); bus_regB.regA_out(regB_out);

        bus_acc.clk(clk); bus_acc.rst(rst); bus_acc.en(en);
        bus_acc.load(load); bus_acc.acc_clear(clr);
        bus_acc.extractedA16(s_acc_in); bus_acc.acc_out(s_acc_out);

        bus_adder.add_a(s_shiftA_out); bus_adder.add_b(s_acc_out);
        bus_adder.sum(sum);

        bus_mux.add_b(s_acc_out); bus_mux.inB(sum);
        bus_mux.sel(b0); bus_mux.outW(s_acc_in);

        bus_shiftA.clk(clk); bus_shiftA.rst(rst); bus_shiftA.en(en);
        bus_shiftA.load(load); bus_shiftA.acc_clear(clr);
        bus_shiftA.inA(inA_ext); bus_shiftA.outW(s_shiftA_out);

        bus_counter.clk(clk); bus_counter.rst(rst); bus_counter.en(en);
        bus_counter.load(load); bus_counter.acc_clear(clr);
        bus_counter.inA(inA); bus_counter.count(count);

        bus_extractA.inA(inA); bus_extractA.posA(s_posA);
        bus_extractA.extractedB(extractedA); bus_extractA.ignored_bits(s_ignoredA);

        bus_extractB.inA(inB); bus_extractB.posB(s_posB);
        bus_extractB.extractedB(extractedB); bus_extractB.ignored_bits(s_ignoredB);

        regA = new Register<8>("regA", bus_regA);
        regB = new ShiftRegisterRight<8>("regB", bus_regB);
        acc = new Register<16>("acc", bus_acc);
        shiftA = new ShifterLeft<16>("shiftA", bus_shiftA);
        adder = new Adder<16>("adder", bus_adder);
        muxAcc = new Mux2to1<16>("muxAcc", bus_mux);
        counter = new Counter4("counter", bus_counter);
        extractA = new ValuableBitsExtractor("extractA", bus_extractA);
        extractB = new ValuableBitsExtractor("extractB", bus_extractB);

        SC_METHOD(extendInA); sensitive << extractedA;
        SC_METHOD(extendInB); sensitive << extractedB;
        SC_METHOD(computeTotalIgnored); sensitive << s_ignoredA << s_ignoredB;
        SC_METHOD(extractB0); sensitive << regB_out;
        SC_METHOD(padZeros); sensitive << s_acc_out << totalIgnored;
        SC_METHOD(assign_result); sensitive << padded_result;
        SC_METHOD(debug_outputs); sensitive << clk.pos();
        bus_regA.bind_dummy_outputs();
        bus_regB.bind_dummy_outputs();
        bus_acc.bind_dummy_outputs();
        bus_adder.bind_dummy_outputs();
        bus_mux.bind_dummy_outputs();
        bus_shiftA.bind_dummy_outputs();
        bus_counter.bind_dummy_outputs();
        bus_extractA.bind_dummy_outputs();
        bus_extractB.bind_dummy_outputs();
        bus_leadA.bind_dummy_outputs();
bus_leadB.bind_dummy_outputs();


    }
};

#endif
