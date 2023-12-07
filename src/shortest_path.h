#pragma once

#include "graph.h"

#include <limits>
#include <queue>
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

    const EdgeDistance INFINITE_DISTANCE = std::numeric_limits<EdgeDistance>::max();

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
        VertexList verts = graph.vertices();
        for (VertexItor pv = verts.begin(); pv != verts.end(); ++pv) {
            (**pv).shortestDistance = INFINITE_DISTANCE;
        }

        (*source).shortestDistance = 0;

        auto cmp = [](Vertex* left, Vertex* right) { return (**left).shortestDistance > (**right).shortestDistance; };
        std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(cmp)> Q(cmp);
        verts = graph.vertices();
        for (VertexItor pv = verts.begin(); pv != verts.end(); ++pv) {
            Vertex& v = *pv;
            Q.push(&v);
        }

        while (!Q.empty()) {
            Vertex* u = Q.top();
            Q.pop();
    
            if ((**u).shortestDistance == INFINITE_DISTANCE) {
                continue;
            }

            EdgeList edges = u->incidentEdges();
            for (EdgeItor pe = edges.begin(); pe != edges.end(); ++pe) {
                Edge edge = (*pe);
                Vertex& z = edge.opposite(*u);
                EdgeDistance r = (**u).shortestDistance + (*edge);
                if (r < (*z).shortestDistance) {
                    (*z).shortestDistance = r;
                    (*z).parent = u;
                    Q.push(&z);
                }
            }
        }

        Vertex* next = &target;
        std::vector<std::pair<VertexElement,EdgeDistance>> vertices;
        while (next != nullptr) {
            verts = graph.vertices();
            for (VertexItor pv = verts.begin(); pv != verts.end(); ++pv) {
                Vertex& v = *pv;
                if (&v == next) {
                    vertices.push_back(std::make_pair((*v).element, (*v).shortestDistance));
                    next = (*v).parent;
                }
            }
        }

        std::reverse(vertices.begin(), vertices.end());

        return vertices;
    }
};
