#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent an item
struct Item {
    int weight;
    int value;
    double ratio; // Value-to-weight ratio
};

// Comparison function for sorting items by value-to-weight ratio
bool compare(Item a, Item b) {
    return a.ratio > b.ratio; // Sort in descending order
}

// Function to solve the Knapsack Problem using a greedy approach
int knapsackTransformAndConquer(const vector<int>& weights, const vector<int>& values, int capacity) {
    int n = weights.size();

    // Create a vector of items
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i] = {weights[i], values[i], (double)values[i] / weights[i]};
    }

    // Sort items by value-to-weight ratio
    sort(items.begin(), items.end(), compare);

    int totalValue = 0;
    int remainingCapacity = capacity;

    // Pick items while there is capacity
    for (int i = 0; i < n && remainingCapacity > 0; i++) {
        if (items[i].weight <= remainingCapacity) {
            // Take the whole item
            totalValue += items[i].value;
            remainingCapacity -= items[i].weight;
        }
    }

    return totalValue;
}

int main() {
    // Example input: weights, values, and capacity
    vector<int> weights = {1, 2, 3, 4}; // Weights of items
    vector<int> values = {10, 20, 30, 40}; // Values of items
    int capacity = 5; // Capacity of the knapsack

    // Call the knapsack function
    int maxValue = knapsackTransformAndConquer(weights, values, capacity);

    cout << "Maximum approximate value: " << maxValue << endl;

    return 0;
}
