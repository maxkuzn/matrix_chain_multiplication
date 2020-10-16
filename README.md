# matrix_chain_multiplication
Project for the course Scientific Computing.  
Implemented algorithm for fast [matrix chain multiplication](https://en.wikipedia.org/wiki/Matrix_chain_multiplication).  


# Installation
```
# Checkout library
$ git clone --recursive https://github.com/maxkuzn/matrix_chain_multiplication.git
# Go to root directory
$ cd matrix_chain_multiplication
# Create directory for build
$ mkdir build && cd build
# Generate Makefiles
$ cmake ../
# Build library
$ make
```

# Run tests and benchmarks
```
# From root directory
$ ./build/tests/bin/<test_name>
$ ./build/benchmarsk/bin/<benchmark_name>
```

# Visualization
You can visualize the multiplication tree in show_tree.ipynb.  
It requires graphviz library.  
