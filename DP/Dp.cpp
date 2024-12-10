#include "DP.h"
#include <algorithm>


using namespace std;

// Recursive function to solve Knapsack using Memoization
int knapSackRec(int W, const vector<int>& wt, const vector<int>& val, int index, vector<vector<int>>& dp) {
    // Base condition
    if (index < 0)
        return 0;

    // If already computed, return the stored value
    if (dp[index][W] != -1)
        return dp[index][W];

    // If weight of the item exceeds capacity, exclude the item
    if (wt[index] > W) {
        dp[index][W] = knapSackRec(W, wt, val, index - 1, dp);
        return dp[index][W];
    } else {
        // Include or exclude the item and take the maximum
        dp[index][W] = max(
            val[index] + knapSackRec(W - wt[index], wt, val, index - 1, dp),
            knapSackRec(W, wt, val, index - 1, dp)
        );

        return dp[index][W];
    }
}

// Function to initialize DP table and start the recursive function
int knapSack(int W, const vector<int>& wt, const vector<int>& val) {
    int n = wt.size();
    
    // DP table initialized to -1
    vector<vector<int>> dp(n, vector<int>(W + 1, -1));

    // Call the recursive function
    return knapSackRec(W, wt, val, n - 1, dp);
}