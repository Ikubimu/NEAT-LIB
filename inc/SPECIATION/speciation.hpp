#pragma once

#include "../GENOME/genome.hpp"

#define C1 1.0
#define C2 1.0
#define C3 0.4

class speciation
{
    public:
    speciation(genome* population, uint32_t num_population);
    genome* population;
    genome* pivot;
    uint32_t num_population;
    void set_species();

    private:
    double threshold;
    double calculate_distance(genome* ptr);
};

double calculate_mean(std::vector<double> *v);