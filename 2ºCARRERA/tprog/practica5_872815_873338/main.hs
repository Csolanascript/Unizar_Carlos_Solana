import SVG
import Turtle
import LSystem
import Rules

main=do 
    let caminoKoch = lsystem koch "F" 3
    let figuraKoch = tplot (1, 60, (0,0), 0) caminoKoch
    savesvg "figuraKoch" figuraKoch

    let caminoKochCuadrada = lsystem kochCuadrada "F" 3
    let figuraKochCuadrada = tplot (1, 90, (0,0), 0) caminoKochCuadrada
    savesvg "figuraKochCuadrada" figuraKochCuadrada

    let caminoSnowFlake = lsystem snowflake "F++F++F" 3
    let figuraSnowFlake = tplot (1, 60, (0,0), 0) caminoSnowFlake
    savesvg "figuraSnowFlake" figuraSnowFlake
    
    let caminoMinkowski = lsystem minkowski "F+F+F+F" 2
    let figuraMinkowski = tplot (1, 90, (0,0), 0) caminoMinkowski
    savesvg "figuraMinkowski" figuraMinkowski
    
    let caminoSierpinsky = lsystem sierpinsky "F-G-G" 6
    let figuraSierpinsky = tplot (1, 120, (0,0), 180) caminoSierpinsky
    savesvg "figuraSierpinsky" figuraSierpinsky
    
    let caminoArrowhead = lsystem arrowhead "F" 6
    let figuraArrowhead = tplot (1, 60, (0,0), 0) caminoArrowhead
    savesvg "figuraArrowhead" figuraArrowhead
    
    let caminoHilbert = lsystem hilbert "f" 4
    let figuraHilbert = tplot (1, 90, (0,0), 180) caminoHilbert
    savesvg "figuraHilbert" figuraHilbert
    
    let caminoGosper = lsystem gosper "F" 3
    let figuraGosper = tplot (1, 60, (0,0), 0) caminoGosper
    savesvg "figuraGosper" figuraGosper