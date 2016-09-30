package testcase

func Swap(x int, y int) (int, int) {
  return y, x
}

func ReturnSwap() func(int, int) (int, int) {
  return Swap;
}
