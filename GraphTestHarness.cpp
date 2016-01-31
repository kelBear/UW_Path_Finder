
#include <iostream>
#include <fstream>
#include <string>
#include "Graph.h"

using namespace std;

//  test-harness operators
enum Op { NONE, mapPtr, building, wreckage, findB, node, remNode, edge, remEdge, delGraph, copyGraph, assignGraph, eq, path, print };

Op convertOp(string opStr) {
	switch (opStr[0]) {
	case 'm': return mapPtr;
	case 'b': return building;
	case 'w': return wreckage;
	case 'f': return findB;
	case 'n': return node;
	case 'v': return remNode;
	case 'e': return edge;
	case 'r': return remEdge;
	case 'd': return delGraph;
	case 'c': return copyGraph;
	case 'a': return assignGraph;
	case 'q': return eq;
	case 'p': return path;
	case 'g': return print;
	default: {
		return NONE;
	}
	}
}

//******************************************************************
// Test Harness for Graph ADT
//******************************************************************

int main(int argc, char *argv[]) {
	Collection buildings;
	Graph map1, map2;
	string num = "1";

	// initialize buildings and map1 with input file, if present
	if (argc > 1) {

		ifstream source(argv[1]);
		if (source.fail()) {
			cerr << "Error: Could not open file \"" << argv[1] << "\"." << endl;
			return 1;
		}

		// create a collection of buildings, and a map of buildings and interior links

		string type;
		source >> type;
		Op op = convertOp(type);
		while (!source.eof()) {
			switch (op) {

				// add a new building to the collection of Buildings, and add the building to map1
			case building: {
				string code;
				string name;
				string name2;
				source >> code >> name;
				getline(source, name2);
				try {
					buildings.insert(code, name + name2);
					map1.addNode(buildings.findBuilding(code));
				}
				catch (BCode::InvalidBCodeException ex) {		//catch exception of invalid building code format
					cout << ex.getmessage();
					cout << "This command results in no changes to the collection of buildings or to maps." << endl;
				}
				catch (Collection::ExistingBuildingCodeException ex) {		// catch exception of building code already existing in the collection
					cout << "\nERROR: Building Code \"" + ex.getcode() + "\" is already in use." << endl;
					cout << "This command results in no changes to the collection of buildings or to maps." << endl;
				}
				catch (Collection::FormerExistingCodeException ex) {		// catch exception of building code used by a former building in collection
					cout << "\nERROR: Building Code \"" + ex.getcode() + "\" was used for a former building." << endl;
					cout << "This command results in no changes to the collection of buildings or to maps." << endl;
				}
				break;
			}

				// add a new link between two existing nodes in map1
			case edge: {
				string code1, code2, type;
				source >> code1 >> code2 >> type;
				try {
					map1.addEdge(code1, code2, type);
				}
				catch (Graph::BuildingNotInMapException ex) {		// catch exception of building of edge not in the map/graph
					cout << "\nERROR: There is no building \"" + ex.getcode() + "\" in map1 to use in the new edge." << endl;
					cout << "This command results in no changes to map1." << endl;
				}
				catch (Graph::ConnectSelfNodeException ex) {		// catch exception of edge connecting to itself
					cout << "\nERROR: Cannot connect node \"" + ex.getcode() + "\" to itself." << endl;
					cout << "This command results in no changes to map1." << endl;
				}
				catch (Graph::InvalidConnectorTypeException ex) {		// catch exception of invalid type of edge
					cout << "\nERROR: There is no building connector of type \"" + ex.gettype() + "\"." << endl;
					cout << "This command results in changes to map1." << endl;
				}
				catch (Graph::EdgeAlreadyExistException ex) {			// catch exception of edge already existing in map
					cout << "\nERROR: There is already an edge between \"" + ex.getcode1() + "\" and \"" + ex.getcode2() + "\" in map1." << endl;
					cout << "This command results in no changes to map1." << endl;
				}
				string junk;
				getline(source, junk);
				break;
			}

			default: {}
			}
			source >> type;
			op = convertOp(type);
		}
	}

	cout << map1;

	Graph* map = &map1;  // input commands affect which ever graph that map points to (map1 or map2)

	cout << "Test harness for Graph ADT:" << endl << endl;

	// get input command
	cout << "Command: ";
	string command;
	cin >> command;

	Op op = convertOp(command);

	while (!cin.eof()) {
		switch (op) {

			// set variable map to point to new graph (map1 or map2)
		case mapPtr: {
			string mapNo;
			cin >> mapNo;
			map = (mapNo[0] == '1') ? &map1 : &map2;
			num = (mapNo[0] == '1') ? "1" : "2";
			break;
		}

			// print the current map to the console
		case print: {
			cout << *map;
			break;
		}

			// add a new building to the collection of buildings
		case building: {
			string code;
			string name;
			string name2;
			bool first = true;
			cin >> code >> name;
			getline(cin, name2);
			while (true) {
				try {
					if (!first) cin >> code;			// reads in new building code if previous input was invlalid
					if (cin.fail()) break;
					first = false;
					buildings.insert(code, name + name2);
					break;
				}
				catch (BCode::InvalidBCodeException ex) {		// catch exception of invalid building code format
					cout << ex.getmessage();
					cout << "Please enter a new building code: ";
				}
				catch (Collection::ExistingBuildingCodeException ex) {		// catch exception of building code already existing in collection
					cout << "\nERROR: Building Code \"" + ex.getcode() + "\" is already in use." << endl;
					cout << "Please enter a new building code: ";
				}
				catch (Collection::FormerExistingCodeException ex) {		// catch exception of building code used by former building in collection
					cout << "\nERROR: Building Code \"" + ex.getcode() + "\" was used for a former building." << endl;
					cout << "Please enter a new building code: ";
				}
			}
			break;
		}

			// add an existing building to the current map
		case node: {
			string code;
			cin >> code;
			try {
				map->addNode(buildings.findBuilding(code));
			}
			catch (Graph::NodeNotInCollectionException ex){		// catch exception of building node not in the collection
				cout << "\nERROR: There is no building with the code \"" + code + "\"." << endl;
				cout << "This command results in no changes to map" + num + "." << endl;
			}
			catch (Graph::NodeAlreadyExistsException ex) {		// catch exception of building node already existing in the current map
				cout << "\nERROR: There is already a node for building \"" + ex.getcode() + "\" in map" + num + "." << endl;
				cout << "This command results in no changes to map" + num + "." << endl;
			}
			string junk;
			getline(cin, junk);
			break;
		}

			// find a building in the current map
		case findB: {
			string code;
			cin >> code;
			Building *b = map->findBuilding(code);
			if (b) {
				cout << *b << endl;
			}
			else {
				cout << "Couldn't find building " << code << endl;
			}
			string junk;
			getline(cin, junk);
			break;
		}

			// add a new link between existing graph nodes in the current map
		case edge: {
			string code1, code2, type;
			cin >> code1 >> code2 >> type;
			try {
				map->addEdge(code1, code2, type);
			}
			catch (Graph::BuildingNotInMapException ex) {		// catch exception of building of edge not in current map
				cout << "\nERROR: There is no building \"" + ex.getcode() + "\" in map" + num + " to use in the new edge." << endl;
				cout << "This command results in no changes to map" + num + "." << endl;
			}
			catch (Graph::ConnectSelfNodeException ex) {		// catch exception of edge connecting to itself
				cout << "\nERROR: Cannot connect node \"" + ex.getcode() + "\" to itself." << endl;
				cout << "This command results in no changes to map" + num + "." << endl;
			}
			catch (Graph::InvalidConnectorTypeException ex) {		// catch exception of invalid type of edge
				cout << "\nERROR: There is no building connector of type \"" + ex.gettype() + "\"." << endl;
				cout << "This command results in changes to map" + num +"." << endl;
			}
			catch (Graph::EdgeAlreadyExistException ex) {		// catch exception of edge already existing in the map
				cout << "\nERROR: There is already an edge between \"" + ex.getcode1() + "\" and \"" + ex.getcode2() + "\" in map" + num + "." << endl;
				cout << "This command results in no changes to map" + num + "." << endl;
			}
			string junk;
			getline(cin, junk);
			break;
		}


			// delete the entire graph (no memory leak).  There is no change to the collection of Buildings.
		case delGraph: {
			map->deleteGraph();
			break;
		}

			// remove an existing edge from the current map
		case remEdge: {
			string code1, code2;
			cin >> code1 >> code2;
			try {
				map->removeEdge(code1, code2);
			}
			catch (Graph::BuildingNotInMapException ex) {		// catch exception of building node in the map
				cout << "\nERROR: There is no building \"" + ex.getcode() + "\" in map" + num + "." << endl;
				cout << "This command results in no changes to map" + num + "." << endl;
			}
			catch (Graph::EdgeNotInMapException ex) {		// catch exception of edge not in the map
				cout << "\nERROR: There is no edge in map" + num + " between buildings \"" + ex.getcode1() + "\" and \"" + ex.getcode2() + "\" to be removed." << endl;
				cout << "This command results in no changes to map" + num + "." << endl;
			}
			string junk;
			getline(cin, junk);
			break;
		}

			// remove an existing node from the current map.  There is no change to the collection of Buildings.
		case remNode: {
			string code;
			cin >> code;
			try {
				map->removeNode(code);
			}
			catch (Graph::BuildingNotInMapException ex) {		// catch exception of building node not in the map
				cout << "\nERROR: There is no building \"" + ex.getcode() + "\" in map" + num + " to be removed." << endl;
				cout << "This command results in no changes to map" + num + "." << endl;
			}
			string junk;
			getline(cin, junk);
			break;
		}

			// remove an existing building from the collection of buildings.  The building also needs to be removed from the two maps, as well as all links involving the building
		case wreckage: {
			string code;
			cin >> code;
			try {
				map1.removeNode(code);
				map2.removeNode(code);
			}
			catch (Graph::BuildingNotInMapException ex) {}		// catch exception if building node not in map - do nothing
			try {
				buildings.remove(code);
			}
			catch (Collection::BuildingNotInCollectionException ex) {		// catch exception of building node not in the collection
				cout << "\nERROR: There is no building with the code \"" + ex.getcode() + "\"." << endl;
				cout << "This command results in no changes to the collection of buildings or to maps." << endl;
			}
			string junk;
			getline(cin, junk);
			break;
		}

			// check whether map1 is equal to map2
		case eq: {
			if (map1 == map2) {
				cout << "Maps 1 and 2 are equal." << endl;
			}
			else {
				cout << "Maps 1 and 2 are NOT equal." << endl;
			}
			break;
		}

			// graph copy constructor
		case copyGraph: {
			Graph map3(*map);
			cout << map3;
			string junk;
			getline(cin, junk);
			break;
		}

			// graph assignment operator
		case assignGraph: {
			map1 = map2;
			cout << map1;
			break;
		}

			// find path(s) in graph from one building to another building
		case path: {
			string code1, code2, all;
			cin >> code1 >> code2 >> all;
			cout << "Paths from " << code1 << " to " << code2 << " are: " << endl;
			bool printall = (all.length() > 0 && all.at(0) == 't') ? true : false;
			try {
				map->printPaths(code1, code2, printall);
			}
			catch (Graph::BuildingNotInMapException ex) {		// catch exception of building node not in the map
				cout << "\nERROR: There is no building \"" + ex.getcode() + "\" in map" + num + "." << endl;
			}
			string junk;
			getline(cin, junk);
			break;
		}

		default: {
			cerr << "Invalid command." << endl;
		}
		}

		cout << endl << "Command: ";
		cin >> command;
		op = convertOp(command);

	} // while cin OK

}
