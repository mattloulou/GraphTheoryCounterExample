#include "Graph.h"



Graph::Graph(DynamicArray<DynamicArray<int>> other_adj_list);


int Graph::VertexDeg(int vertex) const
{
    return adj_list_.Size();
}
int Graph::VertexCount() const;
int Graph::EdgeCount() const;

bool Graph::HasEdge(int vertex1, int vertex2) const;
bool Graph::AddEdge(int vertex1, int vertex2); //returns success; fails if edge already exists
bool Graph::RemoveEdge(int vertex1, int vertex2);

void Graph::Clear();

bool Graph::IsKVertexConnected(int k) const; //representation invariant