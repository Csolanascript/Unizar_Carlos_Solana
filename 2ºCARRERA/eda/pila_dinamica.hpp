#ifndef PILA_DINAMICA_HPP
#define PILA_DINAMICA_HPP

/* Módulo que implementa el TAD pila genérica en memoria dinámica.
   Autores: profs. de la asignatura. 2022-10-17 (versión 2). 
*/
   
using namespace std;

/* INICIO DE LA PARTE PÚBLICA o INTERFAZ  */

/* Los valores del TAD genérico Pila representan secuencias de elementos con acceso LIFO (last in, first out), 
   esto es, el último elemento añadido será el primero en ser borrado.
   El último elemento añadido en la pila es denominado la cima de la pila.
   Todas las operaciones tendrán coste constante, salvo las operaciones duplicar, ==, y liberar, que tendrán coste O(N) siendo N el número de elementos en la pila.
   Igualmente, el coste en memoria de esta implementación será O(N).
   
   Al parámetro formal T le exigimos estar dotado de los operadores habituales de: 
   comparación de igualdad == , y el operador habitual de copia =, que 
   se utilizarán respectivamente en las operaciones == de pilas y duplicar de pilas.
      bool operator==(const T& t1, const T& t2);
      T& operator=(const T& origen)
 */
template<typename T> struct Pila;


/* Devuelve en p la Pila vacía, sin elementos.
*/
template<typename T> void crearVacia(Pila<T>& p);


/* Añade e en p como nueva cima de la Pila p. 
*/
template<typename T> void apilar(Pila<T>& p, const T& e);


/* Si p es no vacía, devuelve p tras eliminar de ella el elemento que era cima de p (el último 
   elemento que fue apilado). Si p es vacía, la deja igual. 
*/
template<typename T> void desapilar(Pila<T>& p);


/* Si p es no vacía, devuelve en e el último elemento apilado en p (es decir, su cima), y devuelve error=false.
   Si p es vacía, devuelve error=true. 
*/
template<typename T> void cima(const Pila<T>& p, T& e, bool& error);


/* Devuelve true si y sólo si p no tiene elementos. 
*/
template<typename T> bool esVacia(const Pila<T>& p);


/* Devuelve el número de elementos que hay en p. 
*/
template<typename T> int altura(const Pila<T>& p);


/* Devuelve en pSal una copia profunda de la Pila pEnt, es decir duplicando la representación en memoria. 
Cada elemento de la pila será copiado utilizando el operador de copia = de los datos de tipo T.
*/
template<typename T> void duplicar(const Pila<T>& pilaEnt, Pila<T>& pilaSal);


/* Devuelve true si y sólo si p1 y p2 tienen los mismos elementos y en las mismas posiciones. 
Cada elemento de la pila será comparado utilizando el operador == de los datos de tipo T.
*/
template<typename T> bool operator==(const Pila<T>& p1, const Pila<T>& p2);


/* Libera toda la memoria dinámica utilizada por p y devuelve en p la Pila vacía. 
*/
template<typename T> void liberar(Pila<T>& p);


/* Las tres operaciones siguientes conforman un iterador interno para la Pila: 
*/

/* Prepara el iterador para que el siguiente elemento a visitar sea un primer 
   elemento de p, si existe (situación de no haber visitado ningún elemento). 
*/
template<typename T> void iniciarIterador(Pila<T>& p);


/* Devuelve false si ya se han visitado todos los elementos de p; devuelve true en caso contrario. 
*/
template<typename T> bool existeSiguiente(const Pila<T>& p);


/* Si existe algún elemento de p pendiente de visitar, devuelve en e el siguiente 
   elemento a visitar y error=false, y además avanza el iterador para que a 
   continuación se pueda visitar otro elemento de p. Si no quedan elementos pendientes 
   de visitar devuelve error=true 
*/
template<typename T> void siguiente(Pila<T>& p, T& e, bool& error);


/* FIN DE LA PARTE PÚBLICA o INTERFAZ  */

