#pragma once
#include <string>
#include "DynamicArray.h"

using Vertex = int;
using AdjList = DynamicArray<DynamicArray<Vertex>>;

//mandatory undirected graph
class Graph
{
	///stores the edge-connection adjacency list.
	AdjList adj_list_;

public:

	Graph() = default; 
	Graph(AdjList other_adj_list); 
	Graph(int vertex_count);
	

	int VertexCount() const; 
	int VertexDeg(Vertex vertex) const;
	bool PushVertex(int count = 1);
	bool PopVertex();


	int EdgeCount() const;
	Vertex* FindEdge(Vertex vertex1, Vertex vertex2);
	bool HasEdge(Vertex vertex1, Vertex vertex2) const;
	bool AddEdge(Vertex vertex1, Vertex vertex2);
	bool RemoveEdge(Vertex vertex1, Vertex vertex2);
	bool ClearEdges(Vertex vertex);
	bool ClearEdges();
	
	void ClearGraph();
	void PrintGraph(int width = 3) const; 

	bool IsConnected() const;
	bool IsKVertexConnected(int k) const; 
	bool IsSimpleGraph() const; 
	bool HasChord(const DynamicArray<Vertex>& cycle) const;
	bool IsValidDirectionalCycle(const DynamicArray<Vertex>& cycle) const;

	explicit operator std::string() const;

	static const Graph K_4;
	static DynamicArray<DynamicArray<Vertex>> AllPermutations(const int& num_choices);
	static DynamicArray<DynamicArray<Vertex>> AllCombinations(const int& num_choices);
	static std::string VertexListToString(const DynamicArray<Vertex>& vertex_list);
};

