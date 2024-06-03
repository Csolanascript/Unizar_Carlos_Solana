#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
int main (){
    unsigned grados, minutos, segundos;
    double radianes, seno, coseno, tangente, gradosTot;
    cout << "Escriba el valor de un angulo (grados, minutos y segundos): ";
    cin >> grados >> minutos >> segundos;
    gradosTot = grados + minutos/60.0 + segundos/3600.0;
    radianes = gradosTot * 3.1416 /180; 
    seno = sin(radianes);
    coseno = cos(radianes);
    tangente = tan(radianes);

    cout << "Valor del angulo en radianes: " << radianes << " radianes." << endl;
    cout << "sen " << radianes << " = " << seno << endl;
    cout << "cos " << radianes << " = " << coseno << endl;
    cout << "tg " << radianes << " = " << tangente << endl;
}
