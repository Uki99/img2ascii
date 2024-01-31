#include "includes.h"
using namespace std;

vector<Char> character_brightness_map;

unsigned long GetPixelAvgBrightnessFromData(int x, int y)
{
	// Row major order pixel configuration in array fontdata
	// https://en.wikipedia.org/wiki/Row-_and_column-major_order

	// Relative luminance based on RGB values
	// https://en.wikipedia.org/wiki/Relative_luminance

    auto pixel = fontdata[y * 760 + x];
    auto alpha = (pixel >> 24) & 0xff;
    auto blue = (pixel >> 16) & 0xff;
    auto green = (pixel >> 8) & 0xff;
    auto red = pixel & 0xff;

    return (red * 2126 + green * 7152 + blue * 722) / 10000;
}

void generateData()
{
    float maxB[9] = { 0 };

	// For each printable ASCII character in table...
    for (auto c = ' '; c <= '~'; ++c)
    {
		// Offset regarding to the first printable character in ASCII table (space).
		// It could be 0, 1, 2, 3...
        auto offset = (c - ' ') * 8; // Multiply by 8 because it's font width. (Terminal, 8x12px)
        
		// Resetting these.
		int count[9] = { 0 };
        vector<float> brightness;

		// Counting number of lit pixels in each section
		{
			// Top left
			for (auto y = 0; y < 3; ++y)
				for (auto x = offset + 0; x < offset + 2; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[0];

			// Top
			for (auto y = 0; y < 3; ++y)
				for (auto x = offset + 2; x < offset + 6; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[1];

			// Top right
			for (auto y = 0; y < 3; ++y)
				for (auto x = offset + 6; x < offset + 8; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[2];

			// Left
			for (auto y = 3; y < 9; ++y)
				for (auto x = offset + 0; x < offset + 2; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[3];

			// Center
			for (auto y = 2; y < 10; ++y)
				for (auto x = offset + 1; x < offset + 7; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[4];

			// Right
			for (auto y = 3; y < 9; ++y)
				for (auto x = offset + 6; x < offset + 8; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[5];

			// Bottom left
			for (auto y = 9; y < 12; ++y)
				for (auto x = offset + 0; x < offset + 2; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[6];

			// Bottom
			for (auto y = 9; y < 12; ++y)
				for (auto x = offset + 2; x < offset + 6; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[7];

			// Bottom right
			for (auto y = 9; y < 12; ++y)
				for (auto x = offset + 6; x < offset + 8; ++x)
					if (GetPixelAvgBrightnessFromData(x, y) > 150) ++count[8];
		}

		// Number of lit pixels divided by section dimensions respectively
		brightness = 
		{
			(float)count[0] / (2.f * 3.f),
			(float)count[1] / (4.f * 3.f),
			(float)count[2] / (2.f * 3.f),
			(float)count[3] / (2.f * 6.f),
			(float)count[4] / (6.f * 8.f),
			(float)count[5] / (2.f * 6.f),
			(float)count[6] / (2.f * 3.f),
			(float)count[7] / (4.f * 3.f),
			(float)count[8] / (2.f * 3.f),
		};

		// Get max brightness level overall for each section
        for (int i = 0; i < 9; ++i)
        {
            maxB[i] = max<>(maxB[i], brightness[i]);
        }

        character_brightness_map.push_back({ c, brightness[0], brightness[1], brightness[2], brightness[3], brightness[4], brightness[5], brightness[6], brightness[7] });
    }

	// Normalize brightness in range 0-1 using maximum overall brightness per each section
    for (auto& character_element : character_brightness_map)
    {
        for (int i = 0; i < 9; ++i)
        {
            character_element.section_brightness[i] /= maxB[i];
        }
    }
}


int main(void)
{
	generateData();
    string final_map = "static const Char luma_char_map_x[95] =\n{\n";
    char character_map_element_tok[] = "\t{ \'%c\', %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff },\n";

	// Classic example of metaprogramming, in which we generate a c++ structure using an automated program.
	// https://en.wikipedia.org/wiki/Metaprogramming

    for (auto& ch : character_brightness_map)
    {
        char buffer[200] {};
        snprintf(buffer, sizeof(buffer), character_map_element_tok, ch.character, ch.TopLeft, ch.TopMiddle, ch.TopRight, ch.MiddleLeft, ch.Middle, ch.MiddleRight, ch.BottomLeft, ch.BottomMiddle, ch.BottomRight);
        final_map += buffer;
    }

    final_map.pop_back();
    final_map += "\n};";

    cout << final_map << endl;

    system("pause");

	return 0;
}