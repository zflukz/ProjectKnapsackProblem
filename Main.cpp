#include "Greedy/TransformAndConquer.cpp"
#include "GA/Ga.cpp"
#include "DP/Dp.cpp"
#include "brutefoce/brutefoce.cpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono> // For runtime measurement

using namespace std;
using namespace chrono;

int main() {
    string filename[] ={"knapsack25.txt","knapsack50.txt","knapsack100.txt"};
    ofstream outputFile("results_3.csv");
    if (!outputFile) {
        cerr << "Error creating CSV file!" << endl;
        return 1;
    }
    for (int j = 0; j < 3; j++)
    {
    
    ifstream inputFile(filename[j]);

    if (!inputFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int n, W;
    inputFile >> n;

    vector<int> weights(n), values(n);
    vector<Item> items(n);

    for (int i = 0; i < n; i++) {
        inputFile >> weights[i] >> values[i];
        items[i] = {values[i], weights[i]};
    }
    inputFile >> W;
    
    outputFile  << filename[j]   << endl;
    outputFile << "Algorithm,1 : Result , Runtime(µs) ,2 : Result , Runtime(µs),3 : Result , Runtime(µs),4 : Result , Runtime(µs),5 : Result , Runtime(µs)" << endl;
    // Measure runtime for Genetic Algorithm
    outputFile << "Genetic Algorithm,";
    for (int i = 0; i < 5; i++)
    { 
    #pragma region GENETIC
    auto startGA = high_resolution_clock::now();
    Population population(15, W, items);
    double mutationRate = 0.01;
    int elitismCount = 1, numBreakpoints = 3, generations = 100;
    for (int i = 0; i < generations; i++) {
        population.evolve(mutationRate, elitismCount, numBreakpoints);
    }
    auto endGA = high_resolution_clock::now();
    auto durationGA = duration_cast<microseconds>(endGA - startGA);

    cout << "Using Genetic Algorithm (Best Individual): ";
    population.displayPopulation(0);
    int gaResult = population.displayFitness(0);
    outputFile  << gaResult << "," << durationGA.count() <<",";
    cout << "Runtime: " << durationGA.count() << " µs" << endl;
    #pragma endregion GENETIC
    }

    outputFile  << endl;
    outputFile << "Greedy Algorithm,";

    // Measure runtime for Greedy Algorithm
    for (int i = 0; i < 5; i++)
    {
    #pragma region GREEDY
    auto startGreedy = high_resolution_clock::now();
    int greedyResult = knapsackTransformAndConquer(weights, values, W);
    auto endGreedy = high_resolution_clock::now();
    auto durationGreedy = duration_cast<microseconds>(endGreedy - startGreedy);

    cout << "Using Greedy Algorithm: " << greedyResult << endl;
    cout << "Runtime: " << durationGreedy.count() << " µs" << endl;
    outputFile << greedyResult << "," << durationGreedy.count() << ",";
    #pragma endregion 
    }
    outputFile  << endl;
    outputFile << "Dynamic Programming,";
    
    // Measure runtime for Dynamic Programming
    for (int i = 0; i < 5; i++)
    {
    #pragma region Dynamic
    auto startDP = high_resolution_clock::now();
    int dpResult = knapSack(W, weights, values);
    auto endDP = high_resolution_clock::now();
    auto durationDP = duration_cast<microseconds>(endDP - startDP);

    cout << "Using Dynamic Programming: " << dpResult << endl;
    cout << "Runtime: " << durationDP.count() << " µs" << endl;
    outputFile  << dpResult << "," << durationDP.count() << ",";
    #pragma endregion
    }
    outputFile  << endl;
    
    // outputFile << "Dynamic Programming,";
    if(j==0)
    {
    // // // Measure runtime for Brute Force
    for (int i = 0; i < 5; i++)
    {
    #pragma region BruteForce
    auto startBruteForce = high_resolution_clock::now();
    int bruteForceResult = knapsackBruteForce(weights, values, W, n);
    auto endBruteForce = high_resolution_clock::now();
    auto durationBruteForce = duration_cast<microseconds>(endBruteForce - startBruteForce);

    cout << "Using Brute Force Algorithm: " << bruteForceResult << endl;
    cout << "Runtime: " << durationBruteForce.count() << " µs" << endl;
    outputFile <<  bruteForceResult << " " << durationBruteForce.count() << ",";
    #pragma endregion
    }
    outputFile  << endl;
    outputFile  << endl;
    }
    }
    return 0;
}
