/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#pragma once
#include "vmp.h"

class Patin: public Vmp {
    public:
        Patin(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0);
};