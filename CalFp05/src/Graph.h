/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // outgoing edges
	bool visited;          // auxiliary field
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void dijkstraShortestPath(const T &s);
	void dijkstraShortestPathOld(const T &s);
	void unweightedShortestPath(const T &s);
	void bellmanFordShortestPath(const T &s);
	vector<T> getPath(const T &origin, const T &dest) const;

	// Fp05 - all pairs
	void floydWarshallShortestPath();
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
	for(typename vector<Vertex<T>*>::iterator it = vertexSet.begin(); it != vertexSet.end(); it++){
		(*it)->dist = INF;
		(*it)->processing = false;
		(*it)->path = NULL;
	}

	Vertex<T> *v = findVertex(origin);
	v->dist = 0;

	MutablePriorityQueue<Vertex<T>> q;
	q.insert(v);
	v->processing = true;

	while(!q.empty()){
		v = q.extractMin();
		for(typename vector<Edge<T>>::iterator it = v->adj.begin(); it != v->adj.end(); it++){
			if((*it).dest->dist > v->dist + (*it).weight){
				(*it).dest->dist = v->dist + (*it).weight;
				(*it).dest->path = v;
				if(!(*it).dest->processing){
					q.insert((*it).dest);
				}
				else{
					q.decreaseKey((*it).dest);
				}
			}
		}
	}
}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
	vector<T> res;
	Vertex<T> *v = findVertex(dest);
	res.push_back(v->info);

	while(v->path != NULL){
		v = v->path;
		res.push_back(v->info);
	}

	reverse(res.begin(), res.end());

	return res;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
	for(typename vector<Vertex<T>*>::iterator it = vertexSet.begin(); it != vertexSet.end(); it++){
		(*it)->dist = INF;
		(*it)->path = NULL;
	}

	Vertex<T> *v = findVertex(orig);
	v->dist = 0;

	queue<Vertex<T>*> q;
	q.push(v);

	while(!q.empty()){
		v = q.front();
		q.pop();
		for(typename vector<Edge<T>>::iterator it = v->adj.begin(); it != v->adj.end(); it++){
			if((*it).dest->dist == INF){
				q.push((*it).dest);
				(*it).dest->dist = v->dist + 1;
				(*it).dest->path = v;
			}
		}
	}

}

template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
	for(typename vector<Vertex<T>*>::iterator it = vertexSet.begin(); it != vertexSet.end(); it++){
		(*it)->dist = INF;
		(*it)->path = NULL;
	}

	Vertex<T> *v = findVertex(orig);
	v->dist = 0;

	for(unsigned i = 1; i < vertexSet.size()-1; i++){
		for(typename vector<Vertex<T>*>::iterator it1 = vertexSet.begin(); it1 != vertexSet.end(); it1++){
			for(typename vector<Edge<T>>::iterator it = (*it1)->adj.begin(); it != (*it1)->adj.end(); it++){
				if((*it).dest->dist > (*it1)->dist + (*it).weight){
					(*it).dest->dist = (*it1)->dist + (*it).weight;
					(*it).dest->path = *it1;
				}
			}
		}
	}

	for(typename vector<Vertex<T>*>::iterator it1 = vertexSet.begin(); it1 != vertexSet.end(); it1++){
		for(typename vector<Edge<T>>::iterator it = (*it1)->adj.begin(); it != (*it1)->adj.end(); it++){
			if((*it1)->dist + (*it).weight < (*it).dest->dist){
				cerr << "there are cycles of negative weight";
			}
		}
	}

}


/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
	/*vector<vector<double>> D, W;
	vector<Vertex<T>*> P;
	D.resize(vertexSet.size());
	W.resize(vertexSet.size());
	P.resize(vertexSet.size());
	for(int i = 0; i < vertexSet.size(); i++){
		D[i].resize(vertexSet.size());
		W[i].resize(vertexSet.size());
		P[i].resize(vertexSet.size());
		for(int j = 0; j < vertexSet.at(i)->adj.size(); j++){
			P[i][j] = NULL;
			W[i][j] = vertexSet.at(i)->adj.at(j).weight;
			D[i][j] = W[i][j];
		}
	}

	for(int i = 0; i < vertexSet.size(); i++){
		for(int j = 0; j < vertexSet.at(i)->adj.size(); j++){
			for(int k = 1; k <= vertexSet.size(); k++){
				if()
			}
		}
	}*/
}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
	vector<T> res;
	// TODO
	return res;
}


#endif /* GRAPH_H_ */
