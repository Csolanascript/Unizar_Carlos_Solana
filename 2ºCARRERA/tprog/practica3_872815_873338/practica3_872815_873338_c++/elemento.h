#pragma once
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

class Elemento
{
    public:
        Elemento(const string& nom, double vol, double pes)
                    :_nombre(nom),_volumen(vol),_peso(pes)
        {}

        string nombre() const {
            return _nombre;
        }
        double volumen() const {
            return _volumen;
        }
        virtual double peso() const {
            return _peso;
        }
        //virtual void mostrar(ostream& os,const string& sep) const;
        virtual string tipo() const {
            return " de Carga Estandar";
        }
    protected:
        string _nombre;
        double _volumen;
        double _peso;
};