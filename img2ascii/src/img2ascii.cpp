#include "img2ascii.hpp"

static const map<FLOAT, vector<CHAR>> luma_char_map =
{
	{ 0.000000f, {' '} },
	{ 0.133333f, {'.'} },
	{ 0.155556f, {'-'} },
	{ 0.177778f, {','} },
	{ 0.266667f, {':'} },
	{ 0.311111f, {'+'} },
	{ 0.333333f, {'~'} },
	{ 0.355556f, {';'} },
	{ 0.400000f, {'('} },
	{ 0.444444f, {'%'} },
	{ 0.488889f, {'x'} },
	{ 0.511111f, {'1'} },
	{ 0.533333f, {'*'} },
	{ 0.555556f, {'n', 'u'} },
	{ 0.577778f, {'T'} },
	{ 0.600000f, {'3'} },
	{ 0.622222f, {'J'} },
	{ 0.644444f, {'5'} },
	{ 0.666667f, {'$'} },
	{ 0.688889f, {'S'} },
	{ 0.711111f, {'4'} },
	{ 0.733333f, {'F', 'P'} },
	{ 0.755556f, {'G', 'O', 'V', 'X'} },
	{ 0.777778f, {'E', 'Z'} },
	{ 0.800000f, {'8', 'A', 'U'} },
	{ 0.844444f, {'D', 'H', 'K', 'W'} },
	{ 0.888889f, {'@'} },
	{ 0.911111f, {'B', 'Q'} },
	{ 0.933333f, {'#'} },
	{ 1.000000f, {'0', 'M', 'N'} }
};

