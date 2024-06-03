#include "cuenta_v2.h"
#include <cmath>

using namespace std;

Cuenta::Cuenta(double ci)
    :capital(ci)
{
}


double Cuenta::valor(int t) const 
{
} 

double Cuenta::tae() const
{
    return 100.0*(valor(12)/valor(0) - 1);
}