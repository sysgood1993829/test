#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <fstream>

#define DIS_INF 2147483647

using namespace std;

class Node;

class Edge{

	public:
		Edge(Node *a, Node *b);
		Node* node[2];

		Node *getNeighbor(Node *n);

		bool operator < (const Edge& rhs) const;

};

class Node{

	public:
		Node(const int& i);
		void addEdge(Edge *e);
		void sortEdge();

		int id;
        int color;
		bool traveled;
		vector<Edge *> edge;

		int d;
		Node *prev;
		int heap_idx;
};

class Graph{

	public:
		Graph(const string& n);
		~Graph();
		
		void addEdge(const int& v1, const int& v2);
		void sortEdgesOfNode();
        void sortNodesByDegree();
        void sortNodesByID();
		void init();
		Node * getNodeById(const int& id);

		map<int, Node *> nodesMap;
        vector<Node *> nodes;
		vector<Edge *> edges;
		string name;
		vector< vector<int> > group;
		vector< vector<double> > coordinate;//ith node 0:x1 1:y1 2:x2 3:y2 4:nodes index
		int x_limit;
		int y_limit;
		int windows_size;
};
extern Graph *graph;
#endif
