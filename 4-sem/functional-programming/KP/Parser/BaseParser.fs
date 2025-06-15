namespace Parser

open FParsec
open Interpreter.Interpreter


module BaseParser =

    let rec makeOtherMap (expr: Expression) =
        match expr with
        | ExpressionList list ->
            let rec process list =
                match list with
                | x :: y :: tail -> OtherMap(x, process (y :: tail))
                | [ x ] -> OtherMap(x, Nothing)
                | _ -> failwith ("List is empty.")
            process list
        | _ -> failwith ("Not an expression list.")


    let expr, exprRef = createParserForwardedToRef()

    let pNumber = pfloat .>> spaces |>> Float

    let pBoolTrue = pstring "true" >>. spaces |>> (fun _ -> Bool true)
    let pBoolFalse = pstring "false" >>. spaces |>> (fun _ -> Bool false)
    let pBool = pBoolFalse <|> pBoolTrue

    let pVariable =
        many1Satisfy2 (fun c -> System.Char.IsLetter(c) || c = '_')
            (fun c -> System.Char.IsLetterOrDigit(c) || c = '_') |>> Variable .>> spaces

    let pArgList = pstring "[" >>. spaces >>. sepBy (expr) (pstring "," .>> spaces) .>> pstring "]" .>> spaces

    let pCallFunction =
        skipChar '@' .>> spaces >>. many1Chars (noneOf " []\n") .>> spaces .>>. pArgList |>> Function

    let pVarDefine =
        pstring "Var" >>. spaces >>. pVariable .>> spaces .>> skipChar '=' .>> spaces .>>. expr .>> spaces
        |>> DefineVariable

    let pMult =
        pstring "mult" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic("*", l, r))
    let pDivide =
        pstring "div" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic("/", l, r))
    let pAdd =
        pstring "add" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic("+", l, r))
    let pSubtr =
        pstring "sub" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic("-", l, r))
    let pEqual =
        pstring "eq" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic("==", l, r))
    let pNotEqual =
        pstring "neq" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic("!=", l, r))
    let pLess =
        pstring "lt" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic("<", l, r))
    let pGreater =
        pstring "gt" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic(">", l, r))
    let pLessEq =
        pstring "leq" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic("<=", l, r))
    let pGreaterEq =
        pstring "geq" >>. spaces >>. skipChar '(' >>. spaces >>. expr .>> skipChar ',' .>> spaces .>>. expr
        .>> skipChar ')' .>> spaces |>> (fun (l, r) -> Arithmetic(">=", l, r))
    let pOperation =
        pMult <|> pDivide <|> pAdd <|> pSubtr <|> pEqual <|> pNotEqual <|> pLess <|> pGreater <|> pLessEq <|> pGreaterEq

    let pIf = stringReturn "if" <| Keyword "if" .>> spaces
    let pElse = stringReturn "else" <| Keyword "else" .>> spaces
    let pThen = stringReturn "->" <| Keyword "then" .>> spaces
    let pKeyword = pIf <|> pElse <|> pThen
    let pCondition =
        pKeyword >>. pOperation .>> pKeyword .>>. expr .>> pKeyword .>>. expr
        |>> (fun ((a, b), c) -> Condition(a, b, c))

    let pLet =
        pstring "Let" >>. spaces >>. many1Chars (noneOf "\"\\ []\n") .>> spaces .>>. pArgList .>> skipChar '='
        .>> spaces .>> skipChar '{' .>> spaces .>>. (sepEndBy1 expr spaces) .>> spaces .>> skipChar '}' .>> spaces
        |>> (fun ((a, b), c) -> DefineFunction(a, b, makeOtherMap (ExpressionList c)))

    let pPrintFn = pstring "printfn" .>> spaces .>>. expr .>> spaces |>> Print
    let pSimplePrint = pstring "print" .>> spaces .>>. expr .>> spaces |>> Print
    let pPrint = pPrintFn <|> pSimplePrint

    exprRef.Value <-
        choice
            [ attempt pBool
              attempt pCallFunction
              attempt pCondition
              attempt pKeyword
              attempt pLet
              attempt pNumber
              attempt pOperation
              attempt pPrint
              attempt pVarDefine
              attempt pVariable ]

    let res = spaces >>. many expr .>> eof |>> ExpressionList

    let ParseText(s: string): Result<Expression, string> =
        match run res s with
        | Success(res, _, _) -> Result.Ok res
        | Failure(err, _, _) -> Result.Error err
