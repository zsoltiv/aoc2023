package main

import (
    "os"
    "strings"
    "unicode"
    "strconv"
    "fmt"
)

type Set struct {
    Red uint
    Green uint
    Blue uint
}

func getIDEnd(data string) int {
    for i, c := range data[5:] {
        if(!unicode.IsDigit(c)) {
            return i + 5
        }
    }
    return 5
}

func GameID(line string) uint {
    res, err := strconv.ParseUint(string(line[5:getIDEnd(line)]), 10, 32)
    if err != nil {
        panic(err)
    }
    return uint(res)
}

func extractFirstDigits(text string) uint {
    start := 0
    end := 0
    for i, c := range text {
        if unicode.IsDigit(c) {
            start = i
            break
        }
    }

    for i, c := range text[start:] {
        if !unicode.IsDigit(c) {
            end = start + i
            break
        }
    }

    res, err := strconv.ParseUint(string(text[start:end]), 10, 32)
    if err != nil {
        panic(err)
    }

    return uint(res)
}

func processGame(game string) []Set {
    game = strings.TrimSpace(game)
    sets := make([]Set, 0)
    for _, strSet := range strings.Split(game, ";") {
        set := Set{0,0,0}
        for _, cubes := range strings.Split(strSet, ",") {
            digits := extractFirstDigits(cubes)
            if strings.Contains(cubes, "red") {
                set.Red = digits
            }  else if strings.Contains(cubes, "green") {
                set.Green = digits
            } else if strings.Contains(cubes, "blue") {
                set.Blue = digits
            }
        }
        sets = append(sets, set)
    }

    return sets
}

func ValidateGame(game []Set) bool {
    const RED uint = 12
    const GREEN uint = 13
    const BLUE uint = 14

    for _, set := range game {
        if(set.Red > RED || set.Green > GREEN || set.Blue > BLUE) {
            return false
        }
    }

    return true
}

func MinimumCubes(sets []Set) Set {
    minSet := Set{0, 0, 0}
    for _, set := range sets {
        minSet.Red = max(set.Red, minSet.Red)
        minSet.Green = max(set.Green, minSet.Green)
        minSet.Blue = max(set.Blue, minSet.Blue)
    }

    return minSet
}

func PowerOfCubes(set Set) uint {
    return set.Red * set.Green * set.Blue
}

func main() {
    if len(os.Args) != 2 {
        return
    }

    data, err := os.ReadFile(os.Args[1])
    if err != nil {
        panic(err)
    }

    var sumID uint = 0
    var sumPower uint = 0
    for _, line := range strings.Split(strings.TrimSpace(string(data)), "\n") {
        id := GameID(line)
        sets := processGame(line[strings.Index(line, ":") + 1:])
        if ValidateGame(sets) {
            sumID += id
        }
        sumPower += PowerOfCubes(MinimumCubes(sets))
    }

    fmt.Println(sumID)
    fmt.Println(sumPower)
}
