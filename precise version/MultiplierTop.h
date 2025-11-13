#ifndef MULTIPLIERTOP_H
#define MULTIPLIERTOP_H

#include <systemc.h>
#include "Datapath.h"
#include "Controller.h"

SC_MODULE(MultiplierTop) {
    sc_in<sc_logic> clk, rst, startB;
    sc_in<sc_uint<8>> inA, inB;
    sc_out<sc_uint<16>> outW;
    sc_out<sc_logic> readyB;

    sc_signal<sc_logic> load, en, b0;
    sc_signal<sc_uint<4>> count;
    sc_signal<sc_uint<16>> datapath_result; 
    sc_signal<sc_logic> acc_clear;

    Datapath *dp;
    Controller *ctrl;

    void assign_output() {
        outW.write(datapath_result.read());
    }

    SC_CTOR(MultiplierTop) {
        dp = new Datapath("datapath");
        ctrl = new Controller("controller");

        dp->clk(clk); dp->rst(rst);
        dp->inA(inA); dp->inB(inB);
        dp->start(startB);
        dp->load(load); dp->en(en);
        dp->b0(b0);
        dp->count(count);

        dp->result(datapath_result);
        dp->clr(acc_clear);
        ctrl->acc_clear(acc_clear);
        ctrl->clk(clk); ctrl->rst(rst); ctrl->start(startB);
        ctrl->load(load); ctrl->en(en); ctrl->ready(readyB);
        ctrl->count(count);
        
        SC_METHOD(assign_output);
        sensitive << datapath_result;
    }
};

#endif