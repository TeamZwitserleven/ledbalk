package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"math"
)

func main() {
	white := []byte{0xff, 0xff, 0xff}
	red := []byte{0x4B, 0x17, 0x0A}
	yellow := []byte{0xEE, 0x95, 0x28}
	//	gray := []byte{0x11, 0x11, 0x11}

	story := [][]byte{
		white,
		white,
		white,
		red,
		yellow,
		red,
		white,
	}

	steps := 128
	regions := len(story) - 1
	stepsPerRegionF := float64(steps) / float64(regions)
	stepsPerRegion := int(math.Ceil(stepsPerRegionF))

	fullStory := make([][]byte, steps)
	for i := 0; i < regions; i++ {
		start := story[i]
		end := story[i+1]
		for r := 0; r < stepsPerRegion; r++ {
			percentage := (1.0 / stepsPerRegionF) * float64(r)
			index := i*stepsPerRegion + r
			if index >= len(fullStory) {
				continue
			}
			fullStory[index] = []byte{
				stepValue(start[0], end[0], percentage),
				stepValue(start[1], end[1], percentage),
				stepValue(start[2], end[2], percentage),
			}
		}
	}

	// Print out
	fmt.Printf("#define NUMCOLORS %d\n", len(fullStory))
	fmt.Printf("uint32_t colorTable[] = {\n")
	for i, c := range fullStory {
		suffix := ", "
		if i == len(fullStory)-1 {
			suffix = ""
		} else if i%4 == 3 {
			suffix = suffix + "\n"
		}
		fmt.Printf("\t0x%02x%02x%02x%s", c[0], c[1], c[2], suffix)
	}
	fmt.Printf(" };\n")

	// Print HTML for ref
	buf := &bytes.Buffer{}
	fmt.Fprintf(buf, "<html><body>\n")
	for _, c := range fullStory {
		fmt.Fprintf(buf, "<div style='background-color: #%02x%02x%02x'>&nbsp;</div>\n", c[0], c[1], c[2])
	}
	fmt.Fprintln(buf, "</body></html>")
	ioutil.WriteFile("test.html", buf.Bytes(), 0x644)
}

func stepValue(begin, end byte, percentage float64) byte {
	d := (float64(end) - float64(begin)) * percentage
	r := byte(float64(begin) + d)

	//fmt.Printf("B=%d, E=%d, Perc=%v --> d=%v, r=%v\n", begin, end, percentage, d, r)
	return r / 2
}
