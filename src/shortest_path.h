#pragma once

#include "graph.h"

#include <vector>

template <typename VertexElement, typename EdgeDistance>
class ShortestPathFinder {
public:

    struct VertexData;

private:

    typedef Graph<VertexData,EdgeDistance> G;
    typedef typename G::VertexList VertexList;
    typedef typename G::VertexItor VertexItor;
    typedef typename G::EdgeList EdgeList;
    typedef typename G::EdgeItor EdgeItor;
    
    G graph;

public:

    typedef typename G::Vertex Vertex;
    typedef typename G::Edge Edge;

    struct VertexData {
        EdgeDistance shortestDistance;
        VertexElement element;
        Vertex* parent;
    };

    Vertex& insertVertex(const VertexElement& element) {
        return graph.insertVertex({false, element, nullptr});
    }

    Edge& insertEdge(const Vertex& v, const Vertex& u, const EdgeDistance& distance) {
        return graph.insertEdge(v, u, distance);
    }

    std::vector<std::pair<VertexElement,EdgeDistance>> findShortestPath(Vertex& source, Vertex& target) {

        // TODO

        std::vector<std::pair<VertexElement,EdgeDistance>> vertices;

        // TODO

        return vertices;
    }
};
