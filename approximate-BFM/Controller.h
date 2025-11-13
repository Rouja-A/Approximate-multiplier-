#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <systemc.h>
#include "Bus_ctrl.h"

SC_MODULE(Controller) {
    BusCTRL* bus;

    enum State { IDLE, LOAD, RUN, DONE };
    State state;

    void fsm() {
        if (bus->rst.read() == SC_LOGIC_1) {
            state = IDLE;
            bus->load.write(SC_LOGIC_0);
            bus->en.write(SC_LOGIC_0);
            bus->readyB.write(SC_LOGIC_0);
            bus->acc_clear.write(SC_LOGIC_1); 
        } 
        else if (bus->clk.read() == SC_LOGIC_1) {
            switch (state) {
                case IDLE:
                    bus->load.write(SC_LOGIC_0);
                    bus->en.write(SC_LOGIC_0);
                    bus->readyB.write(SC_LOGIC_0);
                    if (bus->startB.read() == SC_LOGIC_1) {
                        bus->load.write(SC_LOGIC_1);
                        bus->en.write(SC_LOGIC_1);
                        bus->acc_clear.write(SC_LOGIC_0);  
                        state = LOAD;
                    } else {
                        bus->acc_clear.write(SC_LOGIC_1); 
                    }
                    break;

                case LOAD:
                    bus->load.write(SC_LOGIC_0); 
                    bus->en.write(SC_LOGIC_1);
                    state = RUN;
                    break;

                case RUN:
                    if (bus->count.read() == 8) {  
                        bus->en.write(SC_LOGIC_0);
                        bus->readyB.write(SC_LOGIC_1); 
                        state = DONE;
                    }
                    break;

                case DONE:
                    bus->readyB.write(SC_LOGIC_0); 
                    state = IDLE; 
                    break;
            }
        }
    }

    SC_CTOR(Controller) {
        SC_METHOD(fsm);
        sensitive << bus->clk; 
        dont_initialize(); 
        state = IDLE; 
    }

    Controller(sc_module_name name, BusCTRL& b) : sc_module(name), bus(&b), state(IDLE) {}
};

#endif
