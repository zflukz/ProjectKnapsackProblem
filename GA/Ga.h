#ifndef GA_H
#define GA_H

#include <vector>

struct Item {
    int value;
    int weight;
};

class Population {
private:
    int n, maxWeight, sizeList;
    std::vector<Item> itemList;
    std::vector<std::vector<bool>> individuals;
    std::vector<int> fitness;

    void calculateFitness();
    int spinWheelSelection();
    int tournamentSelection(const std::vector<std::vector<bool>>& individuals, const std::vector<int>& fitness, int tournamentSize);
    void mutate(std::vector<bool>& individual, double mutationRate);
    std::vector<bool> multiPointCrossover(const std::vector<bool>& parent1, const std::vector<bool>& parent2, int numBreakpoints);

public:
    Population(int n, int maxWeight, const std::vector<Item>& itemList);
    void evolve(double mutationRate, int elitismCount, int numBreakpoints);
    void displayPopulation(int i) const;
    void displayPopulation() const;
    int displayFitness(int i) const;
    
};

#endif
