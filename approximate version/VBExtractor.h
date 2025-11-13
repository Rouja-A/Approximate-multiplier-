#ifndef VALUABLE_BITS_EXTRACTOR_H
#define VALUABLE_BITS_EXTRACTOR_H

#include <systemc.h>

SC_MODULE(ValuableBitsExtractor) {
    sc_in<sc_uint<16>> op;
    sc_in<sc_uint<4>> leading_pos;
    sc_out<sc_uint<8>> extracted;
    sc_out<sc_uint<4>> ignored_bits;

    void extract() {
        sc_uint<16> operand = op.read();
        sc_uint<4> pos = leading_pos.read();
        sc_uint<8> result;

        if (pos < 8) {
            result = operand.range(7, 0);
            ignored_bits.write(0);
        } else {
            int high = pos;
            int low = pos - 7;
            result = operand.range(high, low);
            ignored_bits.write(low);
        }
        extracted.write(result);
    }

    SC_CTOR(ValuableBitsExtractor) {
        SC_METHOD(extract);
        sensitive << op << leading_pos;
    }
};

#endif
