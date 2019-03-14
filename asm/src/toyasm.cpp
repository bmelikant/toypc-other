/*
 * toyasm.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: Ben Melikant
 */

#include <asm.h>
#include <instr_list.h>

#include <algorithm>
#include <string>
#include <iostream>

ToyAssembler::ToyAssembler (InputTokenizer *it, CodeOutputList *ol) {

	this->it = it;
	this->ol = ol;

	this->errors_exist = false;
	addr = 0;
	lineno = 1;
	instr = nullptr;
	line_err = false;
}

// void ToyAssembler::assemble (): Assemble the code file!
void ToyAssembler::assemble () {

	// the InputTokenizer and CodeOutputList should already exist
	// begin assembly

	// read the next line of the file while there is data
	while (it->is_good ()) {

		it->next_line ();
		code_line ();
		lineno++;
	}
}

// assemble a single line of code
void ToyAssembler::code_line () {

	line_reset_state ();
	cur_token = it->next_token ();

	// if this token is EOL, we have a blank line
	if (cur_token == "\n")
		return;

	// if this token matches an instruction, process the instruction
	else if (instr_match (cur_token))
		instruction ();
	else if (directive_match (cur_token))
		directive ();
	else if (label_match (cur_token))
		line_label ();

	// if we haven't matched any tokens, error
	else
		store_error ("Expected instruction or directive at start of line");

	// write the instruction to the table, then delete it from the heap
	if (!line_err)
		ol->add_instruction(instr);
}

// process the label for a line
void ToyAssembler::line_label () {

	// store the label at the current address
	ol->add_symbol(cur_token, addr);
	cur_token = it->next_token ();

	// if we have EOL, we are all done!
}

// assemble an instruction
void ToyAssembler::instruction () {

	// write the opcode into the packet
	unsigned char opcode = instructions.find(stringlower(cur_token))->second.opcode;
	InstructionType type = instructions.find(stringlower(cur_token))->second.type;

	instr->add_instr_byte (opcode);

	// grab the next token
	cur_token = it->next_token();

	// if this is a onebyte opcode, we should be done
	if (type == ONEBYTE_OPCODE && cur_token == "\n")
		return;
	else if (type == MATH_OPCODE && register_match (cur_token))
		math_argument_one (opcode);
	else
		store_error ("Invalid combination of opcode and operands");
}

// assemble a math operand
void ToyAssembler::math_argument_one (unsigned char opcode) {

	// look up the register's value
	unsigned char rmone = (unsigned char) rm_values.find(stringlower(cur_token))->second.rmval;
	bool bits16 = rm_values.find(stringlower(cur_token))->second.bits16;

	// there should be another token
	cur_token = it->next_token();

	// there must be a second token
	if (cur_token != "\n") {
		if (register_match (stringlower(cur_token)))
			math_argument_two_register (rmone, bits16);
		else if (mref_match (cur_token))
			math_argument_two_mref (rmone);
		else if (number_match (cur_token) && opcode != instructions.find("sto")->second.opcode)
			math_argument_two_immediate (rmone, bits16);
		else if (fwdref_match (cur_token) && opcode == instructions.find("lod")->second.opcode)
			math_argument_two_fwdref (rmone);
		else
			store_error ("Invalid combination of opcode and operands");
	}

	else
		store_error ("Operand expected");
}

// assemble an argument for math second operand
void ToyAssembler::math_argument_two_register (unsigned char rm, bool bits16) {

	// grab the register's rm value, combine the values then write them in
	unsigned char rmtwo = (unsigned char) rm_values.find(stringlower(cur_token))->second.rmval;
	unsigned char modrm = (unsigned char) (rm << 4) | rmtwo;

	// write the modrm value into the packet
	instr->add_instr_byte (modrm);
	cur_token = it->next_token ();

	// should be the last token on the line
	if (cur_token != "\n")
		store_error ("Invalid combination of opcode and operands");
}

// assemble a math instruction second argument (immediate)
void ToyAssembler::math_argument_two_immediate (unsigned char rm, bool bits16) {
}

// assemble a math instruction second argument (mref)
void ToyAssembler::math_argument_two_mref (unsigned char rm) {
}

// assemble a math instruction second argument (fwdref)
void ToyAssembler::math_argument_two_fwdref (unsigned char rm) {

}

// assemble an argument for a branch opcode
void ToyAssembler::branch_argument () {
}

// assemble an argument for a stack opcode
void ToyAssembler::stack_argument () {
}

// assemble a special argument
void ToyAssembler::special_argument () {
}

// assemble a directive
void ToyAssembler::directive () {

	store_error ("Not supported...");
	return;
}

// assemble the argument for a directive
void ToyAssembler::directive_argument () {
}

// bool ToyAssembler::errors (): See whether or not we had any errors!
bool ToyAssembler::errors () {

	return this->errors_exist;
}

int ToyAssembler::err_count () {

	return error_list.size ();
}

Error ToyAssembler::err_at(int i) {

	return error_list[i];
}

// store an error into the list (takes care of all needed adjustments)
void ToyAssembler::store_error (std::string err) {

	Error new_err;
	errors_exist = line_err = true;
	new_err.set_error(err, lineno);
	error_list.push_back (new_err);
}

// reset the line state
void ToyAssembler::line_reset_state () {

	line_err = false;

	if (instr)
		delete instr;

	instr = new Instruction ();
}

// add an error to the error list
void Error::set_error(std::string err, int lineno) {

	this->err = err;
	this->lineno = lineno;
}

std::string Error::get_err () { return err; }
int Error::get_lineno () { return this->lineno; }
