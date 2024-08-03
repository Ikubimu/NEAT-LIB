#pragma once

#include "crossover.hpp"

#define C1 1.0
#define C2 1.0
#define C3 0.4

class speciation
{
    public:
    speciation(genome* population, uint32_t num_population, uint32_t target, uint32_t num_inputs, uint32_t num_outputs);
    genome* population;
    uint32_t num_population;
    void set_species();
    std::vector<std::vector<genome*>> species;
    void set_adj_fitness();
    genome* set_new_population();

    private:
    crossover cross;
    double threshold;
    uint32_t target;
    std::vector<uint32_t> offspring;
    double calculate_distance(genome* pivot, genome* ptr);
    double calculate_adj_fitness(genome* pivot, uint32_t specie_id);
};

double calculate_mean(std::vector<double> *v);