static const Char luma_char_map_x[95] =
{
	{ ' ',  0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f },
	{ '!',  0.250000f, 0.625000f, 0.000000f, 0.166667f, 0.555556f, 0.000000f, 0.000000f, 0.222222f, 0.000000f },
	{ '"',  0.500000f, 0.500000f, 0.666667f, 0.083333f, 0.277778f, 0.166667f, 0.000000f, 0.000000f, 0.000000f },
	{ '#',  0.500000f, 0.750000f, 0.000000f, 0.666667f, 1.000000f, 0.333333f, 0.200000f, 0.333333f, 0.000000f },
	{ '$',  0.250000f, 1.000000f, 0.000000f, 0.583333f, 0.583333f, 0.000000f, 0.000000f, 0.444444f, 0.000000f },
	{ '%',  0.000000f, 0.000000f, 0.000000f, 0.583333f, 0.444444f, 0.000000f, 0.200000f, 0.222222f, 0.000000f },
	{ '&',  0.750000f, 0.500000f, 0.000000f, 0.916667f, 0.861111f, 0.333333f, 0.200000f, 0.333333f, 0.000000f },
	{ '\'', 0.000000f, 0.500000f, 0.000000f, 0.083333f, 0.166667f, 0.000000f, 0.000000f, 0.000000f, 0.000000f },
	{ '(',  0.000000f, 0.500000f, 0.000000f, 0.250000f, 0.444444f, 0.000000f, 0.000000f, 0.222222f, 0.000000f },
	{ ')',  0.250000f, 0.375000f, 0.000000f, 0.000000f, 0.444444f, 0.000000f, 0.200000f, 0.111111f, 0.000000f },
	{ '*',  0.000000f, 0.000000f, 0.000000f, 0.333333f, 0.611111f, 0.666667f, 0.000000f, 0.000000f, 0.000000f },
	{ '+',  0.000000f, 0.000000f, 0.000000f, 0.083333f, 0.388889f, 0.166667f, 0.000000f, 0.000000f, 0.000000f },
	{ ',',  0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.166667f, 0.000000f, 0.200000f, 0.444444f, 0.000000f },
	{ '-',  0.000000f, 0.000000f, 0.000000f, 0.166667f, 0.166667f, 0.166667f, 0.000000f, 0.000000f, 0.000000f },
	{ '.',  0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.166667f, 0.000000f, 0.000000f, 0.333333f, 0.000000f },
	{ '/',  0.000000f, 0.000000f, 0.333333f, 0.250000f, 0.333333f, 0.166667f, 0.200000f, 0.000000f, 0.000000f },
	{ '0',  0.750000f, 0.625000f, 0.333333f, 1.000000f, 0.916667f, 1.000000f, 0.200000f, 0.444444f, 0.000000f },
	{ '1',  0.000000f, 0.375000f, 0.000000f, 0.166667f, 0.555556f, 0.000000f, 0.400000f, 0.444444f, 0.000000f },
	{ '2',  0.750000f, 0.625000f, 0.000000f, 0.416667f, 0.611111f, 0.000000f, 0.400000f, 0.444444f, 0.000000f },
	{ '3',  0.750000f, 0.625000f, 0.000000f, 0.166667f, 0.583333f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ '4',  0.000000f, 0.625000f, 0.000000f, 0.416667f, 0.777778f, 0.166667f, 0.000000f, 0.333333f, 0.000000f },
	{ '5',  1.000000f, 0.500000f, 0.000000f, 0.666667f, 0.500000f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ '6',  0.250000f, 0.500000f, 0.000000f, 1.000000f, 0.583333f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ '7',  1.000000f, 0.625000f, 0.666667f, 0.166667f, 0.500000f, 0.333333f, 0.000000f, 0.222222f, 0.000000f },
	{ '8',  0.750000f, 0.625000f, 0.000000f, 0.916667f, 0.722222f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ '9',  0.750000f, 0.625000f, 0.000000f, 0.416667f, 0.638889f, 0.000000f, 0.200000f, 0.222222f, 0.000000f },
	{ ':',  0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.333333f, 0.000000f, 0.000000f, 0.000000f, 0.000000f },
	{ ';',  0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.388889f, 0.000000f, 0.000000f, 0.444444f, 0.000000f },
	{ '<',  0.000000f, 0.500000f, 0.000000f, 0.333333f, 0.416667f, 0.000000f, 0.000000f, 0.222222f, 0.000000f },
	{ '=',  0.000000f, 0.000000f, 0.000000f, 0.166667f, 0.333333f, 0.333333f, 0.000000f, 0.000000f, 0.000000f },
	{ '>',  0.250000f, 0.375000f, 0.000000f, 0.000000f, 0.444444f, 0.166667f, 0.200000f, 0.111111f, 0.000000f },
	{ '?',  0.750000f, 0.625000f, 0.000000f, 0.000000f, 0.416667f, 0.000000f, 0.000000f, 0.222222f, 0.000000f },
	{ '@',  0.750000f, 0.625000f, 0.333333f, 1.000000f, 0.777778f, 0.666667f, 0.200000f, 0.444444f, 0.000000f },
	{ 'A',  0.250000f, 0.625000f, 0.000000f, 1.000000f, 0.750000f, 0.000000f, 0.400000f, 0.222222f, 0.000000f },
	{ 'B',  0.750000f, 0.750000f, 0.333333f, 0.500000f, 0.944444f, 0.833333f, 0.400000f, 0.444444f, 0.000000f },
	{ 'C',  0.250000f, 0.750000f, 0.333333f, 0.916667f, 0.583333f, 0.500000f, 0.000000f, 0.444444f, 0.000000f },
	{ 'D',  0.750000f, 0.750000f, 0.000000f, 0.500000f, 0.888889f, 0.833333f, 0.400000f, 0.333333f, 0.000000f },
	{ 'E',  0.750000f, 0.625000f, 0.666667f, 0.500000f, 0.750000f, 0.166667f, 0.400000f, 0.444444f, 0.000000f },
	{ 'F',  0.750000f, 0.750000f, 0.666667f, 0.500000f, 0.694444f, 0.166667f, 0.400000f, 0.222222f, 0.000000f },
	{ 'G',  0.250000f, 0.750000f, 0.333333f, 0.916667f, 0.694444f, 0.666667f, 0.000000f, 0.444444f, 0.000000f },
	{ 'H',  1.000000f, 0.500000f, 0.000000f, 1.000000f, 0.722222f, 0.000000f, 0.400000f, 0.222222f, 0.000000f },
	{ 'I',  0.250000f, 0.625000f, 0.000000f, 0.000000f, 0.500000f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'J',  0.000000f, 0.625000f, 0.333333f, 0.500000f, 0.583333f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'K',  0.750000f, 0.500000f, 0.666667f, 0.500000f, 0.888889f, 0.166667f, 0.400000f, 0.222222f, 0.000000f },
	{ 'L',  0.750000f, 0.375000f, 0.000000f, 0.500000f, 0.694444f, 0.500000f, 0.400000f, 0.444444f, 0.000000f },
	{ 'M',  1.000000f, 0.500000f, 0.666667f, 1.000000f, 0.916667f, 1.000000f, 0.400000f, 0.111111f, 0.000000f },
	{ 'N',  1.000000f, 0.250000f, 0.666667f, 1.000000f, 0.916667f, 1.000000f, 0.400000f, 0.111111f, 0.000000f },
	{ 'O',  0.250000f, 0.750000f, 0.000000f, 0.916667f, 0.722222f, 0.833333f, 0.000000f, 0.333333f, 0.000000f },
	{ 'P',  0.750000f, 0.750000f, 0.333333f, 0.500000f, 0.722222f, 0.333333f, 0.400000f, 0.222222f, 0.000000f },
	{ 'Q',  0.250000f, 0.750000f, 0.000000f, 0.916667f, 0.805556f, 0.833333f, 0.000000f, 0.555556f, 0.000000f },
	{ 'R',  0.750000f, 0.750000f, 0.333333f, 0.500000f, 0.916667f, 0.666667f, 0.400000f, 0.222222f, 0.000000f },
	{ 'S',  0.750000f, 0.625000f, 0.000000f, 0.750000f, 0.611111f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'T',  0.750000f, 0.875000f, 0.000000f, 0.000000f, 0.527778f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'U',  1.000000f, 0.500000f, 0.000000f, 1.000000f, 0.694444f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'V',  1.000000f, 0.500000f, 0.000000f, 0.916667f, 0.666667f, 0.000000f, 0.000000f, 0.222222f, 0.000000f },
	{ 'W',  1.000000f, 0.250000f, 0.666667f, 0.833333f, 0.805556f, 0.666667f, 0.200000f, 0.333333f, 0.000000f },
	{ 'X',  1.000000f, 0.500000f, 0.000000f, 0.666667f, 0.694444f, 0.000000f, 0.400000f, 0.222222f, 0.000000f },
	{ 'Y',  1.000000f, 0.500000f, 0.000000f, 0.416667f, 0.638889f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'Z',  1.000000f, 0.750000f, 0.666667f, 0.416667f, 0.666667f, 0.333333f, 0.400000f, 0.444444f, 0.000000f },
	{ '[',  0.000000f, 0.750000f, 0.000000f, 0.000000f, 0.500000f, 0.000000f, 0.000000f, 0.444444f, 0.000000f },
	{ '\\', 0.250000f, 0.000000f, 0.000000f, 0.250000f, 0.333333f, 0.166667f, 0.000000f, 0.000000f, 0.000000f },
	{ ']',  0.000000f, 0.750000f, 0.000000f, 0.000000f, 0.500000f, 0.000000f, 0.000000f, 0.444444f, 0.000000f },
	{ '^',  0.250000f, 0.875000f, 0.000000f, 0.166667f, 0.194444f, 0.166667f, 0.000000f, 0.000000f, 0.000000f },
	{ '_',  0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.400000f, 0.444444f, 0.000000f },
	{ '`',  0.000000f, 0.750000f, 0.000000f, 0.000000f, 0.055556f, 0.000000f, 0.000000f, 0.000000f, 0.000000f },
	{ 'a',  0.000000f, 0.000000f, 0.000000f, 0.500000f, 0.611111f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'b',  0.750000f, 0.250000f, 0.000000f, 0.500000f, 0.805556f, 0.666667f, 0.400000f, 0.333333f, 0.000000f },
	{ 'c',  0.000000f, 0.000000f, 0.000000f, 0.750000f, 0.444444f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'd',  0.000000f, 0.625000f, 0.000000f, 0.750000f, 0.722222f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'e',  0.000000f, 0.000000f, 0.000000f, 0.750000f, 0.555556f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'f',  0.250000f, 0.750000f, 0.000000f, 0.583333f, 0.583333f, 0.000000f, 0.400000f, 0.222222f, 0.000000f },
	{ 'g',  0.000000f, 0.000000f, 0.000000f, 0.666667f, 0.583333f, 0.166667f, 0.600000f, 0.777778f, 0.000000f },
	{ 'h',  0.750000f, 0.250000f, 0.000000f, 0.500000f, 0.805556f, 0.666667f, 0.400000f, 0.222222f, 0.000000f },
	{ 'i',  0.000000f, 0.500000f, 0.000000f, 0.083333f, 0.555556f, 0.000000f, 0.200000f, 0.444444f, 0.000000f },
	{ 'j',  0.000000f, 0.500000f, 0.000000f, 0.000000f, 0.472222f, 0.000000f, 1.000000f, 0.777778f, 0.000000f },
	{ 'k',  0.750000f, 0.250000f, 0.000000f, 0.500000f, 0.777778f, 0.333333f, 0.400000f, 0.222222f, 0.000000f },
	{ 'l',  0.250000f, 0.625000f, 0.000000f, 0.000000f, 0.555556f, 0.000000f, 0.200000f, 0.444444f, 0.000000f },
	{ 'm',  0.000000f, 0.000000f, 0.000000f, 0.833333f, 0.666667f, 0.666667f, 0.400000f, 0.111111f, 0.000000f },
	{ 'n',  0.000000f, 0.000000f, 0.000000f, 0.833333f, 0.527778f, 0.000000f, 0.400000f, 0.222222f, 0.000000f },
	{ 'o',  0.000000f, 0.000000f, 0.000000f, 0.750000f, 0.555556f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'p',  0.000000f, 0.000000f, 0.000000f, 0.500000f, 0.694444f, 0.666667f, 0.800000f, 0.777778f, 0.000000f },
	{ 'q',  0.000000f, 0.000000f, 0.000000f, 0.750000f, 0.611111f, 0.166667f, 0.200000f, 1.000000f, 0.000000f },
	{ 'r',  0.000000f, 0.000000f, 0.000000f, 0.500000f, 0.583333f, 0.333333f, 0.400000f, 0.222222f, 0.000000f },
	{ 's',  0.000000f, 0.000000f, 0.000000f, 0.500000f, 0.500000f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 't',  0.000000f, 0.125000f, 0.000000f, 0.583333f, 0.583333f, 0.000000f, 0.000000f, 0.333333f, 0.000000f },
	{ 'u',  0.000000f, 0.000000f, 0.000000f, 0.833333f, 0.555556f, 0.000000f, 0.200000f, 0.333333f, 0.000000f },
	{ 'v',  0.000000f, 0.000000f, 0.000000f, 0.750000f, 0.500000f, 0.000000f, 0.000000f, 0.222222f, 0.000000f },
	{ 'w',  0.000000f, 0.000000f, 0.000000f, 0.750000f, 0.611111f, 0.666667f, 0.200000f, 0.333333f, 0.000000f },
	{ 'x',  0.000000f, 0.000000f, 0.000000f, 0.333333f, 0.555556f, 0.166667f, 0.400000f, 0.111111f, 0.000000f },
	{ 'y',  0.000000f, 0.000000f, 0.000000f, 0.333333f, 0.611111f, 0.666667f, 0.400000f, 0.666667f, 0.000000f },
	{ 'z',  0.000000f, 0.000000f, 0.000000f, 0.500000f, 0.500000f, 0.000000f, 0.400000f, 0.444444f, 0.000000f },
	{ '{',  0.000000f, 0.625000f, 0.000000f, 0.333333f, 0.444444f, 0.000000f, 0.000000f, 0.333333f, 0.000000f },
	{ '|',  0.000000f, 0.500000f, 0.000000f, 0.000000f, 0.388889f, 0.000000f, 0.000000f, 0.222222f, 0.000000f },
	{ '}',  0.500000f, 0.375000f, 0.000000f, 0.000000f, 0.444444f, 0.000000f, 0.400000f, 0.111111f, 0.000000f },
	{ '~',  0.750000f, 0.500000f, 1.000000f, 0.166667f, 0.222222f, 0.166667f, 0.000000f, 0.000000f, 0.000000f }
};


