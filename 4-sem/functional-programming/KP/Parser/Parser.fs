namespace Parser

open FParsec
open System.IO
open Interpreter.Interpreter
open Parser.BaseParser


module Parser =

    let parseFile (path: string): Expression option =
        let text = File.ReadAllText(path)
        match BaseParser.ParseText text with
        | Result.Ok parsed ->
            match parsed with
            | ExpressionList list ->
                let otherMap = makeOtherMap parsed
                Some otherMap
            | _ -> None
        | Result.Error err ->
            printfn "%s" err
            None
