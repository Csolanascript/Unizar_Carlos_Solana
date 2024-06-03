module BinaryTree where 
import Data.List (sort)

data BinaryTree t  =  EmptyTree | Node t (BinaryTree t) (BinaryTree t)


empty :: BinaryTree x
empty = EmptyTree

leaf :: x -> BinaryTree x
leaf x = Node x EmptyTree EmptyTree

tree :: t  -> BinaryTree t -> BinaryTree t -> BinaryTree t
tree x lc rc = Node x lc rc

size :: BinaryTree x -> Integer
size (EmptyTree) = 0
size (Node x lc rc)  = 1 + size lc + size rc


tabular :: Integer -> String
tabular 0 = ""
tabular n = "   " ++ tabular (n-1)

instance (Show t) => Show (BinaryTree t) where
    show (arbol) = mostrar arbol 0


mostrar :: (Show t) => BinaryTree t -> Integer -> String
mostrar (EmptyTree) n = "()"
mostrar (Node j (EmptyTree) (EmptyTree)) n = show j
mostrar (Node j (arizq) (arder)) n = show j ++ "\n" ++ tabular n ++ "╰─ " ++ mostrar arizq (n+1) ++ "\n" ++ tabular n ++ "╰─ " ++ mostrar arder (n+1)


add :: (Ord t) => BinaryTree t -> t -> BinaryTree t
add (EmptyTree) x = leaf x
add (Node j (arizq) (arder)) x
    | x < j = Node j (add arizq x) (arder)
    | otherwise = Node j (arizq) (add arder x)


build :: (Ord a) => [a] -> BinaryTree a
build = foldl add EmptyTree


buildBalanced ::(Ord a) => [a] -> BinaryTree a
buildBalanced [] = EmptyTree
buildBalanced xs = Node mid (buildBalanced primeraMitad) (buildBalanced segundaMitad) where
    sorted  = sort xs
    medio = length xs `div` 2
    (primeraMitad, mid:segundaMitad) = splitAt medio sorted


preorder, postorder, inorder :: BinaryTree t -> [t]
preorder (EmptyTree) = []
preorder (Node j (arizq) (arder)) = [j] ++ preorder arizq ++ preorder arder

postorder (EmptyTree) = []
postorder (Node j (arizq) (arder)) = postorder arizq ++ postorder arder ++ [j]

inorder (EmptyTree) = []
inorder (Node j (arizq) (arder)) = inorder arizq ++ [j] ++ inorder arder


balance :: (Ord a) => BinaryTree a -> BinaryTree a
balance arbol = buildBalanced (inorder arbol)


between :: (Ord a) => BinaryTree a -> a -> a -> [a]
between (EmptyTree) _ _ = []
between (Node x (arizq) (arder)) xmin xmax
    | x < xmin = between arizq xmin xmax
    | x > xmax = between arder xmin xmax
    | otherwise = between arizq xmin xmax ++ [x] ++ between arder xmin xmax