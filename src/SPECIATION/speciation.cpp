#include "../../inc/SPECIATION/speciation.hpp"

speciation::speciation(genome* population, uint32_t num_population)
:
population(population),
num_population(num_population)
{
    threshold = 0.3;
}

void speciation::set_species()
{
    species.clear();
    std::vector<genome*> all;
    for(uint32_t i=1; i<num_population; i++)
    {
        all.push_back(population+i);
    }
    std::vector<genome*> first_specie;
    first_specie.push_back(population);
    species.push_back(first_specie);
    genome* pivot;
    for(uint32_t i=0; i<species.size(); i++)
    {
        pivot = species[i][0];
        for(uint32_t j=0; j<all.size(); j++)
        {
            double dis = calculate_distance(pivot, all[j]);
            if(dis<threshold)
            {
                species[i].push_back(all[j]);
                all.erase(all.begin()+j);
                j--;
            } 
        }
        if(!all.empty())
        {
            std::vector<genome*> new_specie;
            new_specie.push_back(all[0]);
            all.erase(all.begin());
            species.push_back(new_specie);
        }
    }
}

void speciation::set_adj_fitness()
{
    for(uint32_t i=0; i<species.size(); i++)
    {
        for(uint32_t j=0; j<species[i].size(); j++)
        {
            genome* ptr = species[i][j];
            ptr->set_adj_fitness(calculate_adj_fitness(ptr, i));
        }
    }
}

double speciation::calculate_adj_fitness(genome* pivot, uint32_t specie_id)
{
    double fit = pivot->get_fitness();
    double sum = 0.0001;
    genome* ptr;
    for(uint32_t i=0; i<species[specie_id].size(); i++)
    {
        ptr = species[specie_id][i];
        if(ptr == pivot) continue;

        sum += sinh(calculate_distance(pivot, ptr));
    }
    return fit/sum;
}

double speciation::calculate_distance(genome* pivot, genome* ptr)
{
    genome* aux;
    uint32_t low_range;
    std::unordered_map<uint32_t, link>* map_link;
    if(ptr->get_inn_range() < pivot->get_inn_range())
    {
        map_link = pivot->get_link_map();
        aux = ptr;
        low_range = ptr->get_inn_range();
    }
    else
    {
        map_link = ptr->get_link_map();
        aux = pivot;
        low_range = pivot->get_inn_range();
    }

    uint32_t excess = 0;
    uint32_t disjoin = 0;
    double n;
    std::vector<double> weight_difference;
    
    auto it = map_link->begin();
    for(;;)
    {
        uint32_t inn_num = it->first;
        link* link_ptr = aux->get_link_by_id(inn_num);
        if(link_ptr != nullptr)
        {
            double w1 = link_ptr->weight;
            double w2 = it->second.weight;
            weight_difference.push_back(abs((w1-w2)));
        }
        else if(inn_num > low_range) excess++;
        else disjoin++;

        std::advance(it, 1);
        if(it == map_link->end()) break;
    }

    n = (double)std::max(aux->get_links_size(), (uint32_t)map_link->size());
    double mean = calculate_mean(&weight_difference);

    return (double)(excess/n)*C1 + (double)(disjoin/n)*C2 +  mean*C3;
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