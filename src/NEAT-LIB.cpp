#include "../inc/NEAT-LIB.hpp"

neat::neat(uint32_t num_population, uint32_t num_inputs, uint32_t num_outputs)
:
num_population(num_population),
num_outputs(num_outputs),
num_inputs(num_inputs)
{
    node_counter = num_outputs + num_inputs;
    population = (genome*)malloc(num_population*sizeof(genome));
    pointer = nullptr;
    for(uint32_t i=0; i<num_population; i++)
    {
        population[i] = genome(num_inputs, num_outputs);
    }

    link_counter = 0;
    for(uint32_t i=0; i<num_inputs; i++)
    {
        id_inputs.push_back(num_outputs+i);
        std::string in = std::to_string(i); 
        for(uint32_t j=num_inputs; j<num_outputs+num_outputs; j++)
        {
            std::string out = std::to_string(j);
            std::string key = in + "-" + out;
            innovation_map.insert({out, link_counter});
            link_counter++;
        }
    }
}


bool neat::set_genome(uint32_t genome_id)
{
    if(&population[genome_id] == nullptr) return false;

    pointer = &population[genome_id];
    return true;
    
}
bool neat::predict(std::vector<double> inputs, std::vector<double> *outputs)
{
    if(pointer == nullptr)
    {
        std::cout<<"pointer_error";
        return false;
    }

    if(inputs.size() != num_inputs)
    {
        std::cout<<"input_error";
        return false;
    }

    pointer->reset_all();

    for(uint32_t i=0; i<num_inputs; i++)
    {
        pointer->set_input(id_inputs[i], inputs[i]);
    }
    pointer->step_forward(outputs);

    return true;
}

void neat::mutate()
{
    genome* ptr;
    for(uint32_t i=0; i<num_population; i++)
    {
        ptr = &population[i];
        genome_mutation(ptr);
    }
}

void neat::genome_mutation(genome* target)
{

}

void neat::create_node(genome* target)
{

    

}