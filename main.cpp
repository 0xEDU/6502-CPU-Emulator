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

	// Write 2 bytes
	void writeWord(Word value, u32 address, u32 &cycles) {
		data[address]     = value & 0xFF;
		data[address + 1] = (value >> 8);
		cycles -= 2;
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

	Word fetchWord(u32 &cycles, Mem &memory) {
		// 6592 is little endian
		Word data = memory[PC];
		PC++;
		data |= (memory[PC] << 8);
		PC++;
		cycles -= 2;
		return data;
	}

	Byte readByte(u32 &cycles, Byte address, Mem &memory) {
		Byte data = memory[address];
		cycles--;
		return data;
	}

	/* opcodes */
	static constexpr Byte
	INS_LDA_IM = 0xA9,
	INS_LDA_ZP = 0xA5,
	INS_LDA_ZPX = 0x85,
	INS_JSR = 0x20;

	void setLDAStatus() {
		Z = (A == 0);
		N = (A & 0b10000000) > 0;
	}

	void execute(u32 cycles, Mem &memory) {
		while (cycles > 0) {
			Byte instruction = fetchByte(cycles, memory);
			switch (instruction) {
				case INS_LDA_IM:
				{
					Byte value = fetchByte(cycles, memory);
					A = value;
					setLDAStatus();
				} break ;
				case INS_LDA_ZP:
				{
					Byte zeroPageAddress = fetchByte(cycles, memory);
					A = readByte(cycles, zeroPageAddress, memory);
					setLDAStatus();
				} break ;
				case INS_LDA_ZPX:
				{
					Byte zeroPageAddress = fetchByte(cycles, memory);
					A = readByte(cycles, zeroPageAddress, memory);
					zeroPageAddress += X; // This might overflow!
					cycles--;
					A = readByte(cycles, zeroPageAddress, memory);
					setLDAStatus();
				} break ;
				case INS_JSR:
				{
					Word subAddr = fetchWord(cycles, memory);
					memory.writeWord(PC - 1, SP, cycles);
					SP++;
					PC = subAddr;
					cycles--;
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
	mem[0xFFFC] = CPU::INS_JSR;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x42;
	mem[0x4242] = CPU::INS_LDA_IM;
	mem[0x4243] = 0x84;
	// end - inline a small program
	cpu.execute(9, mem);
	return 0;
}
