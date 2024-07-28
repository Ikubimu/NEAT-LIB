#include "../../inc/GENOME/genome.hpp"


genome::genome(uint32_t num_inputs, uint32_t num_outputs)
{
    
    uint32_t index = 0;
    uint32_t i =0;
    for(i; i<num_inputs; i++)
    {
        nodes.insert({i, node(node::INPUT, i)});
        for(uint32_t j=num_inputs; j<(num_inputs+num_outputs); j++)
        {
            links.push_back(link(i, j, index));
            index++;
        }
    }
    index = 0;
    for(i; i<num_outputs+num_inputs; i++)
    {
        nodes.insert({i, node(node::OUTPUT, i)});
        id_outputs.push_back(i);
        node *target = find_for_key(&nodes, i);
        for(uint32_t j=0; j<num_inputs; j++)
        {
            target->back_links.push_back(&links[index]);
            index++;
        }
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

std::vector<uint32_t> genome::get_id_outputs()
{
    return id_outputs;
}

void genome::set_input(uint32_t id_input, double input)
{
    //nodes[id_input].set_output(input);
}

void genome::step_forward(std::vector<double> *outputs)
{
    for(uint32_t i=0; i<id_outputs.size(); i++)
    {
        back_recursive_nodes(id_outputs[i]);
        outputs->push_back(find_for_key(&nodes, id_outputs[i])->output);
    }
}

void genome::back_recursive_nodes(uint32_t id_node)
{
    std::vector<input_weight> parametres;
    node *target = find_for_key(&nodes, id_node);
    for(uint32_t i=0; i<target->back_links.size(); i++)
    {
        link* target_link = target->back_links[i];

        if(!target_link->is_enabled()) continue;

        uint32_t back_node_id = target_link->get_node_in();
        node *back_node = find_for_key(&nodes, id_node);
        if(back_node->output == -1.0)
        {
            back_recursive_nodes(back_node_id);
        }
        parametres.push_back({target_link->get_weight(), back_node->output});
    }

    target->calculate_output(parametres);
}

void genome::reset_all()
{
    for (auto& pair : nodes)
    {
        pair.second.set_output(-1.0);
    }
}