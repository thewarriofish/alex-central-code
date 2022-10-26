/* Alex Yu */
/* Bitmap Lab */
/* TAs: Chris Muncey and Zach McMichael */
/* Completed 2/22/2021 */
//https://www.youtube.com/watch?v=1yDXommZwWQ

#include <cstdint>
#include <stdint.h>
#include <cstdio>
using namespace std;

#pragma pack(push,2)  //structs to be used
struct BitmapFileHeader
{
	uint16_t type;
	uint32_t size;
	uint32_t res;
	uint32_t offset;
};
struct BitmapInfoHeader
{
	uint32_t size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitcount;
	uint32_t compression;
	uint32_t imagesize;
	uint32_t x_pixels_per_meter;
	uint32_t y_pixels_per_meter;
	uint32_t color_used;
	uint32_t color_important;
};
#pragma pack(pop)

struct Pixel
{
	char b,g,r;
};

int main(int argc, char** argv)
{
	FILE *fl;		//setup file for reading
	BitmapFileHeader file; //variables to set image headers to
	BitmapInfoHeader info;
	fl = fopen(argv[1], "rb");
	if(nullptr == fl) //error message if file is unable to be opened
	{
		printf("Could not open file.\n");
		return -1;
	}
	fread(&file, 1, sizeof(file), fl); //reads file header
	fread(&info, 1, sizeof(info), fl); //reads info header

	int width = info.width; //sets image dimensions
	int height = info.height;
	int pixels = width * height;

	Pixel *pa = new Pixel[pixels]; //allocates memory for all pixels

	int padding = 0; //finds how much padding is used in image
	if(((width * 3) & 3) != 0)
	{
		padding = 4 - ((width * 3) & 3);
	}
	
	fseek(fl, file.offset, SEEK_SET); //ensures file is beginning fread at the pixels

	for(int i = 0; i < height; ++i) //reads pixels into pixel array
	{
		for(int j = width * i; j < width * (i+1); ++j)
		{
			fread(&pa[j], 1, sizeof(Pixel), fl); //reads into position j of pixel array, 3 bytes worth of data from fl
		}
		fseek(fl, padding, SEEK_CUR); //skips any padding present
	}
	fclose(fl);

	for(int j = 0; j < pixels; ++j) //inverts pixel colors
	{
		pa[j].b = 255 - pa[j].b;
		pa[j].g = 255 - pa[j].g;
		pa[j].r = 255 - pa[j].r;
	}

	Pixel temp; //intializing temp pixel variable for next function
	for(int h = 0; h < height; ++h) //flips image horizontally
	{
		for(int w = width * h; w < (width*h) + (width/2); ++w) //width*h represents row, (width*h) + (width/2) represents middle of row
		{
			temp = pa[w]; //sets temp to current pixel
			pa[w] = pa[(width*(h+1)) - (w - (width * h)) - 1]; //swaps current and correlating pixels
			pa[(width*(h+1)) - (w - (width * h)) - 1] = temp; //sets correlating pixel to temp
		}
	}

	FILE *fo; //setup file for writing
	fo = fopen("output.bmp", "wb");
	fwrite(&file, 1, sizeof(file), fo); //writes file header 
	fwrite(&info, 1, sizeof(info), fo); //writes info header
	char pad[4] = {0,0,0,0}; //variable to represent padding
	fseek(fo, file.offset, SEEK_SET); //ensures file starts writing after headers
	for(int k = 0; k < height; ++k) //pixels are written in
	{
		for(int l = width * k; l < width * (k+1); ++l)
		{
			fwrite(&pa[l], 1, sizeof(Pixel), fo); //3 bytes are written, taken from pixel at position l of pixel array
		}
		fwrite(pad, 1, padding, fo); //inserts padding when needed using padding variable
	}
	fclose(fo);

	delete[] pa; //deletes allocated memory
	return 0;
}

