#include "../../inc/SPECIATION/speciation.hpp"

speciation::speciation(uint32_t num_population, uint32_t num_outputs, uint32_t num_inputs, uint32_t target)
:
population(population),
num_population(num_population),
num_outputs(num_outputs),
num_inputs(num_inputs),
target(target)
{
    population = nullptr;
    threshold = 2.0;
    adj_threshold = 0.1;
}

void speciation::set_species(std::vector<genome> *old_population)
{
    population  = old_population;
    species.clear();
    std::vector<genome> all = *population;

    specie first;
    first.members.push_back(0);
    species.push_back(first);
    all.erase(all.begin());
    uint32_t pivot;
    for(uint32_t i=0; i<species.size(); i++)
    {
        pivot = species[i].members[0];
        for(uint32_t j=0; j<all.size(); j++)
        {
            double dis = calculate_distance(&(*population)[pivot], &all[j]);
            if(dis<threshold)
            {
                species[i].members.push_back(all[j].Id_pop);
                all.erase(all.begin()+j);
                j--;
            } 
        }

        if(!all.empty())
        {
            specie new_specie;
            new_specie.members.push_back(all[0].Id_pop);
            all.erase(all.begin());
            species.push_back(new_specie);
        }
    }
}

void speciation::set_adj_fitness()
{
    double avg_total=0;
    for(uint32_t i=0; i<species.size(); i++)
    {
        double avg=0;
        species[i].sum_fitness = 0;
        for(uint32_t j=0; j<species[i].members.size(); j++)
        {
            uint32_t id = species[i].members[j];
            genome* target = &(*population)[id];

            double adjfit = calculate_adj_fitness(id, i);
            avg += adjfit;

            //std::cout<<avg<<std::endl;
            species[i].sum_fitness += target->get_fitness();

            target->set_adj_fitness(adjfit);
        }

        species[i].avg_fitness = species[i].sum_fitness / (double) species[i].members.size();
        species[i].avg_adjfitness = avg / (double) species[i].members.size();
        avg_total += avg;
    }

    avg_total /= num_population;

    for(uint32_t i=0; i<species.size(); i++)
    {
        //std::cout<<(int)species[i].members.size()<<" "<<species[i].avg_adjfitness<<" "<<avg_total<<std::endl;
        species[i].offspring = (uint32_t)(((double)species[i].members.size()*species[i].avg_adjfitness)/avg_total);
    }
}

double speciation::calculate_adj_fitness(uint32_t ind_id, uint32_t specie_id)
{
    genome pivot = (*population)[ind_id];
    double fit = pivot.get_fitness();
    double sum = 0.0001;
    for(uint32_t i=0; i<species[specie_id].members.size(); i++)
    {
        uint32_t id = species[specie_id].members[i];
        if(id == ind_id) continue;

        sum += sinh(calculate_distance(&pivot, &(*population)[id]));
        //std::cout<<sum<<std::endl;
    }
    //std::cout<<fit<<" "<<sum<<std::endl;
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
    disjoin += (aux->get_link_map()->size() - weight_difference.size()); 

    n = (double)std::max(aux->get_links_size(), (uint32_t)map_link->size());
    double mean = calculate_mean(&weight_difference);
    double sol = (double)(excess/n)*C1 + (double)(disjoin/n)*C2 +  mean*C3;
    //std::cout<<(int)excess<<" "<<(int)disjoin<<" "<<n<<" "<<mean<<" "<<sol<<std::endl;
    return sol;
}

double calculate_mean(std::vector<double> *v)
{
    if(v->size()==0) return 0.0;
    double sum = 0;
    for(uint32_t i=0; i<v->size(); i++)
    {
        sum += (*v)[i];
    }
    return (sum/(double)v->size());
}

void speciation::set_new_population(std::vector<genome> *new_population)
{
    //rand_offspring_population(new_population);
    rating_offspring_population(new_population);
    set_new_threshold();
}

