#pragma once 

class Cuenta 
{
    protected:
        //Atributos (variables) de mi clase
        double capital;

    public:
        //Constructor general
        Cuenta(double ci);
        
        virtual double valor(int t) const;
        double tae() const;
};