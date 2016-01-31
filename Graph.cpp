#include "Graph.h"
#include <string>
#include <iostream>

using namespace std;

Graph::NodeNotInCollectionException::NodeNotInCollectionException(string bcode) : code(bcode) {}		// constructor of exception class for building node not in the collection

string Graph::NodeNotInCollectionException::getcode() const {		// accessor of building code of exception class
	return code;
}

Graph::NodeAlreadyExistsException::NodeAlreadyExistsException(string bcode) : code(bcode) {}		// constructor of exception class for building node already existing in map

string Graph::NodeAlreadyExistsException::getcode() const {		// accessor of building code of exception class
	return code;
}

Graph::BuildingNotInMapException::BuildingNotInMapException(string bcode) : code(bcode) {}		// constructor of exception class for building node in the map
		
string Graph::BuildingNotInMapException::getcode() const {		// accessor of building code of exception class
	return code;
}

Graph::ConnectSelfNodeException::ConnectSelfNodeException(string bcode) : code(bcode) {}		// constructor of exception class for edge connecting to itself

string Graph::ConnectSelfNodeException::getcode() const {		// accessor of building code of exception class
	return code;
}

Graph::InvalidConnectorTypeException::InvalidConnectorTypeException(string contype) : type(contype) {}		// constructor of exception class for invalid type of edge

string Graph::InvalidConnectorTypeException::gettype() const {		// accessor of type of connector for exception class
	return type;
}

Graph::EdgeAlreadyExistException::EdgeAlreadyExistException(string bcode1, string bcode2) : code1(bcode1), code2(bcode2) {}		// constructor of exception class for an already exiting edge in map

string Graph::EdgeAlreadyExistException::getcode1() const {		// accessor for building code of first building
	return code1;
}

string Graph::EdgeAlreadyExistException::getcode2() const {		// accessor for building code of second building
	return code2;
}

Graph::EdgeNotInMapException::EdgeNotInMapException(string bcode1, string bcode2) : code1(bcode1), code2(bcode2) {}		// constructor for exception class of edge not found in the map

string Graph::EdgeNotInMapException::getcode1() const {		// accessor for building code of first building
	return code1;
}

string Graph::EdgeNotInMapException::getcode2() const {		// accessor for building code of second building
	return code2;
}

Graph::Graph() {											// Graph default constructor
	firstNode_ = NULL;
	firstPath_ = NULL;
}

Graph::~Graph() {											// Graph destructor
	deleteGraph();
}

Graph::Graph(const Graph& mapCopy) {						// Graph copy constructor - copies all building node and their edges
	if (mapCopy.firstNode_){
		firstNode_ = new Node;			// copy pointer to first node
		firstNode_ = mapCopy.firstNode_;
		Node* n1 = firstNode_;
		Node* n2 = new Node;
		n2 = mapCopy.firstNode_;
		while (n2 != NULL) {			// copies building nodes
			Connector* c1 = n2->firstConnector;
			Connector* c2 = new Connector;
			c2 = c1;
			while (c1 != NULL) {		// copies edges of node
				c2->nextItem = c1->nextItem;
				c1 = c1->nextItem;
				c2 = c2->nextItem;
			}
			n1->nextBuilding = n2->nextBuilding;
			n1 = n1->nextBuilding;
			n2 = n2->nextBuilding;
		}
	}
}

Graph::Node* Graph::constructNode(Building* building, Connector* first, Node* next) {	// Helper function - constructs a new building node based on information provided and returns the new node
	Node* n = new Node;
	n->building = building;
	n->nextBuilding = next;
	n->firstConnector = first;
	return n;
}

void Graph::addNode(Building* building){						// add a new building node to the graph
	if (building == NULL)
		throw NodeNotInCollectionException("");			// throws exception if building node is not found in collection
	else if (findNode(building->getbcode().getcode()) != NULL)
		throw NodeAlreadyExistsException(building->getbcode().getcode());		// throws exception if building node already exists in the map
	Node* n1 = firstNode_;
	if (firstNode_ == NULL) {		// if linked list is empty
		firstNode_ = constructNode(building, NULL, NULL);
	}
	else {
		if (building->getbcode().getcode().compare(n1->building->getbcode().getcode()) < 0) {		// insert in alphabetical order
			firstNode_ = constructNode(building, NULL, firstNode_);
		}
		else {		
			while (n1->nextBuilding != NULL) {
				if (building->getbcode().getcode().compare(n1->nextBuilding->building->getbcode().getcode()) < 0) {		// insert list in middle
					n1->nextBuilding = constructNode(building, NULL, n1->nextBuilding);
					break;
				}
				n1 = n1->nextBuilding;
			}
			if (n1->nextBuilding == NULL && building->getbcode().getcode().compare(n1->building->getbcode().getcode()) > 0) {	// if new node is located last in alphabetical order
				n1->nextBuilding = constructNode(building, NULL, NULL);
			}
		}
	}

}

