#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <iostream>
#include "Collection.h"

class Graph : public Collection {
public:
	Graph();
	~Graph();                                               // destructor
	Graph(const Graph&);                                 // copy constructor
	void addNode(Building*);                             // mutator - add node to graph
	void removeNode(std::string);                             // mutator - remove node from graph
	Building* findBuilding(std::string) const;                // accessor - find building stored in node in graph
	void addEdge(std::string, std::string, std::string);                // mutator - add edge to graph
	void removeEdge(std::string, std::string);                     // mutator - remove edge from graph
	void printPaths(std::string, std::string, const bool = false) const; // accessor - print path from one node to another
	void deleteGraph();                                     // delete graph
	friend std::ostream& operator<< (std::ostream&, const Graph&);  // insertion operator (insert graph into output stream)
	Graph& operator= (const Graph&);                      // assignment operator for graph objects
	bool operator== (const Graph&) const;                 // equality operator for graph objects
	class NodeNotInCollectionException {				// exception class for building nodes not found in the collection
	public:
		NodeNotInCollectionException(std::string);
		std::string getcode() const;
	private:
		std::string code;
	};
	class NodeAlreadyExistsException {				// exception class for building node already exist in map
	public:
		NodeAlreadyExistsException(std::string);
		std::string getcode() const;
	private:
		std::string code;
	};
	class BuildingNotInMapException {				// exception class for building node not in map
	public:
		BuildingNotInMapException(std::string);
		std::string getcode() const;
	private:
		std::string code;
	};
	class ConnectSelfNodeException {				// exception class for edge connecting to and from the same building
	public:
		ConnectSelfNodeException(std::string);
		std::string getcode() const;
	private:
		std::string code;
	};
	class InvalidConnectorTypeException {			// exception class for invalid type of edge
	public:
		InvalidConnectorTypeException(std::string);
		std::string gettype() const;
	private:
		std::string type;
	};
	class EdgeAlreadyExistException {				// exception class for edge already existing in map
	public:
		EdgeAlreadyExistException(std::string, std::string);
		std::string getcode1() const;
		std::string getcode2() const;
	private:
		std::string code1, code2;
	};
	class EdgeNotInMapException {					// exception class for map not found in map
	public:
		EdgeNotInMapException(std::string, std::string);
		std::string getcode1() const;
		std::string getcode2() const;
	private:
		std::string code1, code2;
	};
private:
	struct Connector {										// Connector struct - represents connectors of each building in graph in a linked list
		Connector* nextItem;
		Building* connectBuild;
		std::string type;
	};
	struct Node {											// Node struct - represents nodes on the graph in a linked list
		Node* nextBuilding;
		Building* building;
		Connector* firstConnector;
	};
	Node* firstNode_;										// Pointer to the first node of the graph
	mutable Building* firstPath_;							// Pointer to the first node of the linked list representing route found
	Node* constructNode(Building*, Connector*, Node*);		// Helper function to construct a new node in the graph
	void deleteConnector(Node*);							// Helper function to delete the connectors of a node
	void constructEdge(std::string, std::string, std::string);				// Helper function to construct an edge connector
	Connector* findConnector(std::string, std::string);		// Helper function to find an edge
	void deleteEdge(std::string, std::string);						// Helper function to delete an edge
	Node* formatPath(std::string, std::string, std::string) const;			// Recursive function to find the route between buildings
	Node* findNode(std::string) const;							// Helper function to find a node in the map based on a building code
	bool checkEqual(Node*, Node*) const;					// Helper function to check for equality of two graphs
	std::string getType(std::string) const;							// Helper function to get the type of the connector edge		
};

#endif
