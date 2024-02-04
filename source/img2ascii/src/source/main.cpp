#include "img2ascii.hpp"

INT wmain(INT argc, WCHAR* argv[])
{
	// Load arguments passed as wide strigns into the vector container
	vector<wstring> arguments(argv, argv + argc);

	INT          line_width = MEDIUM_W128;
	INT          scaled_resolution = SCALED_BITMAP_WIDTH;
	bool         gamma_contrast_correction = false;
	bool         edge_detection = false;
	CONST WCHAR* image_path;

	if (argc < 2)
	{
		cout << endl << "Welcome to img2ascii by Uki99 version 1.0.5 built on summer 2021. With parameter -h you can display help." << endl;
	}
	else
	{
		image_path = arguments[1].c_str();

		for (wstring s : arguments)
		{
			if (s[0] == L'-')
			{
				if (s[1] == L'h')
				{
					cout << "\nimg2ascii is a program for converting raster images to beautiful ASCII art.\n\n";
					cout << "First argument must always be a path to the desired image for conversion.\n\n";
					cout << "The rest of parameters are optional but could lead to improved art output.\n\n";
					cout << "-w{number} specifies number of characters in a single line of ASCII art. Maximum is 400. You can use predefined templates: -tiny, -medium and -large instead\n\n";
					cout << "-s{number} specifies at which width the image should be scaled preserving aspect ratio before processing. Smaller resolution will yield faster processing.\n\n";
					cout << "-g specifies that to the image is applied negative gamma correction and contrast is boosted. This could help flat, overly exposed or underexposed images have more defined edges\n\n";
					cout << "-e specifies that the program should use edge detection and output improved art by using more complex algorithm for generating. Processing time is increased.\n\n";
					cout << "An example of executing would be: img2ascii myImage.png -large -s500" << endl << endl;

					return 0;
				}
				else if (s[1] == L'w' && s.length() >= 4)
				{
					line_width = stoi(s.substr(2));
				}
				else if (s[1] == L's' && s.length() >= 4)
				{
					scaled_resolution = stoi(s.substr(2));
				}
				else if (s == L"-g")
				{
					gamma_contrast_correction = true;
				}
				else if (s == L"-e")
				{
					edge_detection = true;
				}
				else if (s == L"-tiny")
				{
					line_width = TINY_W96;
				}
				else if (s == L"-medium")
				{
					line_width = MEDIUM_W128;
				}
				else if (s == L"-large")
				{
					line_width = LARGE_W192;
				}
			}
		}

		ASCIIFied image(image_path, line_width, scaled_resolution, gamma_contrast_correction, edge_detection);

		image.output_to_console();

		cout << endl << endl << "Do you want to generate an image for this art? (Y/N)" << endl;
		string input;
		cout << "> ";
		cin >> input;

		if (tolower(input[0]) == 'y' && input.length() < 2)
		{
			image.generate_image();
		}
	}

	return 0;
}