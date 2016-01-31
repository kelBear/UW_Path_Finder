#ifndef BCODE_H
#define BCODE_H

#include <string>

class BCode{											  // Building Code ADT
public:
	BCode() {}											  // default constructor
	BCode(std::string code);									  // copy constructor
	~BCode() = default;									  // assignment operator using default
	std::string getcode() const;								  // accessor - gets building code
	bool operator== (const BCode&) const;                 // equality operator for BCode objects
	bool operator!= (const BCode&) const;                 // non-equality operator for BCode objects
	BCode& operator= (const BCode&);					  // assignment operator for BCode objects
	class InvalidBCodeException {							// exception class for invliad format of building code
	public:
		InvalidBCodeException(std::string);
		std::string getmessage() const;
	private:
		std::string message;
	};
private:
	std::string code_;										  // building code of building
};

#endif