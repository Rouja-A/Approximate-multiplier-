#ifndef LEADING_ONE_DETECTOR_H
#define LEADING_ONE_DETECTOR_H

#include <systemc.h>

SC_MODULE(LeadingOneDetector) {
    sc_in<sc_uint<16>> in;
    sc_out<sc_uint<4>> pos; 

    void detect() {
        sc_uint<16> val = in.read();
        for (int i = 15; i >= 0; --i) {
            if (val[i] == 1) {
                pos.write(i);
                return;
            }
        }
        pos.write(0); 
    }

    SC_CTOR(LeadingOneDetector) {
        SC_METHOD(detect);
        sensitive << in;
    }
};

#endif
