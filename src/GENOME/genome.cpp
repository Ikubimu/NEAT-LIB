#include "../../inc/GENOME/genome.hpp"


genome::genome(uint32_t num_inputs, uint32_t num_outputs)
:
num_outputs(num_outputs)
{
    num_hidden = 0;
    uint32_t index = 0;
    uint32_t i = 0;
    fitness = adj_fitness = 0;
    for(i; i<num_outputs; i++)
    {
        nodes.insert({i, node(node::OUTPUT, i)});
        node *target = find_for_key(&nodes, i);
        for(uint32_t j=num_outputs; j<(num_inputs+num_outputs); j++)
        {
            links.insert({index, link(j, i, index)});
            target->back_links.insert({index, find_for_key(&links, index)});
            index++;
        }
    }
    inn_range = index-1;
    for(i; i<num_outputs+num_inputs; i++)
    {
        nodes.insert({i, node(node::INPUT, i)});
    }
}

template<typename K, typename V>
V* find_for_key(std::unordered_map<K, V> *map, const K key)
{
    const auto &it = map->find(key);
    if (it != map->end())
    {
        return &(it->second);
    }
    return nullptr;
}


void genome::set_input(uint32_t id_input, double input)
{
    find_for_key(&nodes, id_input)->set_output(input);
}

void genome::step_forward(std::vector<double> *outputs)
{
    for(uint32_t i=0; i<num_outputs; i++)
    {
        back_recursive_nodes(i);
        outputs->push_back(find_for_key(&nodes, i)->output);
    }
}

void genome::back_recursive_nodes(uint32_t id_node)
{
    std::vector<input_weight> parametres;
    node *target = find_for_key(&nodes, id_node);
    for(auto& pair : target->back_links)
    {
        link* target_link = pair.second;
        if(!target_link->is_enabled()) continue;
        uint32_t back_node_id = target_link->get_node_in();
        node *back_node = find_for_key(&nodes, back_node_id);
        if(back_node->output == -1.0)
        {
            back_recursive_nodes(back_node_id);
        }
        parametres.push_back({target_link->get_weight(), back_node->output});
    }

    if(target->type != node::INPUT) target->calculate_output(parametres);
}

void genome::reset_all()
{
    for (auto& pair : nodes)
    {
        pair.second.set_output(-1.0);
    }
}

uint32_t genome::get_rand_id_link()
{
    uint32_t iter = rand_uint(links.size());
    auto it = links.begin();
    std::advance(it, iter);
    return it->first;
}

link* genome::get_link_by_id(uint32_t id)
{
    return find_for_key(&links, id);
}

uint32_t genome::get_rand_id_node()
{
    uint32_t iter = rand_uint(nodes.size());
    auto it = nodes.begin();
    std::advance(it, iter);
    return it->first;
}

node* genome::get_node_by_id(uint32_t id)
{
    return find_for_key(&nodes, id);
}

void genome::new_node(uint32_t id_node, uint32_t layer)
{
    nodes.insert({id_node, node(node::HIDDEN, id_node, layer)});
    num_hidden++;
}

void genome::new_link(uint32_t node_in, uint32_t node_out, uint32_t innovation_num)
{
    links.insert({innovation_num, link(node_in, node_out, innovation_num)});
    inn_range = innovation_num;
    node* target = find_for_key(&nodes, node_out);
    target->add_back_link(find_for_key(&links, innovation_num));
}

void genome::delete_link(uint32_t innovation_num)
{
    link* target = find_for_key(&links, innovation_num);
    node* target_node = find_for_key(&nodes, target->node_out);
    target_node->delete_back_link(innovation_num);
    links.erase(innovation_num);
}

uint32_t genome::get_num_hidden()
{
    return num_hidden;
}

std::unordered_map<uint32_t, link>* genome::get_link_map()
{
    return &links; 
}

uint32_t genome::get_inn_range()
{
    return inn_range;
}

uint32_t genome::get_links_size()
{
    return links.size();
}

void genome::set_fitness(double fit)
{
    fitness = fit;
}

void genome::set_adj_fitness(double fit)
{
    adj_fitness = fit;
}

double genome::get_fitness()
{
    return fitness;
}

double genome::get_adj_fitness()
{
    return adj_fitness;
}


void genome::propagate_layer(uint32_t inter_layer, uint32_t id_node)
{
    node* target = find_for_key(&nodes, id_node);
    if(target->layer == inter_layer)
    {
        target->layer = target->layer -1;

        for(auto& pair : target->back_links)
        {
            uint32_t id_back_node = pair.second->node_in;
            propagate_layer(inter_layer-1, id_back_node); 
        }
    }
}