#include <systemc.h>

SC_MODULE(Controller) {
    sc_in<sc_logic> clk, rst, start;
    sc_in<sc_uint<4>> count;
    sc_out<sc_logic> load, en, ready, acc_clear;

    enum State { IDLE, LOAD, RUN, DONE };
    sc_signal<State> state;

    void fsm() {
        if (rst.read() == SC_LOGIC_1) {
            state = IDLE;
            load.write(SC_LOGIC_0);
            en.write(SC_LOGIC_0);
            ready.write(SC_LOGIC_0);
        } else if (clk.posedge()) {
            switch (state.read()) {
                case IDLE:
                load.write(SC_LOGIC_0);
                en.write(SC_LOGIC_0);
                ready.write(SC_LOGIC_0);
                if (start.read() == SC_LOGIC_1) {
                    load.write(SC_LOGIC_1);
                    en.write(SC_LOGIC_1);
                    acc_clear.write(SC_LOGIC_0); 
                    state = LOAD;
                } else {
                    acc_clear.write(SC_LOGIC_1); 
                }
                break;
                case LOAD:
                    load.write(SC_LOGIC_0);
                    en.write(SC_LOGIC_1);
                    state = RUN;
                    break;
                case RUN:
                std::cout << "Counter value: " << count.read() << std::endl;
                    if (count.read() == 8) {
                        en.write(SC_LOGIC_0);
                        ready.write(SC_LOGIC_1);
                        state = DONE;
                    }
                    break;
                case DONE:
                    ready.write(SC_LOGIC_0);
                    state = IDLE;
                    break;
            }
        }
    }

    SC_CTOR(Controller) {
        SC_METHOD(fsm);
        sensitive << clk.pos();
    }
};
