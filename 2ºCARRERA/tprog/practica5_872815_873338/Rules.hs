module Rules where

koch :: Char -> String
koch 'F' = "F+F--F+F"
koch p = [p]

kochCuadrada :: Char -> String
kochCuadrada 'F' = "F+F-F-F+F"
kochCuadrada p = [p]

snowflake :: Char -> String
snowflake 'F' = "F-F++F-F"
snowflake p = [p]


minkowski :: Char -> String
minkowski 'F' = "F+F-F-FF+F+F-F"
minkowski p = [p]

sierpinsky :: Char -> String
sierpinsky 'F' = "F-G+F+G-F"
sierpinsky 'G' = "GG"
sierpinsky p = [p]

arrowhead :: Char -> String
arrowhead 'F' = "G-F-G"
arrowhead 'G' = "F+G+F"
arrowhead p = [p]

hilbert :: Char -> String
hilbert 'f' = "-g>+f>f+>g-"
hilbert 'g' = "+f>-g>g->f+"
hilbert p = [p]

gosper :: Char -> String
gosper 'F' = "F-G--G+F++FF+G-"
gosper 'G' = "+F-GG--G-F++F+G"
gosper p = [p]