ASCIIFied::ASCIIFied(CONST WCHAR* image_path, UINT chars_per_line, UINT scaled_image_width, bool gamma_brightness_correction, bool edge_detection)
{
	this->chars_per_line = chars_per_line;
	this->edge_detection = edge_detection;
	this->decoded_art = nullptr;

	if (this->chars_per_line > max_line_length)
	{
		cerr << "Number of characters per single line provided for ASCII art is bigger than allowed. Please provide a smaller number." << endl;
		exit(EXIT_FAILURE);
	}

	wstring extension = this->get_image_extension(image_path);

	if (extension != L"jpeg" && extension != L"jpg" && extension != L"png" && extension != L"gif" && extension != L"bmp" && extension != L"tiff")
	{
		cerr << "File type provided is different than supported. The supported file types are: jpg, png, gif, bmp and tiff." << endl;
		exit(EXIT_FAILURE);
	}

	// Start Gdiplus 
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&this->gdiplusToken, &gdiplusStartupInput, NULL);

	this->image = new Bitmap(image_path);

	if (this->image->GetLastStatus() != Ok)
	{
		cerr << "Error opening the requested image file. Terminating the program." << endl;
		exit(EXIT_FAILURE);
	}

	this->width = this->image->GetWidth();
	this->height = this->image->GetHeight();

	// Check if height and width exceed allowed size
	if (this->width > max_width || this->height > max_height)
	{
		cerr << "File width or height exceedes allowed amount. Please choose a smaller image." << endl;
		exit(EXIT_FAILURE);
	}

	// Scale the input image to the desired width in pixels preserving original aspect ratio
	this->resize_image(scaled_image_width);

	if (gamma_brightness_correction)
	{
		this->apply_gamma_briCon_correction(this->scaledImage);
	}

	// Calculates a suited grid for scaled bitmap and populates each section with corresponding data!
	this->gridify(this->scaledImage);
}

