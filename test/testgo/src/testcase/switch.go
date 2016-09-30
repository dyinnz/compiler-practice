package testcase

func Switch(x int) string {
	switch x {
	case 1:
		return "a"
	case 2:
		return "b"
	default:
		return "c"
	}
}

func MultiCases(x int) int {
	switch {
	case x < 0:
		return -1
	case x > 0:
		return 1
	default:
		return 0
	}
}
