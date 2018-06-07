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
./simp 10
```
this execute the "simp" program with h=10 and the results will be written in dat folder. You can use the scripts in "plt" folder for graphing these results.
## Methods
The methods used in these programs are:
* expl -> Explicit Euler method.
* simp -> Simplectic Euler VT method.
* stor -> Störmer-Verlet method.
* llib -> Störmer-Verlet method with modified potential.
* rk-4 -> *The* Runge-Kutta-4 method.
* rkn4 -> Runge-Kutta-Nyström-4 method.
* rkg4 -> Runge-Kutta-Gauss-Legendre-4 method.
* tjc4 -> Triple jump composition 4th order method.
## Autors
* **Cosmo Cat**  [cosmogat](https://github.com/cosmogat)
## License
See the [LICENSE.md](LICENSE.md)
