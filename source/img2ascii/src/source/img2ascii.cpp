#include "img2ascii.hpp"

ASCIIFied::ASCIIFied(CONST WCHAR* image_path, UINT chars_per_line, UINT scaled_image_width, bool gamma_brightness_correction, bool advaned_algorithm)
{
	this->image_path = image_path;
	this->chars_per_line = chars_per_line;
	this->scaled_image_width = scaled_image_width;
	this->gamma_brightness_correction = gamma_brightness_correction;
	this->advanced_algorithm = advaned_algorithm;
	this->image = nullptr;
	this->decoded_art = nullptr;
	this->section_map = nullptr;
	this->gdiplus_token = 0;
	this->section_map_size = 0;
	this->char_height = 0;

	// Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&this->gdiplus_token, &gdiplusStartupInput, NULL);

	this->image = new Bitmap(this->image_path);

	if (this->image->GetLastStatus() != Ok)
	{
		cerr << "Error opening the requested image file at location [ " << this->image_path << " ]. Terminating the program." << endl;
		exit(EXIT_FAILURE);
	}

	if (this->chars_per_line > MAX_LINE_LENGTH)
	{
		cerr << "Number of characters per single line provided for ASCII art is bigger than allowed, which is " << MAX_LINE_LENGTH << ". Please provide a smaller number." << endl;
		exit(EXIT_FAILURE);
	}
	
	if (!this->is_supported_bitmap_type())
	{
		cerr << "File type provided is different than supported. The supported file types are: jpg, png, bmp and tiff." << endl;
		exit(EXIT_FAILURE);
	}

	if (!this->is_supported_bitmap_size())
	{
		cerr << "File width or height exceedes allowed amount. Current allowed dimension (width x height) are: " << MAX_WIDTH << " x " << MAX_HEIGHT << endl << "Please choose a smaller image." << endl;
		exit(EXIT_FAILURE);
	}

	// Scale the input image to the desired width in pixels preserving original aspect ratio
	// It will not resize the input image if 0 is provided as scaled image width
	if (this->scaled_image_width != 0)
	{
		this->resize_image(this->scaled_image_width);
	}

	if (this->gamma_brightness_correction)
	{
		this->apply_gamma_brightness_contrast_correction(this->image);
	}

	// Calculates a suited grid for scaled bitmap and populates each section with corresponding data!
	this->gridify(this->image);

	// Decode the art from section map data
	this->decode_art();
}

ASCIIFied::~ASCIIFied(VOID)
{
	delete   this->image;
	delete[] this->section_map;
	delete[] this->decoded_art;

	GdiplusShutdown(this->gdiplus_token);
}

UINT ASCIIFied::get_output_width(VOID) CONST
{
	return this->chars_per_line;
}

UINT ASCIIFied::get_output_height(VOID) CONST
{
	return this->char_height;
}

Section* ASCIIFied::get_section_map(VOID) CONST
{
	return this->section_map;
}

UINT ASCIIFied::get_section_map_size(VOID) CONST
{
	return this->section_map_size;
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

VOID ASCIIFied::resize_image(UINT desired_width)
{
	auto aspect_ratio = (FLOAT)(desired_width / this->image->GetWidth());

	// Calculating new dimensions
	auto new_width = desired_width;
	auto new_height = (UINT)(this->image->GetHeight() * aspect_ratio);

	// Creating scaled image
	Bitmap* scaled_image = new Bitmap(new_width, new_height, this->image->GetPixelFormat());

	Graphics g(scaled_image);
	g.DrawImage(this->image, 0, 0, new_width, new_height);

	// Free memory from old image and asign new pointer
	delete this->image;
	this->image = scaled_image;
	this->image = scaled_image;
}

INT ASCIIFied::find_closest_divider(INT divisor, INT divider)
{
	INT dividerS = divider;
	INT dividerB = divider;

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

VOID ASCIIFied::gridify(Bitmap* base)
{
	UINT scaled_width = base->GetWidth();
	UINT scaled_height = base->GetHeight();

	// Section width and height are expressed in pixels!

	// Section width is actually the closest natural divider of scaled image width
	UINT sectionWidth = scaled_width / this->find_closest_divider(scaled_width, this->chars_per_line);

	// Mathematically rounding the result 
	UINT sectionHeight = (INT)round((FLOAT)sectionWidth * FONT_ASPECT_RATIO);

	// Updates chars_per_line and char_height with values that are actually going to be used.
	this->chars_per_line = scaled_width / sectionWidth;

	//this->char_height     = scaled_height / sectionHeight;
	// Celiling the result so that there is enough space for all sections on vertical axis when allocating the memory
	this->char_height = (UINT)ceilf((FLOAT)(scaled_height / sectionHeight));

	// Allocating memory for section map
	UINT allocation_size = this->chars_per_line * this->char_height;
	this->section_map = new Section[allocation_size];


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

			this->section_map[iterator++] = Section(positionX, positionY, sectionWidth, sectionHeight, base, this->advanced_algorithm);
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

			this->section_map[iterator++] = Section(positionX, positionY, sectionWidth, height, base, this->advanced_algorithm);
		}
	}

	// !!!
	this->section_map_size = iterator - 1;
}

