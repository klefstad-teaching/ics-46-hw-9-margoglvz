#include "dijkstras.h"

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distance(n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int curr_dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (curr_dist > distance[u]) continue;  // Skip outdated distance

        for (const Edge& edge : G[u]) {
            int v = edge.dst, weight = edge.weight;
            if (distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                previous[v] = u;
                pq.push({distance[v], v});
            }
        }
    }
    return distance;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;

    // if destination is unreachable then return empty path
    if (distances[destination] == INF) {
        return path;
    }

    // dijkstra is all about backtracking, this allows backtracking from destination to source using the 'previous' array
    for (int at = destination; at != -1; at = previous[at]) {
        path.push_back(at);
    }

    // reverse to get the path from source to destination
    reverse(path.begin(), path.end());
    return path;
}


void print_path(const vector<int>& path, int total) {
    if (path.empty()) {
        cout << "No path found.\n";
        return;
    }
    cout << "Shortest path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i] << (i + 1 < path.size() ? " -> " : "");
    }
    cout << "\nTotal cost: " << total << "\n";
}
