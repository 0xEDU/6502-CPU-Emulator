#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

// References:
// https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37
// http://www.6502.org/users/obelisk/
// https://www.c64-wiki.com/wiki/Reset_(Process)

using Byte = unsigned char; // 1 byte / 8 bits
using Word = unsigned short; // 4 byte / 16 bits

using u32 = unsigned int;

struct Mem {
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte data[MAX_MEM];

	void initialize() {
		std::memset(data, 0, sizeof(data));
	}

	// Read 1 byte
	Byte operator[](u32 address) const {
		// TODO: Assert that address < MAX_MEM
		return data[address];
	}

	// Write 1 byte
	Byte &operator[](u32 address) {
		// TODO: Assert that address < MAX_MEM
		return data[address];
	}
};

struct CPU {
	Word PC; // Program Counter
	Word SP; // Stack Pointer
	
	/* Registers */
	Byte A, X, Y; // Accumulator, X and Y

	/* Status flags */
	Byte C : 1; // Carry flag
	Byte Z : 1; // Zero flag
	Byte I : 1; // Interrupt Disable
	Byte D : 1; // Decimal Mode
	Byte B : 1; // Break Command
	Byte V : 1; // Overflow Flag
	Byte N : 1; // Negative Flag
	
	// Initialize internal values
	void reset(Mem &memory) {
		PC = 0xFFFC;
		SP = 0x0100;
		C = Z = I = D = B = V = N = 0; // This is ugly
		A = X = Y = 0;
		memory.initialize();
	}

	Byte fetchByte(u32 &cycles, Mem &memory) {
		Byte data = memory[PC];
		PC++;
		cycles--;
		return data;
	}

	// opcodes
	static constexpr Byte
	INS_LDA_IM = 0xA9;

	void execute(u32 cycles, Mem &memory) {
		while (cycles > 0) {
			Byte instruction = fetchByte(cycles, memory);
			(void) instruction;
			switch (instruction) {
				case INS_LDA_IM:
				{
					Byte value = fetchByte(cycles, memory);
					A = value;
					Z = (A == 0);
					N = (A & 0b10000000) > 0;
				} break ;
				default:
				{
					std::cout << "Instruction not handled "
						<< instruction << std::endl;
				} break ;
			}
		}
	}
};

int main() {
	Mem mem;
	CPU cpu;

	cpu.reset(mem);
	// start - inline a small program
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x42;
	// end - inline a small program
	cpu.execute(2, mem);
	return 0;
}