void Graph::deleteConnector(Node* n) {							// helper function - deletes all edges from a building node
	Connector* c = n->firstConnector;
	while (n->firstConnector != NULL) {
		removeEdge(n->building->getbcode().getcode(), n->firstConnector->connectBuild->getbcode().getcode());
	}
	delete n->firstConnector;
}

void Graph::removeNode(string code) {							// removes a building node from graph
	if (findNode(code) == NULL)
		throw BuildingNotInMapException(code);			// throws exception if cannot find building node in map
	Node* n1 = firstNode_;
	if (n1 != NULL && n1->building->getbcode().getcode() != code) {
		while (n1->nextBuilding != NULL && n1->nextBuilding->building->getbcode().getcode() != code) {		// find node to delete
			n1 = n1->nextBuilding;
		}
		if (n1->nextBuilding->building->getbcode().getcode() == code) {		// deletes node
			Node* n2 = n1->nextBuilding;
			deleteConnector(n2);
			n1->nextBuilding = n1->nextBuilding->nextBuilding;
			delete n2;
		}
	}
	else {
		if (firstNode_ != NULL && firstNode_->building->getbcode().getcode() == code)		// if first node is to be deleted
		{
			Node * n2 = firstNode_;
			deleteConnector(n2);
			firstNode_ = firstNode_->nextBuilding;
			delete n2;
		}
	}
}

Building* Graph::findBuilding(string code) const {							// finds building object based on building code - returns building if found, otherwise returns NULL
	Node* n1 = new Node;
	n1 = firstNode_;
	while (n1->building->getbcode().getcode() != code && n1->nextBuilding != NULL) {		// loop through linked list until found or end of list is reached
		n1 = n1->nextBuilding;
	}
	if (n1->building->getbcode().getcode() == code) {
		return n1->building;
	}
	return NULL;
}

Graph::Node* Graph::findNode(string code) const {							// helper function - finds building node in graph - returns node if found, otherwise returns NULL
	Node* n = new Node;
	n = firstNode_;
	while (n != NULL && n->building->getbcode().getcode() != code) {
		n = n->nextBuilding;
	}
	return n;
}

Graph::Connector* Graph::findConnector(string code1, string code2) {			// helper function - finds connector in graph - returns connector if found, otherwise returns NULL
	Node* n1 = new Node;
	n1 = firstNode_;
	while (n1 != NULL) {
		if (n1->building->getbcode().getcode() == code1) {		// searches list of edges of building node with same code as code1
			Connector* c1 = new Connector;
			c1 = n1->firstConnector;
			while (c1 != NULL) {
				if (c1->connectBuild->getbcode().getcode() == code2)
					return c1;
				c1 = c1->nextItem;
			}
			if (c1 == NULL)
				return NULL;
		}
		else
			n1 = n1->nextBuilding;
	}
	return NULL;
}

void Graph::constructEdge(string code1, string code2, string type) {				// helper function - constructs a new edge in graph between two provided buildings
	Building* b1 = findBuilding(code1);
	Building* b2 = findBuilding(code2);
	Node* n = new Node;
	n = findNode(code1);
	Connector* c = new Connector;
	c->type = type;
	c->connectBuild = b2;
	c->nextItem = n->firstConnector;
	n->firstConnector = c;
}

void Graph::addEdge(string code1, string code2, string type) {						// adds connector edge between buildings to the graph
	string const connectortypes = "bridge, tunnel, hall";
	if (findNode(code1) == NULL)
		throw BuildingNotInMapException(code1);		// throws exception if cannot find building node1 in map
	else if (findNode(code2) == NULL)
		throw BuildingNotInMapException(code2);		// throws exception if cannot find building node2 in map
	else if (code1 == code2)
		throw ConnectSelfNodeException(code1);		// throws exception if building node1 is the same as building node2
	else if (connectortypes.find(type) == string::npos)
		throw InvalidConnectorTypeException(type);	// throws exception if edge type is invalid
	else if (findConnector(code1, code2) != NULL)
		throw EdgeAlreadyExistException(code1, code2);	// throws exception if edge already exists between the two building nodes
	constructEdge(code1, code2, type);
	constructEdge(code2, code1, type);		// building edge both ways
}

