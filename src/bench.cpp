#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

#include "red_black_tree.hpp"

void runBenchmark(int n) {
	std::vector<int> randomData(n);
	for (int i = 0; i < n; ++i) randomData[i] = i;

	// shuffle data for average case
	auto rng = std::default_random_engine{};
	shuffle(randomData.begin(), randomData.end(), rng);

	RedBlackTree<int> tree;

	// measure insertion
	auto start = std::chrono::high_resolution_clock::now();
	for (int val : randomData) {
		tree.insert(val);
	}
	auto end			= std::chrono::high_resolution_clock::now();
	auto durationInsert = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	// measure randomized search
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; ++i) {
		tree.exists(randomData[i % n]);
	}
	end					= std::chrono::high_resolution_clock::now();
	auto durationSearch = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	std::cout << std::left << std::setw(10) << n
			  << std::left << std::setw(20) << durationInsert.count()
			  << std::left << std::setw(20) << durationSearch.count() / 1000.0 << " microseconds/op" << std::endl;
}

int main() {
	std::cout << "Performance Analysis: Red-Black Tree" << std::endl;
	std::cout << "--------------------------------------------------------" << std::endl;
	std::cout << std::left << std::setw(10) << "Size"
			  << std::left << std::setw(20) << "Total Insert (microseconds)"
			  << std::left << std::setw(20) << "Avg Search (microseconds)" << std::endl;
	std::cout << "--------------------------------------------------------" << std::endl;

	std::vector<int> testSizes = {1000, 10000, 50000, 100000, 500000};

	for (int size : testSizes) {
		runBenchmark(size);
	}

	return 0;
}
