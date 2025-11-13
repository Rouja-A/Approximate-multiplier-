#include <systemc.h>

SC_MODULE(Counter4) {
    sc_in<sc_logic> clk, rst, en, clr;
    sc_out<sc_uint<4>> count;

    sc_uint<4> cnt;

    void do_count() {
        if (rst.read() == SC_LOGIC_1 || clr.read() == SC_LOGIC_1) {
            cnt = 0;
        } else if (clk.posedge() && en.read() == SC_LOGIC_1) {
            cnt = cnt + 1;
        }
        count.write(cnt);
    }

    SC_CTOR(Counter4) {
        SC_METHOD(do_count);
        sensitive << clk.pos();
    }
};