void Graph::deleteEdge(string code1, string code2) {								// helper function - delete edge from graph
	if (findNode(code1) == NULL)
		throw BuildingNotInMapException(code1);		// throw exception if cannot find building node1 in map
	else if (findNode(code2) == NULL)
		throw BuildingNotInMapException(code2);		// throw exception if cannot find building node2 in map
	else if (findConnector(code1, code2) == NULL)
		throw EdgeNotInMapException(code1, code2);	// throw exception if cannot find edge in map
	Building* b1 = findBuilding(code1);
	Building* b2 = findBuilding(code2);
	Node* n = new Node;
	n = findNode(code1);
	Connector* c = new Connector;
	c = n->firstConnector;
	if (c != NULL && c->connectBuild != b2) {
		while (c->nextItem != NULL && c->nextItem->connectBuild != b2) {		// seraches through list of connectors
			c = c->nextItem;
		}
		if (c->nextItem->connectBuild == b2) {
			Connector* c2 = c->nextItem;
			c->nextItem = c->nextItem->nextItem;
			delete c2;
		}
	}
	else {
		if (c != NULL && c->connectBuild == b2) {		// if first connector is to be deleted
			Connector * c2 = n->firstConnector;
			n->firstConnector = n->firstConnector->nextItem;
			delete c2;
		}
	}
}

void Graph::removeEdge(string code1, string code2) {						// removes edge from building1 to building2
	deleteEdge(code1, code2);
	deleteEdge(code2, code1);
}

void Graph::printPaths(string code1, string code2, const bool allpaths) const {			// prints path from building1 to building2
	if (findNode(code1) == NULL)
		throw BuildingNotInMapException(code1);		// throws exception if cannot find building node1 in map
	else if (findNode(code2) == NULL)
		throw BuildingNotInMapException(code2);		// throws exception if cannot find building node2 in map
	if (code1 == code2) {
		cout << code1 << endl;
	}
	else {
		cout << '\t';
		string types = "";		// used to stored the edge types of edges used in path
		formatPath(code1, code2, types);
		cout << endl;
	}
}

Graph::Node* Graph::formatPath(string code1, string code2, string types) const{			// helper function to find route (path)
	Node* n1 = new Node;
	Building* temp;
	Building* b1;
	n1 = findNode(code1);
	if (n1 != NULL && n1->building->getbcode().getcode() == code1) {		// finds building node of the current building
		if (firstPath_ == NULL) {		// adds building to path linked list if list is empty
			b1 = new Building(code1, n1->building->getname(), NULL);
			firstPath_ = b1;
		}
		else {		// if linked list storing path is not empty
			Building* b2 = firstPath_;
			if (b2->getbcode().getcode() == code1)		// return NULL if the current building is already in the list
				return NULL;
			while (b2->getnext() != NULL) {
				if (b2->getnext()->getbcode().getcode() == code1)		// return NULL if current building is in the list
					return NULL;
				b2 = b2->getnext();
			}
			b1 = new Building(code1, n1->building->getname(), NULL);	// if current building not in list, add it in the list
			b2->setnext(b1);
			temp = b1;		// set temp value to current building node added
		}
	}
	if (code1 == code2) {		// if current building is the destination of the path
		Building* b3;
		b3 = firstPath_;
		types += " ";
		while (b3 != NULL) {	// traverse through linked list to output each building that is a part of the path
			if (b3->getnext() == NULL)
				cout << b3->getbcode().getcode();		// output last node of the path
			else {
				cout << b3->getbcode().getcode() << " (" << getType(types) << ") ";		// output path
				types = types.replace(0, getType(types).size() + 1, "");		// updates string storing the types of the edges
			}
			b3 = b3->getnext();
		}
		cout << endl;
	}
	else {		// if current building is not the destination
		Node* n2 = new Node;
		n2 = findNode(code1);
		Connector* c1 = new Connector;
		c1 = n2->firstConnector;
		while (c1 != NULL) {		// look through list of edges of the current building
			types += c1->type + " ";		// adds the type of edge to the types variable to store the type
			if (formatPath(c1->connectBuild->getbcode().getcode(), code2, types) != NULL) {		// if the building that the edge connects to is not yet part of the path
				return firstNode_;		// recurse on the building that the edge connects to
			}
			c1 = c1->nextItem;
		}
		if (types.length() > 1)		// formats types variable to remove extra space
			types.erase(types.end() - 1);
		for (int i = types.length() - 1; i >= 0; i--) {		// remove the types that were added of buildings that were already added but are not actually part of the path
			if (types[i] != ' ') {
				types.erase(types.end() - 1);
			}
		}
		temp = NULL;		// reset temp, remove the building that was added to the path list but is not part of the path
		return NULL;
	}
}

