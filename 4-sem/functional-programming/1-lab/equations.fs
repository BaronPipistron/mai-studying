open System

// Определите функции для решение алгебраических уравнений

let rec dichotomy f (a : float) (b : float) =
    let eps = 0.000001
    let xn = (a + b) / 2.
    let fa = f a
    let fb = f b

    if abs (f xn) < eps then
        xn
    else if fa < fb then
        if (f xn) < 0. then dichotomy f xn b else dichotomy f a xn
    else 
        if (f xn) > 0. then dichotomy f xn b else dichotomy f a xn

let rec iterations phi x0 =
    let eps = 0.000001

    if abs (x0 - (phi x0)) < eps then
        x0
    else
        let next = phi x0
        iterations phi next

let rec newthon f f' x0 =
    let phi x : float = x - (f x) / (f' x)
    iterations phi x0
// используйте функцию 'iterations'

// Решите 3 уравнения (начиная со своего номера варианта) с использованием 3-х методов
let f1 x : float = x * x - log(1. + x) - 3. // #9
let f2 x : float = 2. * x * sin x - cos x // #10 
let f3 x : float = (Math.E) ** x + sqrt (1. + (Math.E) ** (2. * x)) - 2. // #11

let f1' x : float = 2. * x - 1. / (1. + x) // #9
let f2' x : float = 2. * x * cos x + 3. * sin x // #10
let f3' x : float = (Math.E) ** x + (Math.E) ** (2. * x) / (sqrt(1. + (Math.E) ** (2. * x))) // #11

let phi1 x : float = sqrt(log(1. + x) + 3.) // #9
let phi2 x : float = x - (f2 x) / (f2' x) // #10
let phi3 x : float = log(2. - sqrt(1. + (Math.E) ** (2. * x))) // #11 

let main = 
    printfn "| Method     | Dichotomy  | Iterations |   Newton   |"
    printfn "|------------|------------|------------|------------|"
    printfn "| Equation 1 | %10.5f | %10.5f | %10.5f |" (dichotomy f1 2. 3.) (iterations phi1 2.5) (newthon f1 f1' 2.5)
    printfn "| Equation 2 | %10.5f | %10.5f | %10.5f |" (dichotomy f2 0.4 1.) (iterations phi2 0.7) (newthon f2 f2' 0.7)
    printfn "| Equation 3 | %10.5f | %10.5f | %10.5f |" (dichotomy f3 -1. 0.) (iterations phi3 -0.5) (newthon f3 f3' -0.5)
    
main