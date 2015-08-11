// **************************************************************************
// File       [ test_cmd.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#ifndef _TEST_CMD_H_
#define _TEST_CMD_H_

#include "../lib/cmd.h"
#include "graph.h"

namespace CommonNs {

class TestCmd : public CommonNs::Cmd {
public:
         TestCmd(const char * const name);
         ~TestCmd();

    bool exec(int argc, char **argv);
};

class ReadGraph : public Cmd {
public:
	ReadGraph(const char * const name);
	~ReadGraph();
	
	bool exec(int argc, char **argv);
};

class DFS : public Cmd {
public:
	DFS(const char * const name);
	~DFS();
	
	bool exec(int argc, char **argv);
	void dfsvisit(Graph&, Node*, vector<int> &, fstream&);
};

class BFS : public Cmd {
public:
	BFS(const char * const name);
	~BFS();
	
	bool exec(int argc, char **argv);
};

class Coloring : public Cmd {
public:
	Coloring(const char * const name);
	~Coloring();
	
	bool exec(int argc, char **argv);
};

};

#endif
