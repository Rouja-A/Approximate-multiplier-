#include <systemc.h>

template<int N>
SC_MODULE(Register) {
    sc_in<sc_logic> clk, rst, en, clr; // <-- added clr
    sc_in<sc_uint<N>> d;
    sc_out<sc_uint<N>> q;

    void do_register() {
        if (rst.read() == SC_LOGIC_1 || clr.read() == SC_LOGIC_1)
            q.write(0);
        else if (clk.posedge()) {
            if (en.read() == SC_LOGIC_1)
                q.write(d.read());
        }
    }

    SC_CTOR(Register) {
        SC_METHOD(do_register);
        sensitive << clk.pos();
    }
};

