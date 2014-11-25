#!/bin/sh
gnuplot << EOF
set terminal pngcairo font 'Times,20' linewidth 2 size 15in,9in 
set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey
set grid back linestyle 81

set pointsize 2
set output "cost-$1.png"
set style line 1 lt rgb "#A00000" lw 2 pt 2 ps 2
set style line 2 lt rgb "#00A000" lw 2 pt 9 ps 2
set style line 3 lt rgb "#5060D0" lw 2 pt 1 ps 2
set style line 4 lt rgb "#F25900" lw 2 pt 6 ps 2
set style increment

set xlabel "Update Percentage"
set ylabel "Cost"
set xtics nomirror
set ytics nomirror
set key on right top
plot '$1.txt' using 1:2 ti "SKF" with linespoints ls 1, \
     '' using 1:3 ti "SUF" with linespoints ls 2, \
     '' using 1:4 ti "BFM with SKF" with linespoints ls 3, \
     '' using 1:5 ti "BFM with SUF" with linespoints ls 4
EOF
