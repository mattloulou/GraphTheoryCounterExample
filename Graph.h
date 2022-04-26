#pragma once
#include <string>
#include "DynamicArray.h"

using Vertex = int;
using AdjList = DynamicArray<DynamicArray<Vertex>>;

class Graph
{
	///stores the edge-connection adjacency list.
	AdjList adj_list_;

public:

	Graph() = default; 
	Graph(AdjList other_adj_list); 

	int VertexDeg(Vertex vertex) const; 
	int VertexCount() const; 
	int EdgeCount() const;

	Vertex* FindEdge(Vertex vertex1, Vertex vertex2);
	bool HasEdge(Vertex vertex1, Vertex vertex2) const;

	bool AddEdge(Vertex vertex1, Vertex vertex2);
	bool RemoveEdge(Vertex vertex1, Vertex vertex2); 

	void PrintGraph(int width = 3) const; 
	void Clear();

	bool IsKVertexConnected(int k) const; 
	bool IsSimpleGraph() const; 

	explicit operator std::string() const;

	static const Graph K_4;
};

