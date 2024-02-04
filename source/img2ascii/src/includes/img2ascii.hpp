#pragma once
#include "includes.hpp"
#include "structures.hpp"
#include "mappings.hpp"


// Macros for predefined character width of ASCII art compatible with 1920px width
#define TINY_W96    96
#define MEDIUM_W128 128
#define LARGE_W192  192

// Macro for scaled bitmap width. Bigger = more detail and SLOWER to process. 
#define SCALED_BITMAP_WIDTH 1920

// This defines maximum allowed width and height of input image expressed in pixels
#define MAX_WIDTH  4000
#define MAX_HEIGHT 4000

// This defines maximum allowed character amount in one line for ASCII art
#define MAX_LINE_LENGTH 400

// Section height is at least 1.6 times its width and must be devisible by height of the image. Latin alphabet characters are always taller than they are wide.
// 1.6 Works well for font Terminal 8x12px but other values might suit better for different fonts
#define FONT_ASPECT_RATIO 1.6f

class ASCIIFied
{
public:
	ASCIIFied(CONST WCHAR* image_path, UINT chars_per_line = MEDIUM_W128, UINT scaled_image_width = 0, bool gamma_brightness_correction = false, bool advanced_algorithm = false);
	~ASCIIFied(VOID);

	UINT     get_output_width(VOID) CONST;
	UINT     get_output_height(VOID) CONST;
	Section* get_section_map(VOID) CONST;
	UINT     get_section_map_size(VOID) CONST;
	VOID     generate_image(VOID);
	VOID     output_to_console(VOID);

private:
	Bitmap*      image;
	CONST WCHAR* image_path;
	UINT		 chars_per_line;
	UINT		 scaled_image_width;
	bool		 gamma_brightness_correction;
	bool		 advanced_algorithm;
	CHAR*		 decoded_art;
	Section*	 section_map;
	ULONG_PTR	 gdiplus_token;
	UINT		 section_map_size;
	UINT		 char_height;
	
	VOID    resize_image(UINT width);
	INT     find_closest_divider(INT divisor, INT divider);
	VOID    gridify(Bitmap* base);
	VOID    apply_gamma_brightness_contrast_correction(Bitmap* base);
	FLOAT   vector_similarity(CONST FLOAT a[], FLOAT b[]);
	CHAR    decode_section(CONST map<FLOAT, vector<CHAR>>& character_brightness_map, FLOAT character_brightness);
	CHAR    advanced_algorithm_decode_section(CONST DisectedChar desected_character_brightness_map[], FLOAT disected_section[]);
	bool    is_supported_bitmap_type(VOID);
	bool    is_supported_bitmap_size(VOID);
	VOID    decode_art(VOID);
};