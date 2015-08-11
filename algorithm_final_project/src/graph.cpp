#include "graph.h"

Graph* graph =0;
Edge::Edge(Node *a, Node *b)
{
	if ( a->id <= b->id ) { node[0] = a; node[1] = b; }
	else { node[0] = b; node[1] = a; }
}

bool Edge::operator < (const Edge& rhs) const{

	int id1a = node[0]->id;
	int id1b = node[1]->id;

	int id2a = rhs.node[0]->id;
	int id2b = rhs.node[1]->id;


	if ( id1a == id2a ) return id1b < id2b;
	if ( id1a == id2b ) return id1b < id2a;
	if ( id1b == id2a ) return id1a < id2b;
	if ( id1b == id2b ) return id1a < id2a;
    return true;
}
Node * Edge::getNeighbor(Node *n)
{
	if ( node[0] == n ) return node[1];
	if ( node[1] == n ) return node[0];

	return 0;	
}


Node::Node(const int& i)
{
	id = i;
	traveled = false;
    color = 0;
	d = DIS_INF;
	prev = 0;
}

void Node::addEdge(Edge *e)
{
	edge.push_back(e);
}

bool edgeComp( const Edge* A, const Edge* B ){
	if ( (*A) < (*B) ) return true;
	return false;
}


void Node::sortEdge()
{
    sort(edge.begin(), edge.end(), edgeComp);
}

Graph::Graph(const string& n)
{
	name = n;
}

Graph::~Graph()
{
	vector<Edge *>::iterator itE;
	for ( itE = edges.begin() ; itE != edges.end() ; itE++ )
	{
		delete (*itE);
		(*itE) = 0;
	}
	
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}

    vector<Node *>::iterator itN2;
	for ( itN2 = nodes.begin() ; itN2 != nodes.end() ; itN2++ )
	{
		(*itN2) = 0;
	}
}
		
void Graph::addEdge(const int& v1, const int& v2)
{
	Node *a, *b;
	map<int, Node *>::iterator it = nodesMap.find(v1);
	bool fe = false;//no multi-egdes
	if ( it != nodesMap.end() )
		a = (*it).second;
	else
	{
		a = new Node(v1);
		nodesMap[v1] = a;
        nodes.push_back(a);
		coordinate[v1].push_back(nodes.size()-1);
	}

	it = nodesMap.find(v2);
	if ( it != nodesMap.end() )
		b = (*it).second;
	else
	{
		b = new Node(v2);
		nodesMap[v2] = b;
        nodes.push_back(b);
		coordinate[v2].push_back(nodes.size()-1);
	}
	//int a_size = a->edge.size();
	//int b_size = b->edge.size();
	//find if there exist an edge
	for(int i=0;i<a->edge.size();i++)
	{
		if(b == a->edge[i]->getNeighbor(a))
			fe = true;
	}
	if(!fe)
	{
		Edge *e = new Edge(a, b);
		edges.push_back(e);

		a->edge.push_back(e);
		b->edge.push_back(e);
	}
}
	
void Graph::sortEdgesOfNode()
{
	map<int, Node *>::iterator it;
	for ( it = nodesMap.begin() ; it != nodesMap.end() ; it++ )
	{
		Node *node = (*it).second;
		node->sortEdge();
	}

}



bool NodeCompByD( const Node* A, const Node* B ){
	if ( A->edge.size() > B->edge.size() ) 
        return true;
    else if (A->edge.size() == B->edge.size()) {
        if (A->id < B->id) 
            return true;
        else 
            return false;
    }
    else
        return false;
}

void Graph::sortNodesByDegree()
{
    sort(nodes.begin(), nodes.end(), NodeCompByD);
}


bool NodeCompByID( const Node* A, const Node* B ){
	if ( A->id < B->id ) return true;
    else return false;
}

void Graph::sortNodesByID()
{
    sort(nodes.begin(), nodes.end(), NodeCompByID);
}
void Graph::init()
{
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	{
		Node *node = (*itN).second;
		node->traveled = false;
		node->d = DIS_INF;
		node->prev = 0;
        node->color = -1;
	}
	
}

Node * Graph::getNodeById(const int& id)
{
	return nodesMap[id];
}

