open Interpreter.Interpreter
open Parser.Parser


[<EntryPoint>]
let main args: int =
    if args.Length <> 0 then
        printf "Usage: dotnet run --project Main/Main.fsproj"
        1
    else
        let path = "sample.x"

        try
            let result = parseFile path
            let result' = eval (result.Value) Map.empty
            0
        with
        | :? System.NullReferenceException as exc ->
            printfn "Parsing error."
            2
        | exc ->
            printfn "Exception: %A." exc
            2
