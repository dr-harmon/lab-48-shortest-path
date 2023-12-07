#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

template <typename VertexElement, typename EdgeElement>
class Graph {
public:

    class Vertex;
    class VertexItor;
    class Edge;
    class EdgeItor;
    typedef std::unordered_map<int,std::vector<int>> AdjacencyList;
    typedef std::unordered_map<int,Vertex> VertexMap;
    typedef std::unordered_map<int,Edge> EdgeMap;

    class VertexList {
    public:
        VertexItor begin() {
            return VertexItor(vertices.begin(), vertexMap);
        }

        VertexItor end() {
            return VertexItor(vertices.end(), vertexMap);
        }

        VertexList& operator=(const VertexList& other) {
            vertices = other.vertices;
            vertexMap = other.vertexMap;
            return *this;
        }

    private:
        std::vector<int> vertices;
        VertexMap& vertexMap;
        friend Graph;

        VertexList(std::vector<int> _vertices, VertexMap& _vertexMap) : vertices(_vertices), vertexMap(_vertexMap) {}
    };

    class EdgeList {
    public:
        EdgeItor begin() {
            return EdgeItor(edges.begin(), edgeMap);
        }

        EdgeItor end() {
            return EdgeItor(edges.end(), edgeMap);
        }

    private:
        std::vector<int> edges;
        EdgeMap& edgeMap;
        friend Graph;

        EdgeList(std::vector<int>& _edges, EdgeMap& _edgeMap) : edges(_edges), edgeMap(_edgeMap) {}
    };

    class VertexItor {
    public:
        Vertex& operator*() const {
            return vertexMap.at(*it);
        }

        VertexItor& operator++() {
            ++it;
            return *this;
        }

        VertexItor operator++(int) {
            VertexItor temp = *this;
            ++it;
            return temp;
        }

        bool operator!=(const VertexItor& other) const {
            return it != other.it;
        }

    private:
        std::vector<int>::iterator it;
        VertexMap& vertexMap;
        friend VertexList;

        VertexItor(std::vector<int>::iterator _it, VertexMap& _vertexMap) : it(_it), vertexMap(_vertexMap) {}
    };

    class EdgeItor {
    public:
        Edge& operator*() const {
            return edgeMap.at(*it);
        }

        EdgeItor& operator++() {
            ++it;
            return *this;
        }

        EdgeItor operator++(int) {
            EdgeItor temp = *this;
            ++it;
            return temp;
        }

        bool operator!=(const EdgeItor& other) const {
            return it != other.it;
        }

    private:
        std::vector<int>::iterator it;
        EdgeMap& edgeMap;
        friend EdgeList;

        EdgeItor(std::vector<int>::iterator _it, EdgeMap& _edgeMap) : it(_it), edgeMap(_edgeMap)  {}
    };

    class Vertex {
    public:

        bool isAdjacentTo(const Vertex& v) const {
            for (int edgeID : g->adjacencyList.at(id)) {
                Edge& edge = g->edgeMap.at(edgeID);
                if ((id == edge.startVertex && v.id == edge.endVertex) ||
                    (id == edge.endVertex && v.id == edge.startVertex)) {
                    return true;
                }
            }
            return false;
        }

        EdgeList incidentEdges() const {
            return EdgeList(g->adjacencyList.at(id), g->edgeMap);
        }

        VertexElement& operator*() {
            return element;
        }

        bool operator ==(const Vertex& other) const {
            return id == other.id;
        }

        Vertex& operator=(const Vertex& other) {
            element = other.element;
            id = other.id;
            g = other.g;
            return *this;
        }

    private:
        VertexElement element;
        int id;
        Graph* g;
        friend Graph;

        Vertex(const VertexElement& _element, int _id, Graph* _g) : element(_element), id(_id), g(_g) {}
    };

    class Edge {
    public:
        VertexList endVertices() const {
            return VertexList({startVertex, endVertex}, g.vertexMap);
        }

        Vertex& opposite(const Vertex& v) {
            return g.vertexMap.at(v.id == startVertex ? endVertex : startVertex);
        }

        bool isIncidentOn(const Vertex& v) const {
            return v.id == startVertex || v.id == endVertex;
        }

        EdgeElement& operator*() {
            return element;
        }

    private:
        EdgeElement element;
        int id;
        int startVertex, endVertex;
        Graph& g;
        friend Graph;

        Edge(const EdgeElement& _element, int _id, int _startVertex, int _endVertex, Graph& _g) :
            element(_element), id(_id), startVertex(_startVertex), endVertex(_endVertex), g(_g) {}
    };

    Vertex& insertVertex(const VertexElement& element) {
        Vertex v(element, nextVertexID++, this);
        vertexMap.emplace(v.id, v);
        adjacencyList[v.id] = {};
        return vertexMap.at(v.id);
    }

    Edge& insertEdge(const Vertex& v, const Vertex& u, const EdgeElement& element) {
        if (vertexMap.count(v.id) == 0 || vertexMap.count(u.id) == 0) {
            throw std::runtime_error("Cannot add edge between unknown vector");
        }

        Edge e(element, nextEdgeID++, v.id, u.id, *this);
        edgeMap.emplace(e.id, e);
        adjacencyList[v.id].push_back(e.id);
        adjacencyList[u.id].push_back(e.id);
        return edgeMap.at(e.id);
    }

    void eraseVertex(const Vertex& v) {
        vertexMap.erase(v.id);
        for (int edgeID : adjacencyList.at(v.id)) {
            edgeMap.erase(edgeID);
        }
    }

    void eraseEdge(const Edge& e) {
        edgeMap.erase(e.id);
        auto startEdges = adjacencyList.at(e.startVertex);
        startEdges.erase(std::remove(startEdges.begin(), startEdges.end(), e.id), startEdges.end());
        auto endEdges = adjacencyList.at(e.endVertex);
        endEdges.erase(std::remove(endEdges.begin(), endEdges.end(), e.id), endEdges.end());
    }

    VertexList vertices() {
        std::vector<int> vertexIDs;
        std::transform(vertexMap.begin(), vertexMap.end(), std::back_inserter(vertexIDs), [](auto pair){ return pair.first;});
        return VertexList(vertexIDs, vertexMap);
    }

    EdgeList edges() {
        std::vector<int> edgeIDs;
        std::transform(edgeMap.begin(), edgeMap.end(), std::back_inserter(edgeIDs), [](auto pair){ return pair.first;});
        return EdgeList(edgeIDs, edgeMap);
    }

private:
    int nextVertexID = 1;
    int nextEdgeID = 1;
    AdjacencyList adjacencyList;
    VertexMap vertexMap;
    EdgeMap edgeMap;
};
