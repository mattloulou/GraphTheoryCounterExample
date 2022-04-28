//    ---   resources   ---
//Gtest Primer: https://github.com/google/googletest/blob/main/docs/primer.md
//to create this "Test Program": right click this solution >> add >> new project >> add the "Google Test" project.
//to run this file: https://social.msdn.microsoft.com/Forums/en-US/2fd105f5-6314-4035-8001-50640bbeae30/using-debug-to-run-only-one-project-instead-of-all-in-a-solution?forum=aspvisualstudio

#include <iostream>
#include <iomanip>
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

TEST(IsSimpleGraph, DegreeOneSelfLoopGraphExpectsNotSimpleGraph) {
	Graph two_vertex_graph{ AdjList{ {0}, {1} } };
	const std::string expected_output =
		"0:  0\n" //self-loop here
		"1:  1\n";//self-loop here

	//capture printed statements
	testing::internal::CaptureStdout();
	two_vertex_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_DEATH(two_vertex_graph.IsSimpleGraph(), "") << "testing self-loop condition for IsSimpleGraph()";
}

TEST(IsSimpleGraph, LargeSelfLoopGraphExpectsNotSimpleGraph) {
	Graph large_graph{ AdjList{{1,3,4}, {0,2,3,5}, {1,4}, {0,1,4}, {0,2,3,5}, {1,4,5}} };
	const std::string expected_output =
		"0:  1  3  4\n"
		"1:  0  2  3  5\n"
		"2:  1  4\n"
		"3:  0  1  4\n"
		"4:  0  2  3  5\n"
		"5:  1  4  5\n";  //self loop is here

	//capture printed statements
	testing::internal::CaptureStdout();
	large_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_DEATH(large_graph.IsSimpleGraph(), "") << "testing self-loop condition graph for IsSimpleGraph()";
}

TEST(IsSimpleGraph, SmallGraphWithDirectionalEdgeExpectsNotSimpleGraph) {
	Graph small_graph{ AdjList{{1}, {0,2}, {0, 1}} };
	const std::string expected_output =
		"0:  1\n"
		"1:  0  2\n"
		"2:  0  1\n";  //has an edge 2 --> 0, but not the other direction

	//capture printed statements
	testing::internal::CaptureStdout();
	small_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_DEATH(small_graph.IsSimpleGraph(), "") <<
		"testing bidirectional condition graph for IsSimpleGraph()."
		"Graph has directional edge, so it should not be simple, and so should fail an assertion.";
}

TEST(IsSimpleGraph, LargeGraphWithDirectionalEdgeExpectsNotSimpleGraph) {
	Graph large_graph{ AdjList{{1,3,4}, {0,2,3,5}, {1,4}, {0,1,4}, {0,2,3,5}, {1,4,3}} };
	const std::string expected_output =
		"0:  1  3  4\n"
		"1:  0  2  3  5\n"
		"2:  1  4\n"
		"3:  0  1  4\n"
		"4:  0  2  3  5\n"
		"5:  1  4  3\n";  //has an edge 5 --> 3, but not the other direction

	//capture printed statements
	testing::internal::CaptureStdout();
	large_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_DEATH(large_graph.IsSimpleGraph(), "") <<
		"testing bidirectional condition graph for IsSimpleGraph()."
		"Graph has directional edge, so it should not be simple, and so should fail an assertion.";
}

TEST(IsSimpleGraph, SmallDuplicateEdgeGraphExpectsNotSimpleGraph) {
	Graph small_graph{ AdjList{{1,2,2}, {0,2}, {0, 1, 0}} };
	const std::string expected_output =
		"0:  1  2  2\n" //2 <--> 0 twice, which is a duplicate
		"1:  0  2\n"
		"2:  0  1  0\n";  //2 <--> 0 twice, which is a duplicate

	//capture printed statements
	testing::internal::CaptureStdout();
	small_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_DEATH(small_graph.IsSimpleGraph(), "") <<
		"testing no-duplicate-edges condition graph for IsSimpleGraph()."
		"Graph has a duplicate edge, so it should not be simple, and so should fail an assertion.";
}

TEST(IsSimpleGraph, SmallDuplicateAndDirectionalEdgeFromLargestVertexGraphExpectsNotSimpleGraph) {
	Graph small_graph{ AdjList{{1}, {0,2}, {0,1,0}} };
	const std::string expected_output =
		"0:  1\n"
		"1:  0  2\n"
		"2:  0  1  0\n";  //2 --> 0 twice, which is both a duplicate edge and a directional edge.

	//capture printed statements
	testing::internal::CaptureStdout();
	small_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_DEATH(small_graph.IsSimpleGraph(), "") <<
		"testing bidirectional and no-duplicate-edges condition graph for IsSimpleGraph()."
		"Graph has directional edge which is also a duplicate, so it should not be simple, and so should fail an assertion.";
}

TEST(IsSimpleGraph, SmallDuplicateAndDirectionalEdgeFromSmallestVertexGraphExpectsNotSimpleGraph) {
	Graph small_graph{ AdjList{{1,2,2}, {0,2}, {1}} };
	const std::string expected_output =
		"0:  1  2  2\n" //0 --> 2 twice, which is both a duplicate edge and a directional edge.
		"1:  0  2\n"
		"2:  1\n";  

	//capture printed statements
	testing::internal::CaptureStdout();
	small_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_DEATH(small_graph.IsSimpleGraph(), "") <<
		"testing bidirectional and no-duplicate-edges condition graph for IsSimpleGraph()."
		"Graph has directional edge which is also a duplicate, so it should not be simple, and so should fail an assertion.";
}

TEST(IsSimpleGraph, NullGraphExpectsSimpleGraph) {
	Graph K4{};
	const std::string expected_output =
		"";

	//capture printed statements
	testing::internal::CaptureStdout();
	K4.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_TRUE(K4.IsSimpleGraph()) <<
		"testing all conditions for IsSimpleGraph()."
		"Graph should pass all assertions, and thus the function call returns true";
}

