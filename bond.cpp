#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

// Structure to represent a node in the solution space
struct Node {
    int level;
    int profit;
    int weight;
    double bound;
};

// Comparison function for sorting nodes based on their bound
struct CompareBound {
    bool operator()(Node const& a, Node const& b) {
        return a.bound < b.bound;
    }
};

// Function to calculate the bound on the maximum profit
double calculateBound(const vector<int>& weights, const vector<int>& values, int capacity, int n, Node u) {
    if (u.weight >= capacity) return 0;

    double bound = u.profit;
    int totalWeight = u.weight;
    int i = u.level + 1;

    while (i < n && totalWeight + weights[i] <= capacity) {
        totalWeight += weights[i];
        bound += values[i];
        i++;
    }

    if (i < n) {
        bound += (double)values[i] / weights[i] * (capacity - totalWeight);
    }

    return bound;
}

// Function to solve the 0/1 Knapsack Problem using Branch and Bound
int knapsackBranchAndBound(const vector<int>& weights, const vector<int>& values, int capacity) {
    int n = weights.size();

    vector<pair<double, int>> ratioIndex(n);
    for (int i = 0; i < n; i++) {
        ratioIndex[i] = { (double)values[i] / weights[i], i };
    }

    vector<int> sortedWeights(n), sortedValues(n);
    for (int i = 0; i < n; i++) {
        sortedWeights[i] = weights[ratioIndex[i].second];
        sortedValues[i] = values[ratioIndex[i].second];
    }

    priority_queue<Node, vector<Node>, CompareBound> pq;

    Node root = { -1, 0, 0, calculateBound(sortedWeights, sortedValues, capacity, n, { -1, 0, 0, 0 }) };
    pq.push(root);

    int maxProfit = 0;

    while (!pq.empty()) {
        Node u = pq.top();
        pq.pop();

        if (u.bound <= maxProfit) continue;

        Node v;
        v.level = u.level + 1;

        if (v.level < n) {
            v.weight = u.weight + sortedWeights[v.level];
            v.profit = u.profit + sortedValues[v.level];
            v.bound = calculateBound(sortedWeights, sortedValues, capacity, n, v);

            if (v.weight <= capacity && v.profit > maxProfit)
                maxProfit = v.profit;

            if (v.bound > maxProfit)
                pq.push(v);

            v.weight = u.weight;
            v.profit = u.profit;
            v.bound = calculateBound(sortedWeights, sortedValues, capacity, n, v);

            if (v.bound > maxProfit)
                pq.push(v);
        }
    }

    return maxProfit;
}

int main() {
    ifstream inputFile("knapsack50.txt");

    if (!inputFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int n, capacity;
    inputFile >> n;

    vector<int> weights(n), values(n);
    for (int i = 0; i < n; i++) {
        inputFile >> weights[i] >> values[i];
    }

    inputFile >> capacity;

    int maxValue = knapsackBranchAndBound(weights, values, capacity);

    cout << "Maximum value in Knapsack = " << maxValue << endl;

    inputFile.close();
    return 0;
}
