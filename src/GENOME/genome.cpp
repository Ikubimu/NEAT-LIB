#include "../../inc/GENOME/genome.hpp"


genome::genome(uint32_t num_inputs, uint32_t num_outputs)
:
num_outputs(num_outputs)
{
    
    uint32_t index = 0;
    uint32_t i =0;
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