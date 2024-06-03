/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#include "flota.h"

void Flota::anadirBici(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0, int pos){
    vehiculos[0][pos] = new Bici(id, esta_en_base, dispo, x, y);
    nBicis++;
}
void Flota::anadirBiciElec(int id, bool esta_en_base, bool dispo, bool cargando, int carga = 0, double x = 0, double y = 0, int pos){
    vehiculos[1][pos] = new BiciElec(id,esta_en_base, dispo, cargando, carga, x, y);
    nBicis++;
}
void Flota::anadirPatin(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0, int pos){
    vehiculos[2][pos] = new Patin(id, esta_en_base, dispo, x, y);
    nPatines++;
}
void Flota::anadirPatinElec(int id, bool esta_en_base, bool dispo, bool cargando, int carga = 0, double x = 0, double y = 0, int pos){
    vehiculos[3][pos] = new PatinElec(id,esta_en_base, dispo, cargando, carga, x, y);
    nPatines++
}

Vmp Flota::vehiculoMasLejano(){
    Vmp solucion = vehiculos[0][0];
    for(unsigned i = 0; i < 4; i++) {
        for(unsigned j = 0; j < 1000; j++) {
            if(solucion.distancia() < vehiculos[i][j].distancia()) {
                solucion = vehiculos[i][j];
            }
        }
    }
    return solucion;
}

void Flota::eliminarBici(int pos){
    delete vehiculos[0][pos];
    nBicis--;
}
void Flota::eliminarBiciElec(int pos){
    delete vehiculos[1][pos];
    nBicis--;
}
void Flota::eliminarPatin(int pos){
    delete vehiculos[2][pos];
    nPatines--;
}
void Flota::eliminarPatinElec(int pos){
    delete vehiculos[3][pos];
    nPatines--;
}

void Flota::vehiculosAbandonados(double r, Vmp solucion[]) {
    unsigned z = 0;
    for(unsigned i = 0; i < 4; i++) {
        for(unsigned j = 0; j < 1000; j++) {
            if(vehiculos[i][j].distancia() < r) {
                solucion[z] = vehiculos[i][j];
                z++;
                vehiculos[i][j].bloquear();
            }
        }
    }
}

void Flota::vehiculosDeMenorCarga(int cargaRequerida, Vmp solucion[]) {
    unsigned z = 0;
    for(unsigned i = 0; i < 1000; i++) {
        if((vehiculos[1][i]._carga() < cargaRequerida) && (vehiculos[1][i].enBase() == false) && (vehiculos[1][i].disponible() == false)) {
            solucion[z] = vehiculos[1][i];
            z++;
            vehiculos[1][i].bloquear();
        }
        if((vehiculos[3][i]._carga() < cargaRequerida) && (vehiculos[3][i].enBase() == false) && (vehiculos[3][i].disponible() == false)) {
            solucion[z] = vehiculos[3][i];
            z++;
            vehiculos[3][i].bloquear();
        }
    }
}

double Flota::porcentajePatinetes(){
    return (double((nPatines* 100)/ (nPatines + nBicis)));
}