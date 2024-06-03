#include "cuenta.h"
#include <cmath>

using namespace std;

Cuenta::Cuenta(Tipo t, double ci, double in, int p=0)
    :tipo(t),capital(ci),interes(in),plazo(p)
{
}
Cuenta::Cuenta(double ci, double in)
    :tipo(CORRIENTE),capital(ci),interes(in)
{
}

Cuenta::Cuenta(double ci, double in, int p)
    :tipo(CORRIENTE),capital(ci),interes(in),plazo(p)
{
}


double Cuenta::valor(int t) const 
{
    double res;
    switch(tipo)
    {
        case CORRIENTE:
            res = valor_corriente(t);   // Asumimos definida (no la he podido copiar, pero no se ha molestado en explicarlo, asi que no es importante)
            break;
        case PLAZO:
            res = valor_plazo(t);       // Asumimos definida (lo mismo)
            break;
    }
}

double Cuenta::tae() const
{
    return 100.0*(valor(12)/valor(0) - 1);
}