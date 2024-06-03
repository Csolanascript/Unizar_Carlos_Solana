#include "almacen.h"
#include "carga.h"

template<typename T>
class Contenedor: public Carga, public Almacen<T>
{
    public:
        Contenedor(double cap)
                :Carga("Contenedor",cap,0.0),Almacen<T>(cap)
        {}

        double peso() const override {
            double pesoAux = 0.0;
            for(auto i : this->elem){
                pesoAux+= i->peso();
            }
            return pesoAux;
        }
};    