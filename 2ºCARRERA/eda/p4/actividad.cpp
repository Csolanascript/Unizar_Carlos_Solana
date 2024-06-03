//-------------------------------------------------------------------------------------------
// File:   actividad.cpp
// Author: Carlos Solana Melero (872815) -- Diego Mateo Lorente (873338)
// Date:   14-11-2023
//-------------------------------------------------------------------------------------------

#include <iostream>
#include "actividad.hpp"

using namespace std;

/*Devuelve una tarea con el nombre y descripción dados, y con el número indicado tEstimado como
* el número de minutos estimado a dedicar a la actividad.
*/
void crearActividad(string nombre, string descripcion, int tEstimado, actividad& t) {
    t.nombre = nombre;
    t.descripcion = descripcion;
    t.tEstimado = tEstimado;
}

//Devuelve el nombre de la actividad t.
string obtenerNombre(const actividad& t) {
    return t.nombre;
}

//Devuelve la descripción de la actividad t.
string obtenerDescripcion(const actividad& t) {
    return t.descripcion;
}

//Devuelve el número de minutos estimado para la realización de la actividad t.
int obtenerTiempoEstimado(const actividad& t) {
    return t.tEstimado;
}
