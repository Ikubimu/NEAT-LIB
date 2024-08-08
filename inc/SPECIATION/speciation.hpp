#pragma once

#include "../GENOME/genome.hpp"
#include "crossover.hpp"

#define C1 1.2
#define C2 1.2
#define C3 0.6

typedef struct
{
    double avg_fitness, avg_adjfitness, sum_fitness;
    uint32_t offspring;
    std::vector<uint32_t> members;
}specie;

class speciation
{
    public:
    speciation(uint32_t num_population, uint32_t num_outputs, uint32_t num_inputs);
    std::vector<genome> *population;
    uint32_t num_population;
    void set_species(std::vector<genome> *old_population);
    std::vector<specie> species;
    void set_adj_fitness();
    void set_new_population(std::vector<genome> *new_population);

    private:
    double threshold;
    uint32_t num_outputs, num_inputs;
    double calculate_distance(genome* pivot, genome* ptr);
    double calculate_adj_fitness(uint32_t ind_id, uint32_t specie_id);
    void rand_offspring_population(std::vector<genome> *new_population);
};

double calculate_mean(std::vector<double> *v);