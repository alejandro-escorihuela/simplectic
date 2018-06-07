# 26-04-2018
# alex
# err_ext18.plt
reset
set term pdf
set output "../graf/err_ext18.pdf"
# set format x "%.0sx10^{%T}"
# set xtics 5e4
set key out vert
set key right
#set xrange[0:550]
set xrange[-1:2.75]
set title "Error en l'energia per al Sistema Solar exterior i Plutó (t_0 = 2018)"
set ylabel "log((H-H_0)/H_0)"
set xlabel "log(t) (anys)"
set style line 1  lt rgb '#0060AD'
set style line 2  lt rgb '#60AD00'
set style line 3  lt rgb '#AD0B00'
set style line 4  lt rgb '#E7E730'
set style line 5  lt rgb '#c2a677'
set style line 6  lt rgb '#e7d372'
set style line 7  lt rgb '#7ce5eb'
set style line 8  lt rgb '#609db0'
set style line 9  lt rgb '#acafb6'

plot "../dat/expl_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "expl" w l ls 1, \
     "../dat/simp_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "simp" w l ls 2, \
     "../dat/stor_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "stor" w l ls 3, \
     "../dat/llib_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "llib" w l ls 4, \
     "../dat/rk-4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "rk4" w l ls 5, \
     "../dat/rkn4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "rkn4" w l ls 6, \
     "../dat/rkg4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "rkgl4" w l ls 7, \
     "../dat/tjc4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "tjc4" w l ls 8