//    ---   resources   ---
//Gtest Primer: https://github.com/google/googletest/blob/main/docs/primer.md
//to create this "Test Program": right click this solution >> add >> new project >> add the "Google Test" project.
//to run this file: https://social.msdn.microsoft.com/Forums/en-US/2fd105f5-6314-4035-8001-50640bbeae30/using-debug-to-run-only-one-project-instead-of-all-in-a-solution?forum=aspvisualstudio

#include <iostream>
#include "pch.h"
#include "../Graph.h"
#include "../Graph.cpp" //the linker needs this for some reason... It couldn't link on its own. Poor compiler.
#include "../DynamicArray.h"

/////////////////////////////   Dynamic Array Tests   /////////////////////////////
TEST(DynamicArrayCtor, EmptyArray)
{
	DynamicArray<int> vec;
	EXPECT_TRUE(vec.IsEmpty());
}

TEST(DynamicArrayCtor, SingleElement)
{
	DynamicArray<int> vec{1};
	EXPECT_TRUE(vec[0] == 1 );
	EXPECT_TRUE(vec.Size() == 1);
}

TEST(DynamicArrayCtor, MultipleElements)
{
	DynamicArray<int> vec{1, 2, 3, 4};
	for (int i = 0; i < vec.Size(); ++i) {
		ASSERT_TRUE(vec[i] == i+1);
	}
}

TEST(DynamicArrayCopyCtor, EmptyWithCopyConstructor)
{
	DynamicArray<int> vec{};
	DynamicArray<int> vec1(vec);
	ASSERT_TRUE(vec1.IsEmpty());
}

TEST(DynamicArrayCopyCtor, FilledWithCopyConstructor)
{
	DynamicArray<int> vec{0,1,2,3,4};
	DynamicArray<int> vec1(vec);
	for (int i = 0; i < vec.Size(); ++i) {
		ASSERT_TRUE(vec1[i] == i);
	}
}

/////////////////////////////   Graph Tests   /////////////////////////////

TEST(GraphDefaultCtorTest, EmptyGraph) {
	Graph empty_graph{};
	ASSERT_TRUE(empty_graph.IsWellformed()) << "graph should be wellformed";
	ASSERT_TRUE(empty_graph.VertexCount() == 0) << "graph should have a vertex count of 0";
	ASSERT_TRUE(empty_graph.EdgeCount() == 0) << "graph should have an edge count of 0";

}


TEST(GraphAdjListCtorTest, GraphTesting) {
	Graph four_vertex_graph{ AdjList{{1,2,3},{0,2,3},{0,1,3},{0,1,2}} };
	const std::string expected_output = 
		"0:  1  2  3\n" 
		"1:  0  2  3\n"
		"2:  0  1  3\n"
		"3:  0  1  2\n";
	testing::internal::CaptureStdout();
	four_vertex_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);

	ASSERT_TRUE(four_vertex_graph.IsWellformed()) << "testing four_vertex_graph for IsWellformed()";
}









TEST(GraphHasEdgeTest, Test1)
{
	Graph g{ AdjList{{1, 2}, {0}, {0}} };
    ASSERT_TRUE(g.HasEdge(0, 1)) ;
	ASSERT_TRUE(g.HasEdge(0, 2));
	ASSERT_TRUE(g.HasEdge(1, 0));
	ASSERT_TRUE(g.HasEdge(2, 0));
	EXPECT_DEATH(g.HasEdge(2, 2));
	EXPECT_DEATH(g.HasEdge(10, 2));
	EXPECT_DEATH(g.HasEdge(2, 20));
	EXPECT_DEATH(g.HasEdge(20, 20));
	ASSERT_FALSE(g.HasEdge(1, 2));
	ASSERT_TRUE(g.IsWellformed());
}