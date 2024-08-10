#include "../../inc/SPECIATION/crossover.hpp"





uint32_t select_genome(std::vector<fitness_rating> *prob, double sum)
{
    double rand = rand_double(0.0, sum);
    //std::cout<<"binary search "<<rand<<std::endl;
    return binary_search(prob, 0, prob->size()-1, rand);
}

uint32_t binary_search(std::vector<fitness_rating> *prob, uint32_t ini, uint32_t end, double rand)
{
    uint32_t pivot = (end+ini)/2;
    //std::cout<<std::endl<<(*prob)[ini].prob<<" "<<(int)ini<<" - "<<(*prob)[end].prob<<" "<<(int)end<<" ";
    if(pivot==ini) return ini;
   
    double prob_pivot = (*prob)[pivot].prob;
    //std::cout<<prob_pivot;
    if(prob_pivot > rand)
    {
        return binary_search(prob, ini, pivot, rand);
    }
    return binary_search(prob, pivot, end, rand);
}

genome cross_genome(genome* p1, genome* p2, uint32_t Id_pop, bool elitism)
{
    genome* principal;
    genome* secundary;
    if(elitism)
    {
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
    }
    else
    {
        genome* principal = p1;
        genome* secundary = p2;
    }
    
    genome new_genome(principal, Id_pop);

    uint32_t max_range = principal->get_inn_range();

    std::unordered_map<uint32_t, link> *map_link = new_genome.get_link_map();
    std::unordered_map<uint32_t, link> *map_p2_link = secundary->get_link_map();

    for(auto &pair : *map_p2_link)
    {
        uint32_t id = pair.first;
        link* p1_link = new_genome.get_link_by_id(id); 
        if(p1_link != nullptr)
        {
            uint32_t rand = rand_uint(99999999);
            if(rand%2) p1_link->copy_parametres(&(pair.second));
        }
        else if(id<max_range)
        {
            new_genome.new_node_link(secundary, &(pair.second));
        }
    }

    return new_genome;
}