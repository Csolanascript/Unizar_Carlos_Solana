#pragma once
#include "elemento.h"

class Toxico : public Elemento {
    public:
        Toxico(const string& nom, double vol, double pes)
            :Elemento(nom,vol,pes)
        {}

        string tipo() const override
        {
            return " de Productos Toxicos";
        }

};