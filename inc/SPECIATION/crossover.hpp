#pragma once

#include "../GENOME/genome.hpp"

typedef struct
{
    double fitness;
    double prob;
}fitness_rating;


genome cross_genome(genome* p1, genome* p2, uint32_t Id_pop);
uint32_t select_genome(std::vector<fitness_rating> *prob);
uint32_t selection_short(std::vector<fitness_rating> *prob, uint32_t ini, uint32_t end, double rand);