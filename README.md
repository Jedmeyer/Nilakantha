Nilakantha is an algorithm for calculating pi, and this implementation is a simple **multi-threaded** console app.
I know, very *fancy*. 

Nice little loading bar, and will continuously update the approximation as it converges on Pi.

## Dependencies:
Requires GMP (https://gmplib.org/) - the Multiple Precision Arithmetic Library.

Run `make install-deps` from command line to *try* installing it using the apt package manager.

## Building The Code:

Use `make` in the command line to build `Nila` - the executable.

## Running the Code:
Use `./Nila` in the console to run. `Nila` accepts one argument - the number of working threads. **Default is 1 thread**.

```
# Runs Nila using 10 working threads - probably shouldn't use more threads than your machine is capable of.
./Nila 10
# Starting Nila with 10 Threads
# Iteration: 391125 of 1e+08
# [>                                                                     ] 0 %
# Pi: 3.1415926535897932384626395319029935820014106877588426296528817871294269909633926426581827746218309629
```
