#pragma once

#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// OpenCL Kernel für Compare und Sortieren
const char* kernel_code_po1 = R"(
__kernel void sort_rows(__global uchar* image, const int width, const int height, const int channels, const int diff) {
    int row_idx = get_global_id(0);
    if (row_idx >= height) return;

    // Pointer zur aktuellen Zeile
    __global uchar* row = &image[row_idx * width * channels];
    
    // Sortiere die Zeile mit Odd-Even-Sort
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        // Odd Phase
        for (int i = 1; i < width - 1; i += 2) {
            int sum_a = row[i * channels] + row[i * channels + 1] + row[i * channels + 2];
            int sum_b = row[(i + 1) * channels] + row[(i + 1) * channels + 1] + row[(i + 1) * channels + 2];
            if (sum_a > sum_b) {
                for (int c = 0; c < channels; ++c) {
                    int x = row[i * channels + c] - row[(i + 1) * channels + c];
                    if (x < 0) x = -x;
                    if (x > diff) return; // Abbrechen, wenn Unterschied zu groß ist
                }
                for (int c = 0; c < channels; ++c) {
                    uchar tmp = row[i * channels + c];
                    row[i * channels + c] = row[(i + 1) * channels + c];
                    row[(i + 1) * channels + c] = tmp;
                }
                sorted = 0;
            }
        }
        // Even Phase
        for (int i = 0; i < width - 1; i += 2) {
            int sum_a = row[i * channels] + row[i * channels + 1] + row[i * channels + 2];
            int sum_b = row[(i + 1) * channels] + row[(i + 1) * channels + 1] + row[(i + 1) * channels + 2];
            if (sum_a > sum_b) {
                for (int c = 0; c < channels; ++c) {
                    int x = row[i * channels + c] - row[(i + 1) * channels + c];
                    if (x < 0) x = -x;
                    if (x > diff) return; // Abbrechen, wenn Unterschied zu groß ist
                }
                for (int c = 0; c < channels; ++c) {
                    uchar tmp = row[i * channels + c];
                    row[i * channels + c] = row[(i + 1) * channels + c];
                    row[(i + 1) * channels + c] = tmp;
                }
                sorted = 0;
            }
        }
    }
}
)";

int presetOpencl1(int argc, char const* argv[]) {
    try {
        int width, height, channels, diff;
        auto image = readImage(argv[2], width, height, channels);

        std::cout << "Image read successfully! Dimensions: " << width << "x" << height << " Channels: " << channels << std::endl;
        std::cout << "Sortieren ." << std::flush;

        // OpenCL Initialisierung
        cl_platform_id platform;
        cl_device_id device;
        cl_context context;
        cl_command_queue queue;
        cl_program program;
        cl_kernel kernel;

        // Plattform und Gerät auswählen
        clGetPlatformIDs(1, &platform, nullptr);
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
        context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, nullptr);
        queue = clCreateCommandQueue(context, device, 0, nullptr);

        // OpenCL Programm erstellen
        program = clCreateProgramWithSource(context, 1, &kernel_code_po1, nullptr, nullptr);
        clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

        kernel = clCreateKernel(program, "sort_rows", nullptr);

        // Bilddaten in OpenCL-Puffer kopieren
        size_t image_size = width * height * channels;
        std::vector<unsigned char> image_data(image_size);
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                for (int c = 0; c < channels; ++c)
                    image_data[(i * width + j) * channels + c] = image[i][j][c];

        cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, image_size, nullptr, nullptr);
        clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, image_size, image_data.data(), 0, nullptr, nullptr);

        // Kernel-Argumente setzen
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
        clSetKernelArg(kernel, 1, sizeof(int), &width);
        clSetKernelArg(kernel, 2, sizeof(int), &height);
        clSetKernelArg(kernel, 3, sizeof(int), &channels);
        clSetKernelArg(kernel, 4, sizeof(int), &diff);

        // Kernel ausführen
        size_t global_work_size = height;
        clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &global_work_size, nullptr, 0, nullptr, nullptr);

        // Ergebnisse zurücklesen
        clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, image_size, image_data.data(), 0, nullptr, nullptr);

        // Bilddaten zurück in das Bildformat konvertieren
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                for (int c = 0; c < channels; ++c)
                    image[i][j][c] = image_data[(i * width + j) * channels + c];

        writeImage("outanim_final.png", image, width, height, channels);

        // Ressourcen freigeben
        clReleaseMemObject(buffer);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);

        std::cout << "Image processing completed with OpenCL!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
