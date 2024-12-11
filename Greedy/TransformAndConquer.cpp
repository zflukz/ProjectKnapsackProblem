#include "TransformAndConquer.h"
#include <algorithm>

using namespace std;

// Structure to represent an item
struct ItemG {
    int weight;
    int value;
    double ratio; // Value-to-weight ratio
};

// Comparison function for sorting items by value-to-weight ratio
bool compare(ItemG a, ItemG b) {
    return a.ratio > b.ratio; // Sort in descending order
}

// Function to solve the Knapsack Problem using a greedy approach
int knapsackTransformAndConquer(const vector<int>& weights, const vector<int>& values, int capacity) {
    int n = weights.size();

    vector<ItemG> items(n);
    for (int i = 0; i < n; i++) {
        items[i] = {weights[i], values[i], (double)values[i] / weights[i]};
    }

    sort(items.begin(), items.end(), compare);

    int totalValue = 0;
    int remainingCapacity = capacity;

    for (int i = 0; i < n && remainingCapacity > 0; i++) {
        if (items[i].weight <= remainingCapacity) {
            totalValue += items[i].value;
            remainingCapacity -= items[i].weight;
        }
    }

    return totalValue;
}
