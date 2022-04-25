#include "Graph.h"
#include <algorithm>
#include <iomanip>

Graph::Graph(DynamicArray<DynamicArray<Vertex>> other_adj_list) : adj_list_{ other_adj_list } {}

int Graph::VertexDeg(Vertex vertex) const
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

//directional only. Gives the reference to vertex2 in vertex1's list
Vertex* Graph::FindEdge(Vertex vertex1, Vertex vertex2)
{
    //check if vertices valid
    assert(vertex1 != vertex2);
    assert(0 <= vertex1 && vertex1 < adj_list_.Size());
    assert(0 <= vertex2 && vertex2 < adj_list_.Size());

    for(Vertex& dest : adj_list_[vertex1])
    {
        if (dest == vertex2) return &dest;
    }

    return nullptr;
}

bool Graph::HasEdge(Vertex vertex1, Vertex vertex2) const
{
    //check if vertices valid
    assert(vertex1 != vertex2);
    assert(0 <= vertex1 && vertex1 < adj_list_.Size());
    assert(0 <= vertex2 && vertex2 < adj_list_.Size());

    for (const Vertex& dest : adj_list_[vertex1])
    {
        if (dest == vertex2) return true;
    }
    return false;
}

//returns success if legal edge
bool Graph::AddEdge(Vertex vertex1, Vertex vertex2)
{
    //check if vertices valid
    assert(vertex1 != vertex2);
    assert(0 <= vertex1 && vertex1 < adj_list_.Size());
    assert(0 <= vertex2 && vertex2 < adj_list_.Size());


    //check if already exists
    if(!FindEdge(vertex1, vertex2)) return false;
    
    //otherwise add new edge
    adj_list_[vertex1].PushBack(vertex2);
    return true;
}   

bool Graph::RemoveEdge(Vertex vertex1, Vertex vertex2)
{
    //check if vertices valid
    assert(vertex1 != vertex2);
    assert(0 <= vertex1 && vertex1 < adj_list_.Size());
    assert(0 <= vertex2 && vertex2 < adj_list_.Size());


    //get the edge pointers. FindEdge checks the bounds automatically
    Vertex* v2 = FindEdge(vertex1, vertex2);
    Vertex* v1 = FindEdge(vertex2, vertex1);
    if (!v1 || !v2) return false;

    *v1 = adj_list_[vertex2].Back();
    *v2 = adj_list_[vertex1].Back();

    adj_list_[vertex2].PopBack();
    adj_list_[vertex1].PopBack();

    return true;
}

void Graph::Clear()
{
    adj_list_.Clear();
}

bool Graph::IsKVertexConnected(int k) const
{
    return 1;
}

void Graph::PrintGraph() const
{
    for (Vertex vertex1 = 0; vertex1 < VertexCount(); ++vertex1) {
        std::cout << vertex1 << ": ";
        for (const Vertex& vertex2 : adj_list_[vertex1]) {
            std::cout << std::setw(5) << vertex2;
        }
        std::cout << '\n';
    }

    std::cout<<std::flush;
}

bool Graph::IsWellformed() const //representation invariant
{
    //make sure graph is bidirectional, and that all veritices are from [0,VertexCount)
    for (Vertex i = 0; i < VertexCount(); ++i) {
        for (const Vertex& j : adj_list_[i]) {

            //no self-edges
            assert(i != j); 

            //check for i in j's adjacency list.
            assert(HasEdge(j, i)); 
        }
    }

    return true;
}