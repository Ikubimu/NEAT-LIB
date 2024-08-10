#include "inc/NEAT-LIB.hpp"

using namespace std;

#define POPULATION 100


double check_sol(double sol, double output)
{
    double diff = abs(sol-output);
    return diff;
}

int main()
{
    std::vector<std::vector<double>> inputs = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}};
    std::vector<double> sol= {0.0, 1.0, 1.0, 0.0};
    std::vector<double> outputs;
    neat XOR(POPULATION, 2, 1);
    bool is_done = false;
    for(uint32_t i=0; i<1000; i++)
    {
        XOR.mutate();
        for(uint32_t j=0; j<POPULATION; j++)
        {
            double fitness = 4.0;
            outputs.clear();
            XOR.set_genome(j);
            cout<<"No: "<<(int)j;
            for(uint32_t k=0; k<inputs.size(); k++)
            {
                XOR.predict_train(inputs[k], &outputs);
                //cout<<outputs[k]<<endl;
                fitness -= check_sol(sol[k], outputs[k]);
            }
            
            cout<<" fitness: "<<fitness<<endl;
            XOR.train_fitness(max(fitness, 0.0));
            if(round(fitness) == 4.0)
            {
                is_done = true;
                break;
            }
        }
        if(is_done) break;

        std::cout<<"over "<<(int)i<<std::endl;
        XOR.configure_species();
    }
    cout<<"sol found"<<endl;
    for(uint32_t k=0; k<inputs.size(); k++)
    {
        outputs.clear();
        cout<<"inputs: "<<inputs[k][0]<<" "<<inputs[k][1]<<endl;
        XOR.predict(inputs[k], &outputs);
        cout<<"Output: "<<outputs[k]<<endl;  
    }
    
    return 0;
}