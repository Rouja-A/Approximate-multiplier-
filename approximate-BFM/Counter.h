#ifndef COUNTER4_H
#define COUNTER4_H

#include <systemc.h>
#include "Bus.h"

SC_MODULE(Counter4) {
    Bus* bus;
    sc_uint<4> cnt;

    void do_count() {
        if (bus->rst.read() == SC_LOGIC_1 || bus->acc_clear.read() == SC_LOGIC_1) {
            cnt = 0;
        } else if (bus->clk.read() == SC_LOGIC_1 && bus->en.read() == SC_LOGIC_1) {
            cnt++;
        }
        bus->count.write(cnt);
    }

    SC_CTOR(Counter4) {
        SC_METHOD(do_count);
        sensitive << bus->clk;
        dont_initialize();
    }

    Counter4(sc_module_name name, Bus& b) : sc_module(name), bus(&b), cnt(0) {}
};

#endif