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

using namespace std;

// program entry point
int main (int argc, char *argv[]) {

	InputTokenizer it;
	CodeOutputList co;

	// try to open a file on the input tokenizer
	if (!it.open ("test-asm/test.asm")) {
		cout << "Error opening input file: test.asm" << endl;
		return -1;
	}

	// create the new ToyAssembler and assemble the file!
	ToyAssembler ta(&it,&co);
	ta.assemble ();

	if (!ta.errors()) {
		cout << "Assembly success!" << endl;
		return 0;
	}

	else {
		cout << "Assembly failed with errors:" << endl;
		for (ssize_t i = 0; i < ta.err_count(); i++) {
			cout << "Error (line " << ta.err_at(i).get_lineno() << "): " << ta.err_at(i).get_err() << endl;
		}
		return -1;
	}
}
