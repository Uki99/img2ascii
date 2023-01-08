#pragma once
#include <iostream>
#include "src/fontdata.h"
#include <string>
#include <map>
#include <vector>

struct Char
{
	char character;

	union
	{
		float section_brightness[9];
		struct
		{
			float TopLeft, TopMiddle, TopRight, MiddleLeft, Middle, MiddleRight, BottomLeft, BottomMiddle, BottomRight;
		};
	};
};