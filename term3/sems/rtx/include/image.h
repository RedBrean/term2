#pragma once

#include <camera.h>
#include <geometry.h>
#include <fstream>
#include <vector>

#include <cmath>
#include <iostream>
class Image{

    public:
        Image(int width, int height, std::vector<std::vector<Color>> pixels_given) : width(width), height(height), pixels(pixels)
        {
            std::cout << "\nImage size constr: " << pixels.size() << "\n";

        } 
        Image() : width(0), height(0){}
        ~Image(){
        }

        void write_image_pem(std::ostream& os)
        {
            os << "P3\n" << width << ' ' << height << "\n255\n";

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    Color pixel = pixels[i][j];
                    os << std::round(255*pixels[i][j].x) << ' ' << std::round(pixels[i][j].y) << ' ' << std::round(pixels[i][j].z) << '\n';
                }
            }
        }
    public:
        int width;
        int height;
        std::vector<std::vector<Color>> pixels;

};







