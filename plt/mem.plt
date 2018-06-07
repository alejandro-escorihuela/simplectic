# 09-05-2018
# alex
# mem.plt
reset
set term pdf
set output "../tex/img/mem1_1.pdf"
set key out vert
set key right
set xrange[-1:2.75]
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
plot "../dat/expl_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "1.1" w l ls 1, \
     "../dat/simp_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "1.2" w l ls 2, \
     "../dat/stor_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "1.3" w l ls 3, \
     "../dat/llib_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "1.4" w l ls 4

set output "../tex/img/mem2_1.pdf"
plot "../dat/llib_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "1.4" w l ls 4, \
     "../dat/rk-4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "2.1" w l ls 5, \
     "../dat/rkn4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "2.2" w l ls 6, \
     "../dat/rkg4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "2.3" w l ls 7

set output "../tex/img/mem3_1.pdf"
plot "../dat/rkg4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "2.3" w l ls 7, \
     "../dat/tjc4_ext18/err.dat" u (log10($1 / 365.25)):(log10(abs($3-$2) / $2)) t "3.1" w l ls 8

reset
set term pdf
set output "../tex/img/mem1_2.pdf"
unset key
unset tics
unset border
set style line 1  lt rgb '#f3f675'
set style line 2  lt rgb '#c2a677'
set style line 3  lt rgb '#e7d372'
set style line 4  lt rgb '#7ce5eb'
set style line 5  lt rgb '#609db0'
set style line 6  lt rgb '#acafb6'
set style line 11 lc rgb '#f3f675' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 12 lc rgb '#c2a677' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 13 lc rgb '#e7d372' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 14 lc rgb '#7ce5eb' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 15 lc rgb '#609db0' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 16 lc rgb '#acafb6' lt 1 lw 2 pt 7 pi -1 ps 1.5
#set title "Mètode explícit d'Euler"
LIN0 =`wc -l '../dat/expl_ext18/err.dat' | cut -d' ' -f1`
LIN1 = LIN0 - 1
arxiu(i, f, pl) = sprintf("<(sed -n '%d,%dp' ../dat/expl_ext18/%s.dat)", i, f, pl)
splot arxiu(1, LIN0, "sol")    u 2:3:4 w l ls 1, \
      arxiu(1, LIN0, "jup")    u 2:3:4 w l ls 2, \
      arxiu(1, LIN0, "sat")    u 2:3:4 w l ls 3, \
      arxiu(1, LIN0, "ura")    u 2:3:4 w l ls 4, \
      arxiu(1, LIN0, "nep")    u 2:3:4 w l ls 5, \
      arxiu(1, LIN0, "plu")    u 2:3:4 w l ls 6, \
      arxiu(LIN0, LIN1, "sol") u 2:3:4 w lp ls 11, \
      arxiu(LIN0, LIN1, "jup") u 2:3:4 w lp ls 12, \
      arxiu(LIN0, LIN1, "sat") u 2:3:4 w lp ls 13, \
      arxiu(LIN0, LIN1, "ura") u 2:3:4 w lp ls 14, \
      arxiu(LIN0, LIN1, "nep") u 2:3:4 w lp ls 15, \
      arxiu(LIN0, LIN1, "plu") u 2:3:4 w lp ls 16, \
      arxiu(LIN0, LIN1, "jup") u 2:3:4:("j") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "sat") u 2:3:4:("s") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "ura") u 2:3:4:("u") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "nep") u 2:3:4:("n") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "plu") u 2:3:4:("p") with labels textcolor "white"

set output "../tex/img/mem1_3.pdf"
LIN0 =`wc -l '../dat/simp_ext18/err.dat' | cut -d' ' -f1`
LIN1 = LIN0 - 1
arxiu(i, f, pl) = sprintf("<(sed -n '%d,%dp' ../dat/simp_ext18/%s.dat)", i, f, pl)
splot arxiu(1, LIN0, "sol")    u 2:3:4 w l ls 1, \
      arxiu(1, LIN0, "jup")    u 2:3:4 w l ls 2, \
      arxiu(1, LIN0, "sat")    u 2:3:4 w l ls 3, \
      arxiu(1, LIN0, "ura")    u 2:3:4 w l ls 4, \
      arxiu(1, LIN0, "nep")    u 2:3:4 w l ls 5, \
      arxiu(1, LIN0, "plu")    u 2:3:4 w l ls 6, \
      arxiu(LIN0, LIN1, "sol") u 2:3:4 w lp ls 11, \
      arxiu(LIN0, LIN1, "jup") u 2:3:4 w lp ls 12, \
      arxiu(LIN0, LIN1, "sat") u 2:3:4 w lp ls 13, \
      arxiu(LIN0, LIN1, "ura") u 2:3:4 w lp ls 14, \
      arxiu(LIN0, LIN1, "nep") u 2:3:4 w lp ls 15, \
      arxiu(LIN0, LIN1, "plu") u 2:3:4 w lp ls 16, \
      arxiu(LIN0, LIN1, "jup") u 2:3:4:("j") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "sat") u 2:3:4:("s") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "ura") u 2:3:4:("u") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "nep") u 2:3:4:("n") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "plu") u 2:3:4:("p") with labels textcolor "white"