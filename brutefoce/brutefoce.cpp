#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to calculate the maximum value by trying all subsets
int knapsackBruteForce(const vector<int>& weights, const vector<int>& values, int capacity, int n) {
    // Base case: no items left or no capacity left
    if (n == 0 || capacity == 0)
        return 0;

    // If the weight of the nth item is more than the remaining capacity, skip this item
    if (weights[n - 1] > capacity)
        return knapsackBruteForce(weights, values, capacity, n - 1);

    // Otherwise, calculate the maximum of two cases:
    // 1. nth item is included
    // 2. nth item is not included
    int includeItem = values[n - 1] + knapsackBruteForce(weights, values, capacity - weights[n - 1], n - 1);
    int excludeItem = knapsackBruteForce(weights, values, capacity, n - 1);

    return max(includeItem, excludeItem);
}

int main() {
    // Example input: weights, values, and capacity
    vector<int> weights = {1, 2, 3, 4}; // Weights of items
    vector<int> values = {10, 20, 30, 40}; // Values of items
    int capacity = 5; // Capacity of the knapsack

    int n = weights.size();
    
    // Call the brute-force knapsack function
    int maxValue = knapsackBruteForce(weights, values, capacity, n);

    cout << "Maximum value: " << maxValue << endl;

    return 0;
}