ASCIIFied::~ASCIIFied(VOID)
{
	delete   this->image;
	delete   this->scaledImage;
	delete[] this->sectionMap;
	delete[] this->decoded_art;

	GdiplusShutdown(this->gdiplusToken);
}


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


UINT ASCIIFied::get_output_width(VOID) CONST
{
	return this->chars_per_line;
}

UINT ASCIIFied::get_output_height(VOID) const
{
	return this->char_height;
}

Section* ASCIIFied::get_section_map(VOID) CONST
{
	return this->sectionMap;
}

UINT ASCIIFied::get_section_map_size(VOID) CONST
{
	return this->sectionMapSize;
}

VOID ASCIIFied::generate_image(VOID)
{
	if (this->decoded_art == nullptr)
	{
		cerr << "Failed to produce image from art because it's accessing nullptr reference maybe due to failed art generation or art isn't decoded yet.\nTerminating the program." << endl;
		exit(EXIT_FAILURE);
	}

	// In pixels, each letter is 8x12px + 1px for vertical separation + some breathing space (char_height = vertical amount of characters in art, chars_per_line = same but on horizontal axis)
	INT image_width = this->chars_per_line * 8 + 100;
	INT image_height = this->char_height * 13 + 100;

	// Generate bitmap to draw to
	Bitmap art_output(image_width, image_height);
	Graphics g(&art_output);

	// Sets background to black
	Color color(Color::Black);
	g.Clear(color);

	// Creating proper font
	HFONT hfont = CreateFont(-12, -8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, OEM_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, L"Terminal");

	// Text bounding rectangle
	RECT rc = { 50, 50, image_width,  image_height };

	HDC hdc = g.GetHDC();

	// Setting text background and foreground color to console colors along side with font
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(210, 210, 210));

	// Apply font
	SelectObject(hdc, hfont);

	DrawTextA(hdc, this->decoded_art, strlen(this->decoded_art), &rc, DT_LEFT | DT_TOP | DT_WORDBREAK);

	g.ReleaseHDC(hdc);
	DeleteObject(hfont);

	WCHAR path[MAX_PATH];

	// Get Desktop path
	if (SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, path) != S_OK)
	{
		cerr << "DESKTOP path not found!\n";
		exit(EXIT_FAILURE);
	}

	SYSTEMTIME time_s;
	GetSystemTime(&time_s);

	wstring day = to_wstring(time_s.wDay);
	wstring month = to_wstring(time_s.wMonth);
	wstring year = to_wstring(time_s.wYear);
	wstring hour = to_wstring(time_s.wHour);
	wstring minute = to_wstring(time_s.wMinute);
	wstring second = to_wstring(time_s.wSecond);

	wstring image_path(path);
	image_path += L"\\ascii_art_" + day + L"-" + month + L"-" + year + L"-" + hour + L"-" + minute + L"-" + second + L".png";

	CLSID encId = { 0x557cf406, 0x1a04, 0x11d3,{ 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };
	art_output.Save(image_path.c_str(), &encId);

	cout << "Image saved to your desktop." << endl;
}

