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





TEST(HasChordTest, TestAllCyclesOnK4OnlyCyclesWith4VerticesShouldHaveAChord) {


	//this procedure is possible since K_4 is a complete graph (all vertices are connected to each other by unique edges
	//generate all possible cycles consisting of those 4 vertices
	const int num_vertices = 4;
	DynamicArray<DynamicArray<Vertex>> cycles{ 1 };
	
	while (cycles.Back().Size() != num_vertices) { //this should loop 4 times (equal to num_vertices)

		int num_cycles = cycles.Size();
		//loop through each existing cycle
		for (int i = 0; i < num_cycles; ++i) {

			//find which vertices this cycle has
			std::unordered_set<Vertex> vertices;
			for (Vertex v : cycles[i]) {
				vertices.insert(v);
			}

			//for each possible vertex, if it is not in the cycle, we will make a duplicate cycle with this vertex appended
			for (Vertex j = 0; j < num_vertices; ++j) {
				if (!vertices.contains(j)) {
					DynamicArray<Vertex> new_cycle{ cycles[i] };
					new_cycle.PushBack(j);
					cycles.PushBack(new_cycle);
				}
			}
		}
	}

	Graph K4{ Graph::K_4 };

	for (const auto& cycle : cycles) {
		if (cycle.Size() == num_vertices) {
			EXPECT_TRUE(K4.HasChord(cycle));
		}
		else {
			EXPECT_FALSE(K4.HasChord(cycle));
		}
	}


}