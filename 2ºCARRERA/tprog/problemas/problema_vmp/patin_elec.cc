/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#include "patin_elec.h"

using namespace std;

PatinElec::PatinElec(int id, bool esta_en_base, bool dispo, bool cargando, int carga = 0, double x = 0, double y = 0)
        :Patin(id, esta_en_base, dispo, x, y), ElectricVmp(carga, cargando)
{}

void PatinElec::alquilar() {
        if(dispo && carga !=0) {
                dispo = false;
                esta_en_base = false;
                cargando = false;
        }
}
void PatinElec::devolver(){
        if(!dispo) {
                dispo = true;
                esta_en_base = true;
                cargando = true;
        }
}

void PatinElec::liberar(){
        dispo = true;
        cargando = true;
}