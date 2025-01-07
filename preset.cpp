#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "lib/stblibs.h"
#include "oddeven.hpp"

int preset1(int argc, char const *argv[])
{
    try
    {
        int width, height, channels, Sortiermethode, diff;

        // Read an image
        auto image = readImage(argv[2], width, height, channels);
        std::cout << "Image read successfully! Dimensions: " << width << "x" << height << " Channels: " << channels << std::endl;
        Sortiermethode = 6;

        std::cout << "Sortieren ." << std::flush;

        for (diff = 0; diff < 256; diff++)
        {
            for (auto& row : image)
            {
                bool changes_made = true; // Track if changes are made during sorting

                while (changes_made)
                {
                    changes_made = false;

                    // Perform the odd phase
                    bool odd_phase_changes = OddEvenSortStep(row, true, Sortiermethode, diff);
                    {
                        bool changes_made = false;

                        // Determine the starting index based on whether it's an odd or even phase
                        int start_index = true ? 1 : 0;

                        // Iterate through the array in pairs
                        for (size_t i = start_index; i + 1 < row.size(); i += 2)
                        {
                            // If comparison determines the first is greater, swap
                            if (compare(row[i], row[i + 1], Sortiermethode, diff))
                            {
                                std::swap(row[i], row[i + 1]);
                                changes_made = true;
                            }
                        }
                        return changes_made;
                    }

                    // Perform the even phase
                    bool even_phase_changes = OddEvenSortStep(row, false, Sortiermethode, diff);
                    {
                        bool changes_made = false;

                        // Determine the starting index based on whether it's an odd or even phase
                        int start_index = false ? 1 : 0;

                        // Iterate through the array in pairs
                        for (size_t i = start_index; i + 1 < row.size(); i += 2)
                        {
                            // If comparison determines the first is greater, swap
                            if (compare(row[i], row[i + 1], Sortiermethode, diff))
                            {
                                std::swap(row[i], row[i + 1]);
                                changes_made = true;
                            }
                        }
                        return changes_made;
                    }

                    // Update the flag
                    changes_made = odd_phase_changes || even_phase_changes;
                }
                return true; // Sorting completed
            }
            // Write the modified image
            writeImage(std::string("outanim_") + std::to_string(diff) + ".png", image, width, height, channels);
            std::cout << '.' << std::flush;
        }
        
        std::cout << "\nImage written successfully to outanim_[0-255].png!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
