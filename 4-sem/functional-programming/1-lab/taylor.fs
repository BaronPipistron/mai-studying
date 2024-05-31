// Print a table of a given function f, computed by taylor series

// function to compute
let f (x : float) = (3. * x - 5.) / (x * x - 4. * x + 3.)

let eps = 1e-6
let a = 0.0
let b = 0.5
let n = 10

let iter left right func init = 
    let rec iter' left right func acc = 
        if left <= right then
            iter' (left + 1) right func (func left acc)
        else
            acc
    iter' left right func init

// power
let pow x n = iter 1 n (fun _ acc -> acc * x) 1.0

// factorial
let fact n = iter 1 n (fun curr acc -> curr * acc) 1

// abs
let my_abs = function
  | x when x < 0 -> -1 * x
  | x -> x

// Define a function to compute f using naive taylor series method
let taylor_naive x =
    
    let get_i_element i = -1. * (1. + 2. / (pow 3. (i + 1))) * (pow x (int i))
    
    let rec taylor_naive' i acc = 

        let curr = get_i_element i
        let next_curr = get_i_element (i + 1)

        if abs (curr - next_curr) < eps then
            acc + curr, i
        else
            taylor_naive' (i + 1) (acc + curr)

    taylor_naive' 1 0.0


// Define a function to do the same in a more efficient way
let taylor x = 

  let calculate_next_term left_term_half right_term_half n = 
    left_term_half * x,
    right_term_half * (1. / 3.) * x

  let rec taylor' n acc left_term_half right_term_half =
    let curr = left_term_half + right_term_half            
    let next_left_term_half, next_right_term_half =             
      calculate_next_term left_term_half right_term_half n
    let next_curr = next_left_term_half + next_right_term_half  
    

    if abs (curr - next_curr) < eps then
      acc + curr, n
    else
      taylor' (n + 1) (acc + curr) next_left_term_half next_right_term_half
  
  taylor' 1 0 1. (2. / 3.)


let main =
  printfn "|  X   |   Builtin  |Terms |Dumb Taylor |Terms |Smart Taylor |"
  printfn "--------------------------------------------------------------"
  for i = 0 to n do
    let x = a + (float i) / (float n) * (b - a)
    let naive_val, naive_terms = taylor_naive x
    let smart_val, smart_terms = taylor x
    printfn "|%5.2f | %10.6f |  %2d  | %10.6f |  %2d  | %10.6f  |" x (f x) naive_terms naive_val smart_terms smart_val

main