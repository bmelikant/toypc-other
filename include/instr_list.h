/*
 * instr_list.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Ben Melikant
 */

#ifndef _INSTR_LIST_H_
#define _INSTR_LIST_H_

#include <unordered_map>
#include <vector>
#include <string>

enum InstructionType {

	ONEBYTE_OPCODE,
	MATH_OPCODE,
	BRANCH_OPCODE,
	STACK_OPCODE,
	SPECIAL_OPCODE,
	DIRECTIVE
};

// hold data on the instructions
struct InstructionPair {

	InstructionType type;
	unsigned char opcode;
};

// hold data on the registers
struct RegisterPair {

	int rmval;
	bool bits16;
};

// instructions list
extern const std::unordered_map<std::string, InstructionPair> instructions;
extern const std::unordered_map<std::string, RegisterPair> rm_values;
extern const std::vector<std::string> directives;

// matching functions
bool label_match 		(std::string s);
bool instr_match 		(std::string s);
bool directive_match 	(std::string s);

bool register_match (std::string s);
bool mref_match 	(std::string s);
bool number_match	(std::string s);
bool fwdref_match 	(std::string s);

std::string stringlower (std::string s);

#endif /* _INSTR_LIST_H_ */
