#include "StudentPreProcessing.h"
#include <stddef.h>
#include <vector>
#include "IntensityImagePrivate.h"
#include <iostream>


IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	return nullptr;
}

IntensityImagePrivate applyFilter(const IntensityImage &image, const short mask[], const short mask_size, bool horizontal_vertical = false)
{
	IntensityImagePrivate temp = IntensityImagePrivate(image.getWidth(), image.getHeight());
	int avr = 0;
	for (auto i = 0; i < mask_size*mask_size; i++)
	{
		avr += mask[i];
	}
	if(avr < 1)
	{
		avr = 1;
	}

	// Calculate the new pixel values of the image.
	for (auto x = 0; x < image.getHeight(); x++)
	{
		for (auto y = 0; y < image.getWidth(); y++)
		{
			int gx = 0;
			int gy = 0;
			const short t = ((mask_size - 1) / 2);
			for (auto ym = -t; ym <= t; ym++)
			{
				for (auto xm = -t; xm <= t; xm++)
				{
					gx += image.getPixel(x + xm, y + ym) * mask[(xm + t) + ((ym + t) * mask_size)];
					if(horizontal_vertical)
						gy += image.getPixel(x + xm, y + ym) * mask[((xm + t) * mask_size) + (ym + t)];
				}
			}
			if (horizontal_vertical)
				temp.setPixel(x, y, std::hypot(gx, gy));
			else
				temp.setPixel(x, y, gx / avr);
		}
	}
	return temp;
}

const auto gaussian_mask_size5 = 5;
const short gaussian_mask5[gaussian_mask_size5*gaussian_mask_size5] =
{	1,2,4,2,1,
	2,3,6,3,2,
	4,6,9,6,4,
	2,3,6,3,2,
	1,2,4,2,1
};
const auto gaussian_mask_size3 = 3;
const short gaussian_mask3[gaussian_mask_size3*gaussian_mask_size3] =
{	1,2,1,
	2,4,2,
	1,2,1
};

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	IntensityImagePrivate* img = new IntensityImagePrivate(image.getWidth(), image.getHeight());

	img->set(applyFilter(image, gaussian_mask3, gaussian_mask_size3));

	// // Size of masker like number x number.
	// const auto shrap_mask_size = 3;
	// // Masker for shrap detection
	// const short shrap_mask[shrap_mask_size*shrap_mask_size] =
	// {	0,1,0,
	// 	1,-4,1,
	// 	0,1,0 };
	// img->set(applyFilter(image, shrap_mask, shrap_mask_size, true));

	// Size of masker like number x number.
	const auto edge_mask_size = 3;
	// Masker for edge detection
	const short edge_mask[edge_mask_size*edge_mask_size] =
	{	1,0,-1,
		2,0,-2,
		1,0,-1	};
	img->set(applyFilter(*img, edge_mask, edge_mask_size, true));
	return img;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	return nullptr;
}