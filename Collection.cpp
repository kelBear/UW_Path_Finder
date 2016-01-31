#include <string>
#include <vector>
#include "Collection.h"

using namespace std;

Collection::BuildingNotInCollectionException::BuildingNotInCollectionException(string bcode) : code(bcode){}	// constructor for building not found in collection exception class

string Collection::BuildingNotInCollectionException::getcode() const {		// accessor to get building code of building not found in collection exception class
	return code;
}

Collection::ExistingBuildingCodeException::ExistingBuildingCodeException(string bcode) : code(bcode){}		// constructor for exception class of building already exist in collection

string Collection::ExistingBuildingCodeException::getcode() const {		// accessor to get building code
	return code;
}

Collection::FormerExistingCodeException::FormerExistingCodeException(string bcode) : code(bcode){}		// constructor for exception class for a building code used by a former building in collection

string Collection::FormerExistingCodeException::getcode() const {		// accessor to get the building code from exception class
	return code;
}

Collection::Collection() {											// default constructor
	buildings_ = NULL;
}

Collection::~Collection() {											// destructor
	delete buildings_;
}

void Collection::insert(string code, string name) {					// insert building to collection
	bool former = false;
	for (int i = 0; i < wreckedBuildings.size(); i++) {		// check for if the building code has been used by a former building
		if (wreckedBuildings[i] == code) {
			former = true;
			break;
		}
	}
	if (findBuilding(code) != NULL)
		throw ExistingBuildingCodeException(code);		// if building code already exists in the collection, throw an exception
	else if (former)
		throw FormerExistingCodeException(code);		// if building code was used by a former building, throw an exception
	Building* b1 = new Building(code, name, buildings_);		// construct new building node in collection
	buildings_ = b1;
}

Building* Collection::getbuildings() const {						// accessor - get first building pointer
	return buildings_;
}

Building* Collection::findBuilding(string code) const {				// find building from collection based on building code
	Building* b1 = new Building;
	b1 = getbuildings();
	if (b1 != NULL) {
		while (b1->getbcode().getcode() != code && b1->getnext() != NULL) {
			b1 = b1->getnext();
		}
		if (b1->getbcode().getcode() == code) {
			return b1;			// return building if found
		}
	}
	return NULL;		// return NULL if not found
}

void Collection::remove(string code) {								// remove building from collection
	if (findBuilding(code) == NULL)
		throw BuildingNotInCollectionException(code);		// if building cannot be found in collection, throw exception
	Building* b1 = new Building;
	b1 = buildings_;
	if (b1 != NULL && b1->getbcode().getcode() != code) {
		while (b1->getnext() != NULL && b1->getnext()->getbcode().getcode() != code) {		// find building that needs to be removed
			b1 = b1->getnext();
		}
		if (b1->getnext()->getbcode().getcode() == code) {
			Building* b2 = b1->getnext();
			b1->setnext(b1->getnext()->getnext());
			delete b2;
			wreckedBuildings.push_back(b2->getbcode().getcode());
		}
	}
	else {		// if building to be removed is first in the list
		if (b1->getbcode().getcode() == code) {
			Building* b2 = b1;
			buildings_ = b1->getnext();
			wreckedBuildings.push_back(b2->getbcode().getcode());
			delete b2;
		}
	}
}
