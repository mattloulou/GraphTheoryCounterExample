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

	bool IsConnected(const DynamicArray<Vertex>& vertex_exclustion_list = DynamicArray<Vertex>{}) const;
	bool IsKVertexConnected(int k) const; 
	bool IsSimpleGraph() const; 
	bool HasChord(const DynamicArray<Vertex>& cycle) const;
	bool IsValidDirectionalCycle(const DynamicArray<Vertex>& cycle) const;
	bool DoAllLargestCyclesHaveAChord() const;
	bool DoAllLargestCyclesHaveAChordV2(); //WARNING: this method destroys a given graph. Use it on copies only to be safe
	
	bool CheckThomassenConj() const;
	bool CheckThomassenConjV2() const;
	bool CheckThomassenConjV3() const;
	bool CheckThomassenConjV4() const; 
	bool CheckThomassenConjV5() const;
	bool CheckThomassenConjV6() const;

	private:
	static void DFS(const Graph& graph, DynamicArray<DynamicArray<Vertex>>& largest_cycles, DynamicArray<bool>& visited, DynamicArray<Vertex>& current_cycle, Vertex v);
	static void DFSV2(Vertex par, Vertex curr, DynamicArray<int>& colors, DynamicArray<Vertex>& parents, int& cycle_num, const Graph& graph, DynamicArray<DynamicArray<Vertex>>& cycles);


public:


	explicit operator std::string() const;

	static const Graph K_4;
	static DynamicArray<DynamicArray<Vertex>> AllPermutations(const int& num_choices);
	static DynamicArray<DynamicArray<Vertex>> AllPermutations(const int& num_choices, const int& max_length);
	static DynamicArray<DynamicArray<Vertex>> AllCombinations(const int& num_choices);
	static DynamicArray<DynamicArray<Vertex>> AllCombinations(const int& num_choices, const int& max_length);
	static std::string VertexListToString(const DynamicArray<Vertex>& vertex_list);
};

