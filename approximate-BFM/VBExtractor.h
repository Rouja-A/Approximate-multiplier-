#ifndef VALUABLE_BITS_EXTRACTOR_H
#define VALUABLE_BITS_EXTRACTOR_H

#include <systemc.h>
#include "Bus.h"

SC_MODULE(ValuableBitsExtractor) {
    Bus* bus;

    void extract() {
        sc_uint<16> operand = bus->inA.read(); 
        sc_uint<4> pos = bus->count.read();
        sc_uint<8> result;

        if (pos < 8) {
            result = operand.range(7, 0);
            bus->ignored_bits.write(0);
        } else {
            int high = pos;
            int low = pos - 7;
            result = operand.range(high, low);
            bus->ignored_bits.write(low);
        }
        bus->extractedB.write(result); 
    }

    SC_CTOR(ValuableBitsExtractor) {
        SC_METHOD(extract);
        sensitive << bus->inA << bus->count;  
    }

    ValuableBitsExtractor(sc_module_name name, Bus& b) : sc_module(name), bus(&b) {}
};

#endif