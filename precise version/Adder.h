#include <systemc.h>

template<int N>
SC_MODULE(Adder) {
    sc_in<sc_uint<N>> a, b;
    sc_out<sc_uint<N>> sum;

    void do_add() {
        sum.write(a.read() + b.read());
    }

    SC_CTOR(Adder) {
        SC_METHOD(do_add);
        sensitive << a << b;
    }
};
