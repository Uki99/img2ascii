#pragma once

#ifndef GDIPVER
#define GDIPVER 0x0110 
#endif

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <shlobj.h>
#include <string>
#include <time.h>

#pragma comment( lib, "Gdiplus.lib" ) 

// Macros for predefined character width of ASCII art compatible with 1920px width
#define tiny   96
#define medium 128
#define large  192

// Macro for scaled bitmap width. Bigger = more detail and SLOWER to process. 
#define scaled_bitmap_width 1920

// This defines maximum allowed width and height of input image expressed in pixels
#define max_width  4000
#define max_height 4000

// This defines maximum allowed character amount in one line for ASCII art
#define max_line_length 400


using namespace Gdiplus;
using namespace std;


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

class ASCIIFied
{
private:
	Bitmap* image;
	Bitmap* scaledImage;
	ULONG_PTR gdiplusToken;
	Section* sectionMap;
	UINT  width;
	UINT  height;
	UINT  sectionMapSize;
	UINT  chars_per_line;
	UINT  char_height;
	CHAR* decoded_art;
	bool  edge_detection;


	VOID    resize_image(UINT width);
	INT     find_closest_divider(INT divisor, INT divider, bool onlyBiggerDivider);
	wstring get_image_extension(CONST WCHAR* path);
	VOID    gridify(Bitmap* base);
	VOID    apply_gamma_briCon_correction(Bitmap* base);
	FLOAT   similarity(CONST FLOAT a[], FLOAT b[]);
	CHAR    brightness_to_char(CONST map<FLOAT, vector<CHAR>>& map, FLOAT brightness);
	CHAR    brightness_map_to_char(const Char map[], FLOAT brightness_map[]);
	VOID    decode_art(VOID);

public:
	ASCIIFied(CONST WCHAR* image_path, UINT chars_per_line = medium, UINT scaled_image_width = scaled_bitmap_width, bool gamma_brightness_correction = false, bool edge_detection = false);
	~ASCIIFied(VOID);

	UINT     get_output_width(VOID) CONST;
	UINT     get_output_height(VOID) CONST;
	Section* get_section_map(VOID) CONST;
	UINT     get_section_map_size(VOID) CONST;
	VOID     generate_image(VOID);
	VOID     output_to_console(VOID);
};
