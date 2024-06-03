/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#include "vmp.h"
#include <cstdlib>
#include <cmath>

Vmp::Vmp(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0)
        :id(id),esta_en_base(esta_en_base),dispo(dispo),x(x),y(y)
{}

double Vmp::distancia(){
    double loc_x = abs(x);
    double loc_y = abs(y);
    return(pow(pow(loc_x,2)+pow(loc_y,2),1/2));
}
void Vmp::alquilar(){
    if(dispo) {
        dispo = false;
        esta_en_base = false;
    }
}
void Vmp::devolver(){
    if(!dispo) {
        dispo = true;
        esta_en_base = true;
    }
}
void Vmp::liberar(){
    dispo = true;
}
void Vmp::bloquear(){
    dispo = false;
}

int Vmp::identificador(){
    return id;
}
double Vmp::coord_x(){
    return x;
}
double Vmp::coord_y(){
    return y;
}
bool Vmp::enBase(){
    return esta_en_base;;
}
bool Vmp::disponible(){
    return dispo;
}
void Vmp::mod_identificador(int identificador){
    id = identificador;
}
void Vmp::mod_coord_x(double coord_x){
    x = coord_x;
}
void Vmp::mod_coord_y(double coord_y){
    y = coord_y;
}
void Vmp::mod_enBase(bool enBase){
    esta_en_base = enBase;
}
void Vmp::mod_disponible(bool disponible){
    dispo = disponible;
}