TEST(IsSimpleGraph, K4GraphExpectsSimpleGraph) {
	Graph two_vertex_graph{ Graph::K_4 };
	const std::string expected_output =
		"0:  1  2  3\n" 
		"1:  0  2  3\n"
		"2:  0  1  3\n"
		"3:  0  1  2\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	two_vertex_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_TRUE(two_vertex_graph.IsSimpleGraph()) <<
		"testing all conditions for IsSimpleGraph()."
		"Graph should pass all assertions, and thus the function call returns true";
}




TEST(GraphDefaultCtorTest, EmptyGraph) {
	Graph empty_graph{};
	EXPECT_TRUE(empty_graph.IsSimpleGraph()) << "graph should be wellformed";
	EXPECT_TRUE(empty_graph.VertexCount() == 0) << "graph should have a vertex count of 0";
	EXPECT_TRUE(empty_graph.EdgeCount() == 0) << "graph should have an edge count of 0";
}




TEST(GraphCtorForVertexCountTest, SmallDegreeOneGraph) {
	Graph two_vertex_graph{ 2 };
	const std::string expected_output =
		"0:\n"
		"1:\n";

	testing::internal::CaptureStdout();
	two_vertex_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(expected_output, real_output);
	EXPECT_EQ(two_vertex_graph.VertexCount(), 2);
	ASSERT_TRUE(two_vertex_graph.IsSimpleGraph());

	for (int i = 0; i < two_vertex_graph.VertexCount(); ++i) {
		ASSERT_TRUE(two_vertex_graph.VertexDeg(i) == 0);
	}
}

TEST(GraphCtorForVertexCountTest, LargeDegreeOneGraph) {
	Graph two_vertex_graph{ 8 };
	const std::string expected_output =
		"0:\n"
		"1:\n"
		"2:\n"
		"3:\n"
		"4:\n"
		"5:\n"
		"6:\n"
		"7:\n";

	testing::internal::CaptureStdout();
	two_vertex_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(expected_output, real_output);
	EXPECT_EQ(two_vertex_graph.VertexCount(), 8);
	ASSERT_TRUE(two_vertex_graph.IsSimpleGraph());

	for (int i = 0; i < two_vertex_graph.VertexCount(); ++i) {
		ASSERT_EQ(two_vertex_graph.VertexDeg(i), 0);
	}
}

TEST(GraphCtorForVertexCountTest, NegativeVertexCountParameter) {
	ASSERT_DEATH(Graph{ -1 }, "");
	//ASSERT_NO_FATAL_FAILURE(Graph{ -1 }); //this is basically the opposite to ASSERT_DEATH. This macro verifies the test works correctly.
}






TEST(GraphAdjListCtorTest, DegreeOneGraph) {
	Graph two_vertex_graph{ AdjList{ {1}, {0} } };
	const std::string expected_output =
		"0:  1\n"
		"1:  0\n";

	testing::internal::CaptureStdout();
	two_vertex_graph.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(expected_output, real_output);
	ASSERT_TRUE(two_vertex_graph.IsSimpleGraph()) << "testing two_vertex_graph for IsSimpleGraph()";
}

TEST(GraphAdjListCtorTest, DegreeThreeGraph) {
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
	ASSERT_TRUE(four_vertex_graph.IsSimpleGraph()) << "testing four_vertex_graph (same as K4) for IsSimpleGraph()";
}






TEST(VertexDegreeTests, SmallThreeVertexGraph) {
	Graph sample_graph{ AdjList{ {1}, {0}, {}} };
	
	EXPECT_EQ(sample_graph.VertexDeg(0), 1);
	EXPECT_EQ(sample_graph.VertexDeg(1), 1);
	EXPECT_EQ(sample_graph.VertexDeg(2), 0);
	EXPECT_TRUE(sample_graph.IsSimpleGraph());
}

TEST(VertexDegreeTests, K4VertexDegreeTest) {
	Graph sample_graph = Graph::K_4; //uses copy constructor

	for (int i = 0; i < sample_graph.VertexCount(); ++i) {
		ASSERT_EQ(sample_graph.VertexDeg(i), 3);
	}

	EXPECT_TRUE(sample_graph.IsSimpleGraph());
}

TEST(VertexDegreeTests, VertexOutOfRangeExpectsAssertionFailure) {
	Graph sample_graph = Graph::K_4;
	EXPECT_DEATH(sample_graph.VertexDeg(-1), ""); //below range
	EXPECT_DEATH(sample_graph.VertexDeg(4), ""); //above range
}




TEST(VertexCountTests, NullGraphVertexCountShouldBe0) {
	Graph null_graph{ AdjList{} };
	ASSERT_EQ(null_graph.VertexCount(), 0);
}

TEST(VertexCountTests, K4GraphVertexCountShouldBe4) {
	Graph graph{ Graph::K_4 };
	ASSERT_EQ(graph.VertexCount(), 4);
}




TEST(PushVertexTests, NullGraphGetsVerticesAdded) {
	Graph null_graph{ AdjList{} };

	//graph has no vertices right now
	std::string expected_output =
		"";

	//capture printed statements
	testing::internal::CaptureStdout();
	null_graph.PrintGraph(3);
	std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);




	//add extra vertices
	null_graph.PushVertex(4);
	expected_output =
		"0:\n"
		"1:\n"
		"2:\n"
		"3:\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	null_graph.PrintGraph(3);
	real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);




	//add 1 extra vertex using the default parameter for Graph::AddVertex()
	null_graph.PushVertex();
	expected_output =
		"0:\n"
		"1:\n"
		"2:\n"
		"3:\n"
		"4:\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	null_graph.PrintGraph(3);
	real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
}




TEST(PopVertexTests, K4GetsAllVerticesRemoved) {
	Graph g{ Graph::K_4 };


	//remove vertex #3
	g.PopVertex();
	std::string expected_output =
		"0:  1  2\n"
		"1:  0  2\n"
		"2:  0  1\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	g.PrintGraph(3);
	std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_TRUE(g.IsSimpleGraph());


	//remove vertex #2
	g.PopVertex();
	expected_output =
		"0:  1\n"
		"1:  0\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	g.PrintGraph(3);
	real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_TRUE(g.IsSimpleGraph());

	//remove vertex #1
	g.PopVertex();
	expected_output =
		"0:\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	g.PrintGraph(3);
	real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_TRUE(g.IsSimpleGraph());

	//remove vertex #0
	g.PopVertex();
	expected_output = "";

	//capture printed statements
	testing::internal::CaptureStdout();
	g.PrintGraph(3);
	real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_TRUE(g.IsSimpleGraph());
}




