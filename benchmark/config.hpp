#ifndef VSET_BENCHMARK_CONFIG_HPP
#define VSET_BENCHMARK_CONFIG_HPP

#ifdef NDEBUG
const int MAX_COUNT = 10240000;
const int MIN_COUNT = 10000;
const int CHUNK_SIZE = 512;
#else
const int MAX_COUNT = 1024;
const int MIN_COUNT = 16;
const int CHUNK_SIZE = 512;
#endif


#endif
