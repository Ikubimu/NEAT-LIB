#include "../inc/NEAT-LIB.hpp"

neat::neat(uint32_t num_population, uint32_t num_inputs, uint32_t num_outputs)
:
num_population(num_population),
num_outputs(num_outputs),
num_inputs(num_inputs)
{
    node_counter = num_outputs + num_inputs - 1;
    link_counter = num_inputs*num_outputs -1 ;
    population = (genome*)malloc(num_population*sizeof(genome));
    pointer = nullptr;
    for(uint32_t i=0; i<num_population; i++)
    {
        population[i] = genome(num_inputs, num_outputs);
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
        pointer->set_input(i, inputs[i]);
    }
    pointer->step_forward(outputs);

    return true;
}