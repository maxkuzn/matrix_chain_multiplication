#include <benchmark/benchmark.h>

#include <matrix/matrix.h>
#include <matrix/chain_multiplication.h>
#include <matrix/utils.h>

// Some data for benchmarks

static std::vector<size_t> one_matrix{42, 42};

static std::vector<size_t> five_matrices{42, 42, 42, 42, 42};

static std::vector<size_t> small_big_small{10, 100, 10, 1000, 10, 100, 10};

static std::vector<size_t> from_small_to_big{
  1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
};

static std::vector<size_t> from_big_to_small{
  1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1
};

static std::vector<size_t> chain_10{
  32, 93, 468, 49, 473, 274, 264, 406, 300, 9, 352
};

static std::vector<size_t> chain_100{
  5, 84, 2, 117, 95, 2, 124, 81, 30, 165, 98, 22, 200, 93, 189, 9, 191, 100, 7,
  41, 78, 131, 9, 139, 70, 8, 120, 167, 39, 122, 43, 177, 28, 87, 116, 3, 185,
  177, 62, 129, 107, 101, 191, 38, 164, 102, 40, 26, 120, 183, 161, 144, 1, 108,
  43, 144, 192, 186, 9, 2, 97, 169, 7, 68, 143, 115, 10, 21, 194, 30, 66, 15, 191,
  137, 192, 139, 48, 41, 26, 47, 90, 121, 13, 5, 139, 73, 77, 7, 136, 123, 176,
  137, 177, 135, 183, 153, 71, 167, 28, 52, 9
};

