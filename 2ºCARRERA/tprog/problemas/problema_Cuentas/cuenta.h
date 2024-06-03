#pragma once 

class Cuenta 
{
    public:
        enum Tipo {CORRIENTE, PLAZO};
    private:
        //Atributos (variables) de mi clase
        Tipo tipo;
        double capital;
        double interes;
        int plazo;

    private:
        //Funciones auxiliares
        double valor_corriente(int t) const;
        double valor_plazo(int t) const;
    public:
        //Constructor general
        Cuenta(Tipo t, double ci, double in, int p=0);
        //Constructor para cuenta CORRIENTE
        Cuenta(double ci, double in);
        //Constructor para cuenta de PLAZO FIJO
        Cuenta(double ci, double in, int p);
        
        double valor(int t) const;
        double tae() const;
};