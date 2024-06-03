#pragma once
#include "almacen.h"
#include "elemento.h"
#include "carga.h"

//template<typename T>
class Camion:public Elemento, public Almacen<Carga>
{
    public:
        Camion(double cap)
                :Elemento("Camion", cap, 0.0), Almacen(cap)
        {}

        double peso() const override {
            double pesoAux = 0.0;
            for(auto i : elem){
                pesoAux+= i->peso();
            }
            return pesoAux;
        }

        //friend ostream& operator<<(ostream& os, Camion* item);
};  