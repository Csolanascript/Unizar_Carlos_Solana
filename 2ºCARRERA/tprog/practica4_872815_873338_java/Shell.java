/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Shell.java
 */

import java.util.ArrayList;
import java.util.Map;

public class Shell
{
    private Directorio direct = new Directorio("/"); // Directorio root
    private ArrayList<Nodo> ruta = new ArrayList<>();

    public Shell()
    {
        ruta.add(direct);
    }
    public String pwd()
    {
        if(ruta.size() ==  1) {
            return ruta.get(ruta.size()-1).nombre();
        }
        else {
            int principioRuta = 2; // Los 2 primeros elementos de la ruta son distintos al resto de la sintaxis de la ruta: / -> a -> /b -> /c
            String ruta_actual = "";
            for(Nodo elem : ruta) {
                if(principioRuta > 0) { // Primeras 2 iteraciones
                    ruta_actual = ruta_actual + elem.nombre();
                    principioRuta--;
                }
                else { // A partir de la tercera
                    ruta_actual = ruta_actual + "/" + elem.nombre();
                } 
            }
            return ruta_actual;
        }
    }
    public String ls() throws ExcepcionArbolFicheros
    {
        String resul = "";
        if(ruta.get(ruta.size()-1) instanceof Directorio){
            for (Map.Entry<String, Nodo> elem : ((Directorio)ruta.get(ruta.size()-1)).contenido.entrySet()) {
                resul = resul + elem.getValue().nombre() + "\n";
            }
            return resul;
        }
        else if(ruta.get(ruta.size()-1) instanceof Enlace){
            Nodo aux = ruta.get(ruta.size()-1);
            while(aux instanceof Enlace) {
                aux = ((Enlace)aux).link();
            }
            if(aux instanceof Directorio) {
                for (Map.Entry<String, Nodo> elem : ((Directorio)aux).contenido.entrySet()) {
                resul = resul + elem.getValue().nombre() + "\n";
                }
            }
            else{
                throw new EnlaceNoEsDirectorio();
            }

            
            return resul;
        }
        else{
            throw new NoEsDirectorio();
        }
    }
    public String du() throws ExcepcionArbolFicheros
    {
        String resul = "";
        if(ruta.get(ruta.size()-1) instanceof Directorio){
            for (Map.Entry<String, Nodo> elem : ((Directorio)ruta.get(ruta.size()-1)).contenido.entrySet()) {
                resul = resul + elem.getValue().nombre() + ", " + elem.getValue().tamano() + "\n";
            }
            return resul;
        }
        else if(ruta.get(ruta.size()-1) instanceof Enlace){
            Nodo aux = ruta.get(ruta.size()-1);
            while(aux instanceof Enlace) {
                aux = ((Enlace)aux).link();
            }
            if(aux instanceof Directorio) {
                for (Map.Entry<String, Nodo> elem : ((Directorio)aux).contenido.entrySet()) {
                resul = resul + elem.getValue().nombre() + ", " + elem.getValue().tamano() + "\n";
                }
            }
            else{
                throw new EnlaceNoEsDirectorio();
            }

            
            return resul;
        }
        else{
            throw new NoEsDirectorio();
        }     
    }
    public void vi(String name, Integer size) throws ExcepcionArbolFicheros
    {
        if( size < 0){
            throw new TamanoNegativo();
        }
        if(((Directorio)ruta.get(ruta.size()-1)).contenido.containsKey(name)){
            for (Map.Entry<String, Nodo> elem : ((Directorio)ruta.get(ruta.size()-1)).contenido.entrySet()) {
                if(elem.getValue() instanceof Fichero && (elem.getValue().nombre() == name)){
                    ((Fichero)elem.getValue()).setTamano(size);
                    break;           
                }
                if(elem.getValue() instanceof Enlace && (elem.getValue().nombre() == name)){
                    Nodo aux = elem.getValue();
                    while(aux instanceof Enlace) {
                        aux = ((Enlace)aux).link();
                    }
                    if(aux instanceof Fichero) {
                       ((Fichero)aux).setTamano(size); 
                    }
                }
                else {
                    throw new NoEsFichero();
                }
            }
        }
        else{
            Fichero nuevoFich = new Fichero(name, size);
            ((Directorio)ruta.get(ruta.size()-1)).contenido.put(name,nuevoFich);
        }                
            
    }
    public void mkdir(String name) throws ExcepcionArbolFicheros
    {
        if(((Directorio)ruta.get(ruta.size()-1)).contenido.containsKey(name)){
            throw new YaExisteNodo();
        }
        else{
            Directorio new_dir = new Directorio(name);
            ((Directorio)ruta.get(ruta.size()-1)).contenido.put(name, new_dir);
        }
    }
    public void cd(String path) throws ExcepcionArbolFicheros
    {
        if(path.charAt(0) == '/') {
            ruta.clear();
            ruta.add(direct);             
            path = path.substring(1);
            if(path.length() > 0) {
                cd(path);
            }     
        }
        else {
            if(path.startsWith("..")) {
                if(ruta.get(ruta.size()-1).nombre() == "/") {
                    throw new RetrocederRoot();
                }
                else {
                    int ultimoIndice = ruta.size() - 1;
                    ruta.remove(ultimoIndice);
                    path = path.substring(2);
                    if(path.startsWith("/")) {
                        path = path.substring(1);
                    }
                    if(path.length() > 0) {
                        cd(path);
                    }
                }
            }
            else if(path.startsWith("./")) {
                path = path.substring(2);
                if(path.length() > 0) {
                    cd(path);
                }
            }
            else if(path.length() > 0) {
                int ultimoIndice = path.lastIndexOf("/");
                if(ultimoIndice != -1) {
                    String[] parse = path.split("/");
                    for(int i = 0; i < parse.length; i++) {
                        cd(parse[i]);
                    }
                }
                else {
                    Nodo aux = ruta.get(ruta.size()-1);
                    while(aux instanceof Enlace) {
                        aux = ((Enlace)aux).link();
                    }
                    if(aux instanceof Fichero) {
                        throw new SaltoFichero();
                    }
                    else {
                        for (Map.Entry<String, Nodo> elem : ((Directorio)aux).contenido.entrySet()) {
                            if(path.equals(elem.getValue().nombre)) {
                                if(elem.getValue() instanceof Fichero) {
                                    throw new SaltoFichero();
                                }
                                else {
                                    ruta.add(elem.getValue());
                                    break;
                                }
                            }
                        }
                        throw new RutaNoEncontrada();
                    }
                }
            }
        }
    }
    public void ln(String path, String name) throws ExcepcionArbolFicheros
    {
        String aux = pwd();
        int lastIndex = path.lastIndexOf("/");
        if(lastIndex == path.length()-1) {
            path = path.substring(0, path.length() - 1);
            ln(path, name);
        }
        else if (lastIndex == 0) {
            path = path.substring(1);
            cd("/");
        }
        else if (lastIndex > 0) {
            String destino = path.substring(0,lastIndex);
            cd(destino);
            path = path.substring(lastIndex + 1);
            
        }
        if(((Directorio)ruta.get(ruta.size()-1)).contenido.containsKey(name)){
            throw new YaExisteNodo();
        }
        else{
            for (Map.Entry<String, Nodo> elem : ((Directorio)ruta.get(ruta.size()-1)).contenido.entrySet()) {
                if(path.equals(elem.getValue().nombre())){
                    Nodo link = new Enlace(name,elem.getValue());  
                    cd(aux);
                    ((Directorio)ruta.get(ruta.size()-1)).contenido.put(name, link);
                    return;
                }   
            }   
            throw new RutaNoEncontrada();
        }
    }
    
