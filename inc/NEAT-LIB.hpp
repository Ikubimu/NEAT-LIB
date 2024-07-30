
#pragma once
#include "GENOME/genome.hpp"

class neat
{
    public:
    neat(uint32_t num_population, uint32_t num_inputs, uint32_t num_outputs);
    bool set_genome(uint32_t genome_id);
    bool predict(std::vector<double> inputs, std::vector<double> *outputs);
    void mutate();
    private:
    uint32_t num_population;
    uint32_t num_inputs;
    uint32_t num_outputs;
    std::vector<uint32_t> id_inputs;
    uint32_t node_counter;
    uint32_t link_counter;
    void genome_mutation(genome* target);
    void create_node(genome* target);
    void add_rand_link(genome* target);

    void add_link(genome* target, uint32_t id_node_a, uint32_t id_node_b);
    
    std::unordered_map<std::string, uint32_t> innovation_map;
    void add_key_link(uint32_t id_a, uint32_t id_b);
    bool check_key_link(uint32_t id_a, uint32_t id_b);

    genome* population;
    std::vector<std::vector<genome*>> species;
    genome* pointer;
};