static std::vector<size_t> chain_1000{
  70, 47, 30, 94, 57, 82, 26, 83, 88, 13, 74, 76, 32, 70, 46, 70, 8, 93, 66, 92,
  10, 97, 4, 74, 70, 94, 68, 69, 81, 62, 72, 86, 52, 23, 1, 48, 65, 97, 52, 35,
  95, 2, 76, 45, 91, 89, 9, 17, 57, 12, 55, 60, 24, 29, 27, 12, 70, 53, 70, 35,
  34, 99, 4, 1, 6, 61, 98, 78, 25, 63, 100, 52, 81, 32, 58, 47, 62, 92, 86, 51,
  59, 73, 14, 82, 67, 52, 58, 79, 7, 48, 81, 25, 79, 58, 81, 46, 34, 64, 78, 81,
  83, 99, 77, 31, 20, 64, 52, 47, 69, 4, 80, 97, 45, 99, 43, 35, 31, 62, 98, 9,
  60, 54, 65, 57, 72, 92, 41, 5, 79, 31, 66, 52, 25, 8, 22, 93, 1, 37, 11, 50,
  52, 73, 18, 95, 57, 13, 66, 83, 84, 80, 74, 8, 98, 50, 64, 65, 63, 55, 63, 25,
  33, 54, 98, 44, 19, 15, 29, 30, 66, 14, 7, 67, 2, 2, 74, 4, 39, 69, 20, 88, 63,
  44, 33, 71, 63, 36, 40, 7, 17, 1, 87, 66, 71, 100, 67, 58, 27, 58, 75, 42, 14,
  66, 54, 65, 82, 97, 33, 67, 39, 100, 64, 58, 48, 10, 56, 100, 39, 26, 31, 65,
  66, 13, 30, 47, 50, 55, 38, 28, 22, 28, 34, 36, 26, 19, 55, 5, 28, 90, 58, 84,
  42, 51, 79, 25, 43, 39, 43, 62, 34, 11, 58, 11, 76, 8, 11, 27, 87, 63, 91, 30,
  72, 89, 48, 75, 90, 30, 51, 80, 46, 86, 76, 59, 36, 92, 55, 38, 56, 20, 97, 35,
  2, 42, 60, 25, 57, 87, 37, 68, 93, 58, 96, 11, 67, 87, 12, 51, 93, 49, 14, 97,
  79, 60, 32, 91, 42, 21, 100, 24, 53, 27, 41, 50, 71, 94, 94, 7, 63, 13, 22, 80,
  95, 49, 37, 42, 17, 9, 24, 75, 44, 35, 30, 71, 28, 18, 40, 27, 83, 89, 30, 4,
  79, 63, 97, 60, 51, 22, 13, 13, 42, 18, 4, 22, 22, 93, 96, 83, 14, 100, 83, 97,
  7, 88, 78, 95, 95, 40, 19, 99, 73, 85, 13, 6, 23, 69, 50, 42, 96, 7, 17, 62, 89,
  67, 52, 34, 20, 42, 64, 76, 86, 95, 38, 38, 86, 95, 86, 72, 96, 53, 98, 50, 53,
  87, 37, 81, 50, 71, 41, 2, 64, 72, 88, 25, 89, 13, 13, 92, 40, 87, 82, 32, 46,
  1, 86, 20, 68, 23, 46, 69, 99, 98, 97, 74, 63, 17, 89, 96, 19, 48, 68, 47, 44,
  52, 78, 46, 37, 27, 22, 83, 77, 76, 18, 67, 14, 49, 87, 23, 83, 13, 2, 44, 79,
  52, 78, 53, 97, 4, 92, 68, 56, 62, 12, 98, 43, 90, 71, 67, 32, 79, 33, 93, 2,
  33, 77, 91, 27, 73, 91, 28, 74, 78, 87, 60, 25, 41, 85, 48, 12, 84, 51, 11, 97,
  39, 68, 43, 39, 63, 85, 15, 63, 94, 80, 59, 66, 28, 97, 38, 75, 43, 62, 100, 65,
  69, 11, 81, 86, 99, 52, 16, 64, 8, 34, 80, 99, 83, 1, 32, 9, 63, 82, 49, 69, 80,
  3, 43, 1, 41, 62, 29, 25, 49, 48, 83, 74, 5, 27, 34, 17, 5, 55, 38, 80, 56, 94,
  30, 67, 7, 22, 1, 2, 42, 40, 14, 82, 11, 38, 60, 49, 6, 75, 84, 48, 70, 64, 76,
  86, 56, 59, 90, 79, 79, 19, 90, 95, 28, 5, 39, 80, 30, 81, 40, 74, 43, 98, 40,
  26, 86, 68, 12, 58, 61, 53, 36, 64, 39, 49, 45, 1, 74, 42, 16, 45, 9, 80, 72, 67,
  40, 63, 10, 52, 31, 66, 49, 79, 16, 58, 73, 62, 56, 15, 78, 92, 32, 5, 34, 65,
  68, 31, 91, 15, 9, 14, 83, 34, 3, 93, 42, 15, 17, 42, 48, 95, 99, 18, 71, 44,
  62, 53, 60, 70, 39, 57, 34, 33, 3, 57, 41, 7, 98, 92, 81, 44, 95, 66, 15, 70,
  99, 18, 55, 96, 18, 94, 97, 76, 18, 53, 21, 19, 19, 22, 36, 86, 38, 52, 6, 35,
  88, 29, 13, 88, 28, 23, 73, 70, 100, 64, 4, 59, 37, 92, 16, 3, 14, 63, 8, 36,
  30, 99, 19, 90, 37, 16, 78, 69, 80, 1, 48, 70, 5, 10, 40, 20, 65, 11, 67, 20,
  25, 20, 58, 71, 96, 14, 63, 86, 26, 51, 23, 25, 10, 55, 80, 55, 97, 17, 81, 2,
  35, 1, 48, 43, 88, 71, 13, 62, 81, 7, 20, 9, 64, 7, 87, 2, 71, 87, 16, 86, 100,
  54, 77, 22, 83, 5, 66, 48, 63, 52, 48, 82, 55, 39, 56, 17, 20, 62, 3, 97, 78,
  50, 81, 62, 77, 88, 24, 20, 84, 24, 35, 27, 23, 67, 96, 82, 63, 69, 33, 80, 56,
  42, 67, 29, 61, 98, 86, 73, 84, 20, 84, 14, 42, 51, 90, 6, 60, 57, 79, 48, 25,
  56, 79, 73, 71, 74, 5, 16, 89, 91, 30, 97, 91, 67, 80, 14, 41, 90, 99, 22, 63,
  94, 64, 26, 4, 49, 39, 21, 6, 30, 28, 65, 45, 41, 21, 55, 99, 23, 70, 55, 53,
  46, 44, 34, 87, 73, 66, 8, 97, 83, 59, 96, 84, 54, 22, 35, 23, 52, 8, 49, 1, 64,
  44, 14, 59, 81, 75, 18, 75, 19, 7, 46, 53, 19, 31, 40, 17, 98, 88, 99, 17, 35,
  24, 4, 83, 62, 72, 22, 26, 24, 92, 25, 60, 9, 27, 21, 4, 16, 85, 33, 83, 63, 75,
  36, 48, 61, 96, 31, 45, 68, 72, 71, 38, 46, 37, 87, 62, 45, 24, 25, 63, 20, 12,
  72, 95, 47, 67, 3, 98, 43, 49, 74, 19, 62, 52, 98, 81, 61, 58, 87, 53, 28, 78,
  74, 70, 80, 40, 37, 69, 91, 100, 29, 73, 13, 52, 50
};


