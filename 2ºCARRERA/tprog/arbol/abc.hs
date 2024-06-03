import Data.List (minimumBy)
import Data.Function (on)

data Era = ABB | DDB deriving (Eq, Show) -- Antes de la Batalla de Yavin, Después de la Batalla de Yavin

data Año = Año Int Era deriving (Eq, Show)

instance Ord Año where
    compare (Año y1 ABB) (Año y2 ABB) = compare y2 y1
    compare (Año y1 DDB) (Año y2 DDB) = compare y1 y2
    compare (Año _ ABB) (Año _ DDB) = GT
    compare (Año _ DDB) (Año _ ABB) = LT

data Persona = Persona String Año deriving Show

data ArbolFamiliar = Nodo Persona [ArbolFamiliar] deriving Show

-- Función auxiliar para obtener el nombre de una persona
obtenerNombre :: Persona -> String
obtenerNombre (Persona nombre _) = nombre

-- Función auxiliar para obtener la persona de un nodo
obtenerPersona :: ArbolFamiliar -> Persona
obtenerPersona (Nodo persona _) = persona

-- Función auxiliar para obtener el año de una persona
obtenerAño :: Persona -> Año
obtenerAño (Persona _ año) = año

miembroMasJoven :: ArbolFamiliar -> String
miembroMasJoven (Nodo persona []) = obtenerNombre persona
miembroMasJoven (Nodo _ hijos) = obtenerNombre $ obtenerPersona $ minimumBy (compare `on` (obtenerAño . obtenerPersona)) hijos

-- Función para imprimir el árbol de manera estructurada
imprimirArbol :: ArbolFamiliar -> Int -> String
imprimirArbol (Nodo (Persona nombre año) hijos) nivel =
    replicate (nivel * 2) ' ' ++ nombre ++ " - (" ++ show año ++ ")\n" ++ concatMap (`imprimirArbol` (nivel + 1)) hijos

familiaSkywalker :: ArbolFamiliar
familiaSkywalker = Nodo (Persona "Shmi" (Año 72 ABB)) [
    Nodo (Persona "Anakin" (Año 42 ABB)) [
        Nodo (Persona "Luke" (Año 19 ABB)) [
            Nodo (Persona "Ben" (Año 26 DDB)) []
        ],
        Nodo (Persona "Leia" (Año 19 ABB)) [
            Nodo (Persona "Ben" (Año 1 DDB)) [],
            Nodo (Persona "Jaina" (Año 9 DDB)) [],
            Nodo (Persona "Jacen" (Año 9 DDB)) [],
            Nodo (Persona "Anakin" (Año 10 DDB)) []
        ]
    ]
  ]
  

main :: IO ()
main = do
  putStrLn "Familia Skywalker:"
  putStr $ imprimirArbol familiaSkywalker 0

  