VOID ASCIIFied::apply_gamma_brightness_contrast_correction(Bitmap* base)
{
	LevelsParams levels_params { };
	BrightnessContrastParams briConParams { };

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

	// Applying corrections
	base->ApplyEffect(&levels, NULL);
	base->ApplyEffect(&briCon, NULL);
}

FLOAT ASCIIFied::vector_similarity(CONST FLOAT a[], FLOAT b[])
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

CHAR ASCIIFied::decode_section(CONST map<FLOAT, vector<CHAR>>& character_brightness_map, FLOAT character_brightness)
{
	auto it = min_element(character_brightness_map.begin(), character_brightness_map.end(),
		[&](CONST auto& p1, CONST auto& p2)
		{
			return abs(p1.first - character_brightness) < abs(p2.first - character_brightness);
		});

	srand((UINT)time(NULL));
	INT randomIndex = rand() % it->second.size();

	return it->second[randomIndex];
}

CHAR ASCIIFied::advanced_algorithm_decode_section(CONST DisectedChar desected_character_brightness_map[], FLOAT disected_section[])
{
	FLOAT similarity_map[95] { };

	// Contains euclidean distance of each character to the provided brightness map of a single section.
	// The smallest distance is actually the most similar array on a chart
	for (INT i = 0; i < 95; i++)
	{
		similarity_map[i] = this->vector_similarity(desected_character_brightness_map[i].section_brightness, disected_section);
	}

	INT char_index = distance(similarity_map, min_element(similarity_map, similarity_map + 95));

	return desected_character_brightness_map[char_index].character;
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
	UINT allocationSize = this->section_map_size + (UINT)(this->section_map_size * 0.2f);
	this->decoded_art = new CHAR[allocationSize];
	memset(decoded_art, 0, allocationSize);

	if (this->advanced_algorithm)
	{
		for (UINT i = 0; i < this->section_map_size; ++i)
		{
			if (i % line_break == 0)
			{
				this->decoded_art[iterator++] = '\n';
			}

			this->decoded_art[iterator++] = this->advanced_algorithm_decode_section(advanced_algorithm_character_map, this->section_map[i].average_brightness_map);
		}
	}
	else
	{
		for (UINT i = 0; i < this->section_map_size; ++i)
		{
			if (i % line_break == 0)
			{
				this->decoded_art[iterator++] = '\n';
			}

			this->decoded_art[iterator++] = decode_section(fast_character_map, this->section_map[i].average_brightness);
		}
	}
}

bool ASCIIFied::is_supported_bitmap_type(VOID)
{
	if (this->image == nullptr)
	{
		cerr << "Section tried to access null pointer image. Terminating the program" << endl;
		exit(EXIT_FAILURE);
	}

	array<GUID, 4> supported_bitmap_types =
	{
		ImageFormatJPEG,
		ImageFormatPNG,
		ImageFormatBMP,
		ImageFormatTIFF
	};

	GUID image_format;
	this->image->GetRawFormat(&image_format);

	auto found = std::find(std::begin(supported_bitmap_types), std::end(supported_bitmap_types), image_format);

	if (found == std::end(supported_bitmap_types))
		return false;
	else
		return true;
}

bool ASCIIFied::is_supported_bitmap_size(VOID)
{
	return !(this->image->GetWidth() > MAX_WIDTH || this->image->GetHeight() > MAX_HEIGHT);
}

VOID ASCIIFied::output_to_console(VOID)
{
	CONSOLE_FONT_INFOEX cfi { };

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

	cout << this->decoded_art << endl << endl;
}
