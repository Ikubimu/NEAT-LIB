#include "../../inc/GENOME/node.hpp"

node::node(node_t type, uint32_t id_node)
:
type(type),
id_node(id_node)
{
    if(type == OUTPUT) layer = 1000;
    else 
    {
        layer = 0;
    }
    output = -1.0;

}

node::node(node_t type, uint32_t id_node, uint32_t layer)
:
type(type),
id_node(id_node),
layer(layer)
{
    output = -1.0;
}

void node::set_output(double input)
{ 
    output = input;   
}

void node::calculate_output(std::vector<input_weight> inputs)
{
    double activation_input = 0;
    for(uint32_t i=0; i<inputs.size(); i++)
    {
        activation_input += (inputs[i].input)*(inputs[i].weight);
    }
    output = RELU(activation_input);
}

void node::add_back_link(link* new_link)
{
    uint32_t index = new_link->innovation_num;
    back_links.insert({index, new_link});
}

void node::delete_back_link(uint32_t innovation_num)
{
    back_links.erase(innovation_num);
}