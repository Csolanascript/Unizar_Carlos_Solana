#pragma once
#include "cuenta_v2.h"

class PlazoFijo : public Cuenta
{

    private:
        //Atributos (variables) de mi clase
        double interes;
        int plazo;

    public:

        //Constructor para cuenta de PLAZO FIJO
        PlazoFijo(double ci, double in, int p);
        
        double valor(int t) const;
};