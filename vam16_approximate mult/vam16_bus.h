#ifndef BUS_H
#define BUS_H

#include <systemc.h>

SC_MODULE(Bus) {
    sc_in<sc_logic> clk, rst, startB;
    sc_in<sc_uint<16>> inA, inB;
    sc_out<sc_uint<32>> outW;
    sc_out<sc_logic> readyB;

    sc_out<sc_logic> ctrl_start;
    sc_in<sc_logic> ctrl_done;
    sc_out<sc_logic> dp_enable;
    sc_out<sc_uint<16>> dp_A, dp_B;
    sc_in<sc_uint<32>> dp_result;
    sc_in<sc_logic> dp_done;

    sc_signal<sc_uint<16>> latched_A, latched_B;
    sc_signal<bool> inputs_latched;

    void bus_proc() {
        if (rst.read() == SC_LOGIC_1) {
            ctrl_start.write(SC_LOGIC_0);
            dp_enable.write(SC_LOGIC_0);
            readyB.write(SC_LOGIC_0);
            inputs_latched = false;
        } else if (clk.posedge()) {
            if (startB.read() == SC_LOGIC_1 && !inputs_latched) {
                latched_A.write(inA.read());
                latched_B.write(inB.read());
                dp_A.write(inA.read());
                dp_B.write(inB.read());
                ctrl_start.write(SC_LOGIC_1);
                dp_enable.write(SC_LOGIC_1);
                readyB.write(SC_LOGIC_0);
                inputs_latched = true;
            } else if (dp_done.read() == SC_LOGIC_1) {
                outW.write(dp_result.read());
                readyB.write(SC_LOGIC_1);
                ctrl_start.write(SC_LOGIC_0);
                dp_enable.write(SC_LOGIC_0);
                inputs_latched = false;
            } else {
                dp_A.write(latched_A.read());
                dp_B.write(latched_B.read());
                ctrl_start.write(SC_LOGIC_0);
                if (dp_done.read() != SC_LOGIC_1) { 
                    dp_enable.write(SC_LOGIC_1);
                } else {
                    dp_enable.write(SC_LOGIC_0);
                }
            }
        }
    }

    SC_CTOR(Bus) {
        SC_METHOD(bus_proc);
        sensitive << clk.pos();
    }
};

#endif