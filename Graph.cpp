#include "Graph.h"
#include <algorithm>
#include <iomanip>
#include <unordered_set>
#include <queue>

///conastructor using an Adjacency List
Graph::Graph(AdjList other_adj_list) : adj_list_{ std::move(other_adj_list) }
{
    //empty
}

///conastructs a graph with "vertex_count" vertices with no edges
Graph::Graph(int vertex_count)
{
    assert(vertex_count >= 0);

    //add "vertex_count" many vertices 
    for (int i = 0; i < vertex_count; ++i) {
        adj_list_.PushBack(DynamicArray<Vertex>{});
    }
}

///returns the number of vertices in the graph
int Graph::VertexCount() const
{
    return adj_list_.Size();
}

///returns the degree of the given vertex (number of edges it has)
int Graph::VertexDeg(Vertex vertex) const
{
    assert(0 <= vertex && vertex < adj_list_.Size());
    return adj_list_[vertex].Size();
}

///adds room for another vertex in the adjacency list (with the label/name being the new VertexCount() - 1)
bool Graph::PushVertex(int count)
{
    assert(count >= 1);

    adj_list_.Resize(adj_list_.Size() + count);

    return true;

    //for (int i = 0; i < count; ++i) {
    //    adj_list_.PushBack(DynamicArray<Vertex>{});
    //}
}

