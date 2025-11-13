#ifndef REGISTER_H
#define REGISTER_H

#include <systemc.h>
#include "Bus.h"

template<int N>
SC_MODULE(Register) {
    Bus* bus;

    void do_register() {
        if (bus->rst.read() == SC_LOGIC_1 || bus->acc_clear.read() == SC_LOGIC_1) {
            bus->regA_out.write(0);
        } else if (bus->clk.read() == SC_LOGIC_1 && bus->en.read() == SC_LOGIC_1) {
            bus->regA_out.write(bus->extractedA.read());
        }
    }

    SC_HAS_PROCESS(Register);

    Register(sc_module_name name, Bus& b) : sc_module(name), bus(&b) {
        SC_METHOD(do_register);
        sensitive << bus->clk;
    }
};

#endif