string Graph::getType(string types) const{				// helper function to get type of connection between buildings when finding path
	string temp = "";
	int i = 0;
	while (types[i] != ' ') {		// retrieves the type of the edge from the string that stores it (delimited by space)
		temp += types[i];
		i++;
	}
	return temp;
}

void Graph::deleteGraph() {								// deletes all ndoes and edges on graph
	Node* n = firstNode_;
	while (n != NULL) {		// deletes all nodes
		Connector* c = n->firstConnector;
		while (c != NULL) {		// deletes all connectors
			n->firstConnector = c->nextItem;
			delete c;
			c = n->firstConnector;
		}
		delete n->firstConnector;
		firstNode_ = n->nextBuilding;
		delete n;
		n = firstNode_;
	}
	delete n;
}

ostream& operator<< (ostream& sout, const Graph& g1) {			// output operator - formats output of graph 
	Graph::Node* n1 = new Graph::Node;
	n1 = g1.firstNode_;
	while (n1 != NULL) {		// output all building nodes
		sout << n1->building->getbcode().getcode() << '\t' << n1->building->getname() << endl;
		Graph::Connector* c1 = new Graph::Connector;
		c1 = n1->firstConnector;
		if (c1 != NULL)
			sout << '\t' << "Connects to: ";
		while (c1 != NULL) {		// output all edges
			if (c1->nextItem == NULL)
				sout << c1->connectBuild->getbcode().getcode() << " (" << c1->type << ")" << endl;		// format output of last edge
			else
				sout << c1->connectBuild->getbcode().getcode() << " (" << c1->type << "), ";
			c1 = c1->nextItem;
		}
		n1 = n1->nextBuilding;
		sout << endl;
	}
	return sout;
}

Graph& Graph::operator= (const Graph& g1) {								// assignment operator - assigns all building nodes and edges
	if (this == &g1)
		return *this;
	deleteGraph();
	if (g1.firstNode_){			// copy g1 to this
		firstNode_ = new Node;
		firstNode_ = g1.firstNode_;
		Node* n1 = firstNode_;
		Node* n2 = new Node;
		n2 = g1.firstNode_;
		while (n2 != NULL) {		// copy all nodes
			Connector* c1 = n2->firstConnector;
			Connector* c2 = new Connector;
			c2 = c1;
			while (c1 != NULL) {		// copy all edges
				c2->nextItem = c1->nextItem;
				c1 = c1->nextItem;
				c2 = c2->nextItem;
			}
			n1->nextBuilding = n2->nextBuilding;
			n1 = n1->nextBuilding;
			n2 = n2->nextBuilding;
		}
	}
	return *this;		// return this - after assigning g1 to this
}

bool Graph::checkEqual(Node* node1, Node* node2) const {				// helper function - checks for equality between 2 graphs
	Node* n1 = new Node;
	Node* n2 = new Node;
	n1 = node1;
	n2 = node2;
	while (n1 != NULL && n2 != NULL && n1->building->getbcode().getcode() == n2->building->getbcode().getcode() && n1->building->getname() == n2->building->getname()) {		// if building nodes are equal, check their connectors
		Connector* c1 = new Connector;
		Connector* c2 = new Connector;
		c1 = n1->firstConnector;
		c2 = n2->firstConnector;
		while (c1 != NULL) {
			while (c2 != NULL) {
				if (c1->connectBuild->getbcode() != c2->connectBuild->getbcode() || c1->type != c2->type)		// if connector is not found, keep searching
					c2 = c2->nextItem;
				else				// if connector is found, break
					break;
			}
			if (c2 == NULL && c1 != NULL)		// if connector is not found, maps are not equal - return false
				return false;
			c1 = c1->nextItem;
			c2 = n2->firstConnector;
		}
		n1 = n1->nextBuilding;				// check next building node
		n2 = n2->nextBuilding;				
	}
	return (n1 == NULL && n2 == NULL);		// if both building nodes are empty - they are equal and return true, otherwise, return false
}

bool Graph::operator== (const Graph& g1) const {					// equality operator
	return (checkEqual(firstNode_, g1.firstNode_) && checkEqual(g1.firstNode_, firstNode_));		// check both ways if they are equal or not
}
