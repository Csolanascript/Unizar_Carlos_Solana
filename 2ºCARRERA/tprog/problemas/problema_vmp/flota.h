/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#pragma once
#include "bici.h"
#include "electrico.h"
#include "patin.h"
#include "patin_elec.h"
#include "bici_elec.h"

class Flota{
    private:
    Vmp* vehiculos[1000];
    int nBicis;
    int nPatines;
    public:
    void anadirBici(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0, int pos);
    void anadirBiciElec(int id, bool esta_en_base, bool dispo, bool cargando, int carga = 0, double x = 0, double y = 0, int pos);
    void anadirPatin(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0, int pos);
    void anadirPatinElec(int id, bool esta_en_base, bool dispo, bool cargando, int carga = 0, double x = 0, double y = 0, int pos);
    void eliminarBici(int pos);
    void eliminarBiciElec(int pos);
    void eliminarPatin(int pos);
    void eliminarPatinElec(int pos);
    Vmp vehiculoMasLejano();
    void vehiculosAbandonados(double r, Vmp solucion[]);
    void vehiculosDeMenorCarga(int cargaRequerida, Vmp solucion[]);
    double porcentajePatinetes();
}