package testcase

func Second(x int, y int) int {
	return y
}

func ReturnSwap() func(x int, y int) int {
	return Second;
}
