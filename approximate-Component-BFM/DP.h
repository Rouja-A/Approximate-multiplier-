#pragma once
#include <systemc.h>
#include "Bus.h"

SC_MODULE(Datapath) {
    Bus* bus;
    sc_in<sc_logic> clk;
    sc_out<bool> done_dp;

    void datapath_proc() {
        while (true) {
            wait();

            if (bus->compute.read()) {
                sc_uint<16> A = bus->A.read();
                sc_uint<16> B = bus->B.read();

                int first1A = -1, first1B = -1;
                for (int i = 15; i >= 0; --i) if (A[i]) { first1A = i; break; }
                for (int i = 15; i >= 0; --i) if (B[i]) { first1B = i; break; }

                int startA = std::max(0, first1A - 7);
                int startB = std::max(0, first1B - 7);

                bus->ignoreA.write(startA);
                bus->ignoreB.write(startB);

                sc_uint<8> A8 = 0, B8 = 0;
                for (int i = 0; i < 8; ++i) {
                    if (startA + i < 16) A8[i] = A[startA + i];
                    if (startB + i < 16) B8[i] = B[startB + i];
                }

                sc_uint<16> raw = A8 * B8;
                int totalShift = startA + startB;
                sc_uint<32> shifted = raw;
                shifted = shifted << totalShift;

                bus->result.write(shifted);
                done_dp.write(true);
            }
        }
    }

    SC_CTOR(Datapath) {
        SC_THREAD(datapath_proc);
        sensitive << clk.pos();
    }
};