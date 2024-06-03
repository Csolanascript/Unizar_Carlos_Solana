import Data.List (minimumBy)
import Data.Function (on)

data Era = BBY | ABY deriving (Eq, Show)

data Year = Year Int Era deriving Eq

instance Show Year where
    show (Year year era) = show year ++ if era == BBY then "BBY" else "ABY"

instance Ord Year where
    compare (Year y1 BBY) (Year y2 BBY) = compare y2 y1
    compare (Year y1 ABY) (Year y2 ABY) = compare y1 y2
    compare (Year _ BBY) (Year _ ABY) = GT
    compare (Year _ ABY) (Year _ BBY) = LT

data Person = Person String Year deriving Show

data FamilyTree = Node Person [FamilyTree] deriving Show

-- Función auxiliar para obtener el nombre de una persona
getName :: Person -> String
getName (Person name _) = name

-- Función auxiliar para obtener la persona de un nodo
getPerson :: FamilyTree -> Person
getPerson (Node person _) = person

-- Función auxiliar para obtener el año de una persona
getYear :: Person -> Year
getYear (Person _ year) = year

youngest :: FamilyTree -> String
youngest (Node person []) = getName person
youngest (Node _ children) = getName $ getPerson $ minimumBy (compare `on` (getYear . getPerson)) children


parent :: FamilyTree -> String -> [String]
parent (Node (Person name _) children) target = 
    if any (descendant target) children then [name] else concatMap (`parent` target) children
  where
    descendant targetName (Node (Person name _) _) = targetName == name
    descendant targetName (Node _ subChildren) = any (descendant targetName) subChildren


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

main :: IO ()
main = do
  putStrLn $ "The youngest member of the Skywalker family is: " ++ youngest skywalkerFamily
  putStrLn $ "The parents of Anakin are: " ++ show (parent skywalkerFamily "Anakin")
  putStrLn $ "The parents of Ben are: " ++ show (parent skywalkerFamily "Ben")