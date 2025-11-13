#ifndef MULTIPLIERTOP_H
#define MULTIPLIERTOP_H

#include <systemc.h>
#include "Datapath.h"
#include "Controller.h"

SC_MODULE(MultiplierTop) {
    sc_in<sc_logic> clk, rst, startB;
    sc_in<sc_uint<16>> inA, inB;
    sc_out<sc_uint<32>> outW;
    sc_out<sc_logic> readyB;

    sc_signal<sc_logic> load, en, b0;
    sc_signal<sc_uint<4>> count;
    sc_signal<sc_uint<32>> datapath_result; 
    sc_signal<sc_logic> acc_clear;

    BusCTRL* bus;
    Datapath *dp;
    Controller *ctrl;
    
    void assign_output() {
        outW.write(datapath_result.read());
    }

    SC_CTOR(MultiplierTop) {
        bus = new BusCTRL();
        dp = new Datapath("datapath");
        ctrl = new Controller("controller", *bus);
        bus->bind_dummy_outputs();
        dp->clk(clk); dp->rst(rst);
        dp->inA(inA); dp->inB(inB);
        dp->start(startB);
        dp->load(load); dp->en(en);
        dp->count(count);
        dp->b0(b0);

        dp->result(datapath_result);
        dp->clr(acc_clear);
        bus->acc_clear(acc_clear);
        bus->clk(clk); bus->rst(rst); bus->startB(startB);
        bus->load(load); bus->en(en); bus->readyB(readyB);
        bus->count(count);
        
        SC_METHOD(assign_output);
        sensitive << datapath_result;
    }
};

#endif