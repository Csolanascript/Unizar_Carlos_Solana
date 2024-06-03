class Main
{
	public static void main(String[] args)
	{
		// Productos estandar

		Contenedor<Carga> contenedor_estandar = new Contenedor<>(4);

		Producto longaniza_graus = new Producto("Longaniza de Graus",0.5,0.8);
		contenedor_estandar.guardar(longaniza_graus);

		Producto adoquines_zgz = new Producto("Adoquines de Zaragoza",0.1,5.0);
		contenedor_estandar.guardar(adoquines_zgz);

		// Productos especiales
		Toxico discos_melendi = new Toxico("Discos de Melendi",1,10);
		Contenedor<Toxico>	contenedor_toxico   = new Contenedor<>(3);
		contenedor_toxico.guardar(discos_melendi);

		// Esto no deberia compilar
		// contenedor_estandar.guardar(discos_melendi);

		SerVivo elvis_presley = new SerVivo("Elvis Presley",0.1,100);
		Contenedor<SerVivo>	contenedor_elvis  = new Contenedor<>(1);
		contenedor_elvis.guardar(elvis_presley);

		SerVivo frogosaurio = new SerVivo("Frogosaurio",5,1000);
		Contenedor<SerVivo> contenedor_frogo = new Contenedor<>(8);
		contenedor_frogo.guardar(frogosaurio);

		// Contenedores especiales dentro de contenedor estándar
		Contenedor<Carga> contenedor_bichos = new Contenedor<>(10);
		contenedor_bichos.guardar(contenedor_elvis);
		contenedor_bichos.guardar(contenedor_frogo);

		Camion camion = new Camion(20);

		// Esto no deberia compilar
		// camion.guardar(elvis_presley);

		if (!camion.guardar(contenedor_estandar))
			System.out.println("Camion lleno con contenedor estandar");
		if (!camion.guardar(contenedor_toxico))
			System.out.println("Camion lleno con contenedor toxico");
		if (!camion.guardar(contenedor_bichos))
			System.out.println("Camion lleno con contenedor de seres vivos");

		Producto apuntes_tepro = new Producto("Apuntes de TePro",0.5,0.1);
		if (!camion.guardar(apuntes_tepro))
			System.out.println("Camion lleno con "+apuntes_tepro.nombre());

		Producto trenzas_almudevar = new Producto("Trenzas de Almudevar",3.5,50);
		if (!camion.guardar(trenzas_almudevar))
			System.out.println("Camion lleno con "+trenzas_almudevar.nombre());

		System.out.println();
		System.out.println(camion);

/*
La salida del programa deberia ser parecida a lo siguiente:

> java Main
Camion lleno con Trenzas de Almudevar

Camion [20.0 m3] [1115.8999999999999 kg]
  Contenedor [4.0 m3] [5.8 kg] de Carga Estándar
    Longaniza de Graus [0.5 m3] [0.8 kg]
    Adoquines de Zaragoza [0.1 m3] [5.0 kg]
  Contenedor [3.0 m3] [10.0 kg] de Productos Toxicos
    Discos de Melendi [1.0 m3] [10.0 kg]
  Contenedor [10.0 m3] [1100.0 kg] de Contenedor
    Contenedor [1.0 m3] [100.0 kg] de Seres Vivos
      Elvis Presley [0.1 m3] [100.0 kg]
    Contenedor [8.0 m3] [1000.0 kg] de Seres Vivos
      Frogosaurio [5.0 m3] [1000.0 kg]
  Apuntes de TePro [0.5 m3] [0.1 kg]

*/

	}
}

