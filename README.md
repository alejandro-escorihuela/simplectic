# solar
C programs to check numerical methods applied to the outer Solar System and Pluto
![simulacio](./img.png)
## Getting Started
For compile this programs you need the gcc libquadmath if you want quad-precision. In Debian GNU/Linux you can do this:
```
apt install libquadmath0
```
For compile the codes you only do:
```
make
```
# Configuration
In the cnf directory there are configuration files for Sun, planets and Pluto. These files has the name, mass, position (x, y, z) and speed (x, y, z). There is another file call param.cnf for some parameters of the execution.
## Running
You can run any program with:
```
./02_simp 10
```
this execute the "simp" program with h=10 and the results will be written in dat folder. You can use the scripts in "plt" folder for graphing these results.
## Methods
The methods used in these programs are:
* 01_expl -> Explicit Euler method.
* 02_simp -> Simplectic Euler VT method.
* 03_stor -> Störmer-Verlet method.
* 04_llib -> Störmer-Verlet method with modified potential.
* 05_rk-4 -> *The* Runge-Kutta-4 method.
* 06_rkn4 -> Runge-Kutta-Nyström-4 method.
* 07_rkg4 -> Runge-Kutta-Gauss-Legendre-4 method.
* 08_tjc4 -> Symmetric composition of symmetric second-order methods: 4th order, 3 steps (Triple Jump Composition).
* 09_ss45 -> Symmetric composition of symmetric second-order methods: 4th order, 5 steps.
* 10_ss69 -> Symmetric composition of symmetric second-order methods: 6th order, 9 steps.
* 11_ss817 -> Symmetric composition of symmetric second-order methods: 8th order, 17 steps.
* 12_s46, 13_s46s -> Composition of method and adjoint: 4th order, 6 steps.
* 14_nb46 -> 4th order Runge-Kutta-Nyström 6 steps.
* 15_nia2 -> (4, 2) order near integrable method with 2 steps.
* 16_nia5 -> (8, 4) near integrable method with 5 steps.
* 17_nia8 -> (10, 6, 4) near integrable method with 8 steps.

## References
* S. Blanes and F. Casas: A Concise Introduction to Geometric Numerical Integration. CRC Press.
* E. Hairer, Ch. Lubich, and G. Wanner: Geometric Numerical Integration. Structure-Preserving Algorithms for Ordinary Differential Equations, Springer. 

## Autors
* **Cosmo Cat**  [cosmogat](https://github.com/cosmogat)
## License
See the [LICENSE](LICENSE)
