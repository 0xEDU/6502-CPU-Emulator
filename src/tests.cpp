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

TEST_F(M6502Test1, LDAZeroPageCanLoadAValueIntoTheARegister) {
    // Given:
    // start - inline a small program
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x37;
    // end - inline a small program

    // When:
    cpu.execute(3, mem);

    // Then:
    EXPECT_EQ(cpu.A, 0x37);
}

TEST_F(M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegister) {
    // Given:
    cpu.X = 5;
    // start - inline a small program
    mem[0xFFFC] = CPU::INS_LDA_ZPX;
    mem[0xFFFD] = 0x42;
    mem[0x0047] = 0x37;
    // end - inline a small program

    // When:
    cpu.execute(4, mem);

    // Then:
    EXPECT_EQ(cpu.A, 0x37);
}

TEST_F(M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps) {
    // Given:
    cpu.X = 0xFF;
    // start - inline a small program
    mem[0xFFFC] = CPU::INS_LDA_ZPX;
    mem[0xFFFD] = 0x80;
    mem[0x007F] = 0x37;
    // end - inline a small program

    // When:
    CPU cpuCopy = cpu;
    cpu.execute(4, mem);

    // Then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.Z);
    EXPECT_FALSE(cpu.N);
    EXPECT_EQ(cpu.C, cpuCopy.C);
    EXPECT_EQ(cpu.I, cpuCopy.I);
    EXPECT_EQ(cpu.D, cpuCopy.D);
    EXPECT_EQ(cpu.B, cpuCopy.B);
    EXPECT_EQ(cpu.V, cpuCopy.V);
}

#if 0
int main() {

    return 0;
}
#endif
