#include <cstdio>
#include <cstdlib>
#include <cstring>

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
};

struct CPU {
	Word PC; // Program Counter
	Word SP; // Stack Pointer
	
	/* Registers */
	Byte A, X, Y;

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

};

int main() {
	Mem mem;
	CPU cpu;

	cpu.reset(mem);
	return 0;
}