///removes the specified vertex, and renames the largest vertex to the deleted vertex
bool Graph::PopVertex()
{
    assert(VertexCount() > 0);
    const Vertex last_vertex = VertexCount() - 1;

    //remove the edges on the vertex-to-delete
    for (const Vertex& c : adj_list_[last_vertex]) {
        RemoveEdge(last_vertex, c);
    }

    adj_list_.PopBack();

    return true;
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

//removes all edges from a single specified vertex
bool Graph::ClearEdges(Vertex vertex)
{
    assert(0 <= vertex && vertex < VertexCount());

    for (int c : adj_list_[vertex])
    {
        RemoveEdge(vertex, c);
    }

    return true;
}

///removes all edges from all vertices in the graph
bool Graph::ClearEdges()
{
    for(int p = 0; p < VertexCount(); ++p)
    {
        for(int c : adj_list_[p])
        {
            RemoveEdge(p, c);
        }
    }

    return true;
}

///remove all edges from a graph
void Graph::ClearGraph()
{
    adj_list_.Clear();
}

//BIDIRECTIONAL ONLY. checks if the graph is connected. 
bool Graph::IsConnected(const DynamicArray<Vertex>& vertex_exclustion_list) const
{
    //the graph should be simple
    assert(IsSimpleGraph());

    //this implementation assumes graphs with 0 vertices are connected. Idk if this is standard?
    if (VertexCount() == 0) return true;

    //The exclusion list should not be all vertices unless there are 0 vertices.
    assert(vertex_exclustion_list.Size() < VertexCount());

    std::unordered_set<Vertex> traversed;

    //place all excluded vertices in the set
    for (const Vertex& v : vertex_exclustion_list) {
        assert(!traversed.contains(v)); //to make sure they are all unique
        traversed.insert(v);
    }

    //find which vertex to begin with
    Vertex starting_vertex;
    for (Vertex v = 0; v < VertexCount(); ++v) {
        if (!traversed.contains(v)) {
            starting_vertex = v;
            break;
        }
    }

    //begin with the first vertex and try traversing through the graph until we reach all vertices or can't reach all.
    traversed.insert(starting_vertex);
    std::queue<Vertex> queue;
    queue.push(starting_vertex);

    //keep traversing through
    while (traversed.size() < VertexCount() && !queue.empty()) { 
        
        int target_vertex = queue.front();
        queue.pop();

        for (const auto& v : adj_list_[target_vertex]) {
            if (!traversed.contains(v)) {
                queue.push(v);
            }
            traversed.insert(v);
        }
    }

    if (traversed.size() == VertexCount()) return true;
    return false;
}

///checks if the graph is K-vertex connected
bool Graph::IsKVertexConnected(int k) const
{
    //there must be more than k vertices, and since to be k-vertex-connected the Graph must remain connected whenever fewer than k vertices are removed, 
    //if k <= 0, then we will remove <0 vertices which makes no sense.
    assert(0 < k && k < VertexCount());

    //For a graph to be k-vertex-connected, it must remain connected when fewer than k arbitrary vertices are removed.
    //If all combinations of exactly (k-1) vertices being removed from the graph leave it still connected, this implies it will remain connected when removing any any combination fewer than (k-1) vertices.
    //So, we only need to consider combinations of exactly (k-1) vertices for analyzing if a graph if k-connected or not.
    DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(VertexCount(), k - 1);

    for (const auto& combination : combinations) {
        if (combination.Size() == k - 1) {
            
            Graph copy{ *this };
            
            for (const Vertex& v : combination) {

                //this method isolates the vertice such that it is essentially deleted. We then include the  combination in the IsConnected() call.
                //this makes v essentially deleted
                copy.ClearEdges(v); 
            }

            if (!copy.IsConnected(combination)) return false;
        }
    }
    return true;
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

/// The cycle must have a unique list of vertices (end != beginning), and must be a valid cycle
bool Graph::HasChord(const DynamicArray<Vertex>& cycle) const
{
    /*if (cycle.Size() <= 3) return false;*/ //I turned this off just to test the algorithm
    
    //this cycle must be a valid cycle to check if it has a chord...
    if (!IsValidDirectionalCycle(cycle)) return false;



    //make sure cycle is unique
    std::unordered_set<Vertex> vertices;
    for (const Vertex& v : cycle) {
        assert(!vertices.contains(v));
        vertices.insert(v);
    }

    //loop through all Vertices in the cycle until we find one with a chord
    for (int i = 0; i < cycle.Size() - 1; ++i) { //these are the indicies for the Vertices within cycle. //it is not necessary to check the final Vertex?

        //get adjacent and current vertices
        const Vertex adj1 = cycle[(i + 1) % cycle.Size()];
        const Vertex adj2 = cycle[(i - 1 + cycle.Size()) % cycle.Size()];
        const Vertex current_vertex = cycle[i];

        //check if it has an edge with a vertex in the cycle other than with the neighbours
        for (const Vertex& c : adj_list_[current_vertex]) {
            if (c != adj1 && c != adj2 && vertices.contains(c)) {
                return true;
            }
        }
    }



    //try to replace above with HasEdge() 

    return false;
}

//checks if the given cycle is a valid directional cycle. Cycle traversal direction is v0 --> v1 --> v2, etc
bool Graph::IsValidDirectionalCycle(const DynamicArray<Vertex>& cycle) const
{
    //make sure cycle is unique
    #ifdef _DEBUG
    std::unordered_set<Vertex> vertices;
    for (const Vertex& v : cycle) {
        assert(!vertices.contains(v));
        vertices.insert(v);
    }
    #endif

    if (cycle.Size() < 3) return false;

    for (int i = 0; i < cycle.Size(); ++i) {

        const Vertex current_vertex = cycle[i]; //current vertex in the cycle
        const Vertex next_vertex = cycle[(i + 1) % cycle.Size()]; //next vertex in the cycle

        if (!HasEdge(current_vertex, next_vertex)) return false;
    }

    return true;
}


///Checks if every cycle of the largest length in this graph has a chord.
bool Graph::DoAllLargestCyclesHaveAChord() const
{
    DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(VertexCount());

    DynamicArray<Vertex> current_permutation;
    bool cycle_found = false;
    int largest_cycle_size = VertexCount(); //just a default value. Not important what it is.
    int current_index = permutations.Size() - 1;

    //we want to loop through all combinations, largest to smallest, until we find the first combination that is a cycle. We then keep checking all cycles until we reach a smaller size.\

    while (current_index >= 0) {
        current_permutation = permutations[current_index];

        //in this case, it means we have found a cycle of some size, and are now inspecting combinations consisting of fewer vertices (means we are done)
        if (cycle_found && largest_cycle_size != current_permutation.Size()) {
            break;
        }


        if (IsValidDirectionalCycle(current_permutation)) {
            cycle_found = true;
            largest_cycle_size = current_permutation.Size();

            if (!HasChord(current_permutation)) return false; //all longest cycles should have a chord
        }

        --current_index;
    }

    //in this implementation, if there are no cycles, it will return true. I guess this is techincally correct, as the implication (For all largest cycles, they have a chord) would be vacuously true 
    return true;
}

///Checks if every cycle of the largest length in this graph has a chord.
bool Graph::DoAllLargestCyclesHaveAChord() const
{
    //this is required for this algorithm (I think)
    assert(IsKVertexConnected(2)); 
    
    //the gist of this method is that we will begin with vertex 0, and try to find the largest cycles it is a part of.
    //Once we have done that, if there can theoretically be cycles remaining that don't involve vertex 0, we will remove that vertex and recursively follow this procedure on the resulting graph.
    //This will keep happening until we removed all vertices, and/or there can not be any theoretically longer cycles left.

    int largest_cycle = 2; //default value. Can also be set to 0
    int vertices_left = VertexCount(); //the number of vertices that have not yet been excluded from search. This is also the theoretical maximum sized cycle that we can find left (after removing vertices)
    DynamicArray<DynamicArray<Vertex>> cycles_of_largest_length;


    //keep searching while there can theoretically be a larger cycle
    while (largest_cycle <= vertices_left) { //since largest_cycle is set to 2 by default, and that is the lower bound, it guarantees that we will never have fewer than 2 vertices left.

        DynamicArray<int> index_path_taken; //this stores the indices of the current path taken
        DynamicArray<Vertex> vertex_path_taken; //this stores the vertices of the path taken
        std::unordered_set<Vertex> vertices_travelled; //the same as above, but in a map.

        bool done_with_current_vertex = false;
        bool backtracking = false;
        int current_vertex = VertexCount() - vertices_left;

        vertex_path_taken.PushBack(current_vertex); //start the DynamicArray off with the virst vertex we are using.

        while (!done_with_current_vertex) {

            if (!backtracking) {

                //go to the next vertex; It will always have elements in the adj_list
                Vertex next_vertex = adj_list_[vertex_path_taken.Back()][0];
                
                index_path_taken.PushBack(0);
                vertex_path_taken.PushBack(next_vertex);

                //if we have found a new cycle
                if (next_vertex == current_vertex) {

                    const int cycle_size = vertex_path_taken.Size() - 1;

                    //if we found a new largest-size cycle
                    if (cycle_size > largest_cycle) {

                        //update longest-cycle information
                        largest_cycle = cycle_size;
                        cycles_of_largest_length.Clear();

                        DynamicArray<Vertex> cycle_to_add{ vertex_path_taken };
                        cycle_to_add.PopBack();
                        cycles_of_largest_length.PushBack(cycle_to_add);

                        //gotta back up now and take a different path
                        backtracking = true;
                    }

                    //if we found a cycle of the same size as the current largest:
                    if (cycle_size == largest_cycle) {
                           
                        //add the new cycle found
                        DynamicArray<Vertex> cycle_to_add{ vertex_path_taken };
                        cycle_to_add.PopBack();
                        cycles_of_largest_length.PushBack(cycle_to_add);

                        //gotta back up now and take a different path
                        backtracking = true;
                    }


                }

                //if we return to a previous vertex already visited in this path
                else if (vertices_travelled.contains(next_vertex)) {
                    backtracking = true;
                }



                else {
                    vertices_travelled.insert(next_vertex);
                }




            //backtracking:
            } else {

            }
            

        }
    }

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

///Returns a list of all the possible permutations of all the vertices in a graph with "num_choices" vertices.
///The number of elements of each permutation will be in the range [0, num_choices]. Each permutation has no more than 1 of each vertex
DynamicArray<DynamicArray<Vertex>> Graph::AllPermutations(const int& num_choices)
{
    assert(num_choices >= 0);

    DynamicArray<DynamicArray<Vertex>> cycles{ 1 };
    int first_cycle = 0; //index in "cycles" of the first cycle of the size we are now generating
    int current_batch_cycle_count = 0; //quantity of cycles that have just been processed
    int next_batch_cycle_count = 1; //quantity of cycles that must still be processed (1 size larger).

    while (cycles.Back().Size() != num_choices) { //this should loop num_choices times. Each loop will create all permutations of 1 size larger

        int num_cycles = cycles.Size(); //saves the total quantity of cycles at the start of this batch

        first_cycle += current_batch_cycle_count;
        current_batch_cycle_count = next_batch_cycle_count;
        next_batch_cycle_count = 0;

        //loop through each existing cycle
        for (int i = first_cycle; i < num_cycles; ++i) {

            const DynamicArray<Vertex> this_cycle = cycles[i]; //the targeted cycle to derive new cycles from

            //find which vertices this cycle has
            std::unordered_set<Vertex> vertices;
            for (const auto& v : this_cycle) {
                vertices.insert(v);
            }

            //for each possible vertex, if it is not in this_cycle, we will make a duplicate cycle with this vertex appended
            for (Vertex j = 0; j < num_choices; ++j) {
                if (!vertices.contains(j)) {
                    DynamicArray<Vertex> new_cycle{ this_cycle };
                    new_cycle.PushBack(j);
                    cycles.PushBack(new_cycle);
                    ++next_batch_cycle_count;
                }
            }
        }
    }

    return cycles;
}

///Returns a list of all the possible permutations of all the vertices in a graph with "num_choices" vertices.
///The number of elements of each permutation will be in the range [0, max_size <= num_choices]. Each permutation has no more than 1 of each vertex
DynamicArray<DynamicArray<Vertex>> Graph::AllPermutations(const int& num_choices, const int& max_size)
{
    assert(num_choices >= 0);
    assert(0 <= max_size && max_size <= num_choices);

    DynamicArray<DynamicArray<Vertex>> cycles{ 1 };
    int first_cycle = 0; //index in "cycles" of the first cycle of the size we are now generating
    int current_batch_cycle_count = 0; //quantity of cycles that have just been processed
    int next_batch_cycle_count = 1; //quantity of cycles that must still be processed (1 size larger).

    while (cycles.Back().Size() != max_size) { //this should loop max_size times. Each loop will create all permutations of 1 size larger

        int num_cycles = cycles.Size(); //saves the total quantity of cycles at the start of this batch

        first_cycle += current_batch_cycle_count;
        current_batch_cycle_count = next_batch_cycle_count;
        next_batch_cycle_count = 0;

        //loop through each existing cycle
        for (int i = first_cycle; i < num_cycles; ++i) {

            const DynamicArray<Vertex> this_cycle = cycles[i]; //the targeted cycle to derive new cycles from

            //find which vertices this cycle has
            std::unordered_set<Vertex> vertices;
            for (const auto& v : this_cycle) {
                vertices.insert(v);
            }

            //for each possible vertex, if it is not in this_cycle, we will make a duplicate cycle with this vertex appended
            for (Vertex j = 0; j < num_choices; ++j) {
                if (!vertices.contains(j)) {
                    DynamicArray<Vertex> new_cycle{ this_cycle };
                    new_cycle.PushBack(j);
                    cycles.PushBack(new_cycle);
                    ++next_batch_cycle_count;
                }
            }
        }
    }

    return cycles;
}

///Returns a list of all the possible combinations of all the vertices in a graph with "num_choices" vertices.
///The number of elements of each combination will be in the range [0, num_choices]. Each combination has no more than 1 of each vertex
DynamicArray<DynamicArray<Vertex>> Graph::AllCombinations(const int& num_choices)
{
    assert(num_choices >= 0);

    DynamicArray<DynamicArray<Vertex>> cycles{ 1 };
    int first_cycle = 0; //index in "cycles" of the first cycle of the size we are now generating
    int current_batch_cycle_count = 0; //quantity of cycles that have just been processed
    int next_batch_cycle_count = 1; //quantity of cycles that must still be processed (1 size larger).

    while (cycles.Back().Size() != num_choices) { //this should loop num_choices times. Each loop will create all permutations of 1 size larger

        int num_cycles = cycles.Size(); //saves the total quantity of cycles at the start of this batch

        first_cycle += current_batch_cycle_count;
        current_batch_cycle_count = next_batch_cycle_count;
        next_batch_cycle_count = 0;

        //loop through each existing cycle
        for (int i = first_cycle; i < num_cycles; ++i) {

            const DynamicArray<Vertex> this_cycle = cycles[i]; //the targeted cycle to derive new cycles from

            //find which vertices this cycle has
            std::unordered_set<Vertex> vertices;
            for (const auto& v : this_cycle) {
                vertices.insert(v);
            }

            //for each possible vertex, if it is not in this_cycle, we will make a duplicate cycle with this vertex appended
            //the only difference between this and for permutations is that we will only check vertices larger than the last 
            //element of this_cycle (to avoid duplicate combinations).
            Vertex starting_index = (this_cycle.Size() == 0) ? 0 : this_cycle.Back() + 1;
            for (Vertex j = starting_index; j < num_choices; ++j) {
                if (!vertices.contains(j)) {
                    DynamicArray<Vertex> new_cycle{ this_cycle };
                    new_cycle.PushBack(j);
                    cycles.PushBack(new_cycle);
                    ++next_batch_cycle_count;
                }
            }
        }
    }

    return cycles;
}

///Returns a list of all the possible combinations of all the vertices in a graph with "num_choices" vertices.
///The number of elements of each combination will be in the range [0, max_size <= num_choices]. Each combination has no more than 1 of each vertex
DynamicArray<DynamicArray<Vertex>> Graph::AllCombinations(const int& num_choices, const int& max_size)
{
    assert(num_choices >= 0);
    assert(0 <= max_size && max_size <= num_choices);

    DynamicArray<DynamicArray<Vertex>> cycles{ 1 };
    int first_cycle = 0; //index in "cycles" of the first cycle of the size we are now generating
    int current_batch_cycle_count = 0; //quantity of cycles that have just been processed
    int next_batch_cycle_count = 1; //quantity of cycles that must still be processed (1 size larger).

    while (cycles.Back().Size() != max_size) { //this should loop max_size times. Each loop will create all permutations of 1 size larger

        int num_cycles = cycles.Size(); //saves the total quantity of cycles at the start of this batch

        first_cycle += current_batch_cycle_count;
        current_batch_cycle_count = next_batch_cycle_count;
        next_batch_cycle_count = 0;

        //loop through each existing cycle
        for (int i = first_cycle; i < num_cycles; ++i) {

            const DynamicArray<Vertex> this_cycle = cycles[i]; //the targeted cycle to derive new cycles from

            //find which vertices this cycle has
            std::unordered_set<Vertex> vertices;
            for (const auto& v : this_cycle) {
                vertices.insert(v);
            }

            //for each possible vertex, if it is not in this_cycle, we will make a duplicate cycle with this vertex appended
            //the only difference between this and for permutations is that we will only check vertices larger than the last 
            //element of this_cycle (to avoid duplicate combinations).
            Vertex starting_index = (this_cycle.Size() == 0) ? 0 : this_cycle.Back() + 1;
            for (Vertex j = starting_index; j < num_choices; ++j) {
                if (!vertices.contains(j)) {
                    DynamicArray<Vertex> new_cycle{ this_cycle };
                    new_cycle.PushBack(j);
                    cycles.PushBack(new_cycle);
                    ++next_batch_cycle_count;
                }
            }
        }
    }

    return cycles;
}


///convertes a vertex list to a string in the form of: "v0 v1 v2 v3".
std::string Graph::VertexListToString(const DynamicArray<Vertex>& vertex_list)
{
    std::string output = "";
    for (Vertex i = 0; i < vertex_list.Size() - 1; ++i) {
        output += std::to_string(vertex_list[i]) + " ";
    }

    if (vertex_list.Size() > 0) {
        output += std::to_string(vertex_list.Back());
    }
    return output;
}