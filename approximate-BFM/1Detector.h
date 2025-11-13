#ifndef LEADING_ONE_DETECTOR_H
#define LEADING_ONE_DETECTOR_H

#include <systemc.h>
#include "Bus.h"

SC_MODULE(LeadingOneDetector) {
    Bus* bus;
    sc_out<sc_uint<4>> pos; 

    void detect() {
        sc_uint<16> val = bus->inA.read();  
        for (int i = 15; i >= 0; --i) {
            if (val[i] == 1) {
                bus->posA(i);
                return;
            }
        }
        pos.write(0);  
    }

    SC_CTOR(LeadingOneDetector) {
        SC_METHOD(detect);
        sensitive << bus->inA; 
    }

    LeadingOneDetector(sc_module_name name, Bus& b) : sc_module(name), bus(&b) {}
};

#endif