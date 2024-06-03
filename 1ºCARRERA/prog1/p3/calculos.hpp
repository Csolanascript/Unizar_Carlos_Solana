/******************************************************************************\
 * Curso de Programación 1. Tema 7 y práctica 3
 * Autores originales: Miguel Ángel Latre y Javier Martínez
 * Modificado por: ¡PON AQUÍ TU NOMBRE! (funciones establecidas en la práctica 3)
 * Última revisión: 23 de octubre de 2020
 * Resumen: Fichero calculos.hpp de interfaz del módulo de biblioteca
 *          «calculos» que facilita algunas funciones para trabajar con
 *          datos de tipo entero
 *          Versión específica de la práctica 3.
\******************************************************************************/


/*
 * Pre:  ---
 * Post: Devuelve el número de cifras de «n» cuando «n» se escribe en
 *       base 10.
 */
unsigned numCifras(int n);


/*
 * Pre:  ---
 * Post: Devuelve la suma de las cifras de «n» cuando «n» se escribe en
 *       base 10.
 */
unsigned sumaCifras(int n);


/*
 *  Pre:  i ≥ 1
 *  Post: Devuelve la «i»-ésima cifra menos significativa de «n» cuando
 *        «n» se escribe en base 10.
 */
unsigned cifra(int n, unsigned i);


/*
 *  Pre:  ---
 *  Post: Devuelve el número que escrito en base 10 es la imagen
 *        especular de «n» cuando «n» se escribe también en base 10.
 */
int imagen(int n);


/*
 *  Pre:  ---
 *  Post: Devuelve el factorial de «n».
 */
unsigned factorial(unsigned n);


/*
 *  Pre:  ---
 *  Post: Devuelve true si y solo si «n» es un número primo.
 */
bool esPrimo(unsigned n);


/*
 *  Pre: a ≠ 0 o b ≠ 0
 *  Post: Devuelve el máximo común divisor de <a> y <b>.
 */
unsigned mcd(int a, int b);

/*
 *  Pre: a ≠ 0 o b ≠ 0
 *  Post: Devuelve el minimo comun multiplo de <a> y <b>.
 */
unsigned mcm(int a, int b);

/*
* Pre: ---
* Post: Ha devuelto true si y solo si el número «n» es capicúa cuando se escribe en base 10.
*/
bool esCapicua(int n);