#include <systemc.h>
#include "vam16_top.h"
using namespace sc_dt;
using namespace sc_core;

int sc_main(int argc, char* argv[]) {
    sc_signal<sc_logic> clk, rst, startB, readyB;
    sc_signal<sc_uint<16>> inA, inB;
    sc_signal<sc_uint<32>> outW;

    MultiplierTop dut("Multiplier");
    dut.clk(clk); dut.rst(rst); dut.startB(startB);
    dut.inA(inA); dut.inB(inB);
    dut.outW(outW); dut.readyB(readyB);

    sc_time period(10, SC_NS);
    auto toggle_clk = [&]() {
        clk.write(SC_LOGIC_0);
        sc_start(period / 2);
        clk.write(SC_LOGIC_1);
        sc_start(period / 2);
    };

    std::cout << "Applying reset..." << std::endl;
    rst.write(SC_LOGIC_1);
    toggle_clk();
    rst.write(SC_LOGIC_0);
    toggle_clk();
    std::cout << "Reset complete." << std::endl;

    struct TestCase {
        sc_uint<16> A;
        sc_uint<16> B;
        sc_uint<16> expected;
    };

    TestCase test_cases[] = {
        {2, 3, 6},   
        {14, 15, 210}, 
        {0, 0, 0},     
        {312, 255, 65025} 
    };

    for (const auto& test : test_cases) {
        std::cout << "Testing A = " << test.A << ", B = " << test.B << std::endl;

        inA.write(test.A);
        inB.write(test.B);
        startB.write(SC_LOGIC_1);
        toggle_clk();
        startB.write(SC_LOGIC_0);

        while (readyB.read() != SC_LOGIC_1) {
            toggle_clk();
        }

        sc_uint<32> result = outW.read();
        std::cout << "Result: " << result << ", Expected: " << test.expected << std::endl;

        if (result == test.expected) {
            std::cout << "Test passed." << std::endl;
        } else {
            std::cout << "Test failed!" << std::endl;
        }
        for (int i = 0; i < 2; ++i) {
        toggle_clk();
    }
    }

    std::cout << "All tests completed." << std::endl;
    return 0;
}