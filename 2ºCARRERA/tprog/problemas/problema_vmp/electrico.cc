/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#include "electrico.h"

using namespace std;

ElectricVmp::ElectricVmp(bool cargando, int carga = 0)
        :cargando(cargando),carga(carga)
{}
bool ElectricVmp::_cargando() {
        return cargando;
}
int ElectricVmp::_carga() {
        return carga;
}

void ElectricVmp::mod_cargando(bool enchufado) {
        cargando = enchufado;
}

void ElectricVmp::mod_carga(int carga_restante) {
        carga = carga_restante;
}