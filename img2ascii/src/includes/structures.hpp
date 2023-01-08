#pragma once
#include "includes.hpp"

// Structure that should contain information of each character and its luma levels (brightness) of each division. 
struct Char
{
	CHAR character;

	union
	{
		FLOAT section_brightness[9];
		struct
		{
			FLOAT TopLeft, TopMiddle, TopRight, MiddleLeft, Middle, MiddleRight, BottomLeft, BottomMiddle, BottomRight;
		};
	};
};

// Structure that should contain information of each section of a "gridified" image
struct Section
{
	Bitmap* image;
	UINT    width;
	UINT    height;
	INT     posX;
	INT     posY;
	FLOAT   averageBrightness;
	FLOAT   averageBrightnessMap[9];

	Section(INT posX, INT posY, UINT width, UINT height, Bitmap* image);
	Section(VOID);

	VOID calculate_average_brightness(VOID);
	VOID calculate_disected_brightness(VOID);

private:
	FLOAT calculate_pixel_brightness(INT x, INT y, BitmapData& imageData);
};