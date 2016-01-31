#include "Building.h"
#include <string>

using namespace std;

Building::Building() {														// default constructor
	bname_ = "";
	bcode_ = BCode();
	next_ = NULL;
}

Building::Building(string bcode, string bname, Building* node){				// overloaded constructor
	bname_ = bname;
	bcode_ = BCode(bcode);
	next_ = node;
}

Building::~Building() {														// destructor
	delete next_;
}

string Building::getname() const {											// accessor
	return bname_;
}

BCode Building::getbcode() const {											// accessor
	return bcode_;
}

Building* Building::getnext() const {										// accessor
	return next_;
}

void Building::setnext(Building* next) {									// mutator
	next_ = next;
}

ostream& operator<< (ostream& sout, const Building& b1) {				// output operator - formats output of building objects
	sout << b1.getbcode().getcode() << '\t' << b1.getname();
	return sout;
}