#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <systemc.h>

SC_MODULE(Controller) {
    sc_in<sc_logic> clk;
    sc_in<sc_logic> rst;
    sc_in<sc_logic> start;
    sc_in<sc_logic> dp_done;
    sc_out<sc_logic> done, clr;
    sc_out<sc_logic> dp_start;

    enum State {IDLE, START, WAIT, FINISH};
    sc_signal<State> state, next_state;

    void fsm_transition() {
        if (rst.read() == SC_LOGIC_1) {
            state = IDLE;
        } else {
            state = next_state;
        }
    }

    void fsm_output() {
        switch (state.read()) {
            case IDLE:
                clr.write(SC_LOGIC_1);
                done.write(SC_LOGIC_0);
                dp_start.write(SC_LOGIC_0);
                if (start.read() == SC_LOGIC_1) {
                    next_state = START;
                } else {
                    next_state = IDLE;
                }
                break;

            case START:
                clr.write(SC_LOGIC_0);
                done.write(SC_LOGIC_0);
                dp_start.write(SC_LOGIC_1);
                next_state = WAIT;
                break;

            case WAIT:
                clr.write(SC_LOGIC_0);
                done.write(SC_LOGIC_0);
                dp_start.write(SC_LOGIC_0);
                if (dp_done.read() == SC_LOGIC_1) {
                    next_state = FINISH;
                } else {
                    next_state = WAIT;
                }
                break;

            case FINISH:
                clr.write(SC_LOGIC_0);
                done.write(SC_LOGIC_1);
                dp_start.write(SC_LOGIC_0);
                next_state = IDLE;
                break;

            default:
                next_state = IDLE;
                clr.write(SC_LOGIC_1);
                done.write(SC_LOGIC_0);
                dp_start.write(SC_LOGIC_0);
                break;
        }
    }

    SC_CTOR(Controller) {
        SC_METHOD(fsm_transition);
        sensitive << clk.pos();
        SC_METHOD(fsm_output);
        sensitive << state << start << dp_done;
    }
};

#endif