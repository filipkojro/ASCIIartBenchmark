# ASCIIartBenchmark

Simpler version of ASCIIart for benchmark only.

## Building
```bash
cmake -B build .
cmake --build build/
./build/ASCIIartBenchmark
```

## Running 
* `./build/ASCIIart --core (number)` number of threads to use (default - number of threads in cpu)
* `./build/ASCIIart --test (number)` number of tests in one run (default - 20)
* `./build/ASCIIart --size (number) (number)` width and height of proccesed image (default - 4000 2000)
