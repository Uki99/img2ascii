#pragma once
#include "includes.hpp"
#include "structures.hpp"
#include "mappings.hpp"


// Macros for predefined character width of ASCII art compatible with 1920px width
#define TINY   96
#define MEDIUM 128
#define LARGE  192

// Macro for scaled bitmap width. Bigger = more detail and SLOWER to process. 
#define SCALED_BITMAP_WIDTH 1920

// This defines maximum allowed width and height of input image expressed in pixels
#define MAX_WIDTH  4000
#define MAX_HEIGHT 4000

// This defines maximum allowed character amount in one line for ASCII art
#define MAX_LINE_LENGTH 400

class ASCIIFied
{
public:
	ASCIIFied(CONST WCHAR* image_path, UINT chars_per_line = MEDIUM, UINT scaled_image_width = SCALED_BITMAP_WIDTH, bool gamma_brightness_correction = false, bool edge_detection = false);
	~ASCIIFied(VOID);

	UINT     get_output_width(VOID) CONST;
	UINT     get_output_height(VOID) CONST;
	Section* get_section_map(VOID) CONST;
	UINT     get_section_map_size(VOID) CONST;
	VOID     generate_image(VOID);
	VOID     output_to_console(VOID);

private:
	Bitmap* image;
	Bitmap* scaled_image;
	CONST WCHAR* image_path;
	UINT  chars_per_line;
	UINT  scaled_image_width;
	bool  gamma_brightness_correction;
	bool  edge_detection;
	CHAR* decoded_art;
	Section* section_map;
	ULONG_PTR gdiplus_token;
	UINT  section_map_size;
	UINT  char_height;
	
	VOID    resize_image(UINT width);
	INT     find_closest_divider(INT divisor, INT divider, bool only_bigger_divider);
	VOID    gridify(Bitmap* base);
	VOID    apply_gamma_briCon_correction(Bitmap* base);
	FLOAT   similarity(CONST FLOAT a[], FLOAT b[]);
	CHAR    brightness_to_char(CONST map<FLOAT, vector<CHAR>>& map, FLOAT brightness);
	CHAR    brightness_map_to_char(const Char map[], FLOAT brightness_map[]);
	bool    is_supported_bitmap_type(VOID);
	bool    is_supported_bitmap_size(VOID);
	VOID    decode_art(VOID);
};