template<typename T>
struct Pila {
    friend void crearVacia<T>(Pila<T>& p);
    friend void apilar<T>(Pila<T>& p, const T& e);
    friend void desapilar<T>(Pila<T>& p);
    friend void cima<T>(const Pila<T>& p, T& e, bool& error);
    friend bool esVacia<T>(const Pila<T>& p);
    friend int altura<T>(const Pila<T>& p);
    friend void duplicar<T>(const Pila<T>& pilaEnt, Pila<T>& pilaSal);
    friend bool operator==<T>(const Pila<T>& p1, const Pila<T>& p2);
    friend void liberar<T>(Pila<T>& p);
    friend void iniciarIterador<T>(Pila<T>& p);
    friend bool existeSiguiente<T>(const Pila<T>& p);
    friend void siguiente<T>(Pila<T>& p, T& e, bool& error);
 private:
    /* En esta implementación la pila se representará en memoria dinámica como una lista enlazada de Nodos, simple y no circular, con: 
	    - el puntero cim: apuntando al Nodo que contiene el elemento que está en la cima de la pila, será nullptr si la pila está vacía
		- el campo alt: mantendrá calculada la altura o número de elementos en la pila, será 0 si la pila está vacía, y 
		- el puntero itr: se utilizará para mantener el estado del iterador, y únicamente será utilizado por las operaciones iniciarIterador, existeSiguiente y siguiente.
	   Cada Nodo contendrá un elemento de la pila (en su campo dato) y apuntará con su puntero sig: al Nodo que contenga el elemento que fue cima de la pila antes que 
      el suyo, o dicho puntero tendrá valor nullptr si antes la pila estaba vacía.	
	   
    Todas las operaciones tienen coste constante, salvo las operaciones duplicar, ==, y liberar, que tendrán coste O(N) siendo N el número de elementos en la pila. 
	Igualmente, el coste en memoria de esta implementación es de O(N).
	*/
	struct Nodo { // tipo de dato para cada nodo de la lista enlazada
		T dato;  
		Nodo* sig;			
	};

	Nodo* cim; // cima de la pila		
	int alt;   // altura de la pila
	Nodo* itr; // para implementar el iterador
};



/* Devuelve en p la Pila vacía, sin elementos.
*/
template<typename T> 
void crearVacia(Pila<T>& p) {
    p.cim = nullptr;
    p.alt = 0;
}



/* Añade e en p como nueva cima de la Pila p. 
*/
template<typename T> 
void apilar(Pila<T>& p, const T& e) {
    typename Pila<T>::Nodo* aux = p.cim; //guardar puntero a la anterior cima
    p.cim = new typename Pila<T>::Nodo;  // reservar memoria para el Nodo que contendrá la nueva cima
    //asignar el elemento de la nueva cima, encadenarla con la anterior cima, e incrementar en 1 la altura:
    p.cim->dato = e;
    p.cim->sig = aux;
    p.alt = p.alt + 1;
}


/* Si p es no vacía, devuelve p tras eliminar de ella el elemento que era cima de p (el último 
   elemento que fue apilado). Si p es vacía, la deja igual. 
*/
template<typename T> 
void desapilar(Pila<T>& p) {
    if (!esVacia(p)) { // si la pila no es vacía
        typename Pila<T>::Nodo* aux = p.cim; //puntero al Nodo con la cima a eliminar
        p.cim = p.cim->sig; // actualizar cim a la anterior cima de la pila
        delete aux;         //liberar la memoria ocupada por el Nodo a eliminar
		p.alt = p.alt - 1;  //decrementar la altura de la pila en 1
    }
}


/* Si p es no vacía, devuelve en e el último elemento apilado en p (es decir, su cima), y devuelve error=false.
   Si p es vacía, devuelve error=true. 
*/
template<typename T> 
void cima(const Pila<T>& p, T& e, bool& error) {
    if (esVacia(p)) {
		error = true;
    } else {
		error = false;
		e = p.cim->dato;
    }
}


/* Devuelve true si y sólo si p no tiene elementos, es decir la pila está vacía y p.cim es nullptr.
*/
template<typename T> 
bool esVacia(const Pila<T>& p) {
    return p.cim == nullptr;
}

/* Devuelve el número de elementos que hay en p. 
*/
template<typename T> 
int altura(const Pila<T>& p) {
    return p.alt;
}

