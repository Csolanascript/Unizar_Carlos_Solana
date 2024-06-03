/* Nombre, apellidos y NIP del (de los) autor(es).
*/

#ifndef AGENDA_HPP
#define AGENDA_HPP

#include "contacto.hpp"

// PREDECLARACIÓN DEL TAD agenda

const int MAX = 40;

/* Los valores del TAD agenda representan colecciones de contactos a las 
   que se pueden añadir elementos de tipo contacto, y de las que se pueden eliminar 
   sus contactos de uno en uno, eliminándose siempre el último contacto añadido de 
   todos los que contenga la agenda.
   Dado que la implementación es estática, se impone una máxima capacidad de la
   agenda igual al valor de la constante MAX.
 */
struct agenda;

/* Devuelve una agenda vacía en a, sin contactos.
*/
void iniciar(agenda& a);

/* Si la agenda a no está llena (numero de contactos almacenados menor que MAX),
   la función devuelve verdad y añade el contacto c a la agenda a.
   Si la agenda está llena, la función devuelve falso y la agenda a no se modifica.
*/
bool anyadir(agenda& a, const contacto& c);

/* Devuelve verdad si y sólo si la agenda a está vacía. 
*/
bool vacia(const agenda& a);

/* Si a no está vacía, la función devuelve verdad y de la agenda a se elimina el último contacto 
   añadido a ella. Si a está vacía, la función devuelve falso y la agenda no se modifica.
*/
bool borrarUltimo(agenda& a);

/* Dada una agenda a y un contacto c, devuelve verdad si y sólo si en 
   a hay algún contacto igual a c (en el sentido de la función == del TAD contacto), 
   falso en caso contrario.
 */
bool esta(const agenda& a, const contacto& c);

//COMPLETAR OPERACIONES ITERADOR

/* Se pone el puntero al inicio de la agenda a 
*/
void iniciarIterador(agenda& a);

/* La funcion devuelve true si y solo el puntero de a está en el intervalo [0-total]
*/
bool haySiguiente(agenda& a);

/* Si la función devuelve true, se habrá cargado en c el nombre dirección y teléfono
*  del contacto ubicado en el puntero de la agenda a y se cumple que existe un siguiente elemento en la agenda.
*/
bool siguiente(agenda& a, contacto& c);

// iniciar iterador,avanzar, hay siguiente?, 

// DECLARACIÓN DEL TAD agenda

struct agenda{
   friend void iniciar(agenda& a);
   friend bool anyadir(agenda& a, const contacto& c);
	friend bool vacia(const agenda& a);
   friend bool borrarUltimo(agenda& a);
	friend bool esta(const agenda& a, const contacto& c);
   //COMPLETAR OPERACIONES ITERADOR
   friend void iniciarIterador(agenda& a);
   friend bool haySiguiente(agenda& a);
   friend bool siguiente(agenda& a, contacto& c);
  
   private: 
      //campos
      contacto datos[MAX];
      int total;
      //COMPLETAR CAMPOS ITERADOR
      int puntero;
};

#endif //fin de agenda.hpp

