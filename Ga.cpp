#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

class Item {
public:
    int value;
    int weight;
    
};

class Population {
private:
    int n, maxWeight, sizeList;
    vector<Item> itemList;
    vector<vector<bool>> individuals; // Population of individuals
    vector<int> fitness;

    void calculateFitness() {
        fitness.clear();
        for (const auto& individual : individuals) {
            int totalValue = 0, totalWeight = 0;
            for (size_t i = 0; i < individual.size(); i++) {
                if (individual[i]) {
                    totalValue += itemList[i].value;
                    totalWeight += itemList[i].weight;
                }
            }
            // Only valid individuals contribute to fitness
            fitness.push_back((totalWeight <= maxWeight) ? totalValue : 0);
        }
    }

public:
    Population(int n, int maxWeight, const vector<Item>& itemList) {
        this->n = n;
        this->maxWeight = maxWeight;
        this->itemList = itemList;
        this->sizeList = itemList.size();

        // Initialize random population
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 1);

        for (int i = 0; i < n; i++) {
            vector<bool> individual(sizeList);
            for (int j = 0; j < sizeList; j++) {
                individual[j] = dist(gen); // Randomly set selection
            }
            individuals.push_back(individual);
        }

        calculateFitness();
    }

    
    void displayPopulation(int i) const {
        for (bool gene : individuals[i]) 
        {
                cout << gene;
        }
        
            cout << " -> Fitness: " << fitness[i] << endl;
    }
    void displayPopulation() const {
        for (size_t i = 0; i < individuals.size(); i++) {
            for (bool gene : individuals[i]) {
                cout << gene;
            }
            cout << " -> Fitness: " << fitness[i] << endl;
        }
    }

    int spinWheelSelection() {
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
        return fitness.size() - 1; // Fallback in case of rounding errors
    }

    vector<bool> crossover(const vector<bool>& parent1, const vector<bool>& parent2) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(1, sizeList - 1); // Ensure valid crossover point

        int crossoverPoint = dist(gen);
        vector<bool> child(sizeList);

        for (int i = 0; i < sizeList; i++) {
            child[i] = (i < crossoverPoint) ? parent1[i] : parent2[i];
        }

        return child;
    }

    void mutate(vector<bool>& individual, double mutationRate) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dist(0.0, 1.0);

        for (size_t i = 0; i < individual.size(); i++) {
            if (dist(gen) < mutationRate) {
                individual[i] = !individual[i]; // Flip the bit
            }
        }
    }

    void evolve(double mutationRate, int elitismCount) {
        // Sort individuals by fitness in descending order
        vector<size_t> indices(individuals.size());
        iota(indices.begin(), indices.end(), 0); // Generate indices [0, 1, ..., n-1]
        sort(indices.begin(), indices.end(), [&](size_t i, size_t j) {
            return fitness[i] > fitness[j];
        });

        // Create a new population and add the elite individuals
        vector<vector<bool>> newPopulation;
        for (int i = 0; i < elitismCount; i++) {
            newPopulation.push_back(individuals[indices[i]]);
        }

        // Fill the rest of the new population
        while (newPopulation.size() < n) {
            // Select parents
            int parent1Index = spinWheelSelection();
            int parent2Index = spinWheelSelection();

            // Crossover
            vector<bool> child = crossover(individuals[parent1Index], individuals[parent2Index]);

            // Mutation
            mutate(child, mutationRate);

            // Add to new population
            newPopulation.push_back(child);
        }

        // Replace old population with the new one and recalculate fitness
        individuals = newPopulation;
        calculateFitness();
    }
};

int main() {
    ifstream inputFile("knapsack50.txt");

    if (!inputFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int n, maxWeight;
    inputFile >> n;

    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        inputFile >> items[i].weight >> items[i].value;
    }

    inputFile >> maxWeight;


    Population population(15, maxWeight, items);
    cout << "Initial Population:\n";
    population.displayPopulation();

    double mutationRate = 0.99; // 10% mutation rate
    int elitismCount = 2;     // Preserve top 2 individuals
    int generation = 1000;
    for (int i = 0; i < generation; i++)
    {
        population.evolve(mutationRate, elitismCount);
        cout << "gen "<<i<<" :";
        population.displayPopulation(0);

        /* code */
    } 

    return 0;
}
