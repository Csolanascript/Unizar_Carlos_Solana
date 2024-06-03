#pragma once
#include "carga.h"

class Producto : public Carga {
    public:
        Producto(const string& nom, double vol, double pes)
                :Carga(nom,vol,pes)
        {}
};