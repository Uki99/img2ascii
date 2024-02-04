#pragma once
#include "includes.hpp"

// Structure that should contain information of each character and its luma levels (brightness) of each division.
// This structure is reserved for font character data only.
struct DisectedChar
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
	FLOAT   average_brightness;
	FLOAT   average_brightness_map[9];

	Section(INT posX, INT posY, UINT width, UINT height, Bitmap* image, BOOL advanced_algorithm);
	Section(VOID);

private:
	FLOAT calculate_pixel_brightness(INT x, INT y, BitmapData& image_data);
	VOID  calculate_average_brightness_fast(VOID);
	VOID  calculate_disected_brightness(VOID);
};