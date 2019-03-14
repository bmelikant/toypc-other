/*
 * instr_list.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: Ben Melikant
 */

#include <instr_list.h>
#include <asm.h>

#include <regex>
#include <vector>

using namespace std;

// valid argument type definitions
static const int ARGUMENTS_NONE 		= 0;
static const int ARGUMENTS_REGISTER 	= 1;
static const int ARGUMENTS_MEMREF 		= 2;
static const int ARGUMENTS_IMMEDIATE 	= 4;
static const int ARGUMENTS_CONSTANT		= 8;

// pattern definitions
static const string label_pattern = "[a-zA-Z0-9_]*:?";
static const string mref_pattern	= "\\[[a-zA-Z0-9_]*\\]";
static const string str_pattern	= "\".*\"|'.*'";

static const string xdigit_pattern	= "(0x[a-fA-F0-9]*)|([a-fA-F0-9]*(h|H))";
static const string digit_pattern		= "([0-9]*(d|D)?)";
static const string binary_pattern	= "([0|1]*(b|B))";

static const string number_pattern = xdigit_pattern + "|" + digit_pattern + "|" + binary_pattern;
static const string register_pattern = "(r|R){1}[0-4]{1}";

// instruction list definition
static const unordered_map<string, vector<InstructionPair>> instructions = {

	// single byte opcodes
	{ "nop", { ONEBYTE, 0xFE, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "stfz", { ONEBYTE, 0x10, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "stfc", { ONEBYTE, 0x30, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "stfg", { ONEBYTE, 0x50, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "stfl", { ONEBYTE, 0x70, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "clfz", { ONEBYTE, 0x90, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "clfc", { ONEBYTE, 0xB0, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "clfg", { ONEBYTE, 0xD0, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "clfl", { ONEBYTE, 0xF0, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	{ "halt", { ONEBYTE, 0xFF, (ARGUMENTS_NONE),(ARGUMENTS_NONE) }},
	
	// two byte opcodes
	{ "mov", { TWOBYTE, 0x01, (ARGUMENTS_REGISTER),(ARGUMENTS_REGISTER) }},

	// directives
	{ "db", { DIRECTIVE, 0x01 }},
	{ "dw", { DIRECTIVE, 0x02 }},
	{ "dd", { DIRECTIVE, 0x04 }}
};

// try to match an instruction
bool instr_match (string s) {
	int count = instructions.count(stringlower(s));
	
	if (count > 0) {
		InstructionPair instr = instructions.find(stringlower(s))->second;
		if (instr.type != DIRECTIVE) {
			return true;
		}
	}

	return false;
}

// try to match a directive
bool directive_match (string s) {
	// if the list contains this directive, we have a hit!
	if (instructions.find (stringlower (s)) != instructions.end () &&
			instructions.find (stringlower(s))->second.type == DIRECTIVE)
		return true;

	return false;
}

// try to match a token to a register
bool register_match (string s) {
	return regex_match(s, regex(register_pattern));
}

// try to match a token to a memory reference
bool mref_match (string s) {
	return (s[0] == '[' && s[s.length()] == ']');
}

// try to match a token to a number
bool number_match (string s) {
	return regex_match(s, regex (number_pattern));
}

// try to match a token to a forward reference or label
bool fwdref_match (string s) {
	return regex_match (s, regex(label_pattern));
}

// convert a string to lowercase
string stringlower (string s) {
	string lower = s;

	for (string::size_type i = 0; i < lower.length(); i++) {
		lower[i] = tolower(lower[i]);
	}

	return lower;
}
