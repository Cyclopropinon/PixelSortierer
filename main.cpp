#include <cstdlib> // For std::atoi

#include "pixelsortierer.cpp"

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
    
    default:
        break;
    }
}
