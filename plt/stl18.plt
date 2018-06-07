# 30-04-2018
# alex
# stl18.plt
reset
set term pdf
set output "../graf/stl18.pdf"
unset key
unset tics
unset border
set style line 1  lt rgb '#f3f675'
set style line 2  lt rgb '#7ce5eb'
set style line 3  lt rgb '#acafb6'
set style line 11 lc rgb '#f3f675' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 12 lc rgb '#7ce5eb' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 13 lc rgb '#acafb6' lt 1 lw 2 pt 7 pi -1 ps 1.5
set title "Mètode explícit d'Euler"
LIN0 =`wc -l '../dat/expl_stl18/err.dat' | cut -d' ' -f1`
LIN1 = LIN0 - 1
arxiu(i, f, pl) = sprintf("<(sed -n '%d,%dp' ../dat/expl_stl18/%s.dat)", i, f, pl)
splot arxiu(1, LIN0, "sol")    u 2:3:4 w l ls 1, \
      arxiu(1, LIN0, "ter")    u 2:3:4 w l ls 2, \
      arxiu(1, LIN0, "llu")    u 2:3:4 w l ls 3, \
      arxiu(LIN0, LIN1, "sol") u 2:3:4 w lp ls 11, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4 w lp ls 12, \
      arxiu(LIN0, LIN1, "llu") u 2:3:4 w lp ls 13, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4:("t") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "llu") u 2:3:4:("l") with labels textcolor "white"
set title "Mètode simplèctic d'Euler TV"
LIN0 =`wc -l '../dat/simp_stl18/err.dat' | cut -d' ' -f1`
LIN1 = LIN0 - 1
arxiu(i, f, pl) = sprintf("<(sed -n '%d,%dp' ../dat/simp_stl18/%s.dat)", i, f, pl)
splot arxiu(1, LIN0, "sol")    u 2:3:4 w l ls 1, \
      arxiu(1, LIN0, "ter")    u 2:3:4 w l ls 2, \
      arxiu(1, LIN0, "llu")    u 2:3:4 w l ls 3, \
      arxiu(LIN0, LIN1, "sol") u 2:3:4 w lp ls 11, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4 w lp ls 12, \
      arxiu(LIN0, LIN1, "llu") u 2:3:4 w lp ls 13, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4:("t") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "llu") u 2:3:4:("l") with labels textcolor "white"
set title "Mètode Störmer-Verlet"
LIN0 =`wc -l '../dat/stor_stl18/err.dat' | cut -d' ' -f1`
LIN1 = LIN0 - 1
arxiu(i, f, pl) = sprintf("<(sed -n '%d,%dp' ../dat/stor_stl18/%s.dat)", i, f, pl)
splot arxiu(1, LIN0, "sol")    u 2:3:4 w l ls 1, \
      arxiu(1, LIN0, "ter")    u 2:3:4 w l ls 2, \
      arxiu(1, LIN0, "llu")    u 2:3:4 w l ls 3, \
      arxiu(LIN0, LIN1, "sol") u 2:3:4 w lp ls 11, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4 w lp ls 12, \
      arxiu(LIN0, LIN1, "llu") u 2:3:4 w lp ls 13, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4:("t") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "llu") u 2:3:4:("l") with labels textcolor "white"
set title "Mètode llibre pàgina 31"
LIN0 =`wc -l '../dat/llib_stl18/err.dat' | cut -d' ' -f1`
LIN1 = LIN0 - 1
arxiu(i, f, pl) = sprintf("<(sed -n '%d,%dp' ../dat/llib_stl18/%s.dat)", i, f, pl)
splot arxiu(1, LIN0, "sol")    u 2:3:4 w l ls 1, \
      arxiu(1, LIN0, "ter")    u 2:3:4 w l ls 2, \
      arxiu(1, LIN0, "llu")    u 2:3:4 w l ls 3, \
      arxiu(LIN0, LIN1, "sol") u 2:3:4 w lp ls 11, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4 w lp ls 12, \
      arxiu(LIN0, LIN1, "llu") u 2:3:4 w lp ls 13, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4:("t") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "llu") u 2:3:4:("l") with labels textcolor "white"
set title "Mètode Runge-Kutta 4"
LIN0 =`wc -l '../dat/rk-4_stl18/err.dat' | cut -d' ' -f1`
LIN1 = LIN0 - 1
arxiu(i, f, pl) = sprintf("<(sed -n '%d,%dp' ../dat/rk-4_stl18/%s.dat)", i, f, pl)
splot arxiu(1, LIN0, "sol")    u 2:3:4 w l ls 1, \
      arxiu(1, LIN0, "ter")    u 2:3:4 w l ls 2, \
      arxiu(1, LIN0, "llu")    u 2:3:4 w l ls 3, \
      arxiu(LIN0, LIN1, "sol") u 2:3:4 w lp ls 11, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4 w lp ls 12, \
      arxiu(LIN0, LIN1, "llu") u 2:3:4 w lp ls 13, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4:("t") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "llu") u 2:3:4:("l") with labels textcolor "white"
set title "Mètode Runge-Kutta-Nyström 4"
LIN0 =`wc -l '../dat/rkn4_stl18/err.dat' | cut -d' ' -f1`
LIN1 = LIN0 - 1
arxiu(i, f, pl) = sprintf("<(sed -n '%d,%dp' ../dat/rkn4_stl18/%s.dat)", i, f, pl)
splot arxiu(1, LIN0, "sol")    u 2:3:4 w l ls 1, \
      arxiu(1, LIN0, "ter")    u 2:3:4 w l ls 2, \
      arxiu(1, LIN0, "llu")    u 2:3:4 w l ls 3, \
      arxiu(LIN0, LIN1, "sol") u 2:3:4 w lp ls 11, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4 w lp ls 12, \
      arxiu(LIN0, LIN1, "llu") u 2:3:4 w lp ls 13, \
      arxiu(LIN0, LIN1, "ter") u 2:3:4:("t") with labels textcolor "white", \
      arxiu(LIN0, LIN1, "llu") u 2:3:4:("l") with labels textcolor "white"