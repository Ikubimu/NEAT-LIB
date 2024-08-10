#include "../../inc/GENOME/link.hpp"


link::link(uint32_t node_in, uint32_t node_out, uint32_t innovation_num, bool enable, double weight)
:
node_in(node_in),
node_out(node_out),
innovation_num(innovation_num),
enable(enable),
weight(weight)
{

}

uint32_t link::get_node_in()
{
    return node_in;
}

bool link::is_enabled()
{
    return enable;
}

double link::get_weight()
{
    return weight;
}

void link::copy_parametres(link* copy)
{
    enable = copy->enable;
    weight = copy->weight;
}