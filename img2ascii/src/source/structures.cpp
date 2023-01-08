#include "structures.hpp"

Section::Section(INT posX, INT posY, UINT width, UINT height, Bitmap* image)
{
	this->image = image;
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->height = height;
	this->averageBrightness = 0.f;
	memset(this->averageBrightnessMap, 0, sizeof(FLOAT) * 9);
}

Section::Section(VOID)
{
	this->image = nullptr;
	this->height = 0;
	this->width = 0;
	this->posX = 0;
	this->posY = 0;
	this->averageBrightness = 0;
	memset(this->averageBrightnessMap, 0, sizeof(FLOAT) * 9);
}

VOID Section::calculate_average_brightness(VOID)
{
	if (this->image == nullptr)
	{
		cerr << "Section tried to access null pointer image. Terminating the program" << endl;
		exit(EXIT_FAILURE);
	}

	// Calculates average brightness of a section
	// Locking image data and passing to the function calculate_pixel_brightness() for speed performance!
	BitmapData image_data;

	this->image->LockBits(new Rect(0, 0, this->image->GetWidth(), this->image->GetHeight()), ImageLockModeRead, PixelFormat24bppRGB, &image_data);


	for (UINT i = this->posX; i < this->posX + this->width; ++i)
	{
		for (UINT j = this->posY; j < this->posY + this->height; ++j)
		{
			this->averageBrightness = this->averageBrightness + this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightness = this->averageBrightness / (FLOAT)(this->width * this->height);

	// Unlock the memory
	this->image->UnlockBits(&image_data);
}

VOID Section::calculate_disected_brightness(VOID)
{
	if (this->image == nullptr)
	{
		cerr << "Section tried to access null pointer image. Terminating the program" << endl;
		exit(EXIT_FAILURE);
	}

	// Locking image data and passing to the function calculate_pixel_brightness() for speed performance!
	BitmapData image_data;

	this->image->LockBits(new Rect(0, 0, this->image->GetWidth(), this->image->GetHeight()), ImageLockModeRead, PixelFormat24bppRGB, &image_data);

	INT w25 = (INT)round((FLOAT)this->width * 0.25f);   // 25% width
	INT h25 = (INT)round((FLOAT)this->height * 0.25f);  // 25% height
	INT w50 = (INT)round((FLOAT)this->width * 0.5f);    // 50% width
	INT h50 = (INT)round((FLOAT)this->height * 0.5f);   // 50% height
	INT w75 = (INT)round((FLOAT)this->width * 0.75f);   // 75% width
	INT h67 = (INT)round((FLOAT)this->height * 0.67f);  // 67% height

	// Top left
	for (INT i = this->posX; i < this->posX + w25; ++i)
	{
		for (INT j = this->posY; j < this->posY + h25; ++j)
		{
			this->averageBrightnessMap[0] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[0] = this->averageBrightnessMap[0] / (w25 * h25);


	// Top Middle
	for (INT i = this->posX + w25; i < this->posX + w25 + w50; ++i)
	{
		for (INT j = this->posY; j < this->posY + h25; ++j)
		{
			this->averageBrightnessMap[1] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[1] = this->averageBrightnessMap[1] / (w50 * h25);


	// Top Right
	for (INT i = this->posX + w25 + w50; i < this->posX + (INT)this->width; ++i)
	{
		for (INT j = this->posY; j < this->posY + h25; ++j)
		{
			this->averageBrightnessMap[2] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[2] = this->averageBrightnessMap[2] / (w25 * h25);


	// Middle left
	for (INT i = this->posX; i < this->posX + w25; ++i)
	{
		for (INT j = this->posY + h25; j < this->posY + h25 + h50; ++j)
		{
			this->averageBrightnessMap[3] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[3] = this->averageBrightnessMap[3] / (w25 * h50);


	// Middle (!!!)
	for (INT i = this->posX + (INT)(w25 / 2); i < this->posX + (INT)this->width - (INT)(w25 / 2); ++i)
	{
		for (INT j = this->posY + (INT)(h25 * 2 / 3); j < ((this->posY + (INT)this->height) - (INT)(h25 * 2 / 3)); ++j)
		{
			this->averageBrightnessMap[4] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[4] = this->averageBrightnessMap[4] / (w75 * h67);


	// Middle right
	for (INT i = this->posX + w50 + w25; i < this->posX + (INT)this->width; ++i)
	{
		for (INT j = this->posY + h25; j < this->posY + h25 + h50; ++j)
		{
			this->averageBrightnessMap[5] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[5] = this->averageBrightnessMap[5] / (w25 * h50);


	// Bottom left
	for (INT i = this->posX; i < this->posX + w25; ++i)
	{
		for (INT j = this->posY + h50 + h25; j < this->posY + (INT)this->height; ++j)
		{
			this->averageBrightnessMap[6] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[6] = this->averageBrightnessMap[6] / (w25 * h25);


	// Bottom middle
	for (INT i = this->posX + w25; i < this->posX + w25 + w50; ++i)
	{
		for (INT j = this->posY + h50 + h25; j < this->posY + (INT)this->height; ++j)
		{
			this->averageBrightnessMap[7] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[7] = this->averageBrightnessMap[7] / (w50 * h25);


	// Bottom right
	for (INT i = this->posX + w25 + w50; i < this->posX + (INT)this->width; ++i)
	{
		for (INT j = this->posY + h50 + h25; j < this->posY + (INT)this->height; ++j)
		{
			this->averageBrightnessMap[8] += this->calculate_pixel_brightness(i, j, image_data);
		}
	}

	this->averageBrightnessMap[8] = this->averageBrightnessMap[8] / (w25 * h25);

	// Unlock the bits once done with calculations
	this->image->UnlockBits(&image_data);
}

FLOAT Section::calculate_pixel_brightness(INT x, INT y, BitmapData& imageData)
{
	// Calculating average brightness of a single pixel from the given RGB value for pixel positioned at XY coordinate
	// Pixel luminance = (0.2126 * R + 0.7152 * G + 0.0722 * B)
	// https://en.wikipedia.org/wiki/Relative_luminance

	byte* pixel = (byte*)imageData.Scan0 + (y * imageData.Stride);

	// Watch out for actual order (BGR)!
	// 3 stands for bytes per pixel
	INT bIndex = x * 3;
	INT gIndex = bIndex + 1;
	INT rIndex = bIndex + 2;

	FLOAT red = (FLOAT)pixel[rIndex];
	FLOAT green = (FLOAT)pixel[gIndex];
	FLOAT blue = (FLOAT)pixel[bIndex];

	return ((0.2126f * red + 0.7152f * green + 0.0722f * blue) / 255.f);
}