#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <iomanip>

std::pair<double, int> measure_time_and_comparisons(std::function<int(std::vector<int>&)> sort_func, std::vector<int>& array) {
    auto start_time = std::chrono::high_resolution_clock::now();
    int comparisons = sort_func(array);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> execution_time = end_time - start_time;
    return {execution_time.count(), comparisons};
}

std::vector<int> almost_sorted_array(int size, double percent) {
    std::vector<int> sorted_array(size);
    std::iota(sorted_array.begin(), sorted_array.end(), 0);
    int num_swaps = static_cast<int>((percent / 100) * size);
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int swap = 0; swap < num_swaps; ++swap) {
        std::uniform_int_distribution<> dis(0, size - 1);
        int i = dis(gen), j = dis(gen);
        std::swap(sorted_array[i], sorted_array[j]);
    }
    return sorted_array;
}

std::vector<int> randomly_distributed_array(int size) {
    std::vector<int> array(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000); // Change range as needed
    for (int& value : array) {
        value = dis(gen);
    }
    return array;
}

std::vector<int> reverse_sorted_array(int size) {
    std::vector<int> array(size);
    std::generate(array.rbegin(), array.rend(), [n = 0] () mutable { return n++; });
    return array;
}

std::vector<int> array_with_duplicates(int size, double duplicate_chance) {
    std::vector<int> array;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_int_distribution<> dis_int(0, size);
    for (int i = 0; i < size; ++i) {
        if (dis(gen) < duplicate_chance && !array.empty()) {
            array.push_back(array[dis_int(gen) % array.size()]);
        } else {
            array.push_back(dis_int(gen));
        }
    }
    return array;
}

int bubble_sort(std::vector<int>& arr) {
    int comparisons = 0;
    bool swapped;

    for (int i = 0; i < arr.size() - 1; i++) {
        swapped = false;
        for (int j = 0; j < arr.size() - i - 1; j++) {
            comparisons += 1;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
    return comparisons;
}


int main(){
    std::vector<int> array_sizes = {1000, 10000, 25000};
    double percent_almost_sorted = 10;
    double chance_duplicates = 0.1;
    std::vector<std::pair<std::string, std::function<std::vector<int>(int)>>> datasets = {
            {"Almost Sorted", [percent_almost_sorted](int size) {
                return almost_sorted_array(size, percent_almost_sorted);
            }},
            {"Randomly Distributed", randomly_distributed_array},
            {"Reverse Sorted", reverse_sorted_array},
            {"Duplicates", [chance_duplicates](int size) {
                return array_with_duplicates(size, chance_duplicates);
            }}
    };

    for (int size: array_sizes) {
        std::cout << "\nTesting with array size: " << size << std::endl;
        for (auto &[dataset_name, generate_func]: datasets) {
            std::cout << "\nDataset: " << dataset_name << std::endl;
            std::vector<int> r_array = generate_func(size);
            auto [execution_time, comparisons] = measure_time_and_comparisons(bubble_sort, r_array);
            // Round execution time to 6 decimal places for microseconds
            std::cout << "Execution Time: " << std::fixed << std::setprecision(6) << execution_time << " seconds"
                      << std::endl;
            std::cout << "Number of Comparisons: " << comparisons << std::endl;
        }
    }
    return 0;
}
