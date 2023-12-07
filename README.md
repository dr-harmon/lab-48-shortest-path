# Lab 48: Dijkstra's algorithm

In this lab we will implement a single-source shortest-paths algorithm using Dijkstra's method.

A graph ADT has been provided for you, as well as a `ShortestPathFinder` subclass that extends the graph ADT. It adds a method, `findShortestPath`, to find the shortest path between two vertices. It returns a vector of pairs such that the first item in each pair is a vertex along the shortest path, and the second item is the distance to that vertex. For example:

    ShortestPathFinder<string,int> pathFinder;
    auto& a = pathFinder.insertVertex("A");
    auto& b = pathFinder.insertVertex("B");
    auto& c = pathFinder.insertVertex("C");
    auto e1 = pathFinder.insertEdge(a, b, 1);
    auto e2 = pathFinder.insertEdge(a, c, 2);

    pathFinder.findShortestPath(a, b);  // {{"A", 0}, {"B", 1}}

Your task is to implement `findShortestPath`.

## Dijkstra's Method

To guide your implementation, use the pseudocode in Example 13.24 from the textbook, as well as the pseudocode variations presented in the textbook's slides. For this first step, you do not need to keep track of the parent vertices.

One of the difficulties in implementing Dijkstra in C++ is the need to locate and update an arbitrary item in the priority queue in `O(log n)` time. This operation is unfortunately not provided in the `std::priority_queue` implementation from the STL, as it is typically not needed and adds significant overhead and complexity. There are several ways to handle this problem:

* Forgo the priority queue altogether and iterate over all edges and all vertices. This increases the time complexity to `O(n^2)`.
* Implement your own priority queue that supports an efficient `replaceKey` (update) operation and use it instead of `std::priority_queue`.
* Instead of locating the vertex's entry in the queue and updating its key, simply add another entry with the updated distance into the queue. This will result in more than one entry in the queue for the same vertex, but the algorithm will still work because any entries with a higher shortest-path value will be ignored. Also, the time complexity will increase, but not asymptotically, because the number of extra vertices added is bounded by the number of edges.

For this lab, the latter option is recommended. That is, after relaxing a vertex, simply insert it back into the queue. Because the orginal (stale) entries are not updated and remain in the queue, you will need an additional check when popping vertices off the queue to skip any whose shortest path is infinity. Otherwise, integer overflow will occur in the relaxation check.

Implement your method so that it returns a vector with just one entry with the shortest path to the target vertex. For example:

    ShortestPathFinder<string,int> pathFinder;
    auto& a = pathFinder.insertVertex("A");
    auto& b = pathFinder.insertVertex("B");
    auto& c = pathFinder.insertVertex("C");
    auto e1 = pathFinder.insertEdge(a, b, 1);
    auto e2 = pathFinder.insertEdge(a, c, 2);

    pathFinder.findShortestPath(a, b);

This should produce the vector `{{"B", 1}}`.

At this point, when you run the unit tests, the "Shortest path (target only)" test should succeed but the others will fail.

## Finding the Shortest Path

Next, improve the `findShortestPath` implementation so that in addition to finding the distance to the shortest path, it also finds the vertices along that path. For example, if finding the distance from BWI to SFO as in Figure 13.14, the function should produce:

    {"BWI", 0}, {"ORD", 621}, {"SFO", 2467}

To find the path:

1. Update the `parent` pointer in `VertexData` on each relaxation, as descried in the slides.
2. Before returning from `findShortestPath`, create a `Vertex* next` pointer and assign it to `&target`.
3. Search all vertices for the vertex that equals the `next` pointer, then add that vertex to the return vector.
4. Set the `next` pointer equal to that vertex's parent and repeat the previous step, stopping when the parent is null.
5. Reverse the elements of the return vector.

Ideally, the parent pointers would be kept in a hash table to make the path finding `O(n)` in time complexity, but for this lab, an `O(n^2)` time is acceptable.

Make sure all unit tests pass before continuing.

## Negative Edges

Devise a simple test case in `main` that contains at least one cycle with a negative edge. What happens when you run `findShortestPath`?

## Hints

* The graph ADT requires that references be used whenever possible. e.g., Instead of `Vertex t = Q.top();` to `Vertex& t = Q.top();`.
* Use `std::numeric_limits<T>::max()` to represent an "infinite" edge distance. (Note: Do not set this value to a variable named `INFINITY`, as that symbol is already defined.)
* Assigning a raw pointer to the address of a reference is generally a _bad idea_ but it is necessary in this lab due to limitations of the graph ADT and restrictions on forward declations. For example, is is acceptable to write `Vertex& v = *pv; Q.push(&v);` (but only for the purposes of this lab!).
* You should store raw pointers in the priority queue, and you will need to define a comparison operator for them, such as a lambda. For example:

```
    auto cmp = [](Vertex* left, Vertex* right) { ... };
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(cmp)> Q(cmp);
```
