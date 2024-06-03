#define N 4 // número de reinas
extern void resolverReinas (int n, int reinas[]);
int main () {
 int solucion[N]; // Vector de posiciones de reinas en cada fila
 int nreinas = N; // Numero de reinas
 int i; // Contador
 // Inicializar vector (ninguna reina colocada)
 for (i=0; i<nreinas; i++) solucion[i] = -1;
 if (nreinas>=4) resolverReinas (nreinas,solucion);
 while(1);
 } 