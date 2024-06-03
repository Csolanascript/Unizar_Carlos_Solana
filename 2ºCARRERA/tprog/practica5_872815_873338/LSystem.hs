module LSystem where

import SVG
import Turtle
import Data.Char (isUpper)

esMayuscula :: Char -> Bool
esMayuscula c = isUpper c

lsystem :: (Char -> String) -> String -> Int -> String
lsystem _ sol 0 = sol
lsystem rule p n = lsystem rule (concat (map rule p)) (n-1)

tplot :: Turtle -> String -> [Position]
tplot (paso, giro, pos, orn) [] = [pos]
tplot (paso, giro, pos, orn) (p:ps)
    | p == '>' = pos : tplot (moveTurtle (paso, giro, pos, orn) Forward) ps
    | p == '+' = pos : tplot (moveTurtle (paso, giro, pos, orn) TurnRight) ps
    | p == '-' = pos : tplot (moveTurtle (paso, giro, pos, orn) TurnLeft) ps
    | esMayuscula p = pos : tplot (moveTurtle (paso, giro, pos, orn) Forward) ps
    | otherwise = tplot (paso, giro, pos, orn) ps
