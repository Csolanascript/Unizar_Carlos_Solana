/**
 * @file comun.h
 *
 * Este fichero contiene cabeceras comunes para trabajar con 
 * estructuras de direcciones.
 */

#ifndef COMUN_H
#define COMUN_H

void printsockaddr(struct sockaddr_storage * saddr);

struct addrinfo* obtener_struct_direccion(char *dir_servidor, char *servicio, char f_verbose);

#endif /* COMUN_H */
