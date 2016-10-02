package main

import "fmt"
import "testcase"

func main() {
	fmt.Println("hello world")

	// basic type
	fmt.Println(testcase.String())
	fmt.Println(testcase.Int())
	fmt.Println(testcase.Float32())

	// comment
	fmt.Println(testcase.TestComment())

	// for
	fmt.Println(testcase.ThreeCases())
	fmt.Println(testcase.JustMiddle())
	fmt.Println(testcase.OneCase())
	fmt.Println(testcase.DeadLoop())

	// func
	fmt.Println(testcase.Second(1, 2))
	fmt.Println(testcase.ReturnSwap()(1, 2))

	// if
	fmt.Println(testcase.OnlyIf(1))
	fmt.Println(testcase.IfElse(1))
	fmt.Println(testcase.ForwardDeclare())

	// import
	fmt.Println(testcase.TestImport())

	// switch
	fmt.Println(testcase.OnlyIf(0))
	fmt.Println(testcase.IfElse(0))
	fmt.Println(testcase.ForwardDeclare())

	// var
	fmt.Println(testcase.Declare())
	fmt.Println(testcase.InitVar())
	fmt.Println(testcase.ShortVar())
}
