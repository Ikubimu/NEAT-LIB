#pragma once

#include "../GENOME/genome.hpp"

#define C1 1.2
#define C2 1.2
#define C3 0.6

class speciation
{
    public:
    speciation(genome* population, uint32_t num_population);
    genome* population;
    uint32_t num_population;
    void set_species();
    std::vector<std::vector<genome*>> species;
    void set_adj_fitness();

    private:
    double threshold;
    double calculate_distance(genome* pivot, genome* ptr);
    double calculate_adj_fitness(genome* pivot, uint32_t specie_id);
};

double calculate_mean(std::vector<double> *v);