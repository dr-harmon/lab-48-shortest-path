#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include "shortest_path.h"

#include <string>
#include <vector>

using namespace std;

TEST_CASE("Shortest path (target only)")
{
	ShortestPathFinder<string,int> pathFinder;
	auto& a = pathFinder.insertVertex("A");
	auto& b = pathFinder.insertVertex("B");
	auto& c = pathFinder.insertVertex("C");
	auto& d = pathFinder.insertVertex("D");
	auto& e = pathFinder.insertVertex("E");
	auto& f = pathFinder.insertVertex("F");
	auto e1 = pathFinder.insertEdge(a, b, 8);
	auto e2 = pathFinder.insertEdge(a, c, 2);
	auto e3 = pathFinder.insertEdge(a, d, 4);
	auto e4 = pathFinder.insertEdge(b, c, 7);
	auto e5 = pathFinder.insertEdge(b, e, 2);
	auto e6 = pathFinder.insertEdge(c, d, 1);
	auto e7 = pathFinder.insertEdge(c, e, 3);
	auto e8 = pathFinder.insertEdge(c, f, 9);
	auto e9 = pathFinder.insertEdge(d, f, 5);

    auto paths = pathFinder.findShortestPath(a, b);
    REQUIRE(paths[paths.size() - 1].second == 7);

    paths = pathFinder.findShortestPath(a, f);
    REQUIRE(paths[paths.size() - 1].second == 8);
}

TEST_CASE("Shortest path")
{
	ShortestPathFinder<string,int> pathFinder;
	auto& a = pathFinder.insertVertex("A");
	auto& b = pathFinder.insertVertex("B");
	auto& c = pathFinder.insertVertex("C");
	auto& d = pathFinder.insertVertex("D");
	auto& e = pathFinder.insertVertex("E");
	auto& f = pathFinder.insertVertex("F");
	auto e1 = pathFinder.insertEdge(a, b, 8);
	auto e2 = pathFinder.insertEdge(a, c, 2);
	auto e3 = pathFinder.insertEdge(a, d, 4);
	auto e4 = pathFinder.insertEdge(b, c, 7);
	auto e5 = pathFinder.insertEdge(b, e, 2);
	auto e6 = pathFinder.insertEdge(c, d, 1);
	auto e7 = pathFinder.insertEdge(c, e, 3);
	auto e8 = pathFinder.insertEdge(c, f, 9);
	auto e9 = pathFinder.insertEdge(d, f, 5);
    
    REQUIRE(pathFinder.findShortestPath(a, b) == vector<pair<string,int>> {
        {"A", 0},
        {"C", 2},
        {"E", 5},
        {"B", 7},
    });

    REQUIRE(pathFinder.findShortestPath(a, f) == vector<pair<string,int>> {
        {"A", 0},
        {"C", 2},
        {"D", 3},
        {"F", 8},
    });
}

TEST_CASE("Shortest path (flights)")
{
	ShortestPathFinder<string,int> pathFinder;
	auto& sfo = pathFinder.insertVertex("SFO");
	auto& lax = pathFinder.insertVertex("LAX");
	auto& ord = pathFinder.insertVertex("ORD");
	auto& dfw = pathFinder.insertVertex("DFW");
	auto& mia = pathFinder.insertVertex("MIA");
	auto& bwi = pathFinder.insertVertex("BWI");
	auto& jfk = pathFinder.insertVertex("JFK");
	auto& pvd = pathFinder.insertVertex("PVD");
	auto& bos = pathFinder.insertVertex("BOS");
	auto e1 = pathFinder.insertEdge(sfo, lax, 337);
	auto e2 = pathFinder.insertEdge(sfo, dfw, 1464);
	auto e3 = pathFinder.insertEdge(sfo, ord, 1846);
	auto e4 = pathFinder.insertEdge(sfo, bos, 2704);
	auto e5 = pathFinder.insertEdge(lax, dfw, 1235);
	auto e6 = pathFinder.insertEdge(lax, mia, 2342);
	auto e7 = pathFinder.insertEdge(dfw, mia, 1121);
	auto e8 = pathFinder.insertEdge(dfw, ord, 802);
	auto e9 = pathFinder.insertEdge(dfw, jfk, 1391);
	auto e10 = pathFinder.insertEdge(ord, bwi, 621);
	auto e11 = pathFinder.insertEdge(ord, bos, 867);
	auto e12 = pathFinder.insertEdge(ord, pvd, 849);
	auto e13 = pathFinder.insertEdge(ord, jfk, 740);
	auto e14 = pathFinder.insertEdge(bwi, mia, 946);
	auto e15 = pathFinder.insertEdge(bwi, jfk, 184);
	auto e16 = pathFinder.insertEdge(jfk, pvd, 144);
	auto e17 = pathFinder.insertEdge(jfk, bos, 187);
	auto e18 = pathFinder.insertEdge(mia, bos, 1258);
	auto e19 = pathFinder.insertEdge(jfk, mia, 1090);
    
    REQUIRE(pathFinder.findShortestPath(bwi, sfo) == vector<pair<string,int>> {
        {"BWI", 0},
        {"ORD", 621},
        {"SFO", 2467},
    });

    REQUIRE(pathFinder.findShortestPath(bwi, lax) == vector<pair<string,int>> {
        {"BWI", 0},
        {"ORD", 621},
        {"DFW", 1423},
        {"LAX", 2658},
    });

    REQUIRE(pathFinder.findShortestPath(bwi, bos) == vector<pair<string,int>> {
        {"BWI", 0},
        {"JFK", 184},
        {"BOS", 371},
    });
}
