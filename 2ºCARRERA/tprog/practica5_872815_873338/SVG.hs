module SVG(savesvg) where

import Text.Printf

polyline :: [(Float, Float)] -> String
polyline vertices =
    "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
    ++ printf " viewBox=\"%f %f %f %f\"\n" x0 y0 w h
    ++ ">\n"
    ++ "<polyline\n"
    ++ " fill=\"none\" stroke=\"green\" stroke-width=\"0.25%\"\n"
    ++ " points=\"\n"
    ++ (concat $ map (\(x,y) -> printf "%10.6f,%10.6f\n" x y) vertices)
    ++ " \"\n"
    ++ "/>\n"
    ++ "</svg>\n"
    where
        (minx,miny) = foldl1 (\(mx,my) (x,y) -> (min mx x,min my y)) vertices
        (maxx,maxy) = foldl1 (\(mx,my) (x,y) -> (max mx x,max my y)) vertices
        width  = maxx-minx
        height = maxy-miny
        margin = 0.05*min width height
        w = width+2*margin
        h = height+2*margin
        x0 = minx-margin
        y0 = miny-margin

savesvg name pts = writeFile (name++".svg") (polyline pts)
