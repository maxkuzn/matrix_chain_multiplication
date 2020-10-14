#include <benchmark/benchmark.h>
#include <random>

#include <matrix/matrix.h>

static Matrix gen_random_matrix(size_t rows, size_t columns) {
  static std::mt19937 gen(42);
  std::uniform_real_distribution<double> dist(-1.0, 1.0);
  Matrix m(rows, columns);
  for (size_t i = 0; i != rows; ++i) {
    for (size_t j = 0; j != columns; ++j) {
      m(i, j) = dist(gen);
    }
  }
  return m;
}


static void BM_MatrixMultiplication(benchmark::State& state) {
  auto a = gen_random_matrix(state.range(0), state.range(1));
  auto b = gen_random_matrix(state.range(1), state.range(2));
  for (auto _ : state) {
    benchmark::DoNotOptimize(a * b);
  }
  state.counters["float multiplications"] = a.rows() * b.rows() * b.columns();
}
// Register the function as a benchmark
BENCHMARK(BM_MatrixMultiplication)
  ->Args({10, 10, 10})
  ->Args({256, 256, 256})
  ->Args({1024, 16, 16})
  ->Args({16, 1024, 16})
  ->Args({16, 16, 1024});

// Run the benchmark
BENCHMARK_MAIN();

