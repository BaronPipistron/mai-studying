namespace Interpreter

open System.IO


module Interpreter =
    type Id = string

    type Expression =
        | Float of float
        | Bool of bool
        | DefineVariable of Expression * Expression
        | DefineFunction of Id * List<Expression> * Expression
        | FuncDef of List<Expression> * Expression
        | Function of Id * List<Expression>
        | Variable of Id
        | OtherMap of Expression * Expression
        | Arithmetic of Id * left_expression: Expression * right_expression: Expression
        | Print of Id * Expression
        | Condition of Expression * Expression * Expression
        | ExpressionList of List<Expression>
        | Keyword of string
        | Nothing

    and Map = Map<Id, Expression>

    let solve (expr: Expression): Expression =

        match expr with
        | Bool(x) -> Bool(x)
        | Arithmetic("+", Float(x), Float(y)) -> Float(x + y)
        | Arithmetic("-", Float(x), Float(y)) -> Float(x - y)
        | Arithmetic("*", Float(x), Float(y)) -> Float(x * y)
        | Arithmetic("/", Float(x), Float(y)) -> Float(x / y)
        | Arithmetic("==", Float(x), Float(y)) ->
            if x = y then Bool(true) else Bool(false)
        | Arithmetic("!=", Float(x), Float(y)) ->
            if x <> y then Bool(true) else Bool(false)
        | Arithmetic("<", Float(x), Float(y)) ->
            if x < y then Bool(true) else Bool(false)
        | Arithmetic(">", Float(x), Float(y)) ->
            if x > y then Bool(true) else Bool(false)
        | Arithmetic("<=", Float(x), Float(y)) ->
            if x <= y then Bool(true) else Bool(false)
        | Arithmetic(">=", Float(x), Float(y)) ->
            if x <= y then Bool(true) else Bool(false)
        | Arithmetic("==", Bool(x), Bool(y)) ->
            if x = y then Bool(true) else Bool(false)
        | Arithmetic("!=", Bool(x), Bool(y)) ->
            if x <> y then Bool(true) else Bool(false)
        | _ -> failwith ("Operation not implemented.")

    let PrintFunc (expr: Expression): Expression =
        match expr with
        | Print("print", Float(value)) ->
            printf "%f" value
            Nothing

        | Print("printfn", Float(value)) ->
            printfn "%f" value
            Nothing

        | Print("print", arg) ->
            printfn "%A" arg
            Nothing

        | Print("printfn", arg) ->
            printfn "%A" arg
            Nothing

        | _ -> Nothing


    let rec eval (expr: Expression) (map: Map): Expression =

        match expr with

        | Float(x) -> Float(x)

        | Bool(x) -> Bool(x)

        | Arithmetic(id, left, right) ->
            let left' = eval left map
            let right' = eval right map
            solve (Arithmetic(id, left', right'))

        | Variable(x) ->
            match Map.tryFind x map with
            | Some(value) -> eval value map
            | None -> failwith ("Undefined variable.")

        | Condition(condition, first, second) ->
            match eval condition map with
            | Bool(true) -> eval first map
            | Bool(false) -> eval second map
            | _ -> failwith ("Return value error.")

        | Function(id, variable_values) ->
            match Map.tryFind id map with
            | Some(FuncDef(variable_names, instruction)) ->
                if List.length variable_names <> List.length variable_values then
                    failwith ("Function parameters error.")
                else
                    let map' = addVariablesToMap variable_names variable_values map
                    eval instruction map'

            | _ -> failwith ("Undefined function.")

        | OtherMap(left, right) ->
            let map', result_left = processDefinition left map
            match eval right map' with
            | Nothing -> result_left
            | _ -> result_left
        
        | Print(id, arg) ->
            let arg' = eval arg map
            PrintFunc (Print(id, arg'))
        
        | Nothing -> Nothing
        | _ -> failwith ("Parsing error.")
        

    and processDefinition expression map: Map * Expression =
        match expression with
        | DefineVariable(Variable(id), tp) -> (Map.add id tp map, Nothing)
        | DefineFunction(id, list, instruction) -> (Map.add id (FuncDef(list, instruction)) map, Nothing)
        | _ -> (map, eval expression map)


    and addVariablesToMap (ids: List<Expression>) (values: List<Expression>) (map: Map) =
        if ids = [] then
            map
        else
            let head = List.head ids
            match head with
            | Variable(id) ->
                addVariablesToMap (List.tail ids) (List.tail values) (Map.add id (eval (List.head values) map) map)
            | _ -> failwith ("Function argument error.")
