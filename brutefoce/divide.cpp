#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>


using namespace std;

// Function to solve the 0/1 Knapsack Problem using Divide and Conquer
int knapsackDivideAndConquer(const vector<int>& weights, const vector<int>& values, int capacity, int n) {
    // Base case: no items left or no capacity left
    if (n == 0 || capacity == 0)
        return 0;
    cout << "HIT";
    // If the weight of the nth item is more than the remaining capacity, exclude it
    if (weights[n - 1] > capacity)
        return knapsackDivideAndConquer(weights, values, capacity, n - 1);

    // Otherwise, calculate the maximum value by either including or excluding the nth item
    int includeItem = values[n - 1] + knapsackDivideAndConquer(weights, values, capacity - weights[n - 1], n - 1);
    int excludeItem = knapsackDivideAndConquer(weights, values, capacity, n - 1);

    // Return the maximum of the two
    return max(includeItem, excludeItem);
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

    // Call the knapsack function
    int maxValue = knapsackDivideAndConquer(weights, values, capacity, n);

    cout << "Maximum value: " << maxValue << endl;

    return 0;
}
