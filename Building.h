#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <iostream>
#include "BCode.h"

class Building {															// Building ADT
public:
	Building();																// default constructor
	~Building();															// destructor
	explicit Building(std::string, std::string, Building*);							// overloaded constructor for initialization
	Building& operator= (const Building&) = delete;							// assignment operator disabled
	bool operator== (const Building&) = delete;								// equality operator disabled
	std::string getname() const;													// accessor - gets name of building
	BCode getbcode() const;													// accessor - gets building code of building
	Building* getnext() const;												// accessor - gets next building in the linked list
	void setnext(Building* next);											// mutator - sets pointer to next building in the linked list
private:
	std::string bname_;															// building name
	BCode bcode_;															// building code object
	Building* next_;														// pointer to next building object in linked list
};

std::ostream& operator<< (std::ostream&, const Building&);							// building output stream operator function			

#endif