TEST(EdgeCountTests, NullGraphEdgeCountShouldBe0) {
	Graph null_graph{ AdjList{} };
	EXPECT_EQ(null_graph.EdgeCount(), 0);
	ASSERT_TRUE(null_graph.IsSimpleGraph());
}

TEST(EdgeCountTests, K4GraphEdgeCountShouldBe6) {
	Graph K4_graph{ Graph::K_4 };
	EXPECT_EQ(K4_graph.EdgeCount(), 6);
	ASSERT_TRUE(K4_graph.IsSimpleGraph());
}



TEST(FindEdgeTests, ValidVertex)
{
	Graph g{ AdjList{{1, 2}, {0}, {0}} };
	ASSERT_TRUE(g.FindEdge(0, 1) != nullptr);
	ASSERT_TRUE(g.FindEdge(0, 2) != nullptr);
	ASSERT_TRUE(g.FindEdge(1, 0) != nullptr);
	ASSERT_TRUE(g.FindEdge(2, 0) != nullptr);
	ASSERT_TRUE(g.IsSimpleGraph());
}

TEST(FindEdgeTests, InvalidVertex)
{
	Graph g{ AdjList{{1, 2}, {0}, {0}} };
	EXPECT_DEATH(g.FindEdge(2, 2), "");
	EXPECT_DEATH(g.FindEdge(10, 2), "");
	EXPECT_DEATH(g.FindEdge(2, 20), "");
	EXPECT_DEATH(g.FindEdge(20, 20), "");
	ASSERT_TRUE(g.IsSimpleGraph());
}

TEST(FindEdgeTests, NonExistantEdge)
{
	Graph g{ AdjList{{1, 2}, {0}, {0}} };
	EXPECT_TRUE(g.FindEdge(1, 2) == nullptr);
	ASSERT_TRUE(g.IsSimpleGraph());
}





TEST(HasEdgeTest, ValidVertex)
{
	Graph g{ AdjList{{1, 2}, {0}, {0}} };
    ASSERT_TRUE(g.HasEdge(0, 1)) ;
	ASSERT_TRUE(g.HasEdge(0, 2));
	ASSERT_TRUE(g.HasEdge(1, 0));
	ASSERT_TRUE(g.HasEdge(2, 0));
	ASSERT_TRUE(g.IsSimpleGraph());
}

TEST(HasEdgeTest, InvalidVertex)
{
	Graph g{ AdjList{{1, 2}, {0}, {0}} };
	EXPECT_DEATH(g.HasEdge(2, 2), "");
	EXPECT_DEATH(g.HasEdge(10, 2), "");
	EXPECT_DEATH(g.HasEdge(2, 20), "");
	EXPECT_DEATH(g.HasEdge(20, 20), "");
	ASSERT_TRUE(g.IsSimpleGraph());
}

TEST(HasEdgeTest, NonExistantEdge)
{
	Graph g{ AdjList{{1, 2}, {0}, {0}} };
	EXPECT_FALSE(g.HasEdge(1, 2));
	ASSERT_TRUE(g.IsSimpleGraph());
}






