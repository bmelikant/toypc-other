/*
 * instruction.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: Ben Melikant
 */

#include <asm.h>

using namespace std;

// intitialize this instruction
Instruction::Instruction () {
	byte_sz = 0;
	times = 1;
}

// add a byte to this instruction's data
void Instruction::add_instr_byte (unsigned char byteval) {
	this->instr_bytes.push_back (byteval);
	byte_sz++;
}

// add a forward reference to this instruction's data
void Instruction::add_fwd_ref (string ref) {
	this->fwd_ref = ref;
	byte_sz += 2;
}

// set this instruction to occur a certain number of times
void Instruction::set_times_count (int tms) {
	this->times = tms;
}

// get the byte data from this Instruction
int Instruction::get_bytes (unsigned char *bytes, int count) {
	ssize_t i = 0;
	for ( ; i < (ssize_t)(this->instr_bytes.size()); i++) {
		if (i >= count) {
			break;
		}

		bytes[i] = instr_bytes[i];
	}
	// return the number of bytes copied into the buffer
	return i;
}

// get the number of bytes contained in this Instruction
int Instruction::get_size () {
	return this->byte_sz;
}

// get the forward ref label contained in this Instruction
string Instruction::get_fwd_ref () {
	return this->fwd_ref;
}
