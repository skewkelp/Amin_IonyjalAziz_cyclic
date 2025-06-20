#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <queue>

using namespace std;

vector<char> nodeLabels;

// Function to find the index of a node label
int labelToIndex(char ch) {
    for (int i = 0; i < nodeLabels.size(); ++i) {
        if (nodeLabels[i] == ch)
            return i;
    }
    return -1;
}

// BFS-based cycle detection using Kahn's Algorithm (Topological Sort)
bool isCyclicBFS(vector<vector<int>>& adj) {
    int numNodes = nodeLabels.size();
    vector<int> in_degree(numNodes, 0);

    // Calculate in-degrees of all nodes
    for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
            if (adj[j][i] == 1) {
                in_degree[i]++;
            }
        }
    }

    // Create a queue and enqueue all nodes with an in-degree of 0
    queue<int> q;
    for (int i = 0; i < numNodes; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    int visitedCount = 0;
    vector<int> topOrder;

    // Process nodes from the queue
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topOrder.push_back(u);

        // Reduce in-degree for all neighbors
        // Note: The neighbors are determined lexicographically to maintain consistency
        vector<int> neighbors;
        for (int v_idx = 0; v_idx < numNodes; ++v_idx) {
            if (adj[u][v_idx]) {
                neighbors.push_back(v_idx);
            }
        }
        sort(neighbors.begin(), neighbors.end(), [&](int a, int b) {
            return nodeLabels[a] < nodeLabels[b];
        });

        for (int v : neighbors) {
            if (--in_degree[v] == 0) {
                q.push(v);
            }
        }
        visitedCount++;
    }

    // Check if there was a cycle
    if (visitedCount != numNodes) {
        cout << "Cycle detected in the graph." << endl;

        // The remaining nodes with in_degree > 0 are part of a cycle.
        // We can trace back from one of these nodes to find the cycle path.
        vector<int> cycle_nodes_indices;
        for (int i = 0; i < numNodes; i++) {
            if (in_degree[i] > 0) {
                cycle_nodes_indices.push_back(i);
            }
        }

        // Find a predecessor for each node within the cycle subgraph
        vector<int> parent(numNodes, -1);
        for (int v : cycle_nodes_indices) {
            for (int u : cycle_nodes_indices) {
                if (adj[u][v]) {
                    parent[v] = u;
                    break; 
                }
            }
        }

        // Trace back from an arbitrary node to find the cycle
        vector<int> cycle_path;
        int start_node_in_cycle = -1;

        if (!cycle_nodes_indices.empty()) {
            vector<bool> visited_in_trace(numNodes, false);
            int current = cycle_nodes_indices.front();

            // Move until a visited node is found, which marks the cycle's start
            while (parent[current] != -1 && !visited_in_trace[current]) {
                visited_in_trace[current] = true;
                current = parent[current];
            }
            start_node_in_cycle = current;

            // Now, build the cycle path starting from the identified node
            int path_tracer = start_node_in_cycle;
            do {
                cycle_path.push_back(path_tracer);
                path_tracer = parent[path_tracer];
            } while (path_tracer != start_node_in_cycle && path_tracer != -1);
            reverse(cycle_path.begin(), cycle_path.end());
        }

        if (!cycle_path.empty()) {
            cout << "Cycle detected: ";
            for (size_t i = 0; i < cycle_path.size(); ++i) {
                cout << nodeLabels[cycle_path[i]] << " ";
            }
            cout << nodeLabels[cycle_path[0]] << endl;
        }
        return true;
    } else {
        cout << "No cycle found in the graph. Valid topological sort: ";
        for(size_t i = 0; i < topOrder.size(); ++i) {
            cout << nodeLabels[topOrder[i]] << (i == topOrder.size() - 1 ? "" : " -> ");
        }
        cout << endl;
        return false;
    }
}

// Default graph loader
vector<vector<int>> loadDefaultGraph() {
    int numNodes = nodeLabels.size();
    vector<vector<int>> adj(numNodes, vector<int>(numNodes, 0));

    // Edge mapping from your problem description:
    // D->A, E->A, A->C, C->F, F->B, C->B, C->G, C->E
    if (labelToIndex('D') != -1 && labelToIndex('A') != -1) adj[labelToIndex('D')][labelToIndex('A')] = 1;
    if (labelToIndex('E') != -1 && labelToIndex('A') != -1) adj[labelToIndex('E')][labelToIndex('A')] = 1;
    if (labelToIndex('A') != -1 && labelToIndex('C') != -1) adj[labelToIndex('A')][labelToIndex('C')] = 1;
    if (labelToIndex('C') != -1 && labelToIndex('F') != -1) adj[labelToIndex('C')][labelToIndex('F')] = 1;
    if (labelToIndex('F') != -1 && labelToIndex('B') != -1) adj[labelToIndex('F')][labelToIndex('B')] = 1;
    if (labelToIndex('C') != -1 && labelToIndex('B') != -1) adj[labelToIndex('C')][labelToIndex('B')] = 1;
    if (labelToIndex('C') != -1 && labelToIndex('G') != -1) adj[labelToIndex('C')][labelToIndex('G')] = 1;
    if (labelToIndex('C') != -1 && labelToIndex('E') != -1) adj[labelToIndex('C')][labelToIndex('E')] = 1;

    return adj;
}

void printMatrix(const vector<vector<int>>& adj) {
    cout << "Adjacency Matrix:" << endl;
    cout << "    ";
    for (char label : nodeLabels) {
        cout << label << " ";
    }
    cout << endl;
    cout << "    ";
    for (size_t i = 0; i < nodeLabels.size(); ++i) {
        cout << "--";
    }
    cout << endl;

    for (size_t i = 0; i < nodeLabels.size(); ++i) {
        cout << nodeLabels[i] << " | ";
        for (size_t j = 0; j < nodeLabels.size(); ++j) {
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void setupNodeLabels(int choice) {
    if (choice == 1) {
        nodeLabels = {'D', 'E', 'A', 'C', 'F', 'B', 'G'};
    } else if (choice == 2) {
        cout << "Enter node labels separated by spaces (e.g., A B C D E F G): ";
        string line;
        cin.ignore(); 
        getline(cin, line);
        stringstream ss(line);
        char label;
        while(ss >> label) {
            nodeLabels.push_back(label);
        }
    }
}

int main() {
    cout << "Graph Cycle Detection using BFS (Kahn's Algorithm)\n\n";

    cout << "--- Default Graph Details ---" << endl;
    nodeLabels = {'D', 'E', 'A', 'C', 'F', 'B', 'G'};
    cout << "Default Node order: ";
    for (size_t i = 0; i < nodeLabels.size(); ++i) {
        cout << nodeLabels[i] << (i == nodeLabels.size() - 1 ? "" : ", ");
    }
    cout << endl;
    vector<vector<int>> defaultAdj = loadDefaultGraph();
    printMatrix(defaultAdj);
    nodeLabels.clear();

    int choice;
    cout << "--- Menu ---\n";
    cout << "1. Use default graph (shown above)\n";
    cout << "2. Use default edge structure with a custom node order\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    setupNodeLabels(choice);

    vector<vector<int>> adj;

    if (choice == 1 || choice == 2) {
        adj = loadDefaultGraph();
        if (choice == 2) {
             cout << "\nGenerated Adjacency Matrix for your custom node order:" << endl;
             printMatrix(adj);
        }
    } else {
        cout << "Invalid choice. Using default graph as a fallback." << endl;
        nodeLabels = {'D', 'E', 'A', 'C', 'F', 'B', 'G'};
        adj = loadDefaultGraph();
    }

    isCyclicBFS(adj);
    return 0;
}
