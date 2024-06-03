#include <iostream>
#include "agenda.hpp"

/* Devuelve una agenda vacía en a, sin contactos.
*/
void iniciar(agenda& a) {
    if(vacia(a));
    else {
        a.total = 0;
    }
}

/* Si la agenda a no está llena (numero de contactos almacenados menor que MAX),
   la función devuelve verdad y añade el contacto c a la agenda a.
   Si la agenda está llena, la función devuelve falso y la agenda a no se modifica.
*/
bool anyadir(agenda& a, const contacto& c) {
    if(a.total == MAX) {
        return false;
    }
    else {
        //nombre(a.datos[a.total]) = nombre(c);
        //direccion(a.datos[a.total]) = direccion(c);
        //telefono(a.datos[a.total]) = telefono(c);
        a.datos[a.total] = c;
        a.total++;
        return true;
    }
}

/* Devuelve verdad si y sólo si la agenda a está vacía. 
*/
bool vacia(const agenda& a) {
    return a.total == 0;
}

/* Si a no está vacía, la función devuelve verdad y de la agenda a se elimina el último contacto 
   añadido a ella. Si a está vacía, la función devuelve falso y la agenda no se modifica.
*/
bool borrarUltimo(agenda& a) {
    if(vacia(a)) {
        return false;
    }
    else {
        a.total--;
    }
    return true;
}

/* Dada una agenda a y un contacto c, devuelve verdad si y sólo si en 
   a hay algún contacto igual a c (en el sentido de la función == del TAD contacto), 
   falso en caso contrario.
 */
bool esta(const agenda& a, const contacto& c) {
    if(vacia(a)) {
        return false;
    }
    for(unsigned i = 0; i < a.total; i++) {
        if(operator==(a.datos[i], c)) {
            return true;
        }
    }
    return false;
}

/* Se pone el puntero al inicio de la agenda a 
*/
void iniciarIterador(agenda& a) {
    a.puntero = 0;
}

/* La funcion devuelve true si y solo el puntero de a está en el intervalo [0-total]
*/
bool haySiguiente(agenda& a) {
    return a.puntero < a.total;
}

/* Si la función devuelve true, se habrá cargado en c el nombre dirección y teléfono
*  del contacto ubicado en el puntero de la agenda a y se cumple que existe un siguiente elemento en la agenda.
*/
bool siguiente(agenda& a, contacto& c) {
    //nombre(c) = nombre(a.datos[a.puntero]);
    //direccion(c) = direccion(a.datos[a.puntero]);
    //telefono(c) = telefono(a.datos[a.puntero]);
    if(haySiguiente(a)){
        c = a.datos[a.puntero];
        a.puntero++;
        return true;
    }
    return false;
}
