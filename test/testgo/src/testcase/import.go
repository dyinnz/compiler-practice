package testcase

import "simpleadd"
import "simplesub"

func TestImport() int {
	sum := simpleadd.Add(1, 2)
	sub := simplesub.Sub(1, 2)
	return sum * sub
}
