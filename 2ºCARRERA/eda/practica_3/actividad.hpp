//-------------------------------------------------------------------------------------------
// File:   actividad.hpp
// Author: Carlos Solana Melero (872815) -- Diego Mateo Lorente (873338)
// Date:   14-11-2023
//-------------------------------------------------------------------------------------------

#ifndef ACTIVIDAD_HPP   
#define ACTIVIDAD_HPP

#include <iostream>

using namespace std;

    // PREDECLARACIÓN DEL TAD actividad
    /*Los valores del TAD representan actividades, tales que tendrán
    un nombre, una descripción y una cantidad de minutos que se estima será necesario dedicar para
    su realización.
    */
    struct actividad;

    /*Devuelve una tarea con el nombre y descripción dados, y con el número indicado tEstimado como
    * el número de minutos estimado a dedicar a la actividad.
    */
    void crearActividad(string nombre, string descripcion, double tEstimado, actividad& t);

    //Devuelve el nombre de la actividad t.
    string obtenerNombre(const actividad& t);

    //Devuelve la descripción de la actividad t.
    string obtenerDescripcion(const actividad& t);

    //Devuelve el número de minutos estimado para la realización de la actividad t.
    double obtenerTiempoEstimado(const actividad& t);

    // DECLARACIÓN DEL TAD actividad

    struct actividad {
        friend void crearActividad(string nombre, string descripcion, double tEstimado, actividad& t);
        friend string obtenerNombre(const actividad& t);
        friend string obtenerDescripcion(const actividad& t);
        friend double obtenerTiempoEstimado(const actividad& t);
        private:
            string nombre;
            string descripcion;
            double tEstimado;
    };
    
#endif
