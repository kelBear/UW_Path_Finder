#include "BCode.h"
#include <string>
#include <iostream>
#include "Collection.h"

using namespace std;

BCode::InvalidBCodeException::InvalidBCodeException(string msg){		// exception class constructor for invalid building code format
	message = msg;
}

string BCode::InvalidBCodeException::getmessage() const {			// accessor of message of exception class
	return message;
}

BCode::BCode(string code) {								  // constructor
	string const letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";		// allowed first character & letters of building code
	string const numbers = "0123456789";					// allowed numbers of building code
	string msg = "";
	if (code.length() < 2 || code.length() > 3)			// if building code does not have 2 or 3 characters, update exception message
		msg += "\t- must have length of 2-3 characters\n";
	if (letters.find(code.substr(0, 1)) == string::npos)	// if first character is not part of letters allowed, update exception message
		msg += "\t- must start with a capital letter\n";
	for (int i = 1; i < code.length(); i++) {
		if (letters.find(code[i]) == string::npos && numbers.find(code[i]) == string::npos) {	// if 2nd or 3rd character is not allowed character, update exception message
			msg += "\t- must consist of only capital letters and digits\n";
			break;
		}
	}
	if (msg != "") {			// if exception message contains content, throw the exception of invalid building code format
		msg = "\nERROR: Building Code \"" + code + "\" has an invalid format.\n" + msg;
		throw InvalidBCodeException(msg);
	}
	code_ = code;
}

string BCode::getcode() const {							  // accessor - gets building code
	return code_;
}

bool BCode::operator== (const BCode& bcode) const {		  // comparison operator - equality
	return (bcode.code_ == code_);
}

bool BCode::operator!= (const BCode& bcode) const {		  // comparison operator - not equal
	return !(bcode.code_ == code_);
}

BCode& BCode::operator= (const BCode& bcode) {			  // assignment operator
	code_ = bcode.code_;
	return *this;
}