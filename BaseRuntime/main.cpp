#include <iostream>
#define LZH_EXPORTS
#include "lzh.h"
using namespace lzh;
int main(int argc, char *argv[])
{
    lzh::Image image = lzh::imread("E:\\pc1-D\\10.27\\Image_20221027115803134.bmp", lzh::ColorGamutGray);
    //lzh::Threshold(image, image, 0, 255, lzh::THRESH_OTSU);
    lzh::adaptiveThreshold(image, image, 0, 255, 3, 80);
    lzh::imshow(image);
    lzh::waitKey();
    return 0;
}
