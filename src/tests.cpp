#include "../includes/6502cpu.hpp"
#include <gtest/gtest.h>

class M6502Test1 : public testing::Test  {
    protected:

    Mem mem;
    CPU cpu;

    void SetUp() override {
        cpu.reset(mem);
    }

    void TearDown() override {}
};

TEST_F(M6502Test1, LDAImmediateCanLoadAValueIntoTheARegister) {
    // Given:
    // start - inline a small program
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x84;
    // end - inline a small program

    // When:
    cpu.execute(2, mem);

    // Then:
    EXPECT_EQ(cpu.A, 0x84);
}

#if 0
int main() {

    return 0;
}
#endif
