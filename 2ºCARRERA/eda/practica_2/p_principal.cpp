#include <iostream>
#include <iomanip>
#include "contacto.hpp"
#include "agrupacion.hpp"
using namespace std;


void escribirCabecera() {
    cout << left << setw(13) << "Nombre" << setw(14) << "Direccion" << setw(11) << "Telefono" << endl;
    for(unsigned i = 0; i < 36 ; i++) {
        cout << "-";
    }
    cout << endl;
}

void escribir(const contacto& c) {
    cout << left << setw(13) << nombre(c) << setw(14) << direccion(c) << setw(11) << telefono(c) << endl;
}

int main() {
    agrupacion<contacto> a;
    contacto ejemplos[5]; //{("Antonio", "C/Rodrigo", 678678678),
                           // ("Manolo", "C/Huelva", 632458901),
                            //("Nicolás", "C/Cierzo", 654789903),
                            //("Pedro", "C/Madrid", 987654321),
                            //("Luis", "C/Barcelona", 123456789)};
    string nombres[5] = {"Antonio", "Manolo", "Nicolás", "Pedro", "Luis"};
    string direcciones[5]={"C/Rodrigo","C/Huelva","C/Cierzo","C/Madrid","C/Barcelona"};
    int telefonos[5] = {678678678,632458901,654789903,987654321,123456789};
    for(unsigned i = 0; i < 5; i++) {
        crear(nombres[i], direcciones[i], telefonos[i], ejemplos[i]);
    }
    contacto c;
    iniciar(a);
    cout << "Mostramos 5 contactos:\n";
    escribirCabecera();
    for(unsigned i = 0; i < 5; i++) {
        anyadir(a,ejemplos[i]);
    }
    iniciarIterador(a);
    if(!vacia(a)) {
        while(siguiente_avanza(a,c)) {
            escribir(c);
        }
    }
    cout << "\n\nBorramos el último contacto:\n";
    if(borrarUltimo(a)) {
        escribirCabecera();
        iniciarIterador(a);
        if(!vacia(a)) {
            while(siguiente_avanza(a,c)) {
                escribir(c);
            }
        }      
    }
    else cout << "Agenda vacia, asi que no hay nada que borrar\n";
    cout <<"Introduce un contacto y lo buscaremos: ";
    string nombre;
    string direccion;
    int telefono;
    contacto buscador;
    cin >> nombre >> direccion >> telefono;
    crear(nombre,direccion,telefono,buscador);
    if(esta(a,buscador)) {
        cout << "Si que esta\n";
    }
    else cout << "No esta\n";
    cout << "FIN\n";
    return 0;
}