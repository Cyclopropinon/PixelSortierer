#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "lib/stblibs.h"
#include "oddeven.hpp"

// Function to read an image into a 2D vector of pixels
std::vector<std::vector<std::vector<unsigned char>>> readImage(const std::string& filepath, int& width, int& height, int& channels)
{
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        throw std::runtime_error("Error: Could not load the image at " + filepath);
    }

    // Convert data to 2D vector
    std::vector<std::vector<std::vector<unsigned char>>> image(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(channels)));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int c = 0; c < channels; c++)
            {
                image[i][j][c] = data[(i * width + j) * channels + c];
            }
        }
    }

    stbi_image_free(data); // Free the image memory
    return image;
}

// Function to write an image from a 2D vector of pixels
void writeImage(const std::string& filepath, const std::vector<std::vector<std::vector<unsigned char>>>& image, int width, int height, int channels)
{
    // Flatten the 2D vector back to a 1D array
    std::vector<unsigned char> data(width * height * channels);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int c = 0; c < channels; c++)
            {
                data[(i * width + j) * channels + c] = image[i][j][c];
            }
        }
    }

    // Write the image using stb_image_write
    if (!stbi_write_png(filepath.c_str(), width, height, channels, data.data(), width * channels))
    {
        throw std::runtime_error("Error: Could not write the image to " + filepath);
    }
}

int Pixelsortierer(int argc, char const *argv[])
{
    try
    {
        int width, height, channels, Sortiermethode, diff;

        // Read an image
//        auto image = readImage("input.png", width, height, channels);
        auto image = readImage(argv[1], width, height, channels);
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

        std::cout << "Sortierstärke (0 - 255): ";
        std::cin >> diff;

        std::cout << "Sortieren ." << std::flush;

        for (auto& row : image)
        {
            OddEvenSort(row, Sortiermethode, diff);
            std::cout << '.' << std::flush;
        }

        // Write the modified image
        writeImage("output.png", image, width, height, channels);
        std::cout << "\nImage written successfully to output.png!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
