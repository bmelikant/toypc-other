/*
 *      toyasm main.cpp - Main code execution for toyasm
 *      Ben Melikant, 03/2017
 */

// standard includes
#include <iostream>
#include <fstream>

// project includes
//#include <preproc.h>
#include <asm.h>

// program entry point
int main (int argc, char *argv[]) {

	InputTokenizer it;
	CodeOutputList co;

	// try to open a file on the input tokenizer
	if (!it.open ("test-asm/test.asm")) {
		std::cout << "Error opening input file: test.asm" << std::endl;
		return -1;
	}

	// create the new ToyAssembler and assemble the file!
	ToyAssembler ta(&it,&co);
	ta.assemble ();

	if (!ta.errors()) {
		std::cout << "Assembly success!" << std::endl;
		return 0;
	}

	else {
		std::cout << "Assembly failed with errors:" << std::endl;
		for (ssize_t i = 0; i < ta.err_count(); i++) {
			std::cout << "Error (line " << ta.err_at(i).get_lineno() << "): " << ta.err_at(i).get_err() << std::endl;
		}
		return -1;
	}
}
