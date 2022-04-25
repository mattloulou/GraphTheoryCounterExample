#pragma once
#include "DynamicArray.h"

using Vertex = int;
using AdjList = DynamicArray<DynamicArray<Vertex>>;

class Graph
{
	AdjList adj_list_;

public:

	Graph(AdjList other_adj_list);

	int VertexDeg(Vertex vertex) const;
	int VertexCount() const;
	int EdgeCount() const;

	Vertex* FindEdge(Vertex vertex1, Vertex vertex2);
	bool HasEdge(Vertex vertex1, Vertex vertex2) const;
	bool AddEdge(Vertex vertex1, Vertex vertex2); //returns success if legal edge
	bool RemoveEdge(Vertex vertex1, Vertex vertex2);

	void PrintGraph() const;
	void Clear();

	bool IsKVertexConnected(int k) const; 
	bool IsWellformed() const; //representation invariant

	static const Graph K_4;
};

