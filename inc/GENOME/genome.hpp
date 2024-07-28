#pragma once

#include "node.hpp"
#include "link.hpp"

class genome
{
    public:
    genome(uint32_t num_inputs, uint32_t num_outputs);
    std::vector<uint32_t> get_id_outputs();
    void set_input(uint32_t id_node, double input);
    void step_forward(std::vector<double> *outputs);
    void reset_all();

    private:
    std::vector<uint32_t> id_outputs;
    std::unordered_map<uint32_t, node> nodes;
    std::vector<link> links;
    void back_recursive_nodes(uint32_t id_node);
};

template<typename K, typename V>
V* find_for_key(std::unordered_map<K, V> *map, const K key);