#include <cstdlib> // For std::atoi

#include "pixelsortierer.cpp"
#include "preset.cpp"
#ifdef __Pixelsortierer_ENABLE_OCL__
    #include"preset_opencl.cpp"
#endif

int main(int argc, char const *argv[])
{
    switch (std::atoi(argv[1])) // Sortiermethode
    {
    case 1:
        return Pixelsortierer(argc, argv);
        break;
    case 2:
        return PixelsortiererAnimiert(argc, argv);
        break;
    
    case -1:
        return preset1(argc, argv);
        break;

    case -100: // mit OpenCL kompiliert?
        #ifdef __Pixelsortierer_ENABLE_OCL__
            return 1;
        #else
            return 0;
        #endif

    #ifdef __Pixelsortierer_ENABLE_OCL__
        case -101:
            return presetOpencl1(argc, argv);
            break;
    #endif
    default:
        return -2;
        break;
    }
}
