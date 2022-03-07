# img2ascii
**img2ascii** is [ASCII art] generator that accepts most raster image formats written using [GDI+] and [Windows API]

- JPG
- PNG
- GIF
- BMP
- TIFF

## Features
It's command line based program, which means you supply options via console.

**`-w{number}`** specifies number of characters in a single line of ASCII art. Maximum is 400. You can use predefined templates: 
-tiny, -medium and -large instead.

**`-s{number}`** specifies at which width the image should be scaled preserving aspect ratio before processing. 
Smaller resolution will yield faster processing.

**`-g`** specifies that to the image is applied negative gamma correction and contrast is boosted. 
This could help flat, overly exposed or underexposed images have more defined edges!

**`-e`** (**edge detection**) specifies that the program should use edge detection and output improved art by using more complex algorithm for generation Processing time is slightly increased.


![Alt text](demonstration/example_1.png?raw=true)

## Algorithms
The program features **2 distinct algorithms** for art generation.

Before any processing the image is scaled up or down according to the input. Scaling the image down could result in much faster processing whereas scaling up could
result in finer detail showing in the final art.

The **default algorithm** is ***faster*** to execute. It takes the scaled image and devides it into grid, where each section of that grid is a portion of the image.
Grid size is calculated according to the ammount of characters per line in the art and image width in pixels.
The section luminance ([relative luminance]) is then calculated and [normalized], thus having quantitative value which is then compared to the
character map where right character will be chosen to represent that section.

**Edge detection** algorithm is basically further division of already mentioned sections into 9 non equal parts with predetermined, constant ratios.
Each part of that section will have its [relative luminance] calculated and [normalized]. From data structure point of perspective, it's just an array of 9 floating point
numbers which represent normalized luminance of the section.
That array is compared to *different* __**custom**__ and **precalculated** character map which consists of each printable ASCII character represented by
9 floating point numbers that are generated much like section luminance is. The characters luminance map is precalculated and optimized using [Terminal 8x12px] monospaced raster font, which should be available on any Windows machine.

Comparing each section to the character map is basically searching for the most similar vector to the provided one out of the bunch.
Best results after are given using [Euclidean distance] algorithm.

![Alt text](demonstration/example_2.png?raw=true)

## TO DO:
- Implement multithreading which should speed up the processing
- Add colour to characters which would be averaged out from sections yielding coloured ASCII art

[ASCII art]: <https://en.wikipedia.org/wiki/ASCII_art>
[GDI+]: <https://en.wikipedia.org/wiki/Graphics_Device_Interface>
[Windows API]: <https://en.wikipedia.org/wiki/Windows_API>
[relative luminance]: <https://en.wikipedia.org/wiki/Relative_luminance>
[normalized]: <https://en.wikipedia.org/wiki/Normalized_number>
[Euclidean distance]: <https://en.wikipedia.org/wiki/Euclidean_distance>
[Terminal 8x12px]: <https://en.wikipedia.org/wiki/Terminal_(typeface)>
