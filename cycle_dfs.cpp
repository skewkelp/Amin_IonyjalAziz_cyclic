#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

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

// Print cycle
void printCycle(vector<int>& path, int start) {
    cout << "Cycle detected: ";
    for (int i = start; i < path.size(); ++i)
        cout << nodeLabels[path[i]] << " ";
    cout << nodeLabels[path[start]] << endl;
}

// DFS with lexicographical neighbor selection
bool dfs(int u, vector<vector<int>>& adj, vector<bool>& visited, vector<bool>& recStack, vector<int>& path) {
    visited[u] = true;
    recStack[u] = true;
    path.push_back(u);

    // Collect neighbors in lexicographical order
    vector<int> neighbors;
    for (int v = 0; v < nodeLabels.size(); ++v) {
        if (adj[u][v]) {
            neighbors.push_back(v);
        }
    }

    sort(neighbors.begin(), neighbors.end(), [&](int a, int b) {
        return nodeLabels[a] < nodeLabels[b];
    });

    for (int v : neighbors) {
        if (!visited[v]) {
            if (dfs(v, adj, visited, recStack, path))
                return true;
        } else if (recStack[v]) {
            auto it = find(path.begin(), path.end(), v);
            if (it != path.end()) {
                printCycle(path, it - path.begin());
                return true;
            }
        }
    }

    recStack[u] = false;
    path.pop_back();
    return false;
}

bool isCyclicDFS(vector<vector<int>>& adj) {
    int numNodes = nodeLabels.size();
    vector<bool> visited(numNodes, false), recStack(numNodes, false);
    vector<int> path;

    // Iterate through nodes in the user-provided order to start DFS
    for (int i = 0; i < numNodes; ++i) {
        if (!visited[i]) {
            if (dfs(i, adj, visited, recStack, path))
                return true;
        }
    }

    cout << "No cycle found in the graph.\n";
    return false;
}

// Default graph loader
vector<vector<int>> loadDefaultGraph() {
    int numNodes = nodeLabels.size();
    vector<vector<int>> adj(numNodes, vector<int>(numNodes, 0));

    // Edge mapping from your problem description:
    // D->A, E->A, A->C, C->F, F->B, C->B, C->G, C->E

    // Add edges only if the nodes exist in the current nodeLabels
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
    // Print header
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

    // Print rows
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
        // Default node order
        nodeLabels = {'D', 'E', 'A', 'C', 'F', 'B', 'G'};
    } else if (choice == 2) { // Choice 2 requires custom labels
        cout << "Enter node labels separated by spaces (e.g., A B C D E F G): ";
        string line;
        cin.ignore(); // consume the newline character from previous input
        getline(cin, line);
        stringstream ss(line);
        char label;
        while(ss >> label) {
            nodeLabels.push_back(label);
        }
    }
}

int main() {
    cout << "Graph Cycle Detection using DFS (Lexicographical Depth Rule)\n\n";

    // Display default graph information
    cout << "--- Default Graph Details ---" << endl;
    nodeLabels = {'D', 'E', 'A', 'C', 'F', 'B', 'G'}; // Set to default to show
    cout << "Default Node order: ";
    for (size_t i = 0; i < nodeLabels.size(); ++i) {
        cout << nodeLabels[i] << (i == nodeLabels.size() - 1 ? "" : ", ");
    }
    cout << endl;
    vector<vector<int>> defaultAdj = loadDefaultGraph();
    printMatrix(defaultAdj);
    nodeLabels.clear(); // Reset for user choice

    int choice;
    cout << "--- Menu ---\n";
    cout << "1. Use default graph (shown above)\n";
    cout << "2. Use default edge structure with a custom node order\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    setupNodeLabels(choice);

    vector<vector<int>> adj;

    if (choice == 1 || choice == 2) {
        adj = loadDefaultGraph(); // Default or custom labels, but default edge structure
        if (choice == 2) {
             cout << "\nGenerated Adjacency Matrix for your custom node order:" << endl;
             printMatrix(adj);
        }
    } else {
        cout << "Invalid choice. Using default graph as a fallback." << endl;
        nodeLabels = {'D', 'E', 'A', 'C', 'F', 'B', 'G'};
        adj = loadDefaultGraph();
    }

    isCyclicDFS(adj);
    return 0;
}