VOID ASCIIFied::resize_image(UINT width)
{
	FLOAT aspect_ratio = (FLOAT)width / (FLOAT)this->width;

	// Calculating new dimensions
	UINT new_width = width;
	UINT new_height = (UINT)(this->height * aspect_ratio);

	// Creating scaled image
	this->scaledImage = new Bitmap(new_width, new_height, this->image->GetPixelFormat());

	Graphics g(this->scaledImage);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g.SetPixelOffsetMode(PixelOffsetModeHalf);

	g.DrawImage(this->image, 0, 0, new_width, new_height);
}

INT ASCIIFied::find_closest_divider(INT divisor, INT divider, bool onlyBiggerDivider)
{
	INT dividerS = divider;
	INT dividerB = divider;

	if (onlyBiggerDivider)
	{
		while (true)
		{
			if (divisor % dividerB == 0)
			{
				return dividerB;
			}

			dividerB++;
		}
	}

	while (true)
	{
		if (divisor % dividerB == 0)
		{
			return dividerB;
		}
		else if (divisor % dividerS == 0)
		{
			return dividerS;
		}

		dividerB++;
		dividerS--;
	}
}

wstring	ASCIIFied::get_image_extension(CONST WCHAR* path)
{
	wstring imagePath(path);
	INT pos = imagePath.find_last_of(L".");
	wstring extension = imagePath.substr(pos + 1);

	// Conversion to lower case
	std::transform(extension.begin(), extension.end(), extension.begin(), towlower);

	return extension;
}

