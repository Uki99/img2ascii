#include <iostream>
#include "fontdata.h"

#include <atlbase.h>
#include <msxml6.h>
#include <comutil.h>
#include <atlimage.h>

#include <future>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>

using namespace std;

struct Char
{
    char c;
    union
    {
        float b[9];
        struct
        {
            float _0, _1, _2, _3, _4, _5, _6, _7, _8;
        };
    };
};

std::string setting_url = "http://blog-photocontact.vellozzi.com/wp-content/uploads/2011/10/Martin-Schoeller-Barack-Obama-portrait.jpeg";
bool        setting_gamma = true;
bool        setting_filter = true;
bool        setting_multi = true;
bool        setting_influence = true;
bool        setting_simple = false;
bool        setting_random = false;
int         setting_width = 79;

vector<Char> chars;

unsigned long getPixelAVG(const CImage& image, int x, int y)
{
    auto pixel = image.GetPixel(x, y);
    auto a = (pixel >> 24) & 0xff;
    auto b = (pixel >> 16) & 0xff;
    auto g = (pixel >> 8) & 0xff;
    auto r = pixel & 0xff;

    return (r * 2126 + g * 7152 + b * 722) / 10000;
}

unsigned long getPixelAVGFromASCII(int x, int y)
{
    auto pixel = asciipngdata[y * 760 + x];
    auto a = (pixel >> 24) & 0xff;
    auto b = (pixel >> 16) & 0xff;
    auto g = (pixel >> 8) & 0xff;
    auto r = pixel & 0xff;

    return (r * 2126 + g * 7152 + b * 722) / 10000;
}







void generateData()
{
    map<float, vector<char>> countToC;
    float maxB[9] = { 0 };

    for (auto c = ' '; c <= '~'; ++c)
    {
        auto offset = c - ' ';
        offset *= 8;
        int count[9] = { 0 };

        vector<float> brightness;

        if (setting_influence)
        {
            // Top left
            for (auto y = 0; y < 3; ++y)
                for (auto x = offset; x < offset + 2; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[0];

            // Top
            for (auto y = 0; y < 3; ++y)
                for (auto x = offset + 2; x < offset + 6; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[1];

            // Top right
            for (auto y = 0; y < 3; ++y)
                for (auto x = offset + 6; x < offset + 8; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[2];

            // Left
            for (auto y = 3; y < 9; ++y)
                for (auto x = offset + 0; x < offset + 2; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[3];

            // Center
            for (auto y = 2; y < 10; ++y)
                for (auto x = offset + 1; x < offset + 7; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[4];

            // Right
            for (auto y = 3; y < 9; ++y)
                for (auto x = offset + 6; x < offset + 8; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[5];

            // Bottom left
            for (auto y = 9; y < 12; ++y)
                for (auto x = offset + 0; x < offset + 2; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[6];

            // Bottom
            for (auto y = 9; y < 12; ++y)
                for (auto x = offset + 2; x < offset + 6; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[7];

            // Bottom right
            for (auto y = 9; y < 12; ++y)
                for (auto x = offset + 6; x < offset + 8; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[8];

            brightness = {
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
        }
        else
        {
            // Top left
            for (auto y = 0; y < 3; ++y)
                for (auto x = offset; x < offset + 2; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[0];

            // Top
            for (auto y = 0; y < 3; ++y)
                for (auto x = offset + 2; x < offset + 6; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[1];

            // Top right
            for (auto y = 0; y < 3; ++y)
                for (auto x = offset + 6; x < offset + 8; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[2];

            // Left
            for (auto y = 3; y < 9; ++y)
                for (auto x = offset + 0; x < offset + 2; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[3];

            // Center
            for (auto y = 3; y < 9; ++y)
                for (auto x = offset + 2; x < offset + 6; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[4];

            // Right
            for (auto y = 3; y < 9; ++y)
                for (auto x = offset + 6; x < offset + 8; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[5];

            // Bottom left
            for (auto y = 9; y < 12; ++y)
                for (auto x = offset + 0; x < offset + 2; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[6];

            // Bottom
            for (auto y = 9; y < 12; ++y)
                for (auto x = offset + 2; x < offset + 6; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[7];

            // Bottom right
            for (auto y = 9; y < 12; ++y)
                for (auto x = offset + 6; x < offset + 8; ++x)
                    if (getPixelAVGFromASCII(x, y) > 150) ++count[8];

            brightness = {
                (float)count[0] / (2.f * 3.f),
                (float)count[1] / (4.f * 3.f),
                (float)count[2] / (2.f * 3.f),
                (float)count[3] / (2.f * 6.f),
                (float)count[4] / (4.f * 6.f),
                (float)count[5] / (2.f * 6.f),
                (float)count[6] / (2.f * 3.f),
                (float)count[7] / (4.f * 3.f),
                (float)count[8] / (2.f * 3.f),
            };
        }

        for (int i = 0; i < 9; ++i)
        {
            maxB[i] = max<>(maxB[i], brightness[i]);
        }

        chars.push_back({ c, brightness[0], brightness[1], brightness[2], brightness[3], brightness[4], brightness[5], brightness[6], brightness[7] });
    }

    for (auto& ch : chars)
    {
        for (int i = 0; i < 9; ++i)
        {
            ch.b[i] /= maxB[i];
        }
    }
}


int main(void)
{

	return 0;
}