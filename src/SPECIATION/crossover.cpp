#include "../../inc/SPECIATION/crossover.hpp"





uint32_t select_genome(std::vector<fitness_rating> *prob)
{
    double rand = rand_double(0.0, 1.0);
    return selection_short(prob, 0, prob->size()-1, rand);
}

uint32_t selection_short(std::vector<fitness_rating> *prob, uint32_t ini, uint32_t end, double rand)
{
    uint32_t pivot = (end+ini)/2;
    if(ini==pivot) return ini;

    
    double prob_pivot = (*prob)[pivot].prob;
    if(prob_pivot > rand)
    {
        return selection_short(prob, ini, pivot-1, rand);
    }
    return selection_short(prob, pivot+1, end, rand);
}

genome cross_genome(genome* p1, genome* p2, uint32_t Id_pop)
{
    genome* principal;
    genome* secundary;
    if(p1->get_adj_fitness() > p2->get_adj_fitness())
    {
        principal = p1;
        secundary = p2;
    }
    else
    {
        principal = p2;
        secundary = p1;
    }
    
    genome new_genome(principal, Id_pop);
    return new_genome;
}