#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

class ArrayGenerator {
public:
    ArrayGenerator() {
        srand(time(nullptr));
    }

    std::vector<int> generateRandom(int size, int min_val = 0, int max_val = 6000) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = min_val + rand() % (max_val - min_val + 1);
        }
        return arr;
    }

    std::vector<int> generateReverseSorted(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = size - i;
        }
        return arr;
    }

    std::vector<int> generateAlmostSorted(int size, int swaps = 10) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; i++) {
            arr[i] = i + 1;
        }

        for (int i = 0; i < swaps; i++) {
            int idx1 = rand() % size;
            int idx2 = rand() % size;
            std::swap(arr[idx1], arr[idx2]);
        }
        return arr;
    }
};

void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> left_arr(n1);
    std::vector<int> right_arr(n2);

    for (int i = 0; i < n1; i++) {
        left_arr[i] = arr[left + i];
    }
    for (int i = 0; i < n2; i++) {
        right_arr[i] = arr[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k++] = left_arr[i++];
    }

    while (j < n2) {
        arr[k++] = right_arr[j++];
    }
}

void InsertionMergeSort(std::vector<int>& arr, int left, int right, int threshold = 15) {
    if (left >= right) {
        return;
    }

    if (right - left + 1 <= threshold) {
        insertionSort(arr, left, right);
        return;
    }

    int mid = left + (right - left) / 2;
    InsertionMergeSort(arr, left, mid, threshold);
    InsertionMergeSort(arr, mid + 1, right, threshold);
    merge(arr, left, mid, right);
}

double testSortTime(std::vector<int> arr, int threshold) {
    int runs = 3;
    double total_time = 0;

    for (int run = 0; run < runs; run++) {
        std::vector<int> test_arr = arr;
        auto start = std::chrono::high_resolution_clock::now();
        InsertionMergeSort(test_arr, 0, test_arr.size() - 1, threshold);
        auto end = std::chrono::high_resolution_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    return total_time / runs;
}

int main() {
    ArrayGenerator generator;

    std::vector<int> sizes;
    for (int size = 500; size <= 10000; size += 500) {
        sizes.push_back(size);
    }

    std::vector<int> thresholds = {5, 15, 30};

    std::cout << "Size,Random_5,Random_15,Random_30,Reverse_5,Reverse_15,Reverse_30,Almost_5,Almost_15,Almost_30\n";

    for (int size : sizes) {
        std::vector<int> randomArr = generator.generateRandom(size);
        std::vector<int> reverseArr = generator.generateReverseSorted(size);
        std::vector<int> almostArr = generator.generateAlmostSorted(size);

        std::cout << size;

        for (int threshold : thresholds) {
            double randomTime = testSortTime(randomArr, threshold);
            double reverseTime = testSortTime(reverseArr, threshold);
            double almostTime = testSortTime(almostArr, threshold);

            std::cout << "," << randomTime << "," << reverseTime << "," << almostTime;
        }
        std::cout << "\n";
    }

    return 0;
}