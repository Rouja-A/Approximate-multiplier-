#ifndef DATAPATH_H
#define DATAPATH_H

#include <systemc.h>
#include "Adder.h"
#include "Counter.h"
#include "Mux.h"
#include "Register.h"
#include "Shifter.h"
#include "ShiftRegRight.h"
#include "1Detector.h"
#include "VBExtractor.h"

SC_MODULE(Datapath) {
    sc_in<sc_logic> clk, rst, clr;
    sc_in<sc_logic> load, en;
    sc_in<sc_logic> start;
    sc_in<sc_uint<16>> inA, inB;
    sc_out<sc_logic> b0;
    sc_out<sc_uint<32>> result;
    sc_out<sc_uint<4>> count;

    sc_signal<sc_uint<8>> regA_out, regB_out;
    sc_signal<sc_uint<8>> extractedA, extractedB;
    sc_signal<sc_uint<16>> shiftA_out, acc_out, acc_in, sum, inA_ext, inB_ext;
    sc_signal<sc_uint<32>> padded_result;
    sc_signal<sc_uint<4>> posA, posB, ignoredA, ignoredB, totalIgnored;

    Register<8>* regA;
    Register<16>* acc;
    ShifterLeft<16>* shiftA;
    Adder<16>* adder;
    Mux2to1<16>* muxAcc;
    Counter4* counter;
    ShiftRegisterRight<8>* regB;

    LeadingOneDetector* leadA;
    LeadingOneDetector* leadB;
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
    sc_uint<32> padded = acc_out.read();
    padded = padded << totalIgnored.read()+1;
    padded_result.write(padded);
    }

    void computeTotalIgnored() {
        totalIgnored.write(ignoredA.read() + ignoredB.read());
    }

    SC_CTOR(Datapath) {
        regA = new Register<8>("regA");
        acc = new Register<16>("acc");
        shiftA = new ShifterLeft<16>("shiftA");
        regB = new ShiftRegisterRight<8>("regB");
        adder = new Adder<16>("adder");
        muxAcc = new Mux2to1<16>("muxAcc");
        counter = new Counter4("counter");

        leadA = new LeadingOneDetector("leadA");
        leadA->in(inA);
        leadA->pos(posA);

        leadB = new LeadingOneDetector("leadB");
        leadB->in(inB);
        leadB->pos(posB);

        extractA = new ValuableBitsExtractor("extractA");
        extractA->op(inA);
        extractA->leading_pos(posA);
        extractA->extracted(extractedA);
        extractA->ignored_bits(ignoredA);

        extractB = new ValuableBitsExtractor("extractB");
        extractB->op(inB);
        extractB->leading_pos(posB);
        extractB->extracted(extractedB);
        extractB->ignored_bits(ignoredB);

        SC_METHOD(extendInA);
        sensitive << extractedA;

        SC_METHOD(extendInB);
        sensitive << extractedB;

        SC_METHOD(computeTotalIgnored);
        sensitive << ignoredA << ignoredB;

        SC_METHOD(extractB0);
        sensitive << regB_out;

        SC_METHOD(padZeros);
        sensitive << acc_out;

        SC_METHOD(assign_result);
        sensitive << acc_out << count;

        regA->clk(clk);
        regA->rst(rst);
        regA->en(load);
        regA->d(extractedA);
        regA->q(regA_out);
        regA->clr(clr);

        regB->clk(clk);
        regB->rst(rst);
        regB->clr(clr);
        regB->en(en);
        regB->load(load);
        regB->d(extractedB);
        regB->q(regB_out);

        shiftA->clk(clk);
        shiftA->rst(rst);
        shiftA->clr(clr);
        shiftA->en(en);
        shiftA->load(load);
        shiftA->q(shiftA_out);
        shiftA->d(inA_ext);

        adder->a(shiftA_out);
        adder->b(acc_out);
        adder->sum(sum);

        muxAcc->in0(acc_out);
        muxAcc->in1(sum);
        muxAcc->sel(b0);
        muxAcc->out(acc_in);

        acc->clk(clk);
        acc->rst(rst);
        acc->en(en);
        acc->d(acc_in);
        acc->q(acc_out);
        acc->clr(clr);

        counter->clk(clk);
        counter->rst(rst);
        counter->en(en);
        counter->count(count);
        counter->clr(clr);
    }
};

#endif