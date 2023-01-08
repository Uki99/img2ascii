#include "img2ascii.hpp"

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

			this->sectionMap[iterator++] = Section(positionX, positionY, sectionWidth, sectionHeight, base, this->edge_detection);
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

			this->sectionMap[iterator++] = Section(positionX, positionY, sectionWidth, height, base, this->edge_detection);
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