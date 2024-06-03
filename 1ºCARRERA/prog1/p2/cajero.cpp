#include <iostream>
#include <iomanip>
using namespace std;
/* programa que pide una cantidad de dinero */
int main(){
    
    unsigned billetes50=0;
    unsigned billetes20=0; 
    unsigned billetes10=0;
    unsigned dinero=0;
    
    cout << "Este cajero solo dispone de billetes de 10 ,20 y 50 euros, se entregara el mayor billete posible"<< endl;
    cout << "Cantidad a retirar en euros [positiva y multiplo de 10]: ";
    cin>>dinero;
    billetes50 = dinero / 50;
    dinero = dinero % 50;
    billetes20 = dinero / 20;
    dinero = dinero % 20;
    billetes10 = dinero / 10;
    dinero = dinero % 10;
    cout << "Billetes" << setw(8)<< "Euros" << endl;
    cout << "========"<< setw(8) << "=====" << endl;
    cout << setw(5) << billetes10 << setw(10) << "10 "<< endl;
    cout << setw(5) << billetes20 << setw(10) << "20 "<< endl;
    cout << setw(5) << billetes50 << setw(10) << "50 "<< endl;
}