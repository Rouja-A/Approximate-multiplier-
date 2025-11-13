#ifndef ADDER_H
#define ADDER_H

#include <systemc.h>
#include "Bus.h"

template<int N>
SC_MODULE(Adder) {
    Bus* bus;

    void do_add() {
        sc_uint<N> a = bus->add_a.read();
        sc_uint<N> b = bus->add_b.read();
        bus->sum.write(a + b);      
    }

    SC_CTOR(Adder) {
        SC_METHOD(do_add);
        sensitive << bus->inA << bus->inB;
    }

    Adder(sc_module_name name, Bus& b) : sc_module(name), bus(&b) {}
};

#endif