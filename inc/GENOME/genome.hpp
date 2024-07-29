#pragma once

#include "node.hpp"
#include "link.hpp"

class genome
{
    public:
    genome(uint32_t num_inputs, uint32_t num_outputs);
    void set_input(uint32_t id_node, double input);
    void step_forward(std::vector<double> *outputs);
    void reset_all();

    uint32_t get_rand_id();
    link* get_link_by_id(uint32_t id);
    void new_node(uint32_t id_node);
    void new_link(uint32_t node_in, uint32_t node_out, uint32_t innovation_num);
    void delete_link(uint32_t innovation_num);

    private:
    uint32_t num_outputs;
    std::vector<uint32_t> id_outputs;
    std::unordered_map<uint32_t, node> nodes;
    std::unordered_map<uint32_t, link> links;
    void back_recursive_nodes(uint32_t id_node);
};

template<typename K, typename V>
V* find_for_key(std::unordered_map<K, V> *map, const K key);