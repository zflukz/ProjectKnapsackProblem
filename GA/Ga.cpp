#include "GA.h"
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>

using namespace std;

// Private member functions
void Population::calculateFitness() {
    fitness.clear();
    for (const auto& individual : individuals) {
        int totalValue = 0, totalWeight = 0;
        for (size_t i = 0; i < individual.size(); i++) {
            if (individual[i]) {
                totalValue += itemList[i].value;
                totalWeight += itemList[i].weight;
            }
        }
        fitness.push_back((totalWeight <= maxWeight) ? totalValue : 0);
    }
}

int Population::spinWheelSelection() {
    int totalFitness = accumulate(fitness.begin(), fitness.end(), 0);
    vector<double> probabilities(fitness.size());
    for (size_t i = 0; i < fitness.size(); i++) {
        probabilities[i] = (double)fitness[i] / totalFitness;
    }

    vector<double> cumulativeProbabilities(fitness.size());
    cumulativeProbabilities[0] = probabilities[0];
    for (size_t i = 1; i < probabilities.size(); i++) {
        cumulativeProbabilities[i] = cumulativeProbabilities[i - 1] + probabilities[i];
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);
    double spin = dist(gen);

    for (size_t i = 0; i < cumulativeProbabilities.size(); i++) {
        if (spin <= cumulativeProbabilities[i]) {
            return i;
        }
    }
    return fitness.size() - 1;
}

int Population::tournamentSelection(const vector<vector<bool>>& individuals, const vector<int>& fitness, int tournamentSize) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, individuals.size() - 1);

    vector<int> candidates;
    for (int i = 0; i < tournamentSize; i++) {
        candidates.push_back(dist(gen));
    }

    int bestIndex = candidates[0];
    for (int i = 1; i < tournamentSize; i++) {
        if (fitness[candidates[i]] > fitness[bestIndex]) {
            bestIndex = candidates[i];
        }
    }
    return bestIndex;
}

void Population::mutate(vector<bool>& individual, double mutationRate) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    for (size_t i = 0; i < individual.size(); i++) {
        if (dist(gen) < mutationRate) {
            individual[i] = !individual[i];
        }
    }
}

vector<bool> Population::multiPointCrossover(const vector<bool>& parent1, const vector<bool>& parent2, int numBreakpoints) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, parent1.size() - 1);

    vector<int> breakpoints;
    while (breakpoints.size() < numBreakpoints) {
        int point = dist(gen);
        if (find(breakpoints.begin(), breakpoints.end(), point) == breakpoints.end()) {
            breakpoints.push_back(point);
        }
    }
    sort(breakpoints.begin(), breakpoints.end());

    vector<bool> child(parent1.size());
    bool toggle = true;

    int prevPoint = 0;
    for (int point : breakpoints) {
        for (int i = prevPoint; i < point; i++) {
            child[i] = toggle ? parent1[i] : parent2[i];
        }
        toggle = !toggle;
        prevPoint = point;
    }

    for (int i = prevPoint; i < parent1.size(); i++) {
        child[i] = toggle ? parent1[i] : parent2[i];
    }

    return child;
}

// Public member functions
Population::Population(int n, int maxWeight, const vector<Item>& itemList) {
    this->n = n;
    this->maxWeight = maxWeight;
    this->itemList = itemList;
    this->sizeList = itemList.size();

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 1);

    for (int i = 0; i < n; i++) {
        vector<bool> individual(sizeList);
        for (int j = 0; j < sizeList; j++) {
            individual[j] = dist(gen);
        }
        individuals.push_back(individual);
    }

    calculateFitness();
}

void Population::evolve(double mutationRate, int elitismCount, int numBreakpoints) {
    vector<size_t> indices(individuals.size());
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](size_t i, size_t j) {
        return fitness[i] > fitness[j];
    });

    vector<vector<bool>> newPopulation;
    for (int i = 0; i < elitismCount; i++) {
        newPopulation.push_back(individuals[indices[i]]);
    }

    while (newPopulation.size() < n) {
        int parent1Index = spinWheelSelection();
        int parent2Index = tournamentSelection(individuals, fitness, numBreakpoints);

        vector<bool> child = multiPointCrossover(individuals[parent1Index], individuals[parent2Index], numBreakpoints);
        mutate(child, mutationRate);

        newPopulation.push_back(child);
    }

    individuals = newPopulation;
    calculateFitness();
}

void Population::displayPopulation(int i) const {
    for (bool gene : individuals[i]) {
        cout << gene;
    }
    cout << " -> Fitness: " << fitness[i] << endl;
}
