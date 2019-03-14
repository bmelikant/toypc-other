/*
 * asm.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Ben Melikant
 */

#ifndef _ASM_H_
#define _ASM_H_

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

// instruction class: hold a single instruction for the assembler
class Instruction {

public:

	Instruction ();

	void add_instr_byte 	(unsigned char byteval);
	void add_fwd_ref		(std::string ref);
	void set_times_count	(int tms);

	int 		get_bytes 	(unsigned char *bytes, int count);
	int 		get_size 	();
	std::string get_fwd_ref ();

private:

	std::vector<unsigned char> instr_bytes;
	int byte_sz;
	int times;
	std::string fwd_ref;
};

class Error {

public:

	void set_error (std::string error, int lineno);

	std::string get_err		();
	int			get_lineno 	();
private:

	std::string err;
	int lineno;
};

// input tokenizer class: get input tokens
class InputTokenizer {

public:

	InputTokenizer ();

	bool open 				(std::string fname);
	void next_line 			();
	std::string next_token 	();

	std::string token_data 	();
	std::string line_data 	();

	bool is_good ();

private:

	std::string token;
	std::string linedata;
	std::string lineorig;

	std::ifstream infile;
};

// code list class: output data
class CodeOutputList {

public:

	void add_instruction 	(Instruction *);
	void add_symbol 		(std::string sym, int addr);

private:

	std::vector<Instruction> code_data;
	std::unordered_map<std::string, int> symtable;
};

// toy assembler class
class ToyAssembler {

public:

	ToyAssembler (InputTokenizer *t, CodeOutputList *l);

	void assemble 	();
	bool errors		();

	void test ();

	// error gathering functions
	int 	err_count 	();
	Error 	err_at 		(int i);

private:

	InputTokenizer *it;
	CodeOutputList *ol;
	Instruction *instr;

	std::vector<Error> error_list;
	bool errors_exist;

	int lineno;
	int addr;
	bool line_err;
	std::string cur_token;

	// private member variables
	void code_line 		();
	void line_label		();
	void instruction 	();

	void math_argument_one (unsigned char opcode);
	void math_argument_two_register (unsigned char rm, bool bits16);
	void math_argument_two_immediate (unsigned char rm, bool bits16);
	void math_argument_two_mref (unsigned char rm);
	void math_argument_two_fwdref (unsigned char rm);

	void branch_argument ();
	void stack_argument ();
	void special_argument ();

	void directive 		();
	void directive_argument ();

	void store_error 		(std::string err);
	void line_reset_state 	();
};

#endif /* _ASM_H_ */
