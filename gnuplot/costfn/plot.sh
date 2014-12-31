#!/bin/sh
gnuplot << EOF
set terminal pngcairo font 'Times,25' linewidth 2 size 15in,9in 
set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey
set grid back linestyle 81

set pointsize 2
set output "costfn-$1.png"
set style line 1 lt rgb "#A00000" lw 2 pt 2 ps 3
set style line 2 lt rgb "#00A000" lw 2 pt 9 ps 3
set style line 3 lt rgb "#5060D0" lw 2 pt 1 ps 3
set style line 4 lt rgb "#F25900" lw 2 pt 6 ps 3
set style line 5 lt rgb "#ED0CCB" lw 2 pt 5 ps 3
set style line 6 lt rgb "#686E6B" lw 2 pt 3 ps 3
set style increment

set xlabel "Cost"
set ylabel "Time (in ms)"
set xtics nomirror
set ytics nomirror
set xtics rotate by -90
set key on left top
plot '$1.txt' using 2:3 ti "SI - Uniform" with linespoints ls 1, \
	 '' using 6:7 ti "SI - Zipfian" with linespoints ls 3, \
	 '' using 10:11 ti "SI - Latest" with linespoints ls 6
EOF
