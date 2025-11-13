#pragma once
#include <systemc.h>
#include "Bus.h"
#include "DP.h"
#include "CTRL.h"

SC_MODULE(MultiplierTop) {
    sc_in<sc_logic> clk, rst, startB;
    sc_in<sc_uint<16>> inA, inB;
    sc_out<sc_uint<32>> outW;
    sc_out<sc_logic> readyB;
    sc_signal<bool> dp_done;

    Bus* bus;
    Controller* controller;
    Datapath* datapath;

    SC_CTOR(MultiplierTop) {
        bus = new Bus();

        controller = new Controller("Controller");
        controller->clk(clk);
        controller->rst(rst);
        controller->startA(startB);
        controller->readyA(readyB);
        controller->opndA(inA);
        controller->opndB(inB);
        controller->multW(outW);
        controller->bus = bus;
        
        controller->dp_done(dp_done);

        datapath = new Datapath("Datapath");
        datapath->done_dp(dp_done);
        datapath->clk(clk);
        datapath->bus = bus;
    }

    ~MultiplierTop() {
        delete controller;
        delete datapath;
        delete bus;
    }
};
