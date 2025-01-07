#pragma once

#include <iostream>
#include <vector>
#include <string>

inline bool compare6(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b, int diff)
{
    long sum_a, sum_b, x, y;
    { // Sum of the first 3 numbers, with per-channel diff check
        sum_a = a[0] + a[1] + a[2];
        sum_b = b[0] + b[1] + b[2];
        if(sum_a > sum_b)
        {
            x = a[0] - b[0];
            if(x < 0) x = -x;
            if(x > diff) return false;
            
            x = a[1] - b[1];
            if(x < 0) x = -x;
            if(x > diff) return false;
            
            x = a[2] - b[2];
            if(x < 0) x = -x;
            if(x > diff) return false;
            
            return true;
        } else {
            return false;
        }
    }
}

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
                bool changes_made1 = true; // Track if changes are made during sorting
                //std::cout << 'a' << diff << std::endl;

                while (changes_made1)
                {
                    changes_made1 = false;

                    bool odd_phase_changes;
                    bool even_phase_changes;

                    // Perform the odd phase
                    {
                        bool changes_made2 = false;

                        // Determine the starting index based on whether it's an odd or even phase
                        int start_index = true ? 1 : 0;

                        // Iterate through the array in pairs
                        for (size_t i = start_index; i + 1 < row.size(); i += 2)
                        {
                            // If comparison determines the first is greater, swap
                            if (compare6(row[i], row[i + 1], diff))
                            {
                                std::swap(row[i], row[i + 1]);
                                changes_made2 = true;
                            }
                        }
                        odd_phase_changes = changes_made2;
                    }

                    //std::cout << 'b' << std::endl;

                    // Perform the even phase
                    {
                        bool changes_made2 = false;

                        // Determine the starting index based on whether it's an odd or even phase
                        int start_index = false ? 1 : 0;

                        // Iterate through the array in pairs
                        for (size_t i = start_index; i + 1 < row.size(); i += 2)
                        {
                            // If comparison determines the first is greater, swap
                            if (compare6(row[i], row[i + 1], diff))
                            {
                                std::swap(row[i], row[i + 1]);
                                changes_made2 = true;
                            }
                        }
                        even_phase_changes = changes_made2;
                    }

                    // Update the flag
                    changes_made1 = odd_phase_changes || even_phase_changes;
                }
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
