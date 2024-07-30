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
        std::string in = std::to_string(num_outputs+i); 
        for(uint32_t j=0; j<num_outputs; j++)
        {
            std::string out = std::to_string(j);
            std::string key = out + "-" + in;
            innovation_map.insert({key, link_counter});
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
    //double prob = rand_double(0.0, 100.0);
    create_node(target);
    add_rand_link(target);
}

void neat::create_node(genome* target)
{
    uint32_t target_id = target->get_rand_id_link();
    link* target_link = target->get_link_by_id(target_id);
    
    
    uint32_t link_node_in = target_link->node_in;
    uint32_t link_node_out = target_link->node_out;
    node* node_out = target->get_node_by_id(link_node_out);
    uint32_t layer = node_out->layer;

    target->new_node(node_counter, (uint32_t)(layer-1));

    add_link(target, node_counter, link_node_out);
    add_link(target, link_node_in, node_counter);
    
    target->delete_link(target_id);

    node_counter++;
}

void neat::add_link(genome* target, uint32_t id_node_a, uint32_t id_node_b)
{
    target->new_link(id_node_a, id_node_b, link_counter);
    add_key_link(id_node_a, id_node_b);
    link_counter++;
}

void neat::add_rand_link(genome* target)
{
    uint32_t num = target->get_num_hidden();
    if(num == 0) return;

    for(uint8_t i=0; i<100; i++)
    {
        uint32_t id_a = target->get_rand_id_node();
        uint32_t id_b = target->get_rand_id_node();
        node* a = target->get_node_by_id(id_a);
        node* b = target->get_node_by_id(id_b);
        
        if((id_a == id_b) || ((a->type == b->type) && (a->type != node::HIDDEN))) continue;
        
        if(check_key_link(id_a, id_b) || (a->layer == b->layer)) continue;

        if(a->layer > b->layer)
        {
            add_link(target, id_b, id_a);
        }
        else
        {
            add_link(target, id_a, id_b);
        }
        break;
    }
}

void neat::add_key_link(uint32_t id_a, uint32_t id_b)
{
    if(id_a > id_b)
    {
        uint32_t aux = id_a;
        id_a = id_b;
        id_b = aux;
    }
    std::string key = std::to_string(id_a) + "-" + std::to_string(id_b);
    innovation_map.insert({key, link_counter});
}

bool neat::check_key_link(uint32_t id_a, uint32_t id_b)
{
    if(id_a > id_b)
    {
        uint32_t aux = id_a;
        id_a = id_b;
        id_b = aux;
    }

    std::string key = std::to_string(id_a) + "-" + std::to_string(id_b);
    auto it = innovation_map.find(key);
    if(it != innovation_map.end())
    {
        return true;
    }

    return false;
}