#include "../../inc/SPECIATION/crossover.hpp"


crossover::crossover(uint32_t num_inputs, uint32_t num_outputs)
:
num_outputs(num_outputs),
num_inputs(num_inputs)
{

}
genome* crossover::new_population(std::vector<std::vector<genome*>> *species, std::vector<uint32_t> *offspring, uint32_t num_population)
{
    genome* new_population = new genome[num_population];
    uint32_t index = 0;
    for(uint32_t i=0; i<species->size(); i++)
    {
        species_offspring(&(*species)[i], (*offspring)[i], new_population, &index);
    }
    return new_population;
}

void crossover::species_offspring(std::vector<genome*> *specie, uint32_t offspring, genome* new_population, uint32_t* index)
{
    if(specie->size()==1)
    {
        for(uint32_t i=0; i<offspring; i++)
        {
            new_population[*index] = genome((*specie)[0]);
            (*index)++;
        }
    }
    else if(specie->size()==2)
    {
        for(uint32_t i=0; i<offspring; i++)
        {
            new_population[*index] = cross_genome((*specie)[0], (*specie)[1]);
            (*index)++;
        }
    }
    else
    {
        std::vector<fitness_rating> rating;
        double sum, sum_prob = 0;
        for(uint32_t i=0; i<specie->size(); i++)
        {
            double fit = (*specie)[i]->get_adj_fitness();
            sum += fit;
            rating.push_back({fit, -1});
        }
        for(uint32_t i=0; i<specie->size(); i++)
        {
            double prob = rating[i].fitness/sum;
            rating[i].prob = prob + sum_prob;
            sum_prob += prob;
        }
        for(uint32_t i=0; i<offspring; i++)
        {
            uint32_t p1, p2;
            p1 = select_genome(&rating);
            p2 = p1;
            while(p1==p2)
            {
                p2= select_genome(&rating);
            }
            new_population[*index] = cross_genome((*specie)[p1], (*specie)[p2]);
            (*index)++;

        }
    }

}

uint32_t crossover::select_genome(std::vector<fitness_rating> *prob)
{
    double rand = rand_double(0.0, 1.0);
    return selection_short(prob, 0, prob->size()-1, rand);
}

uint32_t crossover::selection_short(std::vector<fitness_rating> *prob, uint32_t ini, uint32_t end, double rand)
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

genome crossover::cross_genome(genome* p1, genome* p2)
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
    uint32_t min_range = std::min(p1->get_inn_range(), p2->get_inn_range());
    bool excess_ok = false;
    if(secundary->get_inn_range() == min_range) excess_ok = true;

    genome new_genome(num_inputs, num_outputs, false);
    std::unordered_map<uint32_t, link> *principal_map = principal->get_link_map();
    std::unordered_map<uint32_t, link> *secundary_map = secundary->get_link_map();

    auto it_p = principal_map->begin();
    while(it_p != principal_map->end())
    {
        uint32_t id = it_p->first;
        link* p2_link = secundary->get_link_by_id(id);
        if(p2_link != nullptr)
        {
            uint32_t rand = rand_uint(100);
            rand<50 ? new_genome.new_node_link(principal, &(it_p->second)) : new_genome.new_node_link(secundary, p2_link);
        }
        else
        {
            new_genome.new_node_link(principal, &(it_p->second));
        }

        std::advance(it_p, 1);
    }

    auto it_s = secundary_map->begin();
    while(it_s != secundary_map->end())
    {
        uint32_t id = it_s->first;
        link* p1_link = principal->get_link_by_id(id);
        if(p1_link == nullptr && id<min_range)
        {
            new_genome.new_node_link(secundary, &(it_s->second));
        }

        std::advance(it_s, 1);
    }

    return new_genome;
}