TEST(AddEdgeTest, AddValidEdgeToGraphWith2Vertices)
{
	Graph g{AdjList(2)};
	EXPECT_TRUE(g.AddEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.AddEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
}

TEST(AddEdgeTest, AddInvalidEdgesToGraphWith2Vertices)
{
	Graph g{AdjList(2)};
	EXPECT_DEATH(g.AddEdge(3, 1), "");
	EXPECT_DEATH(g.AddEdge(1, 3), "");
	EXPECT_DEATH(g.AddEdge(3, 3), "");
	EXPECT_DEATH(g.AddEdge(0, 0), "");
	EXPECT_DEATH(g.AddEdge(-1, 5), "");
}




TEST(RemoveEdgeTest, RemoveValidEdgesFromK4)
{
	Graph g{ Graph::K_4 };
	
	EXPECT_EQ(g.EdgeCount(), 6);
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_TRUE(g.RemoveEdge(0, 1));
	EXPECT_FALSE(g.HasEdge(0, 1));
	EXPECT_FALSE(g.HasEdge(1, 0));
	EXPECT_EQ(g.EdgeCount(), 5);
	
	EXPECT_EQ(g.EdgeCount(), 5);
	EXPECT_TRUE(g.HasEdge(3, 1));
	EXPECT_TRUE(g.HasEdge(1, 3));
	EXPECT_TRUE(g.RemoveEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_EQ(g.EdgeCount(), 4);

	EXPECT_EQ(g.EdgeCount(), 4);
	EXPECT_TRUE(g.HasEdge(3, 2));
	EXPECT_TRUE(g.HasEdge(2, 3));
	EXPECT_TRUE(g.RemoveEdge(3, 2));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_EQ(g.EdgeCount(), 3);

	EXPECT_EQ(g.EdgeCount(), 3);
	EXPECT_TRUE(g.HasEdge(3, 0));
	EXPECT_TRUE(g.HasEdge(0, 3));
	EXPECT_TRUE(g.RemoveEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_EQ(g.EdgeCount(), 2);

	EXPECT_EQ(g.EdgeCount(), 2);
	EXPECT_TRUE(g.HasEdge(2, 0));
	EXPECT_TRUE(g.HasEdge(0, 2));
	EXPECT_TRUE(g.RemoveEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_EQ(g.EdgeCount(), 1);

	EXPECT_EQ(g.EdgeCount(), 1);
	EXPECT_TRUE(g.HasEdge(2, 1));
	EXPECT_TRUE(g.HasEdge(1, 2));
	EXPECT_TRUE(g.RemoveEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_EQ(g.EdgeCount(), 0);

	const std::string expected_output =
		"0:\n"
		"1:\n"
		"2:\n"
		"3:\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	g.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
}




TEST(ClearEdgesTest, ClearEdgesOneVertexATimeForK4)
{
	Graph g{ Graph::K_4 };

	//remove all edges for vertex #3
	EXPECT_EQ(g.EdgeCount(), 6);
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_TRUE(g.HasEdge(0, 2));
	EXPECT_TRUE(g.HasEdge(2, 0));
	EXPECT_TRUE(g.HasEdge(0, 3));
	EXPECT_TRUE(g.HasEdge(3, 0));
	EXPECT_TRUE(g.HasEdge(2, 1));
	EXPECT_TRUE(g.HasEdge(1, 2));
	EXPECT_TRUE(g.HasEdge(3, 1));
	EXPECT_TRUE(g.HasEdge(1, 3));
	EXPECT_TRUE(g.HasEdge(2, 3));
	EXPECT_TRUE(g.HasEdge(3, 2));
	EXPECT_TRUE(g.ClearEdges(3));
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_TRUE(g.HasEdge(0, 2));
	EXPECT_TRUE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_TRUE(g.HasEdge(2, 1));
	EXPECT_TRUE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_EQ(g.EdgeCount(), 3);

	//remove all edges for vertex #2
	EXPECT_EQ(g.EdgeCount(), 3);
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_TRUE(g.HasEdge(0, 2));
	EXPECT_TRUE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_TRUE(g.HasEdge(2, 1));
	EXPECT_TRUE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_TRUE(g.ClearEdges(2));
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_EQ(g.EdgeCount(), 1);

	//remove all edges for vertex #2 again (nothing should change)
	EXPECT_EQ(g.EdgeCount(), 1);
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_TRUE(g.ClearEdges(2));
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_EQ(g.EdgeCount(), 1);

	//remove all edges for vertex #1
	EXPECT_EQ(g.EdgeCount(), 1);
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_TRUE(g.ClearEdges(1));
	EXPECT_FALSE(g.HasEdge(0, 1));
	EXPECT_FALSE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_EQ(g.EdgeCount(), 0);


	const std::string expected_output =
		"0:\n"
		"1:\n"
		"2:\n"
		"3:\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	g.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
}

TEST(ClearEdgesTest, ClearEdgesAllAtOnceForK4)
{
	Graph g{ Graph::K_4 };

	//remove all edges for vertex #3
	EXPECT_EQ(g.EdgeCount(), 6);
	EXPECT_TRUE(g.HasEdge(0, 1));
	EXPECT_TRUE(g.HasEdge(1, 0));
	EXPECT_TRUE(g.HasEdge(0, 2));
	EXPECT_TRUE(g.HasEdge(2, 0));
	EXPECT_TRUE(g.HasEdge(0, 3));
	EXPECT_TRUE(g.HasEdge(3, 0));
	EXPECT_TRUE(g.HasEdge(2, 1));
	EXPECT_TRUE(g.HasEdge(1, 2));
	EXPECT_TRUE(g.HasEdge(3, 1));
	EXPECT_TRUE(g.HasEdge(1, 3));
	EXPECT_TRUE(g.HasEdge(2, 3));
	EXPECT_TRUE(g.HasEdge(3, 2));
	EXPECT_TRUE(g.ClearEdges());
	EXPECT_FALSE(g.HasEdge(0, 1));
	EXPECT_FALSE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_EQ(g.EdgeCount(), 0);

	//removing all edges again (nothing should change)
	EXPECT_EQ(g.EdgeCount(), 0);
	EXPECT_FALSE(g.HasEdge(0, 1));
	EXPECT_FALSE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_TRUE(g.ClearEdges());
	EXPECT_FALSE(g.HasEdge(0, 1));
	EXPECT_FALSE(g.HasEdge(1, 0));
	EXPECT_FALSE(g.HasEdge(0, 2));
	EXPECT_FALSE(g.HasEdge(2, 0));
	EXPECT_FALSE(g.HasEdge(0, 3));
	EXPECT_FALSE(g.HasEdge(3, 0));
	EXPECT_FALSE(g.HasEdge(2, 1));
	EXPECT_FALSE(g.HasEdge(1, 2));
	EXPECT_FALSE(g.HasEdge(3, 1));
	EXPECT_FALSE(g.HasEdge(1, 3));
	EXPECT_FALSE(g.HasEdge(2, 3));
	EXPECT_FALSE(g.HasEdge(3, 2));
	EXPECT_EQ(g.EdgeCount(), 0);

	const std::string expected_output =
		"0:\n"
		"1:\n"
		"2:\n"
		"3:\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	g.PrintGraph(3);
	const std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
}





TEST(ClearGraphTest, UseClearGraphOnK4ResultsInNullGraph) {
	Graph K4{ Graph::K_4 };
	std::string expected_output =
		"0:  1  2  3\n"
		"1:  0  2  3\n"
		"2:  0  1  3\n"
		"3:  0  1  2\n";

	//capture printed statements
	testing::internal::CaptureStdout();
	K4.PrintGraph(3);
	std::string real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_TRUE(K4.IsSimpleGraph()) <<
		"testing all conditions for IsSimpleGraph()."
		"Graph should pass all assertions, and thus the function call returns true";

	//clear the graph
	K4.ClearGraph();

	expected_output =
		"";

	//capture printed statements
	testing::internal::CaptureStdout();
	K4.PrintGraph(3);
	real_output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(expected_output, real_output);
	EXPECT_TRUE(K4.IsSimpleGraph()) <<
		"testing all conditions for IsSimpleGraph()."
		"Graph should pass all assertions, and thus the function call returns true";
}





TEST(IsConnectedTest, NullGraphIsConnectedExpectsTrue) {
	Graph g{};
	EXPECT_TRUE(g.IsConnected());
}

TEST(IsConnectedTest, GraphWithOneVertexIsConnectedExpectsTrue) {
	Graph g{ 1 };
	EXPECT_TRUE(g.IsConnected());
}

TEST(IsConnectedTest, GraphWithTwoVerticesWithAnEdgeIsConnectedExpectsTrue) {
	Graph g{ AdjList{{1}, {0}} };
	EXPECT_TRUE(g.IsConnected());
}

TEST(IsConnectedTest, K4IsConnectedExpectsTrue) {
	Graph g = Graph::K_4;
	EXPECT_TRUE(g.IsConnected());
}

TEST(IsConnectedTest, GraphWithTwoVerticesButNoEdgesIsConnectedExpectsFalse) {
	Graph g{ 2 };
	EXPECT_FALSE(g.IsConnected());
}

TEST(IsConnectedTest, GraphWith100VerticesButNoEdgesIsConnectedExpectsFalse) {
	Graph g{ 100 };
	EXPECT_FALSE(g.IsConnected());
}

TEST(IsConnectedTest, GraphWithFourVerticesButOneVertexHasNoEdgesIsConnectedExpectsFalse) {
	Graph g = Graph::K_4;
	g.ClearEdges(0);
	EXPECT_FALSE(g.IsConnected());
}

TEST(IsConnectedTest, GraphWithFourVerticesButOneVertexHasOneEdgesIsConnectedExpectsTrue) {
	Graph g = Graph::K_4;
	g.ClearEdges(0);
	EXPECT_FALSE(g.IsConnected());
	g.AddEdge(0, 3);
	EXPECT_TRUE(g.IsConnected());
}


TEST(IsKVertexConnectedTest, GraphWithTwoVerticesIsOnlyOneConnected) {
	Graph g{ AdjList{{1}, {0}} };
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(1));

	//these are all invalid options
	for (int i = -10; i < 10; ++i) {
		if (i != 1) {
			EXPECT_DEATH(g.IsKVertexConnected(i), "");
		}
	}
}

TEST(IsKVertexConnectedTest, K4IsOnly1And2And3VertexConnected) {
	Graph g = Graph::K_4;
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(1));
	EXPECT_TRUE(g.IsKVertexConnected(2));
	EXPECT_TRUE(g.IsKVertexConnected(3));

	for (int i = -10; i < 10; ++i) {
		if (i < 1 || i > 3) {
			EXPECT_DEATH(g.IsKVertexConnected(i), "");
		}
	}
}

TEST(IsKVertexConnectedTest, K4WithEdgeRemovedIsOnly1And2VertexConnected) {
	Graph g = Graph::K_4;
	g.RemoveEdge(0, 1);
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(1));
	EXPECT_TRUE(g.IsKVertexConnected(2));
	EXPECT_FALSE(g.IsKVertexConnected(3));

	for (int i = -10; i < 10; ++i) {
		if (i < 1 || i > 3) {
			EXPECT_DEATH(g.IsKVertexConnected(i), "");
		}
	}
}

TEST(IsKVertexConnectedTest, CreatedGraphIsOnly1And2And3VertexConnected) {
	Graph g{ AdjList{{1,3,5}, {0,2,5}, {1,3,4,5}, {0,2,4}, {2,3,5}, {0,1,2,4}} };
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(1));
	EXPECT_TRUE(g.IsKVertexConnected(2));
	EXPECT_TRUE(g.IsKVertexConnected(3));
	EXPECT_FALSE(g.IsKVertexConnected(4));
	EXPECT_FALSE(g.IsKVertexConnected(5));

	for (int i = -10; i < 10; ++i) {
		if (i < 1 || i > 5) {
			EXPECT_DEATH(g.IsKVertexConnected(i), "");
		}
	}
}

TEST(IsKVertexConnectedTest, AnotherCreatedGraphIsOnly1And2And3VertexConnected) {
	Graph g{ AdjList{{1,2,3}, {3,0,2,5}, {0,1,6,4}, {0,1,6,4}, {3,2,6,5}, {1,4,6}, {4,3,5,2}} };
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(1));
	EXPECT_TRUE(g.IsKVertexConnected(2));
	EXPECT_TRUE(g.IsKVertexConnected(3));
	EXPECT_FALSE(g.IsKVertexConnected(4));
	EXPECT_FALSE(g.IsKVertexConnected(5));
	EXPECT_FALSE(g.IsKVertexConnected(6));

	for (int i = -10; i < 10; ++i) {
		if (i < 1 || i > 6) {
			EXPECT_DEATH(g.IsKVertexConnected(i), "");
		}
	}
}




