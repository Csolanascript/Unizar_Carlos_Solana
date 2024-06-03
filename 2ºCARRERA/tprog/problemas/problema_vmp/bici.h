/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#pragma once
#include "vmp.h"

class Bici: public Vmp {
    public:
        Bici(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0);
};