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
template<typename segm,typename val> bool aniadir(coleccionSeg<segm,val>&c, const segm& s, const val& v);

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





// DECLARACIÓN DEL TAD GENÉRICO coleccionSeg
template<typename segm,typename val>
struct coleccionSeg {
    friend void crear<segm,val>(coleccionSeg<segm,val>&c);
    friend double tamagno<segm,val>(coleccionSeg<segm,val>&c);
    friend bool esVacia<segm,val>(coleccionSeg<segm,val>&c);
    friend bool aniadir<segm,val>(coleccionSeg<segm,val>&c, const segm& s, const val& v);
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
        struct nodo {
            val valor;
            segm segmento;
            nodo* sig;
        };
        nodo* primero;
        int tamanio;
        nodo* puntero;
};

// IMPLEMENTACIÓN DE LAS OPERACIONES DEL TAD GENÉRICO coleccionSeg

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
template<typename segm,typename val> bool aniadir(coleccionSeg<segm,val>&c, const segm& s, const val& v) {
    typename coleccionSeg<segm,val>::nodo* aux = c.primero;
    if(!seSolapa(c, s)) {
        typename coleccionSeg<segm,val>::nodo* nuevoDato = new typename coleccionSeg<segm,val>::nodo;
        nuevoDato->valor = v;
        nuevoDato->segmento = s;
        if(esVacia(c)) {
            c.primero = nuevoDato;
            c.primero->sig = nullptr;
        }
        else if(anterior(s,c.primero->segmento)) {
            c.primero = nuevoDato;
            c.primero->sig = aux;
        }
        else {
            while(aux->sig != nullptr && !anterior(s,aux->sig->segmento)) {   
                aux = aux->sig;
            }
            if(aux->sig = nullptr) {
                aux->sig = nuevoDato;
                nuevoDato->sig = nullptr;
            }
            else {
                typename coleccionSeg<segm,val>::nodo* aux2 = aux->sig;
                aux->sig = nuevoDato;
                nuevoDato->sig = aux2;
            }   
        }
        c.tamanio++;
        return true;
    }
    return false;
}

//Si en c hay algún elemento con segm igual a s, sea (s, v’), devuelve una colección igual a la
//resultante de sustituir dicho elemento por (s, v) en la colección c, no definida si en c no hay ningún elemento con segm igual a s
//Parcial: no definida si en c no hay ningún elemento con segm igual a s
template<typename segm,typename val> bool actualizar(coleccionSeg<segm,val>&c, const segm& s, const val& v) {
    typename coleccionSeg<segm,val>::nodo* aux = c.primero;
    if(!esta(c,s)) {
        return false;
    }
    else {
        while(!iguales(s, aux->segmento)) {
            aux = aux->sig;
        }
        aux->valor = v;
        return true;
    }
}

//Devuelve verdad si y sólo si en c hay algún elemento con segm igual a s
template<typename segm,typename val> bool esta(coleccionSeg<segm,val>&c, const segm& s) {
    if(esVacia(c)) {
        return false;
    }
    else {
        typename coleccionSeg<segm,val>::nodo* aux = c.primero;
        while(aux->sig != nullptr) {
            if(iguales(aux->segmento, s)) {
                return true;
            }
            aux = aux->sig;
        }
        if(iguales(aux->segmento, s)) {
            return true;
        }
        return false;
    }
}

//Si en c hay algún elemento con segm igual a s, devuelve el dato val que forma el elemento con
//dicho s.
//Parcial: no definida si en c no hay ningún elemento con segm igual a s.
template<typename segm,typename val> bool obtener(coleccionSeg<segm,val>&c, const segm& s,val &v){
    if(esVacia(c)) {
        return false;
    }
    else {
        typename coleccionSeg<segm,val>::nodo* aux = c.primero;
        while(aux->sig != nullptr) {
            if(iguales(aux->segmento, s)) {
                v = aux->valor;
                return true;
            }
            aux = aux->sig;
        }
        if(iguales(aux->segmento, s)) {
            v = aux->valor;
            return true;
        }
        return false;
    }
}

