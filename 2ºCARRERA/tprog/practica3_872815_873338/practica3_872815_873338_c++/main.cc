#include "practica3.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	// Productos estandar

	Contenedor<Carga>* contenedor_estandar = new Contenedor<Carga>(4);

	Producto* longaniza_graus = new Producto("Longaniza de Graus",0.5,0.8);
	contenedor_estandar->guardar(longaniza_graus);

	Producto* adoquines_zgz = new Producto("Adoquines de Zaragoza",0.1,5.0);
	contenedor_estandar->guardar(adoquines_zgz);

	// Productos especiales
	Toxico* discos_melendi = new Toxico("Discos de Melendi",1,10);
	Contenedor<Toxico>*	contenedor_toxico = new Contenedor<Toxico>(3);
	contenedor_toxico->guardar(discos_melendi);
#if 0
	// Esto no deberia compilar
	contenedor_estandar->guardar(discos_melendi);
#endif

	SerVivo* elvis_presley = new SerVivo("Elvis Presley",0.1,100);
	Contenedor<SerVivo>* contenedor_elvis = new Contenedor<SerVivo>(1);
	contenedor_elvis->guardar(elvis_presley);

	SerVivo* frogosaurio = new SerVivo("Frogosaurio",5,1000);
	Contenedor<SerVivo>* contenedor_frogo = new Contenedor<SerVivo>(8);
	contenedor_frogo->guardar(frogosaurio);

	// Contenedores especiales dentro de contenedor estándar
	Contenedor<Carga>* contenedor_bichos = new Contenedor<Carga>(10);
	contenedor_bichos->guardar(contenedor_elvis);
	contenedor_bichos->guardar(contenedor_frogo);

	Camion camion(20);

#if 0
	// Esto no deberia compilar
	camion.guardar(elvis_presley);
#endif

	if (!camion.guardar(contenedor_estandar))
		cout << "Camion lleno con contenedor estandar" << endl;
	if (!camion.guardar(contenedor_toxico))
		cout << "Camion lleno con contenedor toxico" << endl;
	if (!camion.guardar(contenedor_bichos))
		cout << "Camion lleno con contenedor de seres vivos" << endl;

	Producto* apuntes_tepro = new Producto("Apuntes de TePro",0.5,0.1);
	if (!camion.guardar(apuntes_tepro))
		cout << "Camion lleno con "  << apuntes_tepro->nombre()<< endl;

	Producto* trenzas_almudevar = new Producto("Trenzas de Almudevar",3.5,50);
	if (!camion.guardar(trenzas_almudevar))
		cout << "Camion lleno con " << trenzas_almudevar->nombre() << endl;

	cout << endl;
	//cout << camion << endl;

/*
La salida del programa deberia ser parecida a la siguiente:

> ./main
Camion lleno con Trenzas de Almudevar

Camion [20 m3] [1115.9 kg]
  Contenedor [4 m3] [5.8 kg] de Carga Estandar
    Longaniza de Graus [0.5 m3] [0.8 kg]
    Adoquines de Zaragoza [0.1 m3] [5 kg]
  Contenedor [3 m3] [10 kg] de Productos Toxicos
    Discos de Melendi [1 m3] [10 kg]
  Contenedor [10 m3] [1100 kg] de Carga Estandar
    Contenedor [1 m3] [100 kg] de Seres Vivos
      Elvis Presley [0.1 m3] [100 kg]
    Contenedor [8 m3] [1000 kg] de Seres Vivos
      Frogosaurio [5 m3] [1000 kg]
  Apuntes de TePro [0.5 m3] [0.1 kg]

*/

	return 0;
}

