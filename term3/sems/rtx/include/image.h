#pragma once

#include <camera.h>
#include <geometry.h>
#include <fstream>

class Image{

    public:
        Image(int width, int hight, Color** pixels_given) : width(width), hight(hight), pixels(pixels)
        {} 

        ~Image()
        {
            for(int i = 0; i<width; i++)
            {
                delete[] pixels[i];
            }
            delete[] pixels;
        }

        void write_image_pem(std::ostream& os)
        {
            os << "P3\n" << width << ' ' << hight << "\n255\n";
            for (int j = 0; j < hight; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    os << pixels[i][j].x << ' ' << pixels[i][j].y << ' ' << pixels[i][j].z << '\n';
                }
            }
        }
    private:
        int width;
        int hight;
        Color** pixels;


};