    public int stat(String path) throws ExcepcionArbolFicheros
    {
        String aux = pwd();
        int lastIndex = path.lastIndexOf("/");
        if(lastIndex == path.length()-1) {
            path = path.substring(0, path.length() - 1);
            return stat(path);
        }
        else if (lastIndex == 0) {
            path = path.substring(1);
            cd("/");
        }
        else if (lastIndex > 0) {
            String destino = path.substring(0,lastIndex);
            cd(destino);
            path = path.substring(lastIndex + 1);
            
        }
        for (Map.Entry<String, Nodo> elem : ((Directorio)ruta.get(ruta.size()-1)).contenido.entrySet()) {
            if(path.equals(elem.getValue().nombre())){
                int resul = elem.getValue().tamano();
                cd(aux);
                return resul;
            }   
        }
    }
    public void rm(String path) throws ExcepcionArbolFicheros
    {
        String aux = pwd();
        int lastIndex = path.lastIndexOf("/");
        if(lastIndex == path.length()-1) {
            path = path.substring(0, path.length() - 1);
            rm(path);
        }
        else if (lastIndex == 0) {
            path = path.substring(1);
            cd("/");
        }
        else if (lastIndex > 0) {
            String destino = path.substring(0,lastIndex);
            cd(destino);
            path = path.substring(lastIndex + 1);
            
        }
        for (Map.Entry<String, Nodo> elem : ((Directorio)ruta.get(ruta.size()-1)).contenido.entrySet()) {
            if(path.equals(elem.getValue().nombre())){
                ((Directorio)ruta.get(ruta.size()-1)).contenido.remove(elem.getValue().nombre());
                cd(aux);
                return;
            }   
        }
        throw new RutaNoEncontrada(); 
    }
}