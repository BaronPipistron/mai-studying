Let factorial [n] = {
	if eq(n, 0) -> 1
	else mult(n, @ factorial [sub(n, 1)])
}
printfn @ factorial [0]
printfn @ factorial [5]
printfn @ factorial [8]

Let sumOfSquares [x, y] = {
	add(mult(x, x), mult(y, y))
}
printfn @ sumOfSquares [3, 4]

Let f [x] = {
	Var z = div(@ sumOfSquares [3, 4], 5)
	printfn z
}
printfn @ f [0]
