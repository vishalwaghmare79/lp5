#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adjList;

public:
    Graph(int V) {
        this->V = V;
        adjList.resize(V);
    }

    void addEdge(int src, int dest) {
        adjList[src].push_back(dest);
        adjList[dest].push_back(src); // For undirected graph
    }

    vector<int> getNeighbors(int vertex) {
        return adjList[vertex];
    }
};

void parallelBFS(Graph& graph, int source, vector<bool>& visited) {
    queue<int> q;
    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        cout << "Visited: " << current << endl;

        vector<int> neighbors = graph.getNeighbors(current);
        #pragma omp parallel for
        for (int i = 0; i < neighbors.size(); ++i) {
            int neighbor = neighbors[i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}



int main() {
    int V, E;
    cout << "Enter the number of vertices: ";
    cin >> V;
    Graph graph(V);
    cout << "Enter the number of edges: ";
    cin >> E;
    cout << "Enter the edges (src dest):" << endl;
    for (int i = 0; i < E; ++i) {
        int src, dest;
        cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    vector<bool> visited(V, false);

    cout << "Parallel BFS:" << endl;
    #pragma omp parallel num_threads(2)
    {
        #pragma omp single nowait
        parallelBFS(graph, 0, visited);
    }

    // Reset visited array for DFS
    

    return 0;
}
