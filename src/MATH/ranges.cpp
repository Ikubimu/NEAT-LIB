
#include "../../inc/MATH/ranges.hpp"

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine gen(seed);

double rand_double(double min, double max)
{
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

uint32_t rand_uint(uint32_t size)
{
    std::uniform_int_distribution<uint32_t> dis(0, size-1);
    return dis(gen);
}