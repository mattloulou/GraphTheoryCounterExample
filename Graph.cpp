#include "Graph.h"
#include <algorithm>
#include <iomanip>
#include <unordered_set>

///conastructor using an Adjacency List
Graph::Graph(AdjList other_adj_list) : adj_list_{ std::move(other_adj_list) }
{
    //empty
}

///returns the degree of the given vertex (number of edges it has)
int Graph::VertexDeg(Vertex vertex) const
{
    assert(0 <= vertex && vertex < adj_list_.Size());
    return adj_list_[vertex].Size();
}

///returns the number of vertices in the graph
int Graph::VertexCount() const
{
    return adj_list_.Size();
}

///returns the number of edges found in the graph
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

///DIRECTIONAL ONLY. Gives the reference to vertex2 in vertex1's list
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

///DIRECTIONAL ONLY. finds if the !directional! given edge exists (if vertex2 is in vertex1's adjacency list)
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

///adds bidirectional edge. Returns false if the edge already exists.
bool Graph::AddEdge(Vertex vertex1, Vertex vertex2)
{
    //check if vertices valid
    assert(vertex1 != vertex2);
    assert(0 <= vertex1 && vertex1 < adj_list_.Size());
    assert(0 <= vertex2 && vertex2 < adj_list_.Size());

    //check if already exists
    #ifdef _DEBUG
    if(FindEdge(vertex1, vertex2) || FindEdge(vertex2, vertex1)) return false;
    #else
    if(FindEdge(vertex1, vertex2)) return false;
    #endif
    
    //otherwise add new edge (bidirectional)
    adj_list_[vertex1].PushBack(vertex2);
    adj_list_[vertex2].PushBack(vertex1);
    return true;
}   


///remove bidirectional edge. Returns false if the edge does not exist (or just as 1-directional)
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

///remove all edges from a graph
void Graph::Clear()
{
    adj_list_.Clear();
}

///checks if the graph is K-vertex connected
bool Graph::IsKVertexConnected(int k) const
{
    return 1;
}

void Graph::AddVertex(int count)
{
    assert(count >= 1);

    for (int i = 0; i < count; ++i) {
        adj_list_.PushBack(DynamicArray<Vertex>{});
    }
}

void Graph::RemoveVertex(Vertex vertex)
{
    assert(vertex < VertexCount() && vertex >= 0);

    //delete the edges on the vertex-to-delete
    for (const Vertex& c : adj_list_[vertex]) {
        RemoveEdge(vertex, c);
    }

    if (vertex < VertexCount() - 1) {
        //copy the edges from the last vertex to the empty vertex
        for (const Vertex& c : adj_list_[VertexCount() - 1]) {
            AddEdge(vertex, c);
        }

        //recursive call to delete the largest vertex now
        RemoveVertex(VertexCount() - 1);
    }
    else {
        adj_list_.PopBack();
    }
}

///prints the graph using a given width for each node-number.
void Graph::PrintGraph(int width) const
{
    for (Vertex vertex1 = 0; vertex1 < VertexCount(); ++vertex1) {
        std::cout << vertex1 << ":";
        for (const Vertex& vertex2 : adj_list_[vertex1]) {
            std::cout << std::setw(width) << vertex2;
        }
        std::cout << '\n';
    }

    std::cout<<std::flush;
}

///checks for if a graph is simple by definition
bool Graph::IsSimpleGraph() const //representation invariant
{
    //make sure graph is bidirectional
    //veritices E [0,VertexCount)
    //no duplicate edges, and no self loops

    for (Vertex p = 0; p < adj_list_.Size(); ++p)
    {
        std::unordered_set<Vertex> edge;

        for (const auto& c : adj_list_[p])
        {
            //no self-edges
            assert(p != c);

            //check for bidirectional
            assert(HasEdge(c, p));

            //check duplicated edge
            assert(!edge.contains(c));
            edge.insert(c);
        }
    }

    return true;
}

bool Graph::HasChord(DynamicArray<Vertex> cycle) const
{
    assert(cycle.Size() >= 2);

    //loop through all Vertices in the cycle until we find one with a chord
    for (Vertex i = 0; i < cycle.Size() - 1; ++i) { //it is not necessary to check the final Vertex?

        //get adjacent vertices
        Vertex adj1 = cycle[(i + 1) % cycle.Size()];
        Vertex adj2 = cycle[(i - 1 + cycle.Size()) % cycle.Size()];

        //check if it has an edge with a vertex other than with the neighbours
        for (const Vertex& c : adj_list_[i]) {
            if (c != adj1 && c != adj2) {
                return true;
            }
        }
    }

    return false;
}

///return string representation of the adjacent list
Graph::operator std::string() const
{
    std::string graph_str;

    for (Vertex vertex1 = 0; vertex1 < VertexCount(); ++vertex1)
    {
        graph_str += std::to_string(vertex1) + ":";

        for (const Vertex& vertex2 : adj_list_[vertex1])
        {
            graph_str += std::to_string(vertex2) + " ";
        }
        graph_str += '\n';
    }

    return graph_str;
}

///standard K_4 graph
const Graph Graph::K_4{ AdjList{{1,2,3},{0,2,3},{0,1,3},{0,1,2}} };