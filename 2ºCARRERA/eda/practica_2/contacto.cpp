#include<iostream>
#include "contacto.hpp"
using namespace std;


/* Dada una cadena nombre, una cadena direccion y un entero telefono, 
devuelve un contacto c con esos datos. 
*/
void crear(string nombre, string direccion, int telefono, contacto& c) {
    c.nombre = nombre;
    c.direccion = direccion;
    c.telefono = telefono;
}

/* Dado un contacto c, devuelve la cadena correspondiente al nombre de c. 
*/
string nombre(const contacto& c) {
    return c.nombre;
}

/* Dado un contacto c, devuelve la cadena correspondiente a la direccion de c. 
*/
string direccion(const contacto& c) {
    return c.direccion;
}

/* Dado un contacto c, devuelve el entero correspondiente al telefono de c. 
*/
int telefono(const contacto& c) {
    return c.telefono;
}
/* Devuelve verdad si y sólo si los contactos c1 y c2 tienen el mismo nombre. 
*/
bool operator==(const contacto& c1, const contacto& c2) {
    return c1.nombre == c2.nombre;
}