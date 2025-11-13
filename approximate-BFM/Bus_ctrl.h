
#ifndef BUSCTRL_H
#define BUSCTRL_H

#include <systemc.h>

class BusCTRL {
public:
    sc_in<sc_logic> clk, rst, startB;
    sc_in<sc_uint<4>> count;
    sc_out<sc_logic> load, en, readyB, acc_clear;


    BusCTRL() {
        load = SC_LOGIC_0;
        en = SC_LOGIC_0;
        readyB = SC_LOGIC_0;
        acc_clear = SC_LOGIC_0;
    }

    void bind_dummy_outputs() {
        static sc_signal<sc_logic> dummy_logic;
        static sc_signal<sc_uint<4>> dummy_4;
        static sc_signal<sc_uint<32>> dummy_32;

        if (!load.get_interface()) load(dummy_logic);
        if (!en.get_interface()) en(dummy_logic);
        if (!readyB.get_interface()) readyB(dummy_logic);
        if (!acc_clear.get_interface()) acc_clear(dummy_logic);
        if (!count.get_interface()) count(dummy_4);
    }
};

#endif