TEST(HasChordTest, TestAllCyclesOnK4OnlyCyclesWith4VerticesShouldHaveAChord) {

	////this procedure is possible since K_4 is a complete graph (all vertices are connected to each other by unique edges
	////generate all possible cycles consisting of those 4 vertices
	//const int num_vertices = 4;
	//DynamicArray<DynamicArray<Vertex>> cycles{ 1 };
	//
	////print out the starting 
	////std::cout << std::endl << "size: " << cycles.Size();
	////std::cout << "List of cycles:" << std::endl;
	////int count2 = 0;
	////for (const auto& cycle : cycles) {
	////	std::cout << "cycle number " << count2 << ": ";
	////	for (const auto& v : cycle) {
	////		std::cout << v << " ";
	////	}
	////	std::cout << "\n";
	////	++count2;
	////}

	////permutation generator (for all unique combinations of 0 to all vertices):

	//int first_cycle = 0; //first cycle of the size we are now generating
	//int current_batch_cycle_count = 0; //size of the current batch of cycles (set to 0 for now for clean code)
	//int next_batch_cycle_count = 1; //size of the next batch of cycles (1 size larger). It is set 1 by default for the batch of cycles of Size()==0 (there is only 1)

	//while (cycles.Back().Size() != num_vertices) { //this should loop 4 times (equal to num_vertices). Each loop will create all permutations of 1 size larger

	//	int num_cycles = cycles.Size();
	//	first_cycle += current_batch_cycle_count;
	//	current_batch_cycle_count = next_batch_cycle_count;
	//	next_batch_cycle_count = 0;

	//	//loop through each existing cycle
	//	for (int i = first_cycle; i < num_cycles; ++i) {

	//		const DynamicArray<Vertex> this_cycle = cycles[i];

	//		//find which vertices this cycle has
	//		std::unordered_set<Vertex> vertices;
	//		for (const auto& v : this_cycle) {
	//			vertices.insert(v);
	//		}

	//		//for each possible vertex, if it is not in this_cycle, we will make a duplicate cycle with this vertex appended
	//		for (Vertex j = 0; j < num_vertices; ++j) {
	//			if (!vertices.contains(j)) {
	//				DynamicArray<Vertex> new_cycle{ this_cycle };
	//				new_cycle.PushBack(j);
	//				cycles.PushBack(new_cycle);
	//				++next_batch_cycle_count;
	//			}
	//		}
	//	}
	//}

	//list out all of the generated cycles
	//std::cout << std::endl << "size: " << cycles.Size();
	//std::cout << "List of cycles:" << std::endl;
	//int count = 0;
	//for (const auto& cycle : cycles) {
	//	std::cout << "cycle number " << count << ": ";
	//	for (const auto& v : cycle) {
	//		std::cout << v << " ";
	//	}
	//	std::cout << "\n";
	//	++count;
	//}


	Graph K4{ Graph::K_4 };

	DynamicArray<DynamicArray<Vertex>> cycles = Graph::AllPermutations(4);
	EXPECT_TRUE(cycles.Size() == 65);

	for (const auto& cycle : cycles) {
		if (cycle.Size() == 4) {
			EXPECT_TRUE(K4.HasChord(cycle));
		}
		else {
			EXPECT_FALSE(K4.HasChord(cycle));
		}
	}
}

