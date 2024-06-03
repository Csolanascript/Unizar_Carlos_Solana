//-------------------------------------------------------------------------------------------
// File:   coleccionSeg.hpp
// Author: Carlos Solana Melero (872815) -- Diego Mateo Lorente (873338)
// Date:   14-11-2023
//-------------------------------------------------------------------------------------------

// El tipo T requerirá tener definida unas funciones:
// igual: segm s1, segm s2  booleano {devuelve verdad si y solo si s1 es igual que s2.}
// anterior: segm s1, segm s2  booleano {devuelve verdad si y solo si s1 es anterior a s2.}
//solapados: segm s1, segm s2  booleano {devuelve verdad si y solo si s1 se solapa con s2.}
#ifndef COLECCIONSEG_HPP   
#define COLECCIONSEG_HPP

#include <iostream>

using namespace std;

// PREDECLARACIÓN DEL TAD GENÉRICO coleccionSeg

//Los valores del TAD representan colecciones de elementos formados como parejas
//(segm, val) tales que, en la colección no se permiten elementos con segm iguales o solapados
template<typename segm,typename val> struct coleccionSeg;

//Crea una colección vacía, sin elementos.
template<typename segm,typename val> void crear(coleccionSeg<segm,val>&c);

//Devuelve el número de elementos que hay en la colección c.
template<typename segm,typename val> double tamagno(coleccionSeg<segm,val>&c);

//Devuelve verdad si y sólo si c no contiene ningún elemento
template<typename segm,typename val> bool esVacia(coleccionSeg<segm,val>&c);

//Si en c no hay ningún elemento con segm igual o solapado con s, devuelve una colección igual a
//la resultante de añadir el elemento (s, v) a la colección c. En caso contrario, devuelve una
//colección igual a c.
template<typename segm,typename val> void aniadir(coleccionSeg<segm,val>&c, const segm& s, const val& v);

//Si en c hay algún elemento con segm igual a s, sea (s, v’), devuelve una colección igual a la
//resultante de sustituir dicho elemento por (s, v) en la colección c, no definida si en c no hay ningún elemento con segm igual a s
template<typename segm,typename val> bool actualizar(coleccionSeg<segm,val>&c, const segm& s, const val& v);

//Si en c hay algún elemento con segm igual a s, sea (s, v’), devuelve una colección igual a la
//resultante de sustituir dicho elemento por (s, v) en la colección c, no definida si en c no hay ningún elemento con segm igual a s
template<typename segm,typename val> bool esta(coleccionSeg<segm,val>&c, const segm& s);

//Devuelve verdad si y sólo si en c hay algún elemento con segm igual a s
template<typename segm,typename val> bool obtener(coleccionSeg<segm,val>&c, const segm& s,val& v);

//Devuelve verdad si y sólo si en c hay algún elemento con un segm que se solape con s.
template<typename segm,typename val> bool seSolapa(coleccionSeg<segm,val>&c, const segm& s);

//Si en c hay algún elemento con segm igual a s, devuelve una colección igual a la resultante de
//eliminar ese elemento de la colección c. En caso contrario, devuelve una colección igual a c.}
template<typename segm,typename val> bool  borrar(coleccionSeg<segm,val>&c, const segm& s);

//Inicializa el iterador para recorrer los elementos de la colección c, de forma que el siguiente
//elemento a visitar sea el que tiene un segm anterior a los de todos los demás elementos de la
//colección (situación de no haber visitado ningún elemento).
template<typename segm,typename val> void iniciarIterador(coleccionSeg<segm,val>&c);

//{Devuelve verdad si queda algún elemento por visitar con el iterador de la colección c, devuelve
//falso si ya se ha visitado el último elemento
template<typename segm,typename val> bool haySiguiente(coleccionSeg<segm,val>&c);

//Devuelve el segm del siguiente elemento a visitar con el iterador de la colección c, que será el
//elemento no visitando con segm anterior a los de todos los demás aún no visitados.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool siguienteSegm(coleccionSeg<segm,val>&c, segm& s);

//Devuelve el val del siguiente elemento a visitar con el iterador de la colección c, que será el
//elemento no visitando con segm anterior a los de todos los demás aún no visitados.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool siguienteVal(coleccionSeg<segm,val>&c, val& v);
//Avanza el iterador de la colección c para que se pueda visitar otro elemento.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool avanza(coleccionSeg<segm,val>&c);

// PREDECLARACIÓN DE FUNCIONES AUXILIARES DEL TAD GENÉRICO coleccionSeg

//Devuelve verdad si y sólo si en la rama abb hay algun elemento cuyo segmento solape con s
template<typename segm,typename val> bool seSolapa_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s);

template<typename segm,typename val> void aniadir_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s, const val& v);


template<typename segm,typename val> bool actualizar_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s, const val& v);

template<typename segm,typename val> bool esta_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s);

template<typename segm,typename val> bool obtener_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s, val& v);




