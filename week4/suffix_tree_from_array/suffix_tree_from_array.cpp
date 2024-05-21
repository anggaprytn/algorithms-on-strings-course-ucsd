#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>

using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::vector;

// Data structure to store edges of a suffix tree.
struct Edge {
    // The ending node of this edge.
    int node;
    // Starting position of the substring of the text 
    // corresponding to the label of this edge.
    int start;
    // Position right after the end of the substring of the text 
    // corresponding to the label of this edge.
    int end;

    Edge(int node_, int start_, int end_) : node(node_), start(start_), end(end_) {}
    Edge(const Edge& e) : node(e.node), start(e.start), end(e.end) {}
};

// Build suffix tree of the string text given its suffix array suffix_array
// and LCP array lcp_array. Return the tree as a mapping from a node ID
// to the vector of all outgoing edges of the corresponding node. The edges in the
// vector must be sorted in the ascending order by the first character of the edge label.
map<int, vector<Edge>> SuffixTreeFromSuffixArray(
    const vector<int>& suffix_array,
    const vector<int>& lcp_array,
    const string& text) {
    
    map<int, vector<Edge>> tree;
    int n = suffix_array.size();
    int node_count = 0;
    vector<int> node_depths;
    vector<int> parent_nodes;
    vector<int> edge_starts;

    node_depths.push_back(0);
    parent_nodes.push_back(-1);
    edge_starts.push_back(-1);

    int lcp_prev = 0;
    for (int i = 0; i < n; ++i) {
        int suffix = suffix_array[i];
        while (node_depths.back() > lcp_prev) {
            node_depths.pop_back();
            parent_nodes.pop_back();
            edge_starts.pop_back();
        }

        if (node_depths.back() == lcp_prev) {
            int parent = parent_nodes.back();
            int edge_start = edge_starts.back();
            if (edge_start != -1) {
                tree[parent].push_back(Edge(node_count, edge_start, suffix_array[i - 1] + lcp_prev));
            }
        } else {
            int parent = parent_nodes.back();
            int edge_start = edge_starts.back();
            int mid_node = ++node_count;
            int prev_node = node_count - 1;
            if (edge_start != -1) {
                tree[parent].push_back(Edge(mid_node, edge_start, suffix_array[i - 1] + lcp_prev));
            }
            node_depths.push_back(lcp_prev);
            parent_nodes.push_back(mid_node);
            edge_starts.push_back(edge_start);
        }

        if (i < n - 1) {
            int parent = parent_nodes.back();
            tree[parent].push_back(Edge(++node_count, suffix + lcp_prev, n));
            node_depths.push_back(n - suffix);
            parent_nodes.push_back(node_count);
            edge_starts.push_back(suffix + lcp_prev);
            lcp_prev = lcp_array[i];
        }
    }

    return tree;
}

int main() {
    char buffer[200001];
    scanf("%s", buffer);
    string text = buffer;
    vector<int> suffix_array(text.length());
    for (int i = 0; i < text.length(); ++i) {
        scanf("%d", &suffix_array[i]);
    }
    vector<int> lcp_array(text.length() - 1);
    for (int i = 0; i + 1 < text.length(); ++i) {
        scanf("%d", &lcp_array[i]);
    }
    // Build the suffix tree and get a mapping from 
    // suffix tree node ID to the list of outgoing Edges.
    map<int, vector<Edge>> tree = SuffixTreeFromSuffixArray(suffix_array, lcp_array, text);
    printf("%s\n", buffer);
    // Output the edges of the suffix tree in the required order.
    // Note that we use here the contract that the root of the tree
    // will have node ID = 0 and that each vector of outgoing edges
    // will be sorted by the first character of the corresponding edge label.
    //
    // The following code avoids recursion to avoid stack overflow issues.
    // It uses a stack to convert recursive function to a while loop.
    // The stack stores pairs (node, edge_index). 
    // This code is an equivalent of 
    //
    //    OutputEdges(tree, 0);
    //
    // for the following _recursive_ function OutputEdges:
    //
    // void OutputEdges(map<int, vector<Edge> > tree, int node_id) {
    //   const vector<Edge>& edges = tree[node_id];
    //   for (int edge_index = 0; edge_index < edges.size(); ++edge_index) {
    //     printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
    //     OutputEdges(tree, edges[edge_index].node);
    //   }
    // }
    //
    vector<pair<int, int>> stack(1, make_pair(0, 0));    
    while (!stack.empty()) {
        pair<int, int> p = stack.back();
        stack.pop_back();
        int node = p.first;
        int edge_index = p.second;
        if (!tree.count(node)) {
            continue;
        }
        const vector<Edge>& edges = tree[node];
        if (edge_index + 1 < edges.size()) {
            stack.push_back(make_pair(node, edge_index + 1));
        }
        printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
        stack.push_back(make_pair(edges[edge_index].node, 0));
    }
    return 0;
}
