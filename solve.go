package main

import "fmt"

func main() {
	var s string
	fmt.Scan(&s)
	A, a, num, punctuation := 1, 1, 1, 1
	nEnough := 4
	invaildc := 0
	repeat := 0
	res := 0

	lens := len(s)
	for i := 0; i < lens; i++ {
		c := s[i]
		if c <= 'Z' && c >= 'A' {
			A = 0
		} else if c <= 'z' && c >= 'a' {
			a = 0
		} else if c <= '9' && c >= '0' {
			num = 0
		} else if c == ',' || c == '.' || c == '!' || c == '?' || c == ';' {
			punctuation = 0
		} else {
			invaildc++
		}

		if i > 0 && s[i] == s[i-1] {
			repeat++
		}
	}

	nEnough = a + A + num + punctuation
	fmt.Println(repeat, nEnough)
	// delete or add
	if lens >= 20 {
		res = lens - 20
	} else if lens <= 6 {
		res = 6 - lens
		nEnough -= res
	}
	repeat -= res

	fmt.Println("after del or add", repeat, nEnough)
	//modify
	for repeat > 0 || nEnough > 0 {
		res++
		repeat--
		nEnough--
	}

	fmt.Println(res)
}
