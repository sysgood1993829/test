// **************************************************************************
// File       [ test_cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#include "user_cmd.h"
#include "graph.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include <queue>
#include "../lib/tm_usage.h"
using namespace std;
using namespace CommonNs;
char G[30];

TestCmd::TestCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("test");
    optMgr_.setDes("test");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ, "print the string of -s", "[string]");
    opt->addFlag("s");
    optMgr_.regOpt(opt);
}

TestCmd::~TestCmd() {}

bool TestCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (optMgr_.getParsedOpt("s")) {
        printf("%s\n", optMgr_.getParsedValue("s"));
    }
    else
        printf("hello world !!\n");


    return true;
}

ReadGraph::ReadGraph(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("read graph");
    optMgr_.setDes("read graph from a dot file");
	optMgr_.regArg(new Arg(Arg::OPT, "file name of the graph.", "FILENAME"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

ReadGraph::~ReadGraph() {}

bool ReadGraph::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }
	else if(argc == 3)
	{
		//printf("aaaa\n");
		ifstream fin(argv[1]);
        string buff;
        int line_idx = 0;
		CommonNs::TmUsage tmusg;
		CommonNs::TmStat stat;
		tmusg.periodStart();
        //Graph G(argv[1]);
		//char* delim = " ";
		string n = argv[1];//cout<<n<<endl;
		if(graph) delete graph;//maintain the repeated usage
		graph = new Graph(n);
        while(getline(fin,buff)){
            if(line_idx<3)
			{
				int temp=0;
				if(buff[0]=='A')
				{
					for(int i=6;i<buff.length();i++)
					{	
						temp = temp*10;
						temp = temp + int(buff[i])-48;
					}					
					graph->x_limit = temp;
				}
				else if(buff[0]=='B')
				{
					for(int i=5;i<buff.length();i++)
					{	
						temp = temp*10;
						temp = temp + int(buff[i])-48;
					}					
					graph->y_limit = temp;
				}
				else
				{
					for(int i=6;i<buff.length();i++)
					{	
						temp = temp*10;
						temp = temp + int(buff[i])-48;
					}					
					graph->windows_size = temp;
				}
			}
			else
			{
				vector<double> cor;
				int temp =0;
				bool nega = false;
				for(int i=0;i<buff.length();i++)
				{	
					if(buff[i]!=',' && buff[i]!='-')
					{					
						temp = temp*10;
						temp = temp + int(buff[i])-48;
					}
					else if(buff[i]==',')
					{
						if(nega)
							cor.push_back(-1*temp);
						else
							cor.push_back(temp);
						nega = false;
						temp = 0;
					}
					else
					{
						nega = true;
					}
				}	
				cor.push_back(temp);
				temp = 0;
				graph->coordinate.push_back(cor);
			}
            line_idx++;
        }
		//debug check
		/*cout<<graph->x_limit<<" "<<graph->y_limit<<" "<<graph->windows_size<<endl;
		for(int i=0;i<graph->coordinate.size();i++)
			cout<<graph->coordinate[i][0]<<","<<graph->coordinate[i][1]<<","<<graph->coordinate[i][2]<<","<<graph->coordinate[i][3]<<" "<<graph->coordinate[i].size()<<endl;
		*/		
		//graph->init();//initial the graph
    	//graph->sortEdgesOfNode();
		//cout<<graph->nodes.size()<<" "<<graph->edges.size()<<endl;
	
		//*******construct graph here*******//
		//cout<<argv[2]<<endl;
		string method(argv[2]);
		if(method == "b")
		{
			for(int i=0;i<graph->coordinate.size();i++)
			{
				for(int j=i+1;j<graph->coordinate.size();j++)
				{
					double middle1x = 0;
					double middle1y = 0;
					double middle2x = 0;
					double middle2y = 0;
					double length1x = 0;
					double length2x = 0;
					double length1y = 0;
					double length2y = 0;
					middle1x = (graph->coordinate[i][0]+graph->coordinate[i][2])/2;
					middle1y = (graph->coordinate[i][1]+graph->coordinate[i][3])/2;
					middle2x = (graph->coordinate[j][0]+graph->coordinate[j][2])/2;
					middle2y = (graph->coordinate[j][1]+graph->coordinate[j][3])/2;
					length1x = (-graph->coordinate[i][0])+graph->coordinate[i][2];
					length1y = (-graph->coordinate[i][1])+graph->coordinate[i][3];
					length2x = (-graph->coordinate[j][0])+graph->coordinate[j][2];
					length2y = (-graph->coordinate[j][1])+graph->coordinate[j][3];
					if(abs(middle1x-middle2x)<length1x/2+length2x/2)
					{//cout<<abs(middle1x-middle2x)<<" "<<i<<" "<<j<<endl;
						if(abs(middle1y-middle2y)<length1y/2+length2y/2+graph->y_limit)
						{
							graph->addEdge(i,j);
						}
					}
					else if(abs(middle1y-middle2y)<length1y/2+length2y/2)
					{
						if(abs(middle1x-middle2x)<length1x/2+length2x/2+graph->x_limit)
						{
							graph->addEdge(i,j);
						}
					}
				}
			}
		}
		else if(method == "g")
		{
			int minx=graph->coordinate[0][0];
			int miny=graph->coordinate[0][1];
			int maxx=0;
			int maxy=0;
			//find the max and min x,y coordinate
			for(int i=0;i<graph->coordinate.size()/2;i++)
			{
				//find x
				if(graph->coordinate[2*i][0] > graph->coordinate[2*i+1][0])
				{
					if(graph->coordinate[2*i+1][0] < minx)
						minx = graph->coordinate[2*i+1][0];
				}
				else
				{
					if(graph->coordinate[2*i][0] < minx)
						minx = graph->coordinate[2*i][0];
				}
				if(graph->coordinate[2*i][2] > graph->coordinate[2*i+1][2])
				{
					if(graph->coordinate[2*i][2] > maxx)
						maxx = graph->coordinate[2*i+1][2];
				}
				else
				{
					if(graph->coordinate[2*i+1][2] > maxx)
						maxx = graph->coordinate[2*i+1][2];
				}
				//find y
				if(graph->coordinate[2*i][1] > graph->coordinate[2*i+1][1])
				{
					if(graph->coordinate[2*i+1][1] < miny)
						miny = graph->coordinate[2*i+1][1];
				}
				else
				{
					if(graph->coordinate[2*i][1] < miny)
						miny = graph->coordinate[2*i][1];
				}
				if(graph->coordinate[2*i][3] > graph->coordinate[2*i+1][3])
				{
					if(graph->coordinate[2*i][3] > maxy)
						maxy = graph->coordinate[2*i][3];
				}
				else
				{
					if(graph->coordinate[2*i+1][3] > maxy)
						maxy = graph->coordinate[2*i+1][3];
				}
			}
			if(graph->coordinate.size()%2 == 1)
			{
				if(graph->coordinate[graph->coordinate.size()-1][0] < minx)
						minx = graph->coordinate[graph->coordinate.size()-1][0];
				if(graph->coordinate[graph->coordinate.size()-1][1] < miny)
						minx = graph->coordinate[graph->coordinate.size()-1][1];
				if(graph->coordinate[graph->coordinate.size()-1][2] > maxx)
						maxx = graph->coordinate[graph->coordinate.size()-1][2];
				if(graph->coordinate[graph->coordinate.size()-1][3] > maxy)
						maxy = graph->coordinate[graph->coordinate.size()-1][3];
			}
			//cout<<minx<<" "<<miny<<" "<<maxx<<" "<<maxy<<endl;
			//chop the whole plain into many windowses
			vector< vector< vector<int> > > map;
			int wide = (maxx-minx)/graph->x_limit;
			int height = (maxy-miny)/graph->y_limit;
			//cout<<wide<<" "<<height<<endl;
			map.resize(wide+2);
			for(int i =0;i<wide+1;i++)
				map[i].resize(height+2);
			//put the chips number into the windows' lattice
			
			for(int j=0;j<graph->coordinate.size();j++)
            {
                int smallx = graph->coordinate[j][0];
                int smally = graph->coordinate[j][1];//cout<<j<<" gg here"<<endl;
				if((smallx-minx)/graph->x_limit == 0)
				{
		            for(int ii = (smallx-minx)/graph->x_limit;ii<=(graph->coordinate[j][2]-minx)/graph->x_limit;ii++)
		            {//cout<<ii<<endl;
						if((smally-miny)/graph->y_limit == 0)
		                	for(int kk= (smally-miny)/graph->y_limit;kk<=(graph->coordinate[j][3]-miny)/graph->y_limit;kk++)
		                {//cout<<kk<<endl;
		                    map[ii][kk].push_back(j);
		                }
						else
							for(int kk= -1+(smally-miny)/graph->y_limit;kk<=(graph->coordinate[j][3]-miny)/graph->y_limit;kk++)
		                {//cout<<kk<<endl;
		                    map[ii][kk].push_back(j);
		                }
		            }
				}
				else
				{
					for(int ii = -1+(smallx-minx)/graph->x_limit;ii<=(graph->coordinate[j][2]-minx)/graph->x_limit;ii++)
		            {
		                if((smally-miny)/graph->y_limit == 0)
		                	for(int kk= (smally-miny)/graph->y_limit;kk<=(graph->coordinate[j][3]-miny)/graph->y_limit;kk++)
		                {
		                    map[ii][kk].push_back(j);
		                }
						else
							for(int kk= -1+(smally-miny)/graph->y_limit;kk<=(graph->coordinate[j][3]-miny)/graph->y_limit;kk++)
		                {
		                    map[ii][kk].push_back(j);
		                }
		            }
				}
            }
			//cout<<"gg"<<endl;
			//debug use
/*
			for(int ii=0;ii<map.size();ii++)
			{
				for(int jj=0;jj<map[ii].size();jj++)
				{
					cout<<map[ii][jj].size()<<" ";
					//for(int kk=0;kk<map[ii][jj].size();kk++)
						//cout<<map[ii][jj][kk]<<" ";
				}
				cout<<endl;
			}
*/
			//compare the chips in the same lattice
			for(int ii=0;ii<map.size();ii++)
			{
				for(int jj=0;jj<map[ii].size();jj++)
				{
					if(map[ii][jj].size()<2)
						continue;
					else
					{
						for(int i=0;i<map[ii][jj].size();i++)
						{
							for(int j=i+1;j<map[ii][jj].size();j++)
							{
								double middle1x = 0;
								double middle1y = 0;
								double middle2x = 0;
								double middle2y = 0;
								double length1x = 0;
								double length2x = 0;
								double length1y = 0;
								double length2y = 0;
								middle1x = (graph->coordinate[map[ii][jj][i]][0]+graph->coordinate[map[ii][jj][i]][2])/2;
								middle1y = (graph->coordinate[map[ii][jj][i]][1]+graph->coordinate[map[ii][jj][i]][3])/2;
								middle2x = (graph->coordinate[map[ii][jj][j]][0]+graph->coordinate[map[ii][jj][j]][2])/2;
								middle2y = (graph->coordinate[map[ii][jj][j]][1]+graph->coordinate[map[ii][jj][j]][3])/2;
								length1x = (-graph->coordinate[map[ii][jj][i]][0])+graph->coordinate[map[ii][jj][i]][2];
								length1y = (-graph->coordinate[map[ii][jj][i]][1])+graph->coordinate[map[ii][jj][i]][3];
								length2x = (-graph->coordinate[map[ii][jj][j]][0])+graph->coordinate[map[ii][jj][j]][2];
								length2y = (-graph->coordinate[map[ii][jj][j]][1])+graph->coordinate[map[ii][jj][j]][3];
								if(abs(middle1x-middle2x)<length1x/2+length2x/2)
								{//cout<<abs(middle1x-middle2x)<<" "<<i<<" "<<j<<endl;
									if(abs(middle1y-middle2y)<length1y/2+length2y/2+graph->y_limit)
									{
										graph->addEdge(map[ii][jj][i],map[ii][jj][j]);
									}
								}
								else if(abs(middle1y-middle2y)<length1y/2+length2y/2)
								{
									if(abs(middle1x-middle2x)<length1x/2+length2x/2+graph->x_limit)
									{
										graph->addEdge(map[ii][jj][i],map[ii][jj][j]);
									}
								}
							}
						}
					}
				}
			}

		}
		cout<<graph->coordinate.size()<<" "<<graph->nodes.size()<<" "<<graph->edges.size()<<endl;
		graph->init();//initial the graph
		graph->sortEdgesOfNode();
		tmusg.getPeriodUsage(stat);
		cout<<"// runtime = "<<(stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
		cout<<"// memory = "<<stat.vmPeak / 1000.0 << "MB" << endl;	
	}
	else
		printf("hello world !!\n");

    return true;
}

DFS::DFS(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("DFS");
    optMgr_.setDes("DFS a graph");
	optMgr_.regArg(new Arg(Arg::OPT, "file name of the DFS tree. ", "FILENAME"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

DFS::~DFS() {}

bool DFS::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

	fstream fout;
	fout.open(argv[1], ios::out);
    if (!fout) {
        fprintf(stderr, "**ERROR DFS::exec(): ");
        fprintf(stderr, "file cannot be opened\n");
        return false;
    }

	graph->init();
	fout << "// DFS tree produced by graphlab" << endl;
	fout << "graph " << graph->name << "_dfs {" << endl;

	int g = 0;
	for(vector<Node *>::iterator i = graph->nodes.begin(); i != graph->nodes.end(); ++i){
		if(!(*i)->traveled){
			vector<int> list;
			g++;
			(*i)->color = 1;
			(*i)->d = g;
			dfsvisit(*graph, *i, list, fout);
			if((*i)->d != 0)
				graph->group.push_back(list);
		}
	}

	for(vector<Node *>::iterator i = graph->nodes.begin(); i != graph->nodes.end(); ++i)
		fout << "v" << (*i)->id << " [label = \"v" << (*i)->id << "_color" << (*i)->d << "\"];" << endl;

	fout << "}" << endl;
	fout.close();
    return true;
}

void DFS::dfsvisit(Graph& graph, Node* n, vector<int> & list, fstream& fout){
	list.push_back(n->id);
	n->traveled = true;
	n->sortEdge();
	Node* n2;
	for(vector<Edge *>::iterator i = n->edge.begin(); i != n->edge.end(); ++i){
		n2 = (*i)->getNeighbor(n);
		if(!(n2->traveled)){
			n2->color = 3 - (n->color);
			n2->d = n->d;
			fout << "v" << n->id << " -- v" << n2->id << ";" << endl;
			dfsvisit(graph, n2, list, fout);
			if(n2->d == 0)
				n->d = 0;
		}else if(n2->color == n->color){
			n2->d = n->d = 0;
		}
	}
}

BFS::BFS(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("BFS");
    optMgr_.setDes("BFS a graph");
	optMgr_.regArg(new Arg(Arg::OPT, "file name of the BFS tree. ", "FILENAME"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

BFS::~BFS() {}

bool BFS::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

	if (argc < 5) {
        fprintf(stderr, "**ERROR BFS::exec(): ");
        fprintf(stderr, "not enough arguments\n");
        return false;
    }
	
	string source(argv[2]);
	int sid = 0;
	for(int i = 1; i < source.size(); i++)
		sid = sid * 10 + (source[i] - '0');
	fstream fout;
	fout.open(argv[4], ios::out);
    if (!fout) {
        fprintf(stderr, "**ERROR BFS::exec(): ");
        fprintf(stderr, "file cannot be opened\n");
        return false;
    }
	
	CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;

	graph->init();
	tmusg.periodStart();
	fout << "// BFS tree produced by graphlab" << endl;
	fout << "graph " << graph->name << "_bfs {" << endl;
	queue<Node*> q;
	Node *s, *n, *n2;
	s = graph->getNodeById(sid);
	s->traveled = true;
	q.push(s);
	while(!q.empty()){
		n = q.front();
		n->sortEdge();
		for(vector<Edge *>::iterator i = n->edge.begin(); i != n->edge.end(); ++i){
			n2 = (*i)->getNeighbor(n);
			if(!(n2->traveled)){
				n2->traveled = true;
				q.push(n2);
				fout << "v" << n->id << " -- v" << n2->id << ";" << endl;
			}
		}
		q.pop();
	}
	fout << "}" << endl;
	tmusg.getPeriodUsage(stat);

	int v = graph->nodes.size();
	fout <<"// vertices = " << v << endl;
	fout <<"// edges = " << (v - 1) << endl;
	fout <<"// runtime = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    fout <<"// memory = " << stat.vmPeak / 1000.0 << "MB" << endl;
	fout.close();
    return true;
}

Coloring::Coloring(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("coloring");
    optMgr_.setDes("color a graph");
	optMgr_.regArg(new Arg(Arg::OPT, "file name of the colored graph-> ", "FILENAME"));
    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);
}

Coloring::~Coloring() {}

bool Coloring::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

	if (argc < 2) {
        fprintf(stderr, "**ERROR Coloring::exec(): ");
        fprintf(stderr, "not enough arguments\n");
        return false;
    }

	fstream fout;
	fout.open(argv[1], ios::out);
    if (!fout) {
        fprintf(stderr, "**ERROR Coloring::exec(): ");
        fprintf(stderr, "file cannot be opened\n");
        return false;
    }
	
	//for(vector< vector<double> >::iterator i = graph->coordinate.begin(); i != graph->coordinate.end(); ++i){
	//	fout << (*i)[0] << " " << (*i)[1] << " " << (*i)[2] << " " << (*i)[3] << endl;
	//}
	int M = 100000;
	int bound[4] = {M, M, -M, -M};
	for(vector< vector<int> >::iterator i = graph->group.begin(); i != graph->group.end(); ++i){
		for(vector<int>::iterator j = (*i).begin(); j != (*i).end(); ++j){
			if(bound[0] > graph->coordinate[*j][0])
				bound[0] = graph->coordinate[*j][0];
			if(bound[1] > graph->coordinate[*j][1])
				bound[1] = graph->coordinate[*j][1];
			if(bound[2] < graph->coordinate[*j][2])
				bound[2] = graph->coordinate[*j][2];
			if(bound[3] < graph->coordinate[*j][3])
				bound[3] = graph->coordinate[*j][3];
		}
	}

	//fout << bound[0] << " " << bound[1] << " " << bound[2] << " " << bound[3] << endl;
	int G = graph->group.size();
	int NX = 1 + (bound[2] - bound[0] - 1) / graph->windows_size;
	int NY = 1 + (bound[3] - bound[1] - 1) / graph->windows_size;

	vector< vector< vector<int> > > D;
	int wx, wy, tmpx, tmpy, min, max;
	D.resize(G);
	for(int i = 0; i < G; i++){
		D[i].resize(NX);
		for(int j = 0; j < NX; j++){
			D[i][j].resize(NY);
			for(int k = 0; k < NY; k++){
				D[i][j][k] = 0;
				if(j == NX - 1)
					wx = bound[2] - graph->windows_size;
				else
					wx = bound[0] + j * graph->windows_size;
				if(k == NY - 1)
					wy = bound[3] - graph->windows_size;
				else
					wy = bound[1] + k * graph->windows_size;

				for(vector<int>::iterator it = (graph->group[i]).begin(); it != (graph->group[i]).end(); ++it){
					min = (wx > graph->coordinate[*it][0]) ? wx : graph->coordinate[*it][0];
					max = (wx + graph->windows_size < graph->coordinate[*it][2]) ? (wx + graph->windows_size) : graph->coordinate[*it][2];
					tmpx = (max > min) ? (max - min) : 0;
					min = (wy > graph->coordinate[*it][1]) ? wy : graph->coordinate[*it][1];
					max = (wy + graph->windows_size < graph->coordinate[*it][3]) ? (wy + graph->windows_size) : graph->coordinate[*it][3];
					tmpy = (max > min) ? (max - min) : 0;
					D[i][j][k] += (3 - 2 * graph->nodes[*it]->color) * tmpx * tmpy;
				}
			}
		}
	}

	for(int i = 0; i < G; i++){
		for(int k = NY - 1; k >= 0; k--){
			for(int j = 0; j < NX; j++){
				fout << D[i][j][k] << " ";
			}
			fout << endl;
		}
		fout << endl;
	}
	fout << endl;

	vector<int> Color(G, 1);
	vector< vector<int> > Diff(NX, vector<int>(NY, 0));
	for(int i = 0; i < G; i++){
		for(int k = NY - 1; k >= 0; k--){
			for(int j = 0; j < NX; j++){
				Diff[j][k] += Color[i] * D[i][j][k];
			}
		}
	}

	int index, sum, goal;
	while(1){
		index = -1;
		goal = 0;
		for(int i = 0; i < G; i++){
			sum = 0;
			for(int k = NY - 1; k >= 0; k--){
				for(int j = 0; j < NX; j++){
					sum += abs(Diff[j][k]) - abs(Diff[j][k] - 2 * Color[i] * D[i][j][k]);
				}
			}
			if(sum > goal){
				goal = sum;
				index = i;
			}
		}

		if(index != -1){
			Color[index] = -Color[index];
			for(int k = NY - 1; k >= 0; k--)
				for(int j = 0; j < NX; j++)
					Diff[j][k] += 2 * Color[index] * D[index][j][k];
		}else
			break;
	}

	for(int k = NY - 1; k >= 0; k--){
		for(int j = 0; j < NX; j++)
			fout << Diff[j][k] << " ";
		fout << endl;
	}
	fout << endl;
	for(int i = 0; i < G; i++)
		fout << Color[i] << " ";
	fout << endl;

	fout.close();
    return true;
}
