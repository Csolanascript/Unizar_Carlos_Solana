/**
 * @file clientevocalesUDP.c
 * Programa *clientevocalesUDP* que envía cadenas de texto a un servidor.
 * Uso: clientevocalesUDP servidor puerto
 * 
 * 
 * El programa crea un socket UDP y lo conecta al servidor y puerto especificado.
 * A través del socket envía cadenas de caracteres hasta llegar a fin de fichero
 * (Control+d para provocarlo desde la entrada estándar).
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include "comun.h"
#define MAX_BUFF_SIZE 1000 

/**
 * Función que crea la conexión y se conecta al servidor.
 *
 * @param servinfo Estructura de dirección a la que conectarse.
 * @param f_verbose Flag.
 * @return Descriptor de socket.
 */
int initsocket(struct addrinfo *servinfo, char f_verbose)
{
    int sock = - 1;

    // crea un extremo de la comunicación y devuelve un descriptor
    if (f_verbose)
    {
        printf("Creando el socket (socket)... ");
        fflush(stdout);
    }

    sock = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);//crea el socket
    if (sock < 0)
    {
        perror("Error en la llamada socket: No se pudo crear el socket");
        // muestra por pantalla el valor de la cadena suministrada por el
        // programador, dos puntos y un mensaje de error que detalla la
        // causa del error cometido
    }
    else
    {   // socket creado correctamente
        if (f_verbose) printf("Estableciendo la comunicación a través del socket...\n");


    }

    if (sock < 0)
    {
        perror("Error en la llamada socket: No se pudo crear el socket");
        exit(1);
    }

    return sock;
}

/**
 * Programa principal
 *
 * @param argc Número de argumentos usados en la línea de comandos.
 * @param argv Vector de punteros a cadenas de caracteres. argv[0]
 *             apunta al nombre del programa, argv[1] al primer
 *             argumento y así sucesivamente.
 * @return 0 si todo ha ido bien, distinto de 0 si hay error.
 */
int main(int argc, char * argv[])
{
    // declaración de variables propias del programa principal (locales a main)
    char f_verbose = 1;         // flag, 1: imprimir información extra
    struct addrinfo * servinfo; // puntero a estructura de dirección destino
    int sock;                   // descriptor del socket
    char msg[MAX_BUFF_SIZE];    // buffer donde almacenar datos para enviar

    ssize_t len,sentbytes;       // número de bytes leídos por la entrada estándar
    
                       // número de bytes enviados/recibidos por el socket
                       // (size_t con signo)
    uint32_t num;      // variable donde anotar el número de vocales
    char fin[MAX_BUFF_SIZE] = "4";         // carácter ASCII end of transmission (EOT)

    // verificación del número de parámetros:
    if (argc != 3)
    {
        printf("Número de parámetros incorrecto \n");
        printf("Uso: %s servidor puerto/servicio\n", argv[0]);
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }

    // obtiene estructura de direccion
    servinfo = obtener_struct_direccion(argv[1], argv[2], f_verbose);

    // crea un extremo de la comunicación con la primera de las
    // direcciones de servinfo e inicia la conexión con el servidor.
    // Devuelve el descriptor del socket
    sock = initsocket(servinfo, f_verbose);

    // bucle que lee texto del teclado y lo envía al servidor
    printf("\nTeclea el texto a enviar y pulsa <Enter>, o termina con <Ctrl+d>\n");
    while (0 < (len = read(0, &msg, MAX_BUFF_SIZE ) ) ){
        // read lee del descriptor 0 (entrada estándar, por defecto el teclado)
        // hasta que se pulsa INTRO,
        // almacena en msg la cadena leída y
        // devuelve la longitud de los datos leídos, en bytes
        if (f_verbose) printf("  Leídos %zd bytes\n", len);

        // envía datos al socket
        if ((sentbytes = (sendto(sock,&msg,len, 0,servinfo->ai_addr, servinfo->ai_addrlen))) < 0)//manda el mensaje usando la función sendto
        {
            perror("Error de escritura en el socket");

            exit(1);
        }
        else
        {
            if (f_verbose) printf("  Enviados %zd bytes al servidor\n",sentbytes);
        }
        // en caso de que el socket sea cerrado por el servidor,
        // al llamar a send() se genera una señal SIGPIPE,
        // que como en este código no se captura,
        // hace que finalice el programa SIN mensaje de error
        // Las señales se estudian en la asignatura Sistemas Operativos

        printf("Teclea el texto a enviar y pulsa <Enter>, o termina con <Ctrl+d>\n"
        );
    }
    
    
    if (sendto(sock, &fin, sizeof(char), 0, servinfo->ai_addr, servinfo->ai_addrlen) < 0){
        perror("Error al escribir en el socket");
        exit(1);
    }
    if (f_verbose)
    {
        printf("Enviada correctamente la señal de finalización.");
        fflush(stdout);
    }
    // recibe del servidor el número de vocales recibidas:
    if (recvfrom(sock,&num,sizeof(uint32_t),0, servinfo->ai_addr, &(servinfo->ai_addrlen))<0)
    {
        printf("Error de lectura en el socket");
        exit(1);
    }

    printf("Todo el texto enviado contenía en total %d vocales\n",ntohl(num));
    // convierte el entero largo sin signo de formato de red a formato de host

    // cierra la conexión del socket
    if (close(sock) < 0)
    {
        perror("Error al cerrar el socket");
        exit(1);
    }
    else
    {
        if (f_verbose) printf("Socket cerrado\n");
    }
    exit(0); // finaliza el programa indicando salida correcta (0)
}
