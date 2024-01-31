#pragma once
#include "includes.hpp"

// Structure that should contain information of each character and its luma levels (brightness) of each division.
// This structure is reserved for font character data only.
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
// This structure is reserved for processing bitmap image data only.
struct Section
{
	Bitmap* image;
	UINT    width;
	UINT    height;
	INT     posX;
	INT     posY;
	FLOAT   averageBrightness;
	FLOAT   averageBrightnessMap[9];

	Section(INT posX, INT posY, UINT width, UINT height, Bitmap* image, BOOL disection_enabled);
	Section(VOID);

private:
	FLOAT calculate_pixel_brightness(INT x, INT y, BitmapData& imageData);
	VOID calculate_average_brightness(VOID);
	VOID calculate_disected_brightness(VOID);
};