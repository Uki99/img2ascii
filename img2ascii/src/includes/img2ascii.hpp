#pragma once
#include "includes.hpp"
#include "structures.hpp"
#include "mappings.hpp"


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

class ASCIIFied
{
public:
	ASCIIFied(CONST WCHAR* image_path, UINT chars_per_line = medium, UINT scaled_image_width = scaled_bitmap_width, bool gamma_brightness_correction = false, bool edge_detection = false);
	~ASCIIFied(VOID);

	UINT     get_output_width(VOID) CONST;
	UINT     get_output_height(VOID) CONST;
	Section* get_section_map(VOID) CONST;
	UINT     get_section_map_size(VOID) CONST;
	VOID     generate_image(VOID);
	VOID     output_to_console(VOID);

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
};