VOID ASCIIFied::gridify(Bitmap* base)
{
	UINT scaled_width = base->GetWidth();
	UINT scaled_height = base->GetHeight();

	// Section width and height are expressed in pixels!

	// Section width is actually the closest natural divider of scaled image width
	UINT sectionWidth = scaled_width / this->find_closest_divider(scaled_width, this->chars_per_line, false);

	// Section height is at least 1.6 times its width and must be devisible by height of the image. Latin alphabet characters are always taller than they are wide.
	// 1.6 Works well for font Terminal 8x12px but other values might suit better for different fonts

	// Disabled because most of the time the closest full divider of image height is much more than 1.6 times bigger than section width, yielding highly distorted (elongated) ASCII art 
	//UINT sectionHeight = this->find_closest_divider(scaled_height, (INT)round((FLOAT)sectionWidth * 1.6f), true);

	// Mathematically rounding the result 
	UINT sectionHeight = (INT)round((FLOAT)sectionWidth * 1.6f);

	// Updates chars_per_line and char_height with values that are actually going to be used.
	this->chars_per_line = scaled_width / sectionWidth;

	//this->char_height     = scaled_height / sectionHeight;
	// Celiling the result so that there is enough space for all sections on vertical axis when allocating the memory
	this->char_height = (UINT)ceilf((FLOAT)(scaled_height / sectionHeight));

	// Allocating memory for section map
	UINT allocation_size = this->chars_per_line * this->char_height;
	this->sectionMap = new Section[allocation_size];


	// Extremely important variable, it will indicate the amount of sections initiated!
	UINT iterator = 0;

	// Populating sections with data
	// Notice the -1, reserving the last row for resedue pixels
	for (UINT i = 0; i < this->char_height - 1; ++i)
	{
		for (UINT j = 0; j < this->chars_per_line; ++j)
		{
			// Order of sections is left to right first, from top to bottom
			INT positionX = j * sectionWidth;
			INT positionY = i * sectionHeight;

			this->sectionMap[iterator] = Section(positionX, positionY, sectionWidth, sectionHeight, base);

			if (this->edge_detection)
			{
				this->sectionMap[iterator++].calculate_disected_brightness();
			}
			else
			{
				this->sectionMap[iterator++].calculate_average_brightness();
			}
		}
	}

	// Last row of sections where Y coordinate is always the same (here precalculated to be used in loop), only X is changing
	INT positionY = this->char_height * sectionHeight;
	// The resedue amount of pixels for the last row of sections
	UINT height = scaled_height % sectionHeight;

	// If the resedue amount of pixels for the last row of sections is greater than 35% of rest of sections height for the image, take it into consideration
	// Otherwise ignore the last row of pixels because it's insignificant and small!
	if (height > sectionHeight * 0.35)
	{
		for (UINT i = 0; i < this->chars_per_line; ++i)
		{
			INT positionX = i * sectionWidth;

			this->sectionMap[iterator] = Section(positionX, positionY, sectionWidth, height, base);

			if (this->edge_detection)
			{
				this->sectionMap[iterator++].calculate_disected_brightness();
			}
			else
			{
				this->sectionMap[iterator++].calculate_average_brightness();
			}
		}
	}

	// !!!
	this->sectionMapSize = iterator - 1;
}

