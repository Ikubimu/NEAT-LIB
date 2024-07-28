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
    uint32_t id_node;
    node_t type;
    double output;
    void calculate_output(std::vector<input_weight> input);
    void set_output(double input);
    std::unordered_map<uint32_t, link*> back_links;

};