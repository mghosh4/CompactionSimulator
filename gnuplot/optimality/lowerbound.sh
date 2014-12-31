#!/bin/sh
gnuplot << EOF
set terminal pngcairo font 'Times,25' linewidth 2 size 15in,10in 
set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey
set grid back linestyle 81

set pointsize 2
set output "lowerbound-$1.png"
set style line 1 lt rgb "#A00000" lw 2 pt 2 ps 3
set style line 2 lt rgb "#00A000" lw 2 pt 9 ps 3
set style line 3 lt rgb "#5060D0" lw 2 pt 1 ps 3
set style line 4 lt rgb "#F25900" lw 2 pt 6 ps 3
set style line 5 lt rgb "#ED0CCB" lw 2 pt 5 ps 3
set style line 6 lt rgb "#686E6B" lw 2 pt 3 ps 3
set style increment

set xlabel "Memtable Size"
set ylabel "Cost"
set yrange[100:*]
set logscale xy
set xtics nomirror
set ytics nomirror
set key on right bottom
plot '$1.txt' using 1:2 ti "BT(I) - Uniform" with linespoints ls 1, \
     '' using 1:3 ti "Lower Bound - Uniform" with linespoints ls 2, \
	 '' using 1:4 ti "BT(I) - Zipfian" with linespoints ls 3, \
     '' using 1:5 ti "Lower Bound - Zipfian" with linespoints ls 4, \
	 '' using 1:6 ti "BT(I) - Latest" with linespoints ls 6, \
     '' using 1:7 ti "Lower Bound - Latest" with linespoints ls 5
EOF