//Devuelve verdad si y sólo si en c hay algún elemento con un segm que se solape con s.
template<typename segm,typename val> bool seSolapa(coleccionSeg<segm,val>&c, const segm& s) {
    if(esVacia(c)) {
        return false;
    }
    else {
        typename coleccionSeg<segm,val>::nodo* aux = c.primero;
        while(aux->sig != nullptr) {
            if(solapados(aux->segmento,s)) {
                return true;
            }
            aux = aux->sig;
        }
        if(solapados(aux->segmento, s)) {
            return true;
        }
        return false;
    }
}

//Si en c hay algún elemento con segm igual a s, devuelve una colección igual a la resultante de
//eliminar ese elemento de la colección c. En caso contrario, devuelve una colección igual a c.}
template<typename segm,typename val> bool borrar(coleccionSeg<segm,val>&c, const segm& s){
    typename coleccionSeg<segm,val>::nodo* aux = c.primero;
    typename coleccionSeg<segm,val>::nodo* borrar;
    if(esVacia(c)) {
        return false;
    }
    else {
        if(iguales(c.primero->segmento,s) || anterior(c.primero->segmento,s)) {
            if(iguales(c.primero->segmento,s)) {
                c.primero = c.primero->sig;
                delete aux;
                c.tamanio--;
                return true;
            }
            else {
                while(aux->sig !=nullptr) {
                    if(iguales(aux->sig->segmento,s)) {
                        borrar = aux->sig;
                        aux->sig = aux->sig->sig;
                        delete borrar;
                        c.tamanio--;
                        return true;
                    }
                    else if(anterior(s,aux->sig->segmento)) {
                        return false;
                    }
                    aux = aux->sig;
                }
                return false;
            }
        }
        return false;
    }
}

//Inicializa el iterador para recorrer los elementos de la colección c, de forma que el siguiente
//elemento a visitar sea el que tiene un segm anterior a los de todos los demás elementos de la
//colección (situación de no haber visitado ningún elemento).
template<typename segm,typename val> void iniciarIterador(coleccionSeg<segm,val>&c){
    c.puntero = c.primero; //Solo esto :) :) :) ):
}

//{Devuelve verdad si queda algún elemento por visitar con el iterador de la colección c, devuelve
//falso si ya se ha visitado el último elemento
template<typename segm,typename val> bool haySiguiente(coleccionSeg<segm,val>&c){
    return c.puntero != nullptr; //ASI  vale ):
}


//Devuelve el segm del siguiente elemento a visitar con el iterador de la colección c, que será el
//elemento no visitando con segm anterior a los de todos los demás aún no visitados.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool siguienteSegm(coleccionSeg<segm,val>&c, segm& s){
    if(haySiguiente(c)) {
        s = c.puntero->segmento;
        return true;        
    }
    return false;
    //pero es solo para el segmento no ? cierto supongo que en avanza será c = c->siguiente y c.puntero++
}
 //calma diego ya lo tengo dame 5 mins BIEN

//Devuelve el val del siguiente elemento a visitar con el iterador de la colección c, que será el
//elemento no visitando con segm anterior a los de todos los demás aún no visitados.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool siguienteVal(coleccionSeg<segm,val>&c, val& v){
    if(haySiguiente(c)) {
        v = c.puntero->valor;
        return true;        
    }
    return false;
}

//Avanza el iterador de la colección c para que se pueda visitar otro elemento.
//Parcial: la operación no está definida si no quedan elementos por visitar (no haySiguiente?(c)).
template<typename segm,typename val> bool avanza(coleccionSeg<segm,val>&c) {
    if(haySiguiente(c)){
        c.puntero = c.puntero->sig;
        return true;
    }
    return false;
}
#endif




