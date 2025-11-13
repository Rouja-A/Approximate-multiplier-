#ifndef DATAPATH_H
#define DATAPATH_H

#include <systemc.h>

SC_MODULE(Datapath) {
    sc_in<sc_logic> clk, rst, clr;
    sc_in<sc_logic> enable;
    sc_in<sc_uint<16>> A, B;
    sc_out<sc_uint<32>> result;
    sc_out<sc_logic> done;

    sc_int<32> acc;
    sc_uint<17> B_ext;
    int i; 

    void process() {
        if (rst.read() == SC_LOGIC_1 || clr.read() == SC_LOGIC_1) {
            acc = 0;
            B_ext = 0;
            i = 0;
            result.write(0);
            done.write(SC_LOGIC_0);
        } else if (clk.posedge() && enable.read() == SC_LOGIC_1) {
            if (i == 0) {
                acc = 0;
                B_ext = (B.read() << 1);
                i = 0;
            }

            if (i < 8) { 
                sc_uint<3> recode_bits = (B_ext.range((i * 2) + 2, i * 2));
                sc_int<32> partial = 0;

                switch (recode_bits) {
                    case 0: 
                    case 7: 
                        partial = 0;
                        break;
                    case 1:
                    case 2:
                        partial = A.read();
                        break;
                    case 3: 
                        partial = A.read() << 1;
                        break;
                    case 4: 
                        partial = -(A.read() << 1);
                        break;
                    case 5: 
                    case 6: 
                        partial = -(A.read());
                        break;
                }

                acc += (partial << (i * 2));
                i++;
                done.write(SC_LOGIC_0);
            } else {
                result.write(static_cast<sc_uint<32>>(acc));
                done.write(SC_LOGIC_1);
                i = 0; 
            }
        }
    }

    SC_CTOR(Datapath) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }
};

#endif