//Authors: Matthew Pechen-Berg & Evan He 
//Initial Creation Date: April 2022
#include <iostream>
#include <random>
#include <iomanip>
#include "Graph.h";
#include "DynamicArray.h" 

int main()
{
    std::cin.tie(nullptr)->sync_with_stdio(false);

    bool counter_example_found = false;
    int graphs_checked = 1;
    int graph_size;
    std::cout << "Select your graph size: ";
    std::cin >> graph_size;

    while (!counter_example_found) {
        Graph g = Graph{ graph_size };
        for (int i = 0; i < graph_size; ++i) {
            for (int j = 0; j < graph_size; ++j) {
                if (i != j) {
                    g.AddEdge(i, j);
                }
            }
        }

        //there should be 90 edges to start with
        //remove 30 random edges
        //source for random generation: https://stackoverflow.com/a/13445752
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, graph_size-1);

        for (int i = 0; i < graph_size*5; ++i) {
            const int int1 = dist(rng);
            const int int2 = dist(rng);
            if (int1 != int2) {
                g.RemoveEdge(int1, int2);
            }
        }

        if (g.IsKVertexConnected(3)) {
            std::cout << std::setw(6) << "Graph #" << graphs_checked << " . Graph checked, 3 connected!\n";
            if (!g.CheckThomassenConjV5()) {
                g.PrintGraph();
                counter_example_found = true;
            }
        }
        else {
            std::cout << std::setw(7) << "Graph #" << graphs_checked << " . Graph checked, not 3 connected :(\n";
        }
        ++graphs_checked;
    }
}