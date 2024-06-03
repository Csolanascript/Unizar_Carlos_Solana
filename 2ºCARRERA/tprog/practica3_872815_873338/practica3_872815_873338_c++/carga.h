#pragma once
#include "elemento.h"

class Carga : public Elemento {
    public:
        Carga(const string& nom, double vol, double pes)
                :Elemento(nom,vol,pes)
        {}
};