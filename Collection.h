#ifndef COLLECTION_H
#define COLLECTION_H

#include <string>
#include <vector>
#include "Building.h"

class Collection {													// Collection ADT
public:
	Collection();													// default constructor
	~Collection();													// destructor
	Collection(const Collection&) = delete;							// copy constructor not allowed
	Collection& operator= (const Collection&) = delete;				// assignment operator not allowed 
	bool operator== (const Collection&) = delete;					// equality operator not allowed
	void insert(std::string, std::string);									// insert building to collection
	Building* findBuilding(std::string) const;							// find building in collection
	void remove(std::string);											// remove building from collection
	Building* getbuildings() const;									// accessor - gets first pointer to linked list of buildings
	class ExistingBuildingCodeException {							// exception class for building code already existing in the collection
	public:
		ExistingBuildingCodeException(std::string);
		std::string getcode() const;
	private:
		std::string code;
	};
	class FormerExistingCodeException {								// exception class for building code used by former building in collection
	public:
		FormerExistingCodeException(std::string);
		std::string getcode() const;
	private:
		std::string code;
	};
	class BuildingNotInCollectionException {						// exception class for building code not found in the collection
	public:
		BuildingNotInCollectionException(std::string);
		std::string getcode() const;
	private:
		std::string code;
	};
private:
	Building* buildings_;											// first Building object in linked list of buildings
	std::vector<std::string> wreckedBuildings;					// vector to contain building codes of wrecked buildings
};


#endif