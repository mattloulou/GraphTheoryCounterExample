#include <iostream>
#include <random>
#include <iomanip>
#include "Graph.h";
#include "DynamicArray.h" 

int main()
{
    bool counter_example_found = false;
    int graphs_checked = 1;

    while (!counter_example_found) {
        Graph g = Graph{ 10 };
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
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
        std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 9);

        for (int i = 0; i < 32; ++i) {
            int int1 = dist10(rng);
            int int2 = dist10(rng);
            if (int1 != int2) {
                g.RemoveEdge(int1, int2);
            }
        }

        if (g.IsKVertexConnected(3)) {
            std::cout << std::setw(6) << "Graph #" << graphs_checked << " . Graph checked, 3 connected!" << std::endl;
            if (!g.DoAllLargestCyclesHaveAChord()) {
                g.PrintGraph();
                counter_example_found = true;
            }
        }
        else {
            std::cout << std::setw(6) << "Graph #" << graphs_checked << " . Graph checked, not 3 connected :(" << std::endl;
        }
        ++graphs_checked;
    }
}