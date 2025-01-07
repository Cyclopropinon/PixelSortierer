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
                OddEvenSort(row, Sortiermethode, diff);
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