// DECLARACIÓN DEL TAD GENÉRICO coleccionSeg
template<typename segm,typename val>
struct coleccionSeg {
    friend void crear<segm,val>(coleccionSeg<segm,val>&c);
    friend double tamagno<segm,val>(coleccionSeg<segm,val>&c);
    friend bool esVacia<segm,val>(coleccionSeg<segm,val>&c);
    friend void aniadir<segm,val>(coleccionSeg<segm,val>&c, const segm& s, const val& v);
    friend bool actualizar<segm,val>(coleccionSeg<segm,val>&c, const segm& s, const val& v);
    friend bool esta<segm,val>(coleccionSeg<segm,val>&c, const segm& s);
    friend bool obtener<segm,val>(coleccionSeg<segm,val>&c, const segm&,val& v);
    friend bool seSolapa<segm,val>(coleccionSeg<segm,val>&c, const segm& s);
    friend bool borrar<segm,val>(coleccionSeg<segm,val>&c, const segm& s);
    friend void iniciarIterador<segm,val>(coleccionSeg<segm,val>&c);
    friend bool haySiguiente<segm,val>(coleccionSeg<segm,val>&c);
    friend bool siguienteSegm<segm,val>(coleccionSeg<segm,val>&c, segm& s);
    friend bool siguienteVal<segm,val>(coleccionSeg<segm,val>&c, val& v);
    friend bool avanza<segm,val>(coleccionSeg<segm,val>&c);
    private:
        struct abb {
            val valor;
            segm segmento;
            abb* izquierda;
            abb* derecha;

        };
        abb* raiz;
        int tamanio;
        Pila<*abb> puntero;
};

// IMPLEMENTACIÓN DE LAS OPERACIONES DEL TAD GENÉRICO coleccionSegArbolBinario.hpp

//Crea una colección vacía, sin elementos.
template<typename segm,typename val> void crear(coleccionSeg<segm,val>&c) {
    c.tamanio = 0;
}

//Devuelve el número de elementos que hay en la colección c.
template<typename segm,typename val> double tamagno(coleccionSeg<segm,val>&c) {
    return c.tamanio;
}

//Devuelve verdad si y sólo si c no contiene ningún elemento
template<typename segm,typename val> bool esVacia(coleccionSeg<segm,val>&c) {
    return c.tamanio == 0;
}

//Si en c no hay ningún elemento con segm igual o solapado con s, devuelve una colección igual a
//la resultante de añadir el elemento (s, v) a la colección c. En caso contrario, devuelve una
//colección igual a c.
template<typename segm,typename val> void aniadir(coleccionSeg<segm,val>&c, const segm& s, const val& v) {
    bool aniadido = aniadir_REC(c.raiz,s,v);
    if(aniadido) {
        c.tamanio++;
    }
    return aniadido;
}

template<typename segm,typename val> bool aniadir_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s, const val& v) {
    if(a == nullptr) {
        typename coleccionSeg<segm,val>::abb* nuevoDato = new typename coleccionSeg<segm,val>::abb;
        nuevoDato->valor = v;
        nuevoDato->segmento = s;
        a = nuevoDato;
        a->izquierda = nullptr;
        a->derecha = nullptr;
        return true
    }
    else if(!solapados(a->segmento,s)){
        if(anterior(s,a->segmento)){
            if(!seSolapa_REC(a->derecha,s)) {// comprueba solapes por la derecha
                return aniadir_REC(a->izquierda,s,v,tamanio);
            }
        }
        else {
            if(!seSolapa_REC(a->izquierda,s)) {// comprueba solapes por la izquierda
                return aniadir_REC(a->derecha,s,v,tamanio);
            }
        }
    }
    return false;
}

//Si en c hay algún elemento con segm igual a s, sea (s, v’), devuelve una colección igual a la
//resultante de sustituir dicho elemento por (s, v) en la colección c, no definida si en c no hay ningún elemento con segm igual a s
//Parcial: no definida si en c no hay ningún elemento con segm igual a s
template<typename segm,typename val> bool actualizar(coleccionSeg<segm,val>&c, const segm& s, const val& v) {
    return actualizar_REC(c.raiz,s,v);
}

template<typename segm,typename val> bool actualizar_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s, const val& v){
    if(a == nullptr) {
        return false;
    }
    if(iguales(s,a->segmento)) {
        a->valor = v;
        return true;
    }
    if(anterior(s,a->segmento)){
        return actualizar_REC(a->izquierda,s,v);
    }
    else {
        return actualizar_REC(a->derecha,s,v);
    }
}

//Devuelve verdad si y sólo si en c hay algún elemento con segm igual a s
template<typename segm,typename val> bool esta(coleccionSeg<segm,val>&c, const segm& s) {
    return esta_REC(c.raiz,s);
}

template<typename segm,typename val> bool esta_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s){
    if(a == nullptr) {
        return false;
    }
    if(iguales(s,a->segmento)) {
        return true;
    }
    if(anterior(s,a->segmento)){
        return esta_REC(a->izquierda,s,v);
    }
    else {
        return esta_REC(a->derecha,s,v);
    }
}

//Si en c hay algún elemento con segm igual a s, devuelve el dato val que forma el elemento con
//dicho s.
//Parcial: no definida si en c no hay ningún elemento con segm igual a s.
template<typename segm,typename val> bool obtener(coleccionSeg<segm,val>&c, const segm& s,val &v){
    return obtener_REC(c.raiz,s,v);
}

