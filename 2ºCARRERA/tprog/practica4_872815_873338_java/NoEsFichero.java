class NoEsFichero extends ExcepcionArbolFicheros {
    public NoEsFichero() {
        super("el nodo no es un fichero y debería serlo");
    }
}