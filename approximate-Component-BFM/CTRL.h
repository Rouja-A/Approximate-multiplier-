#pragma once
#include <systemc.h>
#include "Bus.h"

SC_MODULE(Controller) {
    sc_in<sc_logic> clk, rst, startA;
    sc_in<bool> dp_done;
    sc_out<sc_logic> readyA;
    sc_out<sc_uint<32>> multW;
    sc_in<sc_uint<16>> opndA, opndB;
    Bus* bus;

    enum State { IDLE, LOAD, WAIT1, WAIT2, DONE };
    State state;

    void fsm_proc() {
        readyA.write(SC_LOGIC_0);
        bus->compute.write(false);
        state = IDLE;
        wait();

        while (true) {
            switch (state) {
                case IDLE:
                    readyA.write(SC_LOGIC_0);
                    if (startA.read() == SC_LOGIC_1) {
                        bus->A.write(opndA.read());
                        bus->B.write(opndB.read());
                        state = LOAD;
                    }
                    break;

                case LOAD:
                    bus->compute.write(true);
                    state = WAIT1;
                    break;

                case WAIT1:
                    bus->compute.write(false);
                    state = WAIT2;
                    break;

                case WAIT2:
                    state = DONE;
                    break;

                case DONE:
                    if (dp_done.read()) {
                        multW.write(bus->result.read());
                        readyA.write(SC_LOGIC_1);
                        state = IDLE;
                    }
                    break;
            }
            wait();
        }
    }

    SC_CTOR(Controller) {
        SC_CTHREAD(fsm_proc, clk.pos());
    }
};