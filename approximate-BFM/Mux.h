#ifndef MUX2TO1_H
#define MUX2TO1_H

#include <systemc.h>
#include "Bus.h"

template<int N>
SC_MODULE(Mux2to1) {
    Bus* bus;

    void do_mux() {
        if (bus->sel.read() == SC_LOGIC_1)
            bus->outW.write(bus->inB.read()); 
        else
            bus->outW.write(bus->add_b.read());
    }

    SC_CTOR(Mux2to1) {
        SC_METHOD(do_mux);
        sensitive << bus->inA << bus->inB << bus->sel; 
    }

    Mux2to1(sc_module_name name, Bus& b) : sc_module(name), bus(&b) {}
};

#endif