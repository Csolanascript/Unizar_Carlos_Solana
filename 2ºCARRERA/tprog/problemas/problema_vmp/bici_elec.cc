/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#include "bici_elec.h"

using namespace std;

BiciElec::BiciElec(int id, bool esta_en_base, bool dispo, bool cargando, int carga = 0, double x = 0, double y = 0)
        :Bici(id,esta_en_base,dispo,x,y), ElectricVmp(carga, cargando) 
{}

void BiciElec::alquilar() {
        if(dispo && carga !=0) {
                dispo = false;
                esta_en_base = false;
                cargando = false;
        }
}
void BiciElec::devolver(){
        if(!dispo) {
                dispo = true;
                esta_en_base = true;
                cargando = true;
        }
}

void BiciElec::liberar(){
        dispo = true;
        cargando = true;
}