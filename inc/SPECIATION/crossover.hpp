#pragma once

#include "../GENOME/genome.hpp"

typedef struct
{
    double fitness;
    double prob;
}fitness_rating;

class crossover
{
    public:
    crossover(uint32_t num_inputs, uint32_t num_outputs);
    genome* new_population(std::vector<std::vector<genome*>> *species, std::vector<uint32_t> *offspring, uint32_t num_population);

    private:
    uint32_t num_outputs, num_inputs;
    void species_offspring(std::vector<genome*> *specie, uint32_t offspring, genome* new_population, uint32_t *index);
    genome cross_genome(genome* p1, genome* p2);
    uint32_t select_genome(std::vector<fitness_rating> *prob);
    uint32_t selection_short(std::vector<fitness_rating> *prob, uint32_t ini, uint32_t end, double rand);
};