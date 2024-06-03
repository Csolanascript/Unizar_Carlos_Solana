/**
 * @file migetaddrinfo.c
 * Programa *migetaddrinfo* para construir estructuras de direcciones.
 *
 * Uso: migetaddrinfo [servidor] <puerto/servicio>
 *
 * Con dos argumentos, crea la estructura de direcciones correspondiente
 * al servidor y servicio especificadosi y la imprime por pantalla.
 * Con un argumento, crea la estructura de direcciones correspondiente
 * al servicio especificado, en el equipo actual, y la imprime por pantalla.
 */

// El preprocesador sustituye cada include por el contenido del fichero
// referenciado
// Bibliotecas estándar:
#include <stdio.h>   // printf()
#include <stdlib.h>  // exit()
#include <netdb.h>   // freeaddrinfo()

// Bibliotecas no estándar:
#include "comun.h"   // obtener_struct_direccion()

/**
 * Programa principal migetaddrinfo.
 *
 * @param argc Número de argumentos usados en la línea de comandos.
 * @param argv Vector de punteros a cadenas de caracteres. argv[0]
 *             apunta al nombre del programa, argv[1] al primer
 *             argumento y así sucesivamente.
 * @return 0 si todo ha ido bien, distinto de 0 si hay error.
 */
int main(int argc, char * argv[])
{
    char f_verbose = 1;  // flag, 1: imprimir información por pantalla
    struct addrinfo* direccion; // puntero (no inicializado!) a estructura de dirección

    // verificación del número de parámetros:
    if ((argc != 2) && (argc != 3))
    {
        printf("Número de parámetros incorrecto \n");
        printf("Uso: %s [servidor] <puerto/servicio>\n", argv[0]);
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }

    if (argc == 2)
    {
        // devuelve la estructura de dirección del servicio solicitado
        // asumiendo que vamos a actuar como servidor
        direccion = obtener_struct_direccion(NULL, argv[1], f_verbose);
    }
    else // if (argc == 3)
    {
        // devuelve la estructura de dirección al equipo y servicio solicitado
        direccion = obtener_struct_direccion(argv[1], argv[2], f_verbose);
    }

    // cuando ya no se necesite, hay que liberar la memoria dinámica
    // obtenida en getaddrinfo() mediante freeaddrinfo()
    if (f_verbose)
    {
        printf("Devolviendo al sistema la memoria usada por servinfo (ya no se va a usar)... ");
        fflush(stdout);
    }
    freeaddrinfo(direccion);
    if (f_verbose) printf("hecho\n");
    direccion = NULL;
    // como ya se ha liberado ese bloque de memoria,
    // dejamos de apuntarlo para evitar acceder a ella por error
    // si referenciamos esta variable el programa abortará con
    // ERROR: SEGMENTATION FAULT

    // finaliza el programa indicando salida correcta (0)
    exit(0);
}
