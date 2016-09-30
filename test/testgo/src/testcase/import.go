package testcase

import (
  "simple_add"
  "simple_sub"
)

func TestImport() int {
  sum := simple_add.Add(1, 2)
  sub := simple_sub.Sub(1, 2)
  return sum * sub
}