TEST(HasChordTest, TestSomeCyclesInArbitraryGraphWithFourVertices) {
	Graph sample_graph{ AdjList{{1,2}, {0,2,3}, {0,1,3}, {1,2}} };
	EXPECT_TRUE(sample_graph.IsSimpleGraph()) << "Sample graph is not simple";
	DynamicArray<DynamicArray<Vertex>> cycles = Graph::AllPermutations(4);


	//list out all of the generated cycles
	
	//std::cout << std::endl << "size: " << cycles.Size();
	//std::cout << "List of cycles:" << std::endl;
	//int count = 0;
	//for (const auto& cycle : cycles) {
	//	std::cout << "cycle number " << std::setw(4) << count << ": ";
	//	std::cout << "|" << Graph::VertexListToString(cycle) << "|\n";
	//	++count;
	//}


	std::unordered_set<std::string> permutations_with_chords;

	//valid cycles with all vertices
	permutations_with_chords.insert("0 1 3 2");
	permutations_with_chords.insert("1 3 2 0");
	permutations_with_chords.insert("3 2 0 1");
	permutations_with_chords.insert("2 0 1 3");
	//other direction
	permutations_with_chords.insert("0 2 3 1");
	permutations_with_chords.insert("2 3 1 0");
	permutations_with_chords.insert("3 1 0 2");
	permutations_with_chords.insert("1 0 2 3");


	for (const auto& cycle : cycles) {
		if (permutations_with_chords.contains(Graph::VertexListToString(cycle)))
		{
			EXPECT_TRUE(sample_graph.HasChord(cycle));
			//std::cout << "true " << Graph::VertexListToString(cycle) << std::endl;

		}
		else {
			EXPECT_FALSE(sample_graph.HasChord(cycle));
			//std::cout << "false " << Graph::VertexListToString(cycle) << std::endl;

		}
	}
}

TEST(HasChordTest, TestSomeCyclesInArbitraryGraphWithFiveVertices) {
	Graph sample_graph{ AdjList{{1,2,3}, {0,3,4}, {0,3}, {0,1,2,4}, {1,3}} };
	EXPECT_TRUE(sample_graph.IsSimpleGraph()) << "Sample graph is not simple";
	DynamicArray<DynamicArray<Vertex>> cycles = Graph::AllPermutations(5);


	//list out all of the generated cycles
	
	//std::cout << std::endl << "size: " << cycles.Size();
	//std::cout << "List of cycles:" << std::endl;
	//int count = 0;
	//for (const auto& cycle : cycles) {
	//	std::cout << "cycle number " << std::setw(4) << count << ": ";
	//	std::cout << "|" << Graph::VertexListToString(cycle) << "|\n";
	//	++count;
	//}


	std::unordered_set<std::string> permutations_with_chords;

	//valid cycles with all vertices
	permutations_with_chords.insert("0 1 4 3 2");
	permutations_with_chords.insert("1 4 3 2 0");
	permutations_with_chords.insert("4 3 2 0 1");
	permutations_with_chords.insert("3 2 0 1 4");
	permutations_with_chords.insert("2 0 1 4 3");
	//other direction
	permutations_with_chords.insert("0 2 3 4 1");
	permutations_with_chords.insert("2 3 4 1 0");
	permutations_with_chords.insert("3 4 1 0 2");
	permutations_with_chords.insert("4 1 0 2 3");
	permutations_with_chords.insert("1 0 2 3 4");


	//cycles excluding vertex #2
	permutations_with_chords.insert("0 1 4 3");
	permutations_with_chords.insert("1 4 3 0");
	permutations_with_chords.insert("4 3 0 1");
	permutations_with_chords.insert("3 0 1 4");
	//other direction
	permutations_with_chords.insert("0 3 4 1");
	permutations_with_chords.insert("3 4 1 0");
	permutations_with_chords.insert("4 1 0 3");
	permutations_with_chords.insert("1 0 3 4");

	//cycles excluding vertex #4
	permutations_with_chords.insert("0 1 3 2");
	permutations_with_chords.insert("1 3 2 0");
	permutations_with_chords.insert("3 2 0 1");
	permutations_with_chords.insert("2 0 1 3");
	//other direction
	permutations_with_chords.insert("0 2 3 1");
	permutations_with_chords.insert("2 3 1 0");
	permutations_with_chords.insert("3 1 0 2");
	permutations_with_chords.insert("1 0 2 3");

	for (const auto& cycle : cycles) {
		if (permutations_with_chords.contains(Graph::VertexListToString(cycle)))
		{
			EXPECT_TRUE(sample_graph.HasChord(cycle));
			//std::cout << "true " << Graph::VertexListToString(cycle) << std::endl;
			
		}
		else {
			EXPECT_FALSE(sample_graph.HasChord(cycle));
			//std::cout << "false " << Graph::VertexListToString(cycle) << std::endl;

		}
	}
}





TEST(IsValidDirectionalCycleTest, CheckEmptyCycleEXPFalse)
{
    DynamicArray<Vertex> direction;

    Graph k4 = Graph::K_4;

    EXPECT_FALSE(k4.IsValidDirectionalCycle(direction));
}

