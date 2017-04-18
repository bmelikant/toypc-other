/*
 * instr_list.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: Ben Melikant
 */

#include <instr_list.h>
#include <asm.h>

#include <regex>

// pattern definitions
const std::string label_pattern = "[a-zA-Z0-9_]*:?";
const std::string mref_pattern	= "\\[[a-zA-Z0-9_]*\\]";
const std::string str_pattern	= "\".*\"|'.*'";

const std::string xdigit_pattern	= "(0x[a-fA-F0-9]*)|([a-fA-F0-9]*(h|H))";
const std::string digit_pattern		= "([0-9]*(d|D)?)";
const std::string binary_pattern	= "([0|1]*(b|B))";

const std::string number_pattern = xdigit_pattern + "|" + digit_pattern + "|" + binary_pattern;

// instruction list definition
const std::unordered_map<std::string, InstructionPair> instructions = {

	// one byte opcodes
	{ "nop", { ONEBYTE_OPCODE, 0x60 }},
	{ "hlt", { ONEBYTE_OPCODE, 0x10 }},
	{ "cpuid", { ONEBYTE_OPCODE, 0x15 }},
	{ "pshf", { ONEBYTE_OPCODE, 0x16 }},
	{ "popf", { ONEBYTE_OPCODE, 0x17 }},
	{ "pshall", { ONEBYTE_OPCODE, 0x50 }},
	{ "popall", { ONEBYTE_OPCODE, 0x51 }},
	{ "jmpdi", { ONEBYTE_OPCODE, 0x46 }},
	{ "in", { ONEBYTE_OPCODE, 0x55 }},
	{ "out", { ONEBYTE_OPCODE, 0x56 }},
	{ "ldstr", { ONEBYTE_OPCODE, 0x40 }},
	{ "ststr", { ONEBYTE_OPCODE, 0x41 }},
	{ "ret", { ONEBYTE_OPCODE, 0x45 }},
	{ "iret", { ONEBYTE_OPCODE, 0x46 }},

	// math opcodes
	{ "sto", { MATH_OPCODE, 0x01 }},
	{ "lod", { MATH_OPCODE, 0x02 }},
	{ "add", { MATH_OPCODE, 0x03 }},
	{ "sub", { MATH_OPCODE, 0x04 }},
	{ "mul", { MATH_OPCODE, 0x05 }},
	{ "div", { MATH_OPCODE, 0x06 }},
	{ "mod", { MATH_OPCODE, 0x07 }},
	{ "and", { MATH_OPCODE, 0x11 }},
	{ "or", { MATH_OPCODE, 0x12 }},
	{ "xor", { MATH_OPCODE, 0x13 }},
	{ "cmp", { MATH_OPCODE, 0x14 }},
	{ "shl", { MATH_OPCODE, 0x0a }},
	{ "shr", { MATH_OPCODE, 0x0b }},

	// branch instructions
	{ "call", { BRANCH_OPCODE, 0x0e }},
	{ "jmp", { BRANCH_OPCODE, 0x0f }},
	{ "je", { BRANCH_OPCODE, 0x1f }},
	{ "jne", { BRANCH_OPCODE, 0x2f }},
	{ "jg", { BRANCH_OPCODE, 0x3f }},
	{ "jng", { BRANCH_OPCODE, 0x4f }},
	{ "jl", { BRANCH_OPCODE, 0x5f }},
	{ "jnl", { BRANCH_OPCODE, 0x6f }},
	{ "jz", { BRANCH_OPCODE, 0x7f }},
	{ "jnz", { BRANCH_OPCODE, 0x8f }},
	{ "jc", { BRANCH_OPCODE, 0x9f }},
	{ "jnc", { BRANCH_OPCODE, 0xaf }},

	// stack instructions
	{ "psh", { STACK_OPCODE, 0x08 }},
	{ "pop", { STACK_OPCODE, 0x09 }},

	// special instructions
	{ "inc", { SPECIAL_OPCODE, 0x30 }},
	{ "dec", { SPECIAL_OPCODE, 0x37 }},
	{ "int", { SPECIAL_OPCODE, 0x16 }},

	// directives
	{ "db", { DIRECTIVE, 0x01 }},
	{ "dw", { DIRECTIVE, 0x02 }},
	{ "dd", { DIRECTIVE, 0x04 }}
};

// initialize the register list
const std::unordered_map<std::string, RegisterPair> rm_values {

	{ "ra", { 0x01, false }},
	{ "rb", { 0x02, false }},
	{ "rc", { 0x03, true }},
	{ "rx", { 0x04, true }},
	{ "ry", { 0x05, true }},
	{ "si", { 0x06, true }},
	{ "di", { 0x07, true }}
};

// try to match an instruction
bool instr_match (std::string s) {

	int count = instructions.count(stringlower(s));

	if (count > 0) {

		InstructionPair instr = instructions.find (stringlower(s))->second;

		if (instr.type != DIRECTIVE)
			return true;
	}

	return false;
}

// try to match a directive
bool directive_match (std::string s) {

	// if the list contains this directive, we have a hit!
	if (instructions.find (stringlower (s)) != instructions.end () &&
			instructions.find (stringlower(s))->second.type == DIRECTIVE)
		return true;

	return false;
}

// try to match a token to a register
bool register_match (std::string s) {

	return (rm_values.find(stringlower(s)) != rm_values.end ());
}

// try to match a token to a memory reference
bool mref_match (std::string s) {

	if (s[0] == '[' && s[s.length()] == ']')
		return true;

	return false;
}

// try to match a token to a number
bool number_match (std::string s) {

	return std::regex_match(s, std::regex (number_pattern));
}

// try to match a token to a forward reference or label
bool fwdref_match (std::string s) {

	return std::regex_match (s, std::regex(label_pattern));
}

// convert a std::string to lowercase
std::string stringlower (std::string s) {

	std::string lower = s;

	for (std::string::size_type i = 0; i < lower.length(); i++)
		lower[i] = std::tolower(lower[i]);

	return lower;
}
