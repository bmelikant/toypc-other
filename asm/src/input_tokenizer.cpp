/*
 * input_tokenizer.cpp: Define the class InputTokenzier
 *
 *  Created on: Apr 11, 2017
 *      Author: Ben Melikant
 */

#include <asm.h>

#include <iostream>
#include <fstream>

#include <cstdlib>
#include <cstring>
#include <cerrno>

const char* ws = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = ws)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = ws)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (left & right)
inline std::string& trim(std::string& s, const char* t = ws)
{
    return ltrim(rtrim(s, t), t);
}

// create our input tokenizer
InputTokenizer::InputTokenizer() {

	linedata = "";
	lineorig = "";
	token = "";
}

// open our input tokenizer. return false if error
bool InputTokenizer::open(std::string fname) {

	// open the file for read
	infile.open (fname, std::ifstream::in);
	return (infile.is_open());
}

// fetch the next code line
void InputTokenizer::next_line () {

	if (infile.is_open()) {

		// grab the next line and store it in both lines
		std::getline (infile, linedata);
		lineorig = linedata.substr(0, (linedata.find_first_of (";") - 1));
	}
}

// process the next token out of the line data
std::string InputTokenizer::next_token () {

	// blank out our token
	token = "";

	// trim the line of whitespace
	linedata = trim(linedata);

	// if there is nothing left, just return an empty eol token
	if (linedata.empty()) {

		token = "\n";
		return token;
	}

	// if we have reached a comment, blank out the line and send eol token
	else if (linedata[0] == ';') {

		linedata = "";
		token = "\n";
		return token;
	}

	// for a string literal, put the literal into the token
	else if (linedata[0] == '"' || linedata[0] == '\'') {

		int terminator = linedata[0];

		// read until terminator or eol
		for (size_t i = 0; i < linedata.length(); i++) {

			token += linedata[i];

			// if we have reached the end of the literal, take care of it!
			if (linedata[i] == terminator) {

				// update and trim the line
				linedata = linedata.substr (i+1);
				linedata = trim (linedata);

				// if the next character is a comma we have more tokens
				if (!linedata.empty () && linedata[0] == ',') {

					linedata = linedata.substr (0);
					linedata = trim (linedata);
				}

				return token;
			}
		}

		// linedata should have been consumed; blank it out!
		linedata = "";
	}

	// any other token. Read until separator (whitespace or ,) or until EOL
	else {

		for (size_t i = 0; i < linedata.length(); i++) {

			if (linedata[i] == ',' || linedata[i] == ' ') {

				linedata = linedata.substr (i+1);
				return token;
			}

			token += linedata[i];
		}

		// all line data was consumed; blank out linedata!
		linedata = "";
	}

	return token;
}

std::string InputTokenizer::token_data	() { return token; }
std::string InputTokenizer::line_data 	() { return lineorig; }

bool InputTokenizer::is_good () {

	return infile.good ();
}
