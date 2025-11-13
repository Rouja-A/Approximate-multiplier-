#ifndef VAM16_TOP_H
#define VAM16_TOP_H

#include <systemc.h>
#include "vam16_bus.h"
#include "vam16_controller.h"
#include "vam16_datapath.h"

SC_MODULE(MultiplierTop) {
    sc_in<sc_logic> clk, rst, startB;
    sc_in<sc_uint<16>> inA, inB;
    sc_out<sc_uint<32>> outW;
    sc_out<sc_logic> readyB;

    sc_signal<sc_logic> ctrl_start_sig, ctrl_done_sig, dp_start_sig, dp_done_sig, clr;
    sc_signal<sc_logic> dp_enable_sig;
    sc_signal<sc_uint<16>> dp_A_sig, dp_B_sig;
    sc_signal<sc_uint<32>> dp_result_sig;

    Bus bus;
    Controller ctrl;
    Datapath dp;

    SC_CTOR(MultiplierTop)
        : bus("bus"), ctrl("ctrl"), dp("dp")
    {
        bus.clk(clk); bus.rst(rst); bus.startB(startB);
        bus.inA(inA); bus.inB(inB); bus.outW(outW); bus.readyB(readyB);

        bus.ctrl_start(ctrl_start_sig);
        bus.ctrl_done(ctrl_done_sig);
        bus.dp_enable(dp_enable_sig);
        bus.dp_A(dp_A_sig); bus.dp_B(dp_B_sig);
        bus.dp_result(dp_result_sig); bus.dp_done(dp_done_sig);

        ctrl.clk(clk); ctrl.rst(rst); ctrl.clr(clr);
        ctrl.start(ctrl_start_sig); ctrl.done(ctrl_done_sig);
        ctrl.dp_start(dp_start_sig); ctrl.dp_done(dp_done_sig);

        dp.clk(clk); dp.rst(rst); dp.enable(dp_enable_sig);
        dp.A(dp_A_sig); dp.B(dp_B_sig); dp.clr(clr);
        dp.result(dp_result_sig); dp.done(dp_done_sig);
    }
};

#endif