#pragma once

#include "link.hpp"

class node
{
    public:

    enum node_t
    {
        INPUT,
        OUTPUT,
        HIDDEN
    };

    node(node_t type, uint32_t id_node);
    node(node_t type, uint32_t id_node, uint32_t layer);
    uint32_t id_node;
    node_t type;
    uint32_t layer;
    double output;
    void calculate_output(std::vector<input_weight> input);
    void set_output(double input);
    std::vector<uint32_t> back_links;
    std::unordered_map<uint32_t, link*> front_links;

    void add_back_link(uint32_t innovation_num);
    void delete_back_link(uint32_t innovation_num);

};