/* Devuelve en pSal una copia profunda de la Pila pEnt, es decir duplicando la representación en memoria. 
Cada elemento de la pila será copiado utilizando el operador de copia = de los datos de tipo T.
*/
template<typename T> 
void duplicar(const Pila<T>& pilaEnt, Pila<T>& pilaSal){
	if (esVacia(pilaEnt)) {
		crearVacia(pilaSal);
	} else {
		typename Pila<T>::Nodo* ptEnt = pilaEnt.cim;  //puntero ptEnt apuntará en todo momento al Nodo de pilaEnt que se va a copiar 
		pilaSal.cim = new typename Pila<T>::Nodo;
		pilaSal.cim->dato = ptEnt->dato;              // se copia el elemento cima de la pilaEnt como cima de pilaSal
		typename Pila<T>::Nodo* ptSal = pilaSal.cim;  // puntero ptSal apuntará en todo momento al último Nodo de la lista enlazada que se está construyendo para pilaSal
		ptEnt = ptEnt->sig;
		while (ptEnt != nullptr) {    //Mientras no se hayan copiado los elementos de todos los Nodos de la lista de pilaEnt
			ptSal->sig = new typename Pila<T>::Nodo;   // crear y encadenar un nuevo nodo al final de la lista enlazada en construcción para pilaSal
			ptSal = ptSal->sig;
			ptSal->dato = ptEnt->dato;                 //copiar el dato para ese Nodo
			ptEnt = ptEnt->sig;
		}
		//Se marca el final de la lista enlazada construida para pilaSal, y se copia la altura de las pilas:
		ptSal->sig = nullptr;
		pilaSal.alt = pilaEnt.alt;
	}
}


/* Devuelve true si y sólo si p1 y p2 tienen los mismos elementos y en las mismas posiciones. 
Cada elemento de la pila será comparado utilizando el operador == de los datos de tipo T.
*/
template<typename T> 
bool operator==(const Pila<T>& p1, const Pila<T>& p2){
	if (p1.alt != p2.alt) { 
		return false;
	} else {    // Ambas pilas tienen el mismo número de elementos
		typename Pila<T>::Nodo* pt1 = p1.cim;  
		typename Pila<T>::Nodo* pt2 = p2.cim;   //pt1 y pt2 se utilizan para recorrer las listas enlazadas y comprobar elementos en la misma posición de p1 y p2 respectivamente
		bool iguales = true;
		while (pt1 != nullptr && iguales) {     //mientras no se haya encontrado una diferencia y aún queden elementos por comparar en las listas...
			iguales = (pt1->dato == pt2->dato); //comparamos los 2 elementos en la misma posición actual en ambas listas
			pt1 = pt1->sig;                    // se avanza la posición en ambas listas a la vez, por tanto se acabarán ambas a la vez (tienen el mismo número de elementos)
			pt2 = pt2->sig;
		}
		return iguales;
	}
}


/* Libera toda la memoria dinámica utilizada por p y devuelve en p la Pila vacía. 
*/
template<typename T> 
void liberar(Pila<T>& p) {
    typename Pila<T>::Nodo* aux = p.cim;
    while (aux != nullptr) {
        p.cim = p.cim->sig;
        delete aux;
        aux= p.cim;
    }
	p.alt = 0;
}


/* Las tres operaciones siguientes conforman un iterador interno para la Pila: 
Este iterador permitirá visitar los elementos almacenados en la pila empezando por el elemento que está en la cima y acabando con el primer elemento que se introdujo en la pila.
Si el puntero itr de la pila tiene valor nullptr entonces no existe elemento pendiente por visitar (ya se han visitado todos los elementos), 
en caso contrario el puntero apunta al Nodo cuyo dato es el siguiente elemento a visitar.
*/

/* Prepara el iterador para que el siguiente elemento a visitar sea un primer 
   elemento de p (es decir, el que está en la cima de p), si existe (situación de no haber visitado ningún elemento). 
*/
template<typename T> 
void iniciarIterador(Pila<T>& p) {
	p.itr = p.cim;
}


/* Devuelve false si ya se han visitado todos los elementos de p; devuelve true en caso contrario. 
*/
template<typename T> 
bool existeSiguiente(const Pila<T>& p) {
	return p.itr != nullptr;
}

/* Si existe algún elemento de p pendiente de visitar, devuelve en e el siguiente 
   elemento a visitar y error=false, y además avanza el iterador para que a 
   continuación se pueda visitar otro elemento de p. Si no quedan elementos pendientes 
   de visitar devuelve error=true 
*/
template<typename T> 
void siguiente(Pila<T>& p, T& e, bool& error) {
	if (existeSiguiente(p)) {
		error = false;
		e = p.itr->dato;     // 1o) devolver el elemento que toca visitar
		p.itr = p.itr->sig;  // 2o) avanzar el iterador para que está disponible el siguiente elemento 
	} else {
		error = true;
	}
}

#endif //PILA_DINAMICA_HPP