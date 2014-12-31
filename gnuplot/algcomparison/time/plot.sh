#!/bin/sh
gnuplot << EOF
set terminal pngcairo font 'Times,25' linewidth 2 size 15in,9in 
set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey
set grid back linestyle 81

set pointsize 2
set output "time-$1.png"
set style line 1 lt rgb "#A00000" lw 2 pt 2 ps 3
set style line 2 lt rgb "#00A000" lw 2 pt 9 ps 3
set style line 3 lt rgb "#5060D0" lw 2 pt 1 ps 3
set style line 4 lt rgb "#F25900" lw 2 pt 6 ps 3
set style line 5 lt rgb "#ED0CCB" lw 2 pt 5 ps 3
set style increment

set xlabel "Update Percentage"
set ylabel "Time (in ms)"
set xtics nomirror
set ytics nomirror
set key on left top
plot '$1.txt' using 1:2 with linespoints ls 1 notitle, \
	 '' using 1:2:3 ti "SI" with yerrorbars ls 1, \
     '' using 1:4 with linespoints ls 2 notitle, \
	 '' using 1:4:5 ti "SO" with yerrorbars ls 2, \
     '' using 1:6 with linespoints ls 3 notitle, \
	 '' using 1:6:7 ti "BT(I)" with yerrorbars ls 3, \
     '' using 1:8 with linespoints ls 4 notitle, \
	 '' using 1:8:9 ti "BT(O)" with yerrorbars ls 4, \
     '' using 1:10 with linespoints ls 5 notitle, \
	 '' using 1:10:11 ti "Random" with yerrorbars ls 5
EOF
