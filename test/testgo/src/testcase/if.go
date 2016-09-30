package testcase

func OnlyIf(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func IfElse(x int) int {
	if x < 0 {
		return -x
	} else {
		return x
	}
}

func ForwardDeclare() int {
	if x := 10; x < 0 {
		return x
	} else {
		return -x
	}
}