VOID ASCIIFied::apply_gamma_briCon_correction(Bitmap* base)
{
	LevelsParams levels_params;
	BrightnessContrastParams briConParams;

	// Negative gamma correction (influencing midtones on levels = gamma correction)
	levels_params.highlight = 100;
	levels_params.midtone = -15;
	levels_params.shadow = 0;

	// Typical contrast boost
	briConParams.brightnessLevel = 15;
	briConParams.contrastLevel = 17;

	Levels levels;
	levels.SetParameters(&levels_params);

	BrightnessContrast briCon;
	briCon.SetParameters(&briConParams);

	base->ApplyEffect(&levels, NULL);
	base->ApplyEffect(&briCon, NULL);
}

FLOAT ASCIIFied::similarity(CONST FLOAT a[], FLOAT b[])
{
	// Implementation of euclidean distance
	// https://en.wikipedia.org/wiki/Euclidean_distance

	FLOAT powSum = 0;
	FLOAT similarity = 0;

	for (INT i = 0; i < 9; i++)
	{
		powSum += pow((a[i] - b[i]), 2.f);
	}

	similarity = sqrt(powSum);

	return similarity;
}

CHAR ASCIIFied::brightness_to_char(CONST map<FLOAT, vector<CHAR>>& map, FLOAT brightness)
{
	auto it = min_element(map.begin(), map.end(),
		[&](CONST auto& p1, CONST auto& p2)
		{
			return abs(p1.first - brightness) < abs(p2.first - brightness);
		});

	srand((UINT)time(NULL));
	INT randomIndex = rand() % it->second.size();

	return it->second[randomIndex];
}

CHAR ASCIIFied::brightness_map_to_char(CONST Char map[], FLOAT brightness_map[])
{
	FLOAT similarity_map[95];

	// Contains euclidean distance of each character to the provided brightness map of a single section.
	// The smallest distance is actually the most similar array on a chart
	for (INT i = 0; i < 95; i++)
	{
		similarity_map[i] = this->similarity(map[i].section_brightness, brightness_map);
	}

	INT char_index = distance(similarity_map, min_element(similarity_map, similarity_map + 95));

	return map[char_index].character;
}

VOID ASCIIFied::decode_art(VOID)
{
	if (this->decoded_art != nullptr)
	{
		cout << "Art is already decoded!" << endl;
		return;
	}

	CONST UINT line_break = this->get_output_width();
	UINT iterator = 0;

	// Memory allocation for ASCII art
	// Multiplied by 0.2f to add more memory for new line character which isn't included in section map size itself
	UINT allocationSize = this->sectionMapSize + (UINT)(this->sectionMapSize * 0.2f);
	this->decoded_art = new CHAR[allocationSize];
	memset(decoded_art, 0, allocationSize);

	if (this->edge_detection)
	{
		for (UINT i = 0; i < this->sectionMapSize; ++i)
		{
			if (i % line_break == 0)
			{
				this->decoded_art[iterator++] = '\n';
			}

			this->decoded_art[iterator++] = this->brightness_map_to_char(luma_char_map_x, this->sectionMap[i].averageBrightnessMap);
		}
	}
	else
	{
		for (UINT i = 0; i < this->sectionMapSize; ++i)
		{
			if (i % line_break == 0)
			{
				this->decoded_art[iterator++] = '\n';
			}

			this->decoded_art[iterator++] = brightness_to_char(luma_char_map, this->sectionMap[i].averageBrightness);
		}
	}
}

VOID ASCIIFied::output_to_console(VOID)
{
	CONSOLE_FONT_INFOEX cfi;

	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 12;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Terminal");

	HANDLE readHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Setting font of the console to the optimal one
	SetCurrentConsoleFontEx(readHandle, FALSE, &cfi);

	CONSOLE_SCREEN_BUFFER_INFOEX info{ 0 };
	info.cbSize = sizeof(info);

	GetConsoleScreenBufferInfoEx(readHandle, &info);
	info.srWindow.Right = 499;
	info.srWindow.Bottom = 499;

	// Setting console character width and height properties so that art isn't cut off by smaller screen buffer for console
	SetConsoleScreenBufferInfoEx(readHandle, &info);

	// Decode the art from section map data
	this->decode_art();

	cout << this->decoded_art << endl << endl;
}
