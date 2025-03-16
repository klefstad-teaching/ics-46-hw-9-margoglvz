#include "dijkstras.h"

int main() {
    // Create graph with 4 nodes
    Graph G;
    G.numVertices = 4;
    G.resize(4);
    
    // Adding edges
    G[0].push_back(Edge(0, 1, 3));
    G[0].push_back(Edge(0, 2, 1));
    G[1].push_back(Edge(1, 3, 2));
    G[1].push_back(Edge(1, 2, 4));
    G[2].push_back(Edge(2, 3, 6));

    // Running Dijkstra's Algorithm 
    vector<int> previous(G.numVertices, -1);
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    // Testing extract_shortest_path()
    vector<int> path = extract_shortest_path(distances, previous, 3);

    // Output the path
    cout << "Extracted shortest path: ";
    for (int node : path) {
        cout << node << " ";
    }
    cout << "\nExpected: 0 1 3\n";
}
