#include <systemc.h>

template<int N>
SC_MODULE(ShifterLeft) {
    sc_in<sc_logic> clk, rst, clr, en, load;
    sc_in<sc_uint<N>> d;
    sc_out<sc_uint<N>> q;

    sc_uint<N> reg;

    void do_shiftreg() {
    if (rst.read() == SC_LOGIC_1 || clr.read() == SC_LOGIC_1) {
        reg = 0;
    } else if (clk.posedge()) {
        if (en.read() == SC_LOGIC_1) {
            if (load.read() == SC_LOGIC_1)
                reg = d.read();   
            else
                reg = reg << 1;    
        }
    }
    q.write(reg);
}

    SC_CTOR(ShifterLeft) {
        SC_METHOD(do_shiftreg);
        sensitive << clk.pos();
    }
};
