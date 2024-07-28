
#pragma once
#include "GENOME/genome.hpp"

class neat
{
    public:
    neat(uint32_t num_population, uint32_t num_inputs, uint32_t num_outputs);
    bool set_genome(uint32_t genome_id);
    bool predict(std::vector<double> inputs, std::vector<double> *outputs);
    private:
    uint32_t num_population;
    uint32_t num_inputs;
    uint32_t num_outputs;
    uint32_t node_counter;
    uint32_t link_counter;

    std::unordered_map<std::string, uint32_t> innovation_map;

    genome* population;
    std::vector<std::vector<genome*>> species;
    genome* pointer;
};