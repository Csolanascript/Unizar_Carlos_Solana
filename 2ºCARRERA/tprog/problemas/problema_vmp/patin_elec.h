/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#pragma once
#include "electrico.h"
#include "patin.h"

class PatinElec: public ElectricVmp {
    public:
        PatinElec(int id, bool esta_en_base, bool dispo, bool cargando, int carga = 0, double x = 0, double y = 0);
        void alquilar();
        void devolver();
        void liberar();
};