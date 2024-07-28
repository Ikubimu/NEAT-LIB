
#include "../../inc/MATH/ranges.hpp"

double rand_double(double min, double max)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generador(seed);
    std::uniform_real_distribution<double> distribucion(min, max);
    return distribucion(generador);
}