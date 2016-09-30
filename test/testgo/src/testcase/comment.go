package testcase

func TestComment() int {
	var sum int /* this is a block comment */ = 0
	// sum + = 10
	sum = 10
	return sum
}
