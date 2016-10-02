package testcase

func ThreeCases() int {
	sum := 0
	for i := 0; i < 10; i += 1 {
		sum += i
	}
	return sum
}

func JustMiddle() int {
	sum := 1
	for sum < 1000 {
		sum *= 2
	}
	return sum
}

func OneCase() int {
	times := 10
	for times > 0 {
		times --;
	}
	return times
}

func DeadLoop() int {
	ret := 0
	for {
		return ret
	}
}