// Benchmarks
static void BM_SimpleChainConstruction(benchmark::State& state,
                                       const std::vector<size_t>& dims) {
  auto matrices = gen_random_matrix_chain(dims);
  for (auto _ : state) {
    auto result = simple_chain_order(matrices);
    benchmark::DoNotOptimize(result);
  }
  auto result = simple_chain_order(matrices);
  state.counters["float multiplications"] = result->number_of_float_multiplications();
}

static void BM_OptimalChainConstruction(benchmark::State& state,
                                        const std::vector<size_t>& dims) {
  auto matrices = gen_random_matrix_chain(dims);
  for (auto _ : state) {
    auto result = optimal_chain_order(matrices);
    benchmark::DoNotOptimize(result);
  }
  auto result = optimal_chain_order(matrices);
  state.counters["float multiplications"] = result->number_of_float_multiplications();
}

static void BM_SimpleChainEvaluation(benchmark::State& state,
                                     const std::vector<size_t>& dims) {
  auto matrices = gen_random_matrix_chain(dims);
  auto result = simple_chain_order(matrices);
  for (auto _ : state) {
    benchmark::DoNotOptimize(result->get());
  }
  state.counters["float multiplications"] = result->number_of_float_multiplications();
}

static void BM_OptimalChainEvaluation(benchmark::State& state,
                                      const std::vector<size_t>& dims) {
  auto matrices = gen_random_matrix_chain(dims);
  auto result = optimal_chain_order(matrices);
  for (auto _ : state) {
    benchmark::DoNotOptimize(result->get());
  }
  state.counters["float multiplications"] = result->number_of_float_multiplications();
}

// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_SimpleChainConstruction, one_matrix, one_matrix);
BENCHMARK_CAPTURE(BM_OptimalChainConstruction, one_matrix, one_matrix);
BENCHMARK_CAPTURE(BM_SimpleChainEvaluation, one_matrix, one_matrix);
BENCHMARK_CAPTURE(BM_OptimalChainEvaluation, one_matrix, one_matrix);

BENCHMARK_CAPTURE(BM_SimpleChainConstruction, five_matrices, five_matrices);
BENCHMARK_CAPTURE(BM_OptimalChainConstruction, five_matrices, five_matrices);
BENCHMARK_CAPTURE(BM_SimpleChainEvaluation, five_matrices, five_matrices);
BENCHMARK_CAPTURE(BM_OptimalChainEvaluation, five_matrices, five_matrices);

BENCHMARK_CAPTURE(BM_SimpleChainConstruction, small_big_small, small_big_small);
BENCHMARK_CAPTURE(BM_OptimalChainConstruction, small_big_small, small_big_small);
BENCHMARK_CAPTURE(BM_SimpleChainEvaluation, small_big_small, small_big_small)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainEvaluation, small_big_small, small_big_small)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_SimpleChainConstruction, from_small_to_big, from_small_to_big);
BENCHMARK_CAPTURE(BM_OptimalChainConstruction, from_small_to_big, from_small_to_big);
BENCHMARK_CAPTURE(BM_SimpleChainEvaluation, from_small_to_big, from_small_to_big)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainEvaluation, from_small_to_big, from_small_to_big)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_SimpleChainConstruction, from_big_to_small, from_big_to_small);
BENCHMARK_CAPTURE(BM_OptimalChainConstruction, from_big_to_small, from_big_to_small);
BENCHMARK_CAPTURE(BM_SimpleChainEvaluation, from_big_to_small, from_big_to_small)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainEvaluation, from_big_to_small, from_big_to_small)
  ->Unit(benchmark::kMillisecond);


BENCHMARK_CAPTURE(BM_SimpleChainConstruction, chain_10, chain_10)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainConstruction, chain_10, chain_10)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_SimpleChainEvaluation, chain_10, chain_10)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainEvaluation, chain_10, chain_10)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_SimpleChainConstruction, chain_100, chain_100)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainConstruction, chain_100, chain_100)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_SimpleChainEvaluation, chain_100, chain_100)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainEvaluation, chain_100, chain_100)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_SimpleChainConstruction, chain_1000, chain_1000)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainConstruction, chain_1000, chain_1000)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_SimpleChainEvaluation, chain_1000, chain_1000)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_OptimalChainEvaluation, chain_1000, chain_1000)
  ->Unit(benchmark::kMillisecond);

// Run the benchmark
BENCHMARK_MAIN();

