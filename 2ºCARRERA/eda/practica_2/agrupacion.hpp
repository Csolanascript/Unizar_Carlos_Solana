/* Nombre, apellidos y NIP del (de los) autor(es).
*/

#ifndef AGRUPACION_HPP
#define AGRUPACION_HPP

// PREDECLARACIÓN DEL TAD GENÉRICO agrupacion

// El tipo T requerirá tener definida una función:
// bool operator== (const T& t1, const T& t2);
// {…devolverá true cuando…… devolverá false cuando…}

const int MAX = 40;

/* COMPLETAR CON LA ESPECIFICACIÓN DEL DOMINIO DE VALORES DEL TAD agrupacion
   Y CON LAS CONDICIONES QUE SE LE EXIGEN AL PARÁMETRO DE TIPO T.
*/
template<typename T> struct agrupacion;


/* COMPLETAR LA ESPECIFICACIÓN DE CADA OPERACIÓN DEL TAD
*/

/* Devuelve una agrupación a vacía
*/
template<typename T> void iniciar(agrupacion<T>& a);

/* Si la agrupación a no está vacía, la función devuelve true y se añade el elemento e a la agrupación a. 
*  En caso contrario, la función devuelve false.
*/
template<typename T> bool anyadir(agrupacion<T>& a, const T& e);

/* La función devuelve true si y solo si la agrupación a está vacia. En caso contrario, devolverá false.
*/
template<typename T> bool vacia(const agrupacion<T>& a);

/* La función devuelve true si y solo si la agrupación a no está vacia y 
*  se ha eliminado el último elemento añadido a la agrupación. Si a está vacía, la función devolverá false
*  y devolverá la lista sin ser modificada.
*/
template<typename T> bool borrarUltimo(agrupacion<T>& a);

/* La función devuelve true si y solo si en la agrupación a hay un elemento
*  igual a e ((en el sentido de la función operator== del parámetro formal elem).
*  En caso contrario, devuelve false.
*/
template<typename T> bool esta(const agrupacion<T>& a, const T& e);

//COMPLETAR OPERACIONES ITERADOR
//....

/* Se pone el puntero de la agrupación a a 0.
*/
template<typename T> void iniciarIterador(agrupacion<T>& a);

/* La función devuelve true si y solo si
*/
template<typename T> bool existeSiguiente(agrupacion<T>& a);

/* Si la función devuelve true, se habrá cargado en c los parámetros de la dirección del puntero en 
*  la agrupación a y se cumple que existe un siguiente elemento en la agrupación.
*/
template<typename T> bool siguiente_avanza(agrupacion<T>& a, T& e);

// DECLARACIÓN DEL TAD GENÉRICO agrupacion

template<typename T>
struct agrupacion{
    friend void iniciar<T>(agrupacion<T>& a);
    friend bool anyadir<T>(agrupacion<T>& a, const T& e);
    friend bool vacia<T>(const agrupacion<T>& a);
    friend bool borrarUltimo<T>(agrupacion<T>& a);
    friend bool esta<T>(const agrupacion<T>& a, const T& e);
    //COMPLETAR OPERACIONES ITERADOR
    //....
    friend void iniciarIterador<T>(agrupacion<T>& a);
    friend bool existeSiguiente<T>(agrupacion<T>& a);
    friend bool siguiente_avanza<T>(agrupacion<T>& a, T& e);
  private: 
     //campos
     T datos[MAX];
     int total;
    //COMPLETAR CAMPOS ITERADOR
    //...
    int puntero;
};


// IMPLEMENTACIÓN DE LAS OPERACIONES DEL TAD GENÉRICO agrupacion

template<typename T>
void iniciar(agrupacion<T>& a) {
    if(!vacia(a)) {
        a.total = 0;
    }
}

template<typename T>
bool anyadir(agrupacion<T>& a, const T& e) {
    if(a.total < MAX) {
        a.datos[a.total] = e;
        a.total++;
        return true;
    }
    return false;
}

template<typename T>
bool vacia(const agrupacion<T>& a) {
    return a.total == 0;
}
	
template<typename T>
bool borrarUltimo(agrupacion<T>& a) {
    if(vacia(a)) return false;
    a.total--;
    return true;
}

template<typename T>
bool esta(const agrupacion<T>& a, const T& e) {
    if(vacia(a)) return false;
    for(unsigned i = 0; i < a.total; i++) {
        if(e == a.datos[i]) {
            return true;
        }
    }
    return false;
}

//COMPLETAR IMPLEMENTACION OPERACIONES ITERADOR
//...

template<typename T> void iniciarIterador(agrupacion<T>& a){
    a.puntero = 0;
}

template<typename T> bool existeSiguiente(agrupacion<T>& a){
    return a.puntero < a.total;
}

template<typename T> bool siguiente_avanza(agrupacion<T>& a, T& e){
    if(existeSiguiente(a)) {
        e = a.datos[a.puntero];
        a.puntero ++;
        return true;
    }
    return false;
}

#endif //fin de agrupacion.hpp