template<typename segm,typename val> bool obtener_REC(typename coleccionSeg<segm,val>::abb* a, const segm& s, val& v) {
    if(a == nullptr) {
        return false;
    }
    if(iguales(s,a->segmento)) {
        v = a->valor;
        return true;
    }
    if(anterior(s,a->segmento)){
        return obtener_REC(a->izquierda,s,v);
    }
    else {
        return obtener_REC(a->derecha,s,v);
    }
}



//Pre:  Recibe una colección c, y un segmento s
//Post: Devuelve verdad si y sólo si en c hay algún elemento con un segm que se solape con s
template<typename segm, typename val>
bool seSolapa(coleccionSeg<segm,val>& c, const segm& s){
    return seSolapa_REC(c.raiz, s);
}

template<typename segm, typename val>
bool seSolapa_REC(typename coleccionSeg<segm,val>::nodo*& c, const segm& s){
    if(c == nullptr){
        return false;
    }
    else{
        if(solapados(c->segmento,s)){
            return true;
        }
        else{
            return (seSolapa_REC(c->izquierda, s) || seSolapa_REC(c->derecha, s));
        }
    }
}

template<typename segm, typename val>
bool borrarMax(typename coleccionSeg<segm,val>::nodo& c, const segm& s,const val& v){
    if(c->derecha == nullptr){
        s = c->segmento;
        v = c->valor;
        typename coleccionSeg<segm,val>::nodo aux;
        aux=c;
        c=c->izquierda;
        delete(aux);
        return true;
    }
    else{
        return borrarMax(c->der, s,v);
    }
}

template<typename segm, typename val>
bool borrar_REC(typename coleccionSeg<segm,val>::nodo& c, const segm& s){
    if (c != nullptr){
        if(iguales((c->segmento), s)){
            if(c->izquierda == nullptr){
                typename coleccionSeg<segm,val>::nodo aux=c;
                c=c->derecha;
                delete(aux);
                return true;
            }
            else{
                return borrarMax(c->izquierda, c->segmento,c->valor);
            }
        }

        if((!anterior(c->segmento), s)){
            return borrar_REC(c->derecha,s)
        }
        if(anterior((c->segmento), s)){
	        return borrar_REC(c->izquierda,s)
        }
    }
    return false
}


template<typename segm, typename val>
bool borrar(coleccionSeg<segm,val>& c, const segm& s){
    bool borrado = borrar_REC(c.raiz,s);
    if(borrado) {
        c.tamanio--;
    }
    return borrado;
}

//Inicializa el iterador para recorrer los elementos de la colección c, de forma que el siguiente
//elemento a visitar sea el que tiene un segm anterior a los de todos los demás elementos de la
//colección (situación de no haber visitado ningún elemento).
template<typename segm,typename val> void iniciarIterador(coleccionSeg<segm,val>&c){
    crearVacia(c.puntero);
    typename coleccionSeg<segm,val>::abb* aux = new typename coleccionSeg<segm,val>::abb;
    aux = c.raiz;
    while(aux != nullptr) {
        apilar(c.puntero,aux);
        aux = aux->izquierda;
    }
}

//{Devuelve verdad si queda algún elemento por visitar con el iterador de la colección c, devuelve
//falso si ya se ha visitado el último elemento
template<typename segm,typename val> bool haySiguiente(coleccionSeg<segm,val>&c){
    return !esVacia(c.puntero);
}


//Devuelve el segm del siguiente elemento a visitar con el iterador de la colección c, que será el
//elemento no visitando con segm anterior a los de todos los demás aún no visitados.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool siguienteSegm(coleccionSeg<segm,val>&c, segm& s){
    bool error;
    if(haySiguiente(c)) {
        typename coleccionSeg<segm,val>::abb* aux = new typename coleccionSeg<segm,val>::abb;
        cima(c.puntero,aux,error);
        if(error == false){
            s = aux->segmento;
            return true;
        }
    }
    return false;
}

//Devuelve el val del siguiente elemento a visitar con el iterador de la colección c, que será el
//elemento no visitando con segm anterior a los de todos los demás aún no visitados.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool siguienteVal(coleccionSeg<segm,val>&c, val& v){
    bool error;
    if(haySiguiente(c)) {
        typename coleccionSeg<segm,val>::abb* aux = new typename coleccionSeg<segm,val>::abb;
        cima(c.puntero,aux,error);
        if(error == false){
            s = aux->valor;
            return true;
        }
    }
    return false;
}

//Avanza el iterador de la colección c para que se pueda visitar otro elemento.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool avanza(coleccionSeg<segm,val>&c) {
    bool error;
    if(haySiguiente(c)) {
        typename coleccionSeg<segm,val>::abb* aux = new typename coleccionSeg<segm,val>::abb;
        cima(c.puntero,aux,error);
        if(error == false){
            desapilar(c.puntero);
            aux = aux->derecha;
            while(aux != nullptr) {
                apilar(c.puntero, aux);
                aux = aux->izquierda;
            }
            return true;
        }
    }
    return false;
}
#endif