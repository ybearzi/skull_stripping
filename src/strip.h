#ifndef STRIP_H_
#define STRIP_H_

#include "itkImage.h"

typedef unsigned int PixelType;
typedef char PixelBinaryType;
const unsigned int Dim = 3;

typedef itk::Image< PixelType, Dim > Image;
typedef itk::Image< PixelBinaryType, Dim > ImageBinary;


void strip(const char* input, const char *input_t2, const char* input_label, const char *outputT1, const char *output_t2, const char *output_excel);

ImageBinary::Pointer label2mask(Image::Pointer label, unsigned int element = 0);

ImageBinary::Pointer close(ImageBinary::Pointer image_binary);

Image::Pointer binary_and(Image::Pointer input, ImageBinary::Pointer mask);

void fill_csv(Image::Pointer im1, Image::Pointer im2, Image::Pointer label, const char *output_excel);

unsigned int get_min(Image::Pointer im);
unsigned int get_max(Image::Pointer im);
float get_mean(Image::Pointer im);
unsigned int get_volume(Image::Pointer im);

#endif
