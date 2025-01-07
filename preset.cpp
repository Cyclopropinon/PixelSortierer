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
//        auto image = readImage("input.png", width, height, channels);
        auto image = readImage(argv[2], width, height, channels);
        std::cout << "Image read successfully! Dimensions: " << width << "x" << height << " Channels: " << channels << std::endl;
        std::cout << "SSE aktiviert: " << stbi__sse2_available() << std::endl;

        /*/ Modify the image (optional): Example - invert colors
        for (auto& row : image)
        {
            for (auto& pixel : row)
            {
                for (auto& channel : pixel)
                {
                    channel = 255 - channel; // Simple inversion
                }
            }
        }// */

        std::cout << "Sortiermethode: ";
        std::cin >> Sortiermethode;

        //std::cout << "Sortierstärke (0 - 255): ";
        //std::cin >> diff;

        std::cout << "Sortieren ." << std::flush;

        for (diff = 0; diff < 256; diff++)
        {
            for (auto& row : image)
            {
                OddEvenSort(row, Sortiermethode, diff);
            }
            writeImage(std::string("outanim_") + std::to_string(diff) + ".png", image, width, height, channels);
            std::cout << '.' << std::flush;
        }
        
        // Write the modified image
        //writeImage("output.png", image, width, height, channels);
        std::cout << "\nImage written successfully to outanim_[0-255].png!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
