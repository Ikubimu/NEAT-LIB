#include "inc/NEAT-LIB.hpp"

using namespace std;

#define POPULATION 100

int main()
{
    std::vector<double> inputs = {1.0, 1.0};
    std::vector<double> outputs;
    neat XOR(POPULATION, 2, 4);
    
    XOR.mutate();
    for(uint32_t j=0; j<POPULATION; j++)
    {
        outputs.clear();
        XOR.set_genome(j);
        XOR.predict(inputs, &outputs);
        cout<<"Number: "<<j<<" Output: ";
        for(uint32_t i=0; i<outputs.size(); i++)
        {
            cout<<outputs[i]<<" ";
        }
        cout<<endl;
    }
    
    return 0;
}