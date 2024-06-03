 -- Definimos el tipo Era para representar las eras BBY (Before Battle of Yavin) y ABY (After Battle of Yavin).
data Era = BBY | ABY 
  deriving (Eq, Show) -- Deriva instancias de Eq y Show automáticamente.
                      -- Eq permite comparar por igualdad.
                      -- Show permite convertir el valor a String.

-- Definimos el tipo Year que almacena un entero (Integer) para el año y un valor de tipo Era.
data Year = Year { year :: Integer, era :: Era } -- Define un registro con year y era.
  deriving (Eq) -- Deriva la instancia de Eq automáticamente, permitiendo comparaciones de igualdad.

-- Implementamos la clase Show para el tipo Year para personalizar cómo se convierte a String.
instance Show Year where
  show (Year y e) = show y ++ "." ++ show e 
    -- show y convierte el año a String.
    -- ++ "." ++ concatena el año con un punto y la representación de la era.
    -- show e convierte la era (BBY o ABY) a String.

-- Implementamos la clase Ord para comparar las fechas de manera personalizada.
instance Ord Year where
  compare (Year y1 BBY) (Year y2 BBY) = compare y2 y1
    -- Si ambos años son BBY, compara los años pero en orden inverso porque
    -- un año menor BBY significa más atrás en el tiempo (más antiguo).
    
  compare (Year y1 ABY) (Year y2 ABY) = compare y1 y2
    -- Si ambos años son ABY, compara los años en orden normal porque
    -- un año mayor ABY significa más adelante en el tiempo.
    
  compare (Year _ BBY) (Year _ ABY) = LT
    -- Cualquier año BBY es menor que cualquier año ABY.
    
  compare (Year _ ABY) (Year _ BBY) = GT
    -- Cualquier año ABY es mayor que cualquier año BBY.



--Tipo de Datos para una Persona

--haskell
skywalkerFamily :: FamilyTree
skywalkerFamily = Node (Person "Shmi" (Year 72 BBY)) [
    Node (Person "Anakin" (Year 42 BBY)) [
        Node (Person "Luke" (Year 19 BBY)) [
            Node (Person "Ben" (Year 26 ABY)) []
        ],
        Node (Person "Leia" (Year 19 BBY)) [
            Node (Person "Ben" (Year 1 ABY)) [],
            Node (Person "Jaina" (Year 9 ABY)) [],
            Node (Person "Jacen" (Year 9 ABY)) [],
            Node (Person "Anakin" (Year 10 ABY)) []
        ]
    ]
  ]

-- Tipo de datos para una persona que incluye nombre y año de nacimiento.
data Person = Person {
  name :: String,  -- Nombre de la persona.
  birthYear :: Year  -- Año de nacimiento usando el tipo Year definido antes.
} deriving (Eq, Show)

-- Función para encontrar el miembro más joven de la familia.
youngest :: FamilyTree -> String
youngest (Node person []) = name person  -- Caso base: si no hay descendientes, devuelve el nombre de la persona.
youngest (Node person children) = 
  let -- Extrae el nombre y el año de nacimiento de la persona en este nodo.
      currentName = name person
      currentYear = birthYear person
      -- Encuentra el miembro más joven entre todos los descendientes.
      youngestDescendant = minimumBy (compare `on` (birthYear . fst)) [(youngest child, birthYear $ fst $ getPerson child) | child <- children]
  in  -- Compara el año de nacimiento de la persona actual con el del descendiente más joven.
      if currentYear > snd youngestDescendant then currentName else fst youngestDescendant

-- Helper function to get the person and its year from a node
getPerson :: FamilyTree -> (Person, Year)
getPerson (Node person _) = (person, birthYear person)

-- Función para encontrar los antecesores de un miembro de la familia.
parent :: FamilyTree -> String -> [String]
parent tree target = parentHelper [] tree target

-- Función auxiliar para buscar los antecesores.
parentHelper :: [String] -> FamilyTree -> String -> [String]
parentHelper ancestors (Node person children) target
  | name person == target = ancestors  -- Si la persona es el target, devuelve los antecesores.
  | otherwise = concat [parentHelper (name person : ancestors) child target | child <- children]

  -- Definimos el árbol familiar de los Skywalker.


main :: IO ()
main = do
  putStrLn $ "The youngest member of the Skywalker family is: " ++ youngest skywalkerFamily
  putStrLn $ "The parents of Anakin are: " ++ show (parent skywalkerFamily "Anakin")
  putStrLn $ "The parents of Ben are: " ++ show (parent skywalkerFamily "Ben")


  --Output

--Si ejecutas el main, deberías obtener:

--sql

--The youngest member of the Skywalker family is: Ben
--The parents of Anakin are: ["Shmi"]
--The parents of Ben are: ["Luke", "Anakin", "Shmi", "Leia", "Anakin", "Shmi"]

