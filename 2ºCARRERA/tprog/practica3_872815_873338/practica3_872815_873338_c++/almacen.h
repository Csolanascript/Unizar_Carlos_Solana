#pragma once
#include "elemento.h"
#include <vector>

template<typename T>
class Almacen
{
    public:
        Almacen(double cap)
                :_capacidad(cap)
        {}
        bool guardar(T* tipo)
        {
            double volum = 0.0;
            for(auto i: elem) {
                volum+=i->volumen();
            }
            if((volum + tipo->volumen()) <= _capacidad){
                elem.push_back(tipo);
                return true;
            }
            return false;
        }
        double capacidad() const
        {
            return _capacidad;
        }

        /*void mostrar(ostream& os, const string& sep) const override{
            os << sep << _nombre << " [" << _capacidad << " m3] [" << PesoContenedor() << " kg]" << tipo() << "\n";
            for(auto i : elem){
                    i->mostrar(os,sep + "  ");
            }
        }*/
    protected:
        double _capacidad;
        vector<T*> elem;

};