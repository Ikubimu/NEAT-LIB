#include "../../inc/SPECIATION/speciation.hpp"

speciation::speciation(genome* population, uint32_t num_population)
:
population(population),
num_population(num_population)
{
    threshold = 3.0;
}

void speciation::set_species()
{

}

double speciation::calculate_distance(genome* ptr)
{
    genome* aux;
    uint32_t low_range;
    std::unordered_map<uint32_t, link>* map_link;
    if(ptr->get_inn_range() > pivot->get_inn_range())
    {
        map_link = pivot->get_link_map();
        aux = ptr;
        low_range = pivot->get_inn_range();
    }
    else
    {
        map_link = ptr->get_link_map();
        aux = pivot;
        low_range = ptr->get_inn_range();
    }

    uint32_t excess = 0;
    uint32_t disjoin = 0;
    double n;
    std::vector<double> weight_difference;
    
    auto it = map_link->begin();
    uint32_t i = 0;

    while(it != map_link->end() || i>=low_range)
    {
        uint32_t inn_num = it->first;
        link* link_ptr = aux->get_link_by_id(inn_num);
        if(link_ptr != nullptr)
        {
            double w1 = link_ptr->weight;
            double w2 = it->second.weight;
            weight_difference.push_back(abs((w1-w2)));
        }
        else
        {
            disjoin++;
        }
        i++;
    }

    disjoin += aux->get_links_size() - weight_difference.size();
    excess = map_link->size() - i;
    n = (double)std::max(aux->get_links_size(), (uint32_t)map_link->size());
    double mean = calculate_mean(&weight_difference);

    return (excess/n)*C1 + (disjoin/n)*C2 +  mean*C3;
}

double calculate_mean(std::vector<double> *v)
{
    double sum = 0;
    for(uint32_t i=0; i<v->size(); i++)
    {
        sum += (*v)[i];
    }
    return (sum/(double)v->size());
}