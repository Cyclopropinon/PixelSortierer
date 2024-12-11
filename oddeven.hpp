#pragma once

#include <iostream>
#include <vector>

// Compare function
bool compare(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b, int comparison_method)
{
    // Example comparison methods:
    // 0: Compare first element of each vector
    // 1: Compare sum of elements in each vector
    switch (comparison_method) {
        case 0: // Compare first element
            return a[0] > b[0];
        case 1:
        { // Compare sum of elements
            int sum_a = 0, sum_b = 0;
            for (unsigned char val : a) sum_a += val;
            for (unsigned char val : b) sum_b += val;
            return sum_a > sum_b;
        }
        case 2:
        { // Sum of the first 3 numbers
            int sum_a = a[0] + a[1] + a[2];
            int sum_b = b[0] + b[1] + b[2];
            return sum_a > sum_b;
        }
        default:
            std::cerr << "Invalid comparison method!\n";
            return false;
    }
}

// OddEvenSortStep function
bool OddEvenSortStep(std::vector<std::vector<unsigned char>>& array, bool odd, int comparison_method)
{
    bool changes_made = false;

    // Determine the starting index based on whether it's an odd or even phase
    int start_index = odd ? 1 : 0;

    // Iterate through the array in pairs
    for (size_t i = start_index; i + 1 < array.size(); i += 2)
    {
        // If comparison determines the first is greater, swap
        if (compare(array[i], array[i + 1], comparison_method))
        {
            std::swap(array[i], array[i + 1]);
            changes_made = true;
        }
    }
    return changes_made;
}

// OddEvenSort function
bool OddEvenSort(std::vector<std::vector<unsigned char>>& array, int comparison_method)
{
    bool changes_made = true; // Track if changes are made during sorting

    while (changes_made)
    {
        changes_made = false;

        // Perform the odd phase
        bool odd_phase_changes = OddEvenSortStep(array, true, comparison_method);

        // Perform the even phase
        bool even_phase_changes = OddEvenSortStep(array, false, comparison_method);

        // Update the flag
        changes_made = odd_phase_changes || even_phase_changes;
    }
    return true; // Sorting completed
}