void speciation::rand_offspring_population(std::vector<genome> *new_population)
{
    for(uint32_t i=0; i<species.size(); i++)
    {
        if(species[i].members.size()==1)
        {
            uint32_t id = species[i].members[0];
            genome *p1 = &(*population)[id];
            for(uint32_t j=0; j<species[i].offspring; j++)
            {
                uint32_t rand_id;
                do
                {
                rand_id = rand_uint(num_population);
                }while(id == rand_id);
                genome *p2 = &(*population)[rand_id];
                new_population->push_back(cross_genome(p1, p2, new_population->size(), false));
            }
        }
        else
        {
            for(uint32_t j=0; j<species[i].offspring; j++)
            {
                uint32_t rand_id_1 = species[i].members[rand_uint(species[i].members.size())];
                
                genome *p1 = &(*population)[rand_id_1];
                uint32_t rand_id_2;
                do
                {
                rand_id_2 = species[i].members[rand_uint(species[i].members.size())];
                }while(rand_id_1 == rand_id_2);

                genome *p2 = &(*population)[rand_id_2];
                //new_population->push_back(genome(p1, new_population->size()));
                new_population->push_back(cross_genome(p1, p2, new_population->size()));
            }
        }
    }
    uint32_t less_size = num_population - new_population->size();
    if(less_size>0)
    {
        for(uint32_t i=0; i<less_size; i++)
        {
            uint32_t rand_id_1 = rand_uint(num_population);
            uint32_t rand_id_2;
            do
            {
            rand_id_2 = rand_uint(num_population);
            }while(rand_id_1 == rand_id_2);
            genome *p1 = &(*population)[rand_id_1];
            genome *p2 = &(*population)[rand_id_2];
            //new_population->push_back(genome(p1, new_population->size()));
            new_population->push_back(cross_genome(p1, p2, new_population->size()));
        }
    }
}

void speciation::rating_offspring_population(std::vector<genome> *new_population)
{
    for(uint32_t i=0; i<species.size(); i++)
    {
        if(species[i].members.size()==1)
        {
            uint32_t id = species[i].members[0];
            genome *p1 = &(*population)[id];
            for(uint32_t j=0; j<species[i].offspring; j++)
            {
                uint32_t rand_id;
                do
                {
                rand_id = rand_uint(num_population);
                }while(id == rand_id);
                genome *p2 = &(*population)[rand_id];
                new_population->push_back(cross_genome(p1, p2, new_population->size()));
            }
        }
        else
        {
            std::vector<fitness_rating> rating;
            double mean, sum_prob = 0;
            uint32_t size = species[i].members.size();
            for(uint32_t j=0; j<size; j++)
            {
                uint32_t id = species[i].members[j];
                double fit = (*population)[id].get_fitness();
                mean += fit;
                rating.push_back({fit, 0.0});
            }
            mean = mean/(double)rating.size();
            //std::cout<<"media: "<<mean<<"tamaño: "<<rating.size()<<std::endl;
            for(uint32_t j=1; j<=size; j++)
            {
                double prob;
                double fit = rating[j-1].fitness;
                if(fit>mean) prob = (fit-mean)*(fit-mean);
                else prob = 0;
                //std::cout<<(int)species[i].members[j-1]<<" fit: "<<fit<<" prob: "<<prob<<std::endl;
                rating[j%size].prob = prob + sum_prob;
                sum_prob += prob;
            }
            rating[0].prob = 0;

            for(uint32_t j=0; j<species[i].offspring; j++)
            {
                uint32_t id_1, id_2;
                id_1 = species[i].members[select_genome(&rating, sum_prob)];
                id_2 = species[i].members[select_genome(&rating, sum_prob)];
                //std::cout<<(int)id_1<<" "<<(int)id_2<<"-->"<<(int)new_population->size()<<std::endl;
                #if 0
                do
                {
                    id_2 = select_genome(&rating);
                } while (id_1 == id_2);
                #endif
                genome *p1 = &(*population)[id_1];
                genome *p2 = &(*population)[id_2];
                //new_population->push_back(genome(p1, new_population->size()));
                new_population->push_back(cross_genome(p1, p2, new_population->size()));
            }
        }
    }
    int32_t less_size = num_population - new_population->size();
    if(less_size>0)
    {
        for(uint32_t i=0; i<less_size; i++)
        {
            uint32_t rand_id_1 = rand_uint(num_population);
            uint32_t rand_id_2;
            do
            {
            rand_id_2 = rand_uint(num_population);
            }while(rand_id_1 == rand_id_2);
            genome *p1 = &(*population)[rand_id_1];
            genome *p2 = &(*population)[rand_id_2];
            //new_population->push_back(genome(p1, new_population->size()));
            new_population->push_back(cross_genome(p1, p2, new_population->size()));
        }
    }
}

void speciation::set_new_threshold()
{
    if(species.size()<target)
    {
        threshold = threshold - adj_threshold;
    }
    else
    {
        threshold = threshold + adj_threshold;
    }
}