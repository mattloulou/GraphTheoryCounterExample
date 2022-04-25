#include "Graph.h"
#include <algorithm>


Graph::Graph(DynamicArray<DynamicArray<int>> other_adj_list);


int Graph::VertexDeg(int vertex) const
{
    assert(0 <= vertex && vertex < adj_list_.Size());
    return adj_list_[vertex].Size();
}

int Graph::VertexCount() const
{
    return adj_list_.Size();
}

int Graph::EdgeCount() const
{
    int total = 0;
    for(const auto& vertex : adj_list_)
    {
        total += vertex.Size();
    }

    //check undirected edges
    assert(total % 2 == 0);
    return total / 2;
}

int* Graph::FindEdge(int vertex1, int vertex2)
{
    //check if vertices are within range
    assert(0 <= vertex1 && vertex1 < adj_list_.Size());
    assert(0 <= vertex2 && vertex2 < adj_list_.Size());

    bool 
    for(int dest : adj_list_[vertex1])
    {
        if (dest == vertex2) return true;
    }

    return false;
}

//returns success if legal edge
bool Graph::AddEdge(int vertex1, int vertex2)
{
    //check in range
    assert(0 <= vertex1 && vertex1 < adj_list_.Size());
    assert(0 <= vertex2 && vertex2 < adj_list_.Size());

    //check if already exists
    if(HasEdge(vertex1, vertex2)) return false;
    
    //otherwise add new edge
    adj_list_[vertex1].PushBack(vertex2);
    return true;
}   

bool Graph::RemoveEdge(int vertex1, int vertex2)
{
    //check in range
    assert(0 <= vertex1 && vertex1 < adj_list_.Size());
    assert(0 <= vertex2 && vertex2 < adj_list_.Size());

    //check if already exists
    if(!HasEdge(vertex1, vertex2)) return false;
    
    bool found = false;
    for (int& vertex : adj_list_[vertex1]) {
        if (vertex == vertex2) {
            vertex = adj_list_[vertex1].Back();
            adj_list_.PopBack();
            found = true;
            break;
        }
    }

    if(!found) return false;
    found = false;

    for (int& vertex : adj_list_[vertex2]) {
        if (vertex == vertex1) {
            vertex = adj_list_[vertex2].Back();
            adj_list_.PopBack();
            break;
        }
    }
    
    return 
}

void Graph::Clear()
{
    adj_list_.Clear();
}

bool Graph::IsKVertexConnected(int k) const; 


bool Graph::IsWellformed() const //representation invariant
{
    //make sure graph is bidirectional, and that all veritices are from [0,VertexCount)
    const int vertexCount = VertexCount();
    for (int i = 0; i < vertexCount - 1; ++i) {
        for (int j = 0; j < adj_list_[i].Size(); ++j) {
            assert(HasEdge(j, i));
        }
    }
}