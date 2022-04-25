#pragma once
#include "DynamicArray.h"

using Vertex = int;

class Graph
{
	DynamicArray<DynamicArray<Vertex>> adj_list_;

public:


	Graph(DynamicArray<DynamicArray<Vertex>> other_adj_list);


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
};

