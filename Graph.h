#pragma once
#include "DynamicArray.h"

class Graph
{
	DynamicArray<DynamicArray<int>> adj_list_;

public:


	Graph(DynamicArray<DynamicArray<int>> other_adj_list);


	int VertexDeg(int vertex) const;
	int VertexCount() const;
	int EdgeCount() const;

	int* FindEdge(int vertex1, int vertex2);
	bool AddEdge(int vertex1, int vertex2); //returns success if legal edge
	bool RemoveEdge(int vertex1, int vertex2);

	void Clear();

	bool IsKVertexConnected(int k) const; 
	bool IsWellformed() const; //representation invariant
};

