#pragma once

#include "../MATH/functions.hpp"
#include "../MATH/ranges.hpp"
class link
{
    public:
    link(uint32_t node_in, uint32_t node_out, uint32_t innovation_num, bool enable = true, double weight = rand_double(-1.0, 1.0));
    uint32_t get_node_in();
    bool is_enabled();
    double get_weight();
    void copy_parametres(link* copy);

    uint32_t node_in;
    uint32_t node_out;
    bool enable;
    uint32_t innovation_num;
    double weight;
};

typedef struct
{
    double weight;
    double input;
}input_weight;