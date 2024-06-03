#pragma once
#include "elemento.h"

class SerVivo : public Elemento {
    public:
        SerVivo(const string& nom, double vol, double pes)
                :Elemento(nom,vol,pes)
        {}
        
        string tipo() const override
        {
            return " de Seres Vivos";
        }

};