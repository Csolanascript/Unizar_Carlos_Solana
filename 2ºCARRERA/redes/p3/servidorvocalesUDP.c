/**
 * @file servidorvocalesTCP.c
 *
 * Programa *servidorvocalesTCP* que cuenta vocales.
 *
 * Uso: servidorvocalesTCP puerto
 *
 * El programa crea un socket TCP en el puerto especificado, lo pone en modo
 * escucha y atiende consecutivamente a los clientes que se van conectando.
 * Para cada cliente, recibe cadenas de texto, cuenta las vocales contenidas
 * y al acabar todas las recepciones envía al cliente el número total de
 * vocales contadas.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include "comun.h"

// definición de constantes
#define BUFF_SIZE 1000 ///< Tamaño del buffer para las cadenas de texto.
#define EOT 4

/**
 * Función que crea la conexión y espera conexiones entrantes.
 *
 * @param servinfo Estructura de dirección local.
 * @param f_verbose Flag.
 * @return Descriptor de socket.
 */
int establecer_servicio(struct addrinfo *servinfo, char f_verbose)
{
    int sock = -1;
    // crea un extremo de la comunicación y devuelve un descriptor
    if (f_verbose)
    {
        printf("Creando el socket (socket)... ");
        fflush(stdout);
    }
    sock = socket(AF_INET,SOCK_DGRAM,0);
    if (sock < 0)
    {
        perror("Error en la llamada socket: No se pudo crear el socket");
        // muestra por pantalla el valor de la cadena suministrada por el
        // programador, dos puntos y un mensaje de error que detalla la causa
        // del error cometido
        exit(1);
    }
    if (f_verbose) printf("hecho\n");

    if (f_verbose)
    {
    printf("Asociando socket a puerto (bind)... \n ");
    fflush(stdout);
    }
    
    if (bind(sock, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
    {
    perror("Error asociando el socket");
    exit(1);
    }
    
    if (f_verbose) printf("hecho\n");
    
    return sock;
}

/**
 * Función que cuenta las vocales en una cadena de texto
 *
 * @param msg Cadena con el mensaje al que contar vocales.
 * @param s   Longitud del mensaje.
 * @return Número de vocales en msg[0..s].
 */
uint32_t countVowels(char msg[], size_t s)
{
    uint32_t result = 0;
    size_t i;
    for (i = 0; i < s; i++)
    {
        if (msg[i] == 'a' || msg[i] == 'A' ||
            msg[i] == 'e' || msg[i] == 'E' ||
            msg[i] == 'i' || msg[i] == 'I' ||
            msg[i] == 'o' || msg[i] == 'O' ||
            msg[i] == 'u' || msg[i] == 'U') result++;
    }
    return result;
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
    struct addrinfo * servinfo; // dirección propia (servidor)
    int sock;             // descriptores de socket
    char msg[BUFF_SIZE];        // espacio para almacenar los datos recibidos
    ssize_t readbytes;          // numero de bytes recibidos
    uint32_t num, netNum;       // contador de vocales en formato local y de red
    struct sockaddr_storage caddr; // dirección del cliente
    socklen_t clen=-1;             // longitud de la dirección
    int next=0;

    // verificación del número de parámetros:
    if (argc != 2)
    {
        printf("Número de parámetros incorrecto \n");
        printf("Uso: %s puerto\n", argv[0]);
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }

    // obtiene estructura de direccion
    servinfo = obtener_struct_direccion(NULL, argv[1], f_verbose);

    // crea un extremo de la comunicación. Devuelve el descriptor del socket
    sock = establecer_servicio(servinfo, f_verbose);
    
    // bucle infinito para atender conexiones una tras otra
    while (1)
    {
        
        clen = sizeof caddr;
        do {
            if ((readbytes = recvfrom(sock,&msg, BUFF_SIZE,0,(struct sockaddr*) &caddr,&clen)) < 0)
            {
                perror("Error de lectura en el socket (recv)");
                exit(1);
            }
            printf("\nMensaje recibido del cliente: "); fflush(stdout);
            write(1, msg, readbytes);
            // muestra en pantalla (salida estándar 1) el mensaje recibido
            // evitamos usar printf por si lo recibido no es texto acabado con \0
            num += countVowels(msg, readbytes);
            printf("\nVocales contadas hasta el momento: %d\n \n",num);
            
            if(msg[readbytes-1] == '4'){
                next = 1;
                printf("Marca de fin recibida.\n");
            }

        // condición de final: haber recibido 0 bytes (fin de fichero alcanzado)
        } while ((msg[readbytes-1] != EOT) && (readbytes >0) && next==0);

        
        printf("Contadas %d vocales\n", num);  // muestra las vocales recibidas
        netNum = htonl(num);  // convierte el entero largo sin signo hostlong
            // desde el orden de bytes del host al de la red
            // envia al cliente el número de vocales recibidas:
            
        if (sendto(sock, &netNum, sizeof(netNum),0,(struct sockaddr *)&caddr, clen) < 0)
            {
                perror("Error de escritura en el socket");
                exit(1);
            }
        if (f_verbose) printf("Enviado número de vocales contadas al cliente\n");

        if (f_verbose)
        {
            printf("Cerrada la conexión con el cliente\n");
        }
    }
}
    


