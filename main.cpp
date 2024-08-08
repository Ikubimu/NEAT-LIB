#include "inc/NEAT-LIB.hpp"

using namespace std;

#define POPULATION 500

int main()
{
    std::vector<double> inputs = {1.0, 1.0};
    std::vector<double> outputs;
    neat XOR(POPULATION, 2, 4);
    
    for(uint32_t i=0; i<1000; i++)
    {
        cout<<(int)i<<std::endl;
        XOR.mutate();
        for(uint32_t j=0; j<POPULATION; j++)
        {
            outputs.clear();
            XOR.set_genome(j);
            XOR.predict(inputs, &outputs);
            /*
            cout<<"Number: "<<j<<" Output: ";
            for(uint32_t i=0; i<outputs.size(); i++)
            {
                cout<<outputs[i]<<" ";
            }
            cout<<endl;
            */
            XOR.train_fitness(2.0);
        }

        std::cout<<"over"<<std::endl;
        XOR.configure_species();
    }
    
    return 0;
}