TEST(IsValidDirectionalCycleTest, CheckInvalidCycleWithInvalidVertexEXPDeath)
{
    DynamicArray<Vertex> direction{ 9 ,5, 3, 1};

    Graph k4 = Graph::K_4;

    EXPECT_DEATH(k4.IsValidDirectionalCycle(direction), "");
}

TEST(IsValidDirectionalCycleTest, CheckInvalidCycleWithSameVertexEXPDeath)
{
    DynamicArray<Vertex> direction{ 0, 0 };

    Graph k4 = Graph::K_4;

    EXPECT_DEATH(k4.IsValidDirectionalCycle(direction), "");
}

TEST(IsValidDirectionalCycleTest, CheckInvalidCycleWithSameVertexEXPFalse)
{
    DynamicArray<Vertex> direction{ 0 };

    Graph k4 = Graph::K_4;

    EXPECT_FALSE(k4.IsValidDirectionalCycle(direction));
}


TEST(IsValidDirectionalCycleTest, CheckValidCycleExpectTrue)
{
    DynamicArray<Vertex> direction{ 0, 1, 2, 3};

    Graph k4 = Graph::K_4;

    EXPECT_TRUE(k4.IsValidDirectionalCycle(direction));
}

TEST(IsValidDirectionalCycleTest, CheckValidCycleExpectFalse)
{
    DynamicArray<Vertex> direction{ 0, 1, 2, 3};

    Graph graph{ AdjList{ {1,2},{0,2,3},{0,1,3},{1,2} } };

    EXPECT_FALSE(graph.IsValidDirectionalCycle(direction));
}




TEST(DoAllLargestCyclesHaveAChordTest, K4DoAllLargestCyclesHaveAChordExpectsTrue) {
	Graph g = Graph::K_4;
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(3));
	EXPECT_TRUE(g.DoAllLargestCyclesHaveAChord());
}

TEST(DoAllLargestCyclesHaveAChordTest, SquareGraphWithDiagonalDoAllLargestCyclesHaveAChordExpectsTrue) {
	Graph g{ AdjList{{1,2,3}, {0,3}, {0,3}, {0,1,2}} };
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(2));
	EXPECT_TRUE(g.DoAllLargestCyclesHaveAChord());
}

TEST(DoAllLargestCyclesHaveAChordTest, SquareGraphWithNoDiagonalDoAllLargestCyclesHaveAChordExpectsFalse) {
	Graph g{ AdjList{{1,2}, {0,3}, {0,3}, {1,2}} };
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(2));
	EXPECT_FALSE(g.DoAllLargestCyclesHaveAChord());
}

TEST(DoAllLargestCyclesHaveAChordTest, GraphWithNoCyclesDoAllLargestCyclesHaveAChordExpectsTrue) {
	Graph g{ AdjList{{1}, {0,3}, {3}, {1,2}} };
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(1));
	EXPECT_TRUE(g.DoAllLargestCyclesHaveAChord());
}

TEST(DoAllLargestCyclesHaveAChordTest, GraphWithMultipleCyclesButLargestHaveNoChordsDoAllLargestCyclesHaveAChordExpectsFalse) {
	Graph g{ AdjList{{1,5}, {0,3}, {3,6,7}, {1,2,6,4}, {5,3}, {0,4}, {2,3,7}, {2,6}} };
	EXPECT_TRUE(g.IsSimpleGraph());
	EXPECT_TRUE(g.IsKVertexConnected(1));
	EXPECT_FALSE(g.DoAllLargestCyclesHaveAChord());
}






TEST(AllPermutationsTest, PermutationsWith0Choices) {

	const int num_choices = 0;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices);
	EXPECT_EQ(permutations.Size(), 1);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		//make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, num_choices)  (in the sole permutation here there are no elements)
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
		
	}
}

TEST(AllPermutationsTest, PermutationsWith1Choice) {

	const int num_choices = 1;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices);
	EXPECT_EQ(permutations.Size(), 2);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		//make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		////make sure that all elements in the permutation are unique and within range of [0, num_choices)
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}

	}
}

TEST(AllPermutationsTest, PermutationsWith2Choices) {

	const int num_choices = 2;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices);
	EXPECT_EQ(permutations.Size(), 5);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		////make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllPermutationsTest, PermutationsWith3Choices) {

	const int num_choices = 3;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices);
	EXPECT_EQ(permutations.Size(), 16);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		////make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}




TEST(AllPermutationsWithMaxSizeTest, PermutationsWith0Choices0MaxSizeExpectsSameResultAsWithoutGivingAMaxSize) {

	const int num_choices = 0;
	const int max_size = 0;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices, max_size);
	EXPECT_EQ(permutations.Size(), 1);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		//make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, num_choices)  (in the sole permutation here there are no elements)
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}

	}
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith1Choice1MaxSizeExpectsSameResultAsWithoutGivingAMaxSize) {

	const int num_choices = 1;
	const int max_size = 1;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices, max_size);
	EXPECT_EQ(permutations.Size(), 2);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		//make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		////make sure that all elements in the permutation are unique and within range of [0, num_choices)
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}

	}
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith2Choices2MaxSizeExpectsSameResultAsWithoutGivingAMaxSize) {

	const int num_choices = 2;
	const int max_size = 2;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices, max_size);
	EXPECT_EQ(permutations.Size(), 5);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		////make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith3Choices3MaxSizeExpectsSameResultAsWithoutGivingAMaxSize) {

	const int num_choices = 3;
	const int max_size = 3;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices, max_size);
	EXPECT_EQ(permutations.Size(), 16);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		////make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith3Choices2MaxSize) {

	const int num_choices = 3;
	const int max_size = 2;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices, max_size);
	EXPECT_EQ(permutations.Size(), 10);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		////make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, max_size) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith4Choices1MaxSize) {

	const int num_choices = 4;
	const int max_size = 1;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices, max_size);
	EXPECT_EQ(permutations.Size(), 5);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		////make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, max_size) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith10Choices0MaxSize) {

	const int num_choices = 10;
	const int max_size = 0;
	DynamicArray<DynamicArray<Vertex>> permutations = Graph::AllPermutations(num_choices, max_size);
	EXPECT_EQ(permutations.Size(), 1);

	std::unordered_set<std::string> permutation_set;
	for (const auto& permutation : permutations) {

		////make sure that the permutation is unique
		EXPECT_FALSE(permutation_set.contains(Graph::VertexListToString(permutation)));
		permutation_set.insert(Graph::VertexListToString(permutation));

		//make sure that all elements in the permutation are unique and within range of [0, max_size) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : permutation) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith0Choices2MaxSizeExpectsDeath) {

	const int num_choices = 0;
	const int max_size = 2;
	EXPECT_DEATH(Graph::AllPermutations(num_choices, max_size), "");
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith5Choices7MaxSizeExpectsDeath) {

	const int num_choices = 5;
	const int max_size = 7;
	EXPECT_DEATH(Graph::AllPermutations(num_choices, max_size), "");
}

