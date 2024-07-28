#include "../../inc/MATH/functions.hpp"


double RELU(double input)
{
    return std::max(0.0, input);
}