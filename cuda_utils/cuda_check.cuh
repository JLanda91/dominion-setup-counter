#pragma once

#include <fmt/core.h>

#include <cstdio>

#define CUDA_CHECK(expr, msg, ...) if (cudaError err = expr; err != cudaSuccess) { fmt::println(stderr, msg __VA_OPT__(,) __VA_ARGS__); fflush(stderr); exit(1); }