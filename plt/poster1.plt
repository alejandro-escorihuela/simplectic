# 09-07-2018
# alex
# poster1.plt
reset
set term pdf
set output "../../poster/graf/1.pdf"
set palette defined (0 '#000090', 1 '#000FFF', 2 '#0090FF', 3 '#0FFFEE', 4 '#90FF70', 5 '#FFEE00', 6 '#FF7000', 7 '#EE0000', 8 '#7F0000')
# set grid
set style line 1  lt rgb '#0060AD'
set style line 2  lt rgb '#60AD00'
set style line 3  lt rgb '#AD0B00'
set style line 4  lt rgb '#E7E730'
set style line 5  lt rgb '#E730E7'
set style line 6  lt rgb '#30E7E7'
set style line 7  lt rgb '#707070'
set style line 8  lt rgb '#FF8E1E'
set style line 9  lt rgb '#202020'
set style line 10 lt rgb '#F000F0'
set xrange[0:500]
plot "../dat/01_expl_ext18/err.dat" u ($1 / 365.25):(log10(abs($3-$2) / $2)) w l ls 1 lw 2 t "ex", \
     "../dat/02_simp_ext18/err.dat" u ($1 / 365.25):(log10(abs($3-$2) / $2)) w l ls 2 lw 2 t "si", \
     "../dat/03_stor_ext18/err.dat" u ($1 / 365.25):(log10(abs($3-$2) / $2)) w l ls 3 lw 2 t "sv", \
     "../dat/05_rk-4_ext18/err.dat" u ($1 / 365.25):(log10(abs($3-$2) / $2)) w l ls 4 lw 2 t "rk"