TEST(AllPermutationsWithMaxSizeTest, PermutationsWith5ChoicesNegative1MaxSizeExpectsDeath) {

	const int num_choices = 5;
	const int max_size = -1;
	EXPECT_DEATH(Graph::AllPermutations(num_choices, max_size), "");
}





TEST(AllCombinationsTest, CombinationsWith0Choices) {

	const int num_choices = 0;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices);
	EXPECT_EQ(combinations.Size(), 1);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		//make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, num_choices)  (in the sole combination here there are no elements)
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}

	}
}

TEST(AllCombinationsTest, CombinationsWith1Choice) {

	const int num_choices = 1;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices);
	EXPECT_EQ(combinations.Size(), 2);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		//make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		////make sure that all elements in the combination are unique and within range of [0, num_choices)
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}

	}
}

TEST(AllCombinationsTest, CombinationsWith2Choices) {

	const int num_choices = 2;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices);
	EXPECT_EQ(combinations.Size(), 4);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsTest, CombinationsWith3Choices) {

	const int num_choices = 3;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices);
	EXPECT_EQ(combinations.Size(), 8);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}



TEST(AllCombinationsWithMaxSizeTest, CombinationsWith0Choices0MaxSizeExpectsSameResultAsWithoutGivingAMaxSize) {

	const int num_choices = 0;
	const int max_size = 0;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices, max_size);
	EXPECT_EQ(combinations.Size(), 1);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith1Choice1MaxSizeExpectsSameResultAsWithoutGivingAMaxSize) {

	const int num_choices = 1;
	const int max_size = 1;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices, max_size);
	EXPECT_EQ(combinations.Size(), 2);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith2Choices2MaxSizeExpectsSameResultAsWithoutGivingAMaxSize) {

	const int num_choices = 2;
	const int max_size = 2;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices, max_size);
	EXPECT_EQ(combinations.Size(), 4);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith3Choices3MaxSizeExpectsSameResultAsWithoutGivingAMaxSize) {

	const int num_choices = 3;
	const int max_size = 3;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices, max_size);
	EXPECT_EQ(combinations.Size(), 8);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, num_choices) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith3Choices2MaxSize) {

	const int num_choices = 3;
	const int max_size = 2;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices, max_size);
	EXPECT_EQ(combinations.Size(), 7);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, max_size) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith4Choices1MaxSize) {

	const int num_choices = 4;
	const int max_size = 1;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices, max_size);
	EXPECT_EQ(combinations.Size(), 5);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, max_size) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith10Choices0MaxSize) {

	const int num_choices = 10;
	const int max_size = 0;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices, max_size);
	EXPECT_EQ(combinations.Size(), 1);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, max_size) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith4Choices3MaxSize) {

	const int num_choices = 4;
	const int max_size = 3;
	DynamicArray<DynamicArray<Vertex>> combinations = Graph::AllCombinations(num_choices, max_size);
	EXPECT_EQ(combinations.Size(), 15);

	std::unordered_set<std::string> combination_set;
	for (const auto& combination : combinations) {

		////make sure that the combination is unique
		EXPECT_FALSE(combination_set.contains(Graph::VertexListToString(combination)));
		combination_set.insert(Graph::VertexListToString(combination));

		//make sure that all elements in the combination are unique and within range of [0, max_size) 
		std::unordered_set<Vertex> vertices;
		for (const auto& v : combination) {
			EXPECT_FALSE(vertices.contains(v));
			vertices.insert(v);
			EXPECT_TRUE(0 <= v && v < num_choices);
		}
	}
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith0Choices2MaxSizeExpectsDeath) {

	const int num_choices = 0;
	const int max_size = 2;
	EXPECT_DEATH(Graph::AllCombinations(num_choices, max_size), "");
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith5Choices7MaxSizeExpectsDeath) {

	const int num_choices = 5;
	const int max_size = 7;
	EXPECT_DEATH(Graph::AllCombinations(num_choices, max_size), "");
}

TEST(AllCombinationsWithMaxSizeTest, CombinationsWith5ChoicesNegative1MaxSizeExpectsDeath) {

	const int num_choices = 5;
	const int max_size = -1;
	EXPECT_DEATH(Graph::AllCombinations(num_choices, max_size), "");
}




TEST(VertexListToStringTest, EmptyDynamicArrayShouldGiveEmptyString) {
	
	DynamicArray<Vertex> arr{};
	
	const std::string expected_output = "";
	const std::string actual_output = Graph::VertexListToString(arr);

	EXPECT_EQ(expected_output, actual_output);
}

TEST(VertexListToStringTest, DynamicArrayWithOneElementShouldGiveStringWithJustElement) {

	DynamicArray<Vertex> arr{2};

	const std::string expected_output = "2";
	const std::string actual_output = Graph::VertexListToString(arr);

	EXPECT_EQ(expected_output, actual_output);
}

TEST(VertexListToStringTest, DynamicArrayWithMultipleElementsShouldListOutAllElements) {

	DynamicArray<Vertex> arr{ 1,2,4,8,16 };

	const std::string expected_output = "1 2 4 8 16";
	const std::string actual_output = Graph::VertexListToString(arr);

	EXPECT_EQ(expected_output, actual_output);
}