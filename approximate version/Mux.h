#include <systemc.h>

template<int N>
SC_MODULE(Mux2to1) {
    sc_in<sc_uint<N>> in0, in1;
    sc_in<sc_logic> sel;
    sc_out<sc_uint<N>> out;

    void do_mux() {
        if (sel.read() == SC_LOGIC_1)
            out.write(in1.read());
        else
            out.write(in0.read());
    }

    SC_CTOR(Mux2to1) {
        SC_METHOD(do_mux);
        sensitive << in0 << in1 << sel;
    }
};
