#pragma once

#include "node.hpp"
#include "link.hpp"

class genome
{
    public:
    genome(uint32_t num_inputs, uint32_t num_outputs, uint32_t Id_pop);
    genome(const genome* copy, uint32_t Id_pop);
    genome() = default;

    uint32_t Id_pop;
    void set_input(uint32_t id_node, double input);
    void step_forward(std::vector<double> *outputs);
    void reset_all();

    double get_fitness();
    double get_adj_fitness();
    void set_fitness(double fit);
    void set_adj_fitness(double fit);

    uint32_t get_rand_id_link();
    link* get_link_by_id(uint32_t id);
    uint32_t get_rand_id_node();
    node* get_node_by_id(uint32_t id);

    void new_node(uint32_t id_node, uint32_t layer);
    void new_link(uint32_t node_in, uint32_t node_out, uint32_t innovation_num);
    void delete_link(uint32_t innovation_num);
    void new_node_link(genome* parent, link* n_link);

    uint32_t get_num_hidden();
    std::unordered_map<uint32_t, link>* get_link_map();
    uint32_t get_inn_range();
    uint32_t get_links_size();

    void propagate_layer(uint32_t inter_layer);
    void add_layer_id(uint32_t layer_id, uint32_t node_id);
    void show_genome_path();

    private:
    uint32_t num_outputs;
    uint32_t num_hidden;
    double fitness, adj_fitness;
    uint32_t inn_range;
    std::unordered_map<uint32_t, node> nodes;
    std::unordered_map<uint32_t, link> links;
    std::vector<std::vector<uint32_t>> layers;
    void back_recursive_nodes(uint32_t id_node);
    void recursive_genome_path(uint32_t id_node);
};

template<typename K, typename V>
V* find_for_key(std::unordered_map<K, V> *map, const K key);