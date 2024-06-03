#pragma once
#include "cuenta_v2.h" 

class CuentaCorriente : public Cuenta
{
    private:

        double interes;

    private:
        //Funciones auxiliares
        double valor_corriente(int t) const;
        double valor_plazo(int t) const;
    public:
        //Constructor general
        CuentaCorriente(double ci, double in);
        
        double valor(int t) const;
};