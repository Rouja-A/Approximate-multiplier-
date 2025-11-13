#ifndef SHIFTERLEFT_H
#define SHIFTERLEFT_H

#include <systemc.h>
#include "Bus.h"

template<int N>
SC_MODULE(ShifterLeft) {
    Bus* bus;
    sc_uint<N> reg;

    void do_shiftreg() {
        if (bus->rst.read() == SC_LOGIC_1 || bus->acc_clear.read() == SC_LOGIC_1) {
            reg = 0;
        } else if (bus->clk.read() == SC_LOGIC_1) {
            if (bus->en.read() == SC_LOGIC_1) {
                if (bus->load.read() == SC_LOGIC_1)
                    reg = bus->inA.read();
                else
                    reg = reg << 1;
            }
        }
        bus->outW.write(reg); 
    }

    SC_CTOR(ShifterLeft) {
        SC_METHOD(do_shiftreg);
        sensitive << bus->clk; 
    }

    ShifterLeft(sc_module_name name, Bus& b) : sc_module(name), bus(&b), reg(0) {}
};

#endif