#pragma once
#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Image
{
public:

	struct Header
	{
		Header();
		char idLength;
		char colorMapType;
		char dataTypeCode;
		short colorMapOrigin;
		short colorMapLength;
		char colorMapDepth;
		short xOrigin;
		short yOrigin;
		short width;
		short height;
		char bitsPerPixel;
		char imageDescriptor;

		Header& operator=(const Header rhs);
		
		struct Pixel
		{
			Pixel() {
				this->blue = 0;
				this->green = 0;
				this->red = 0;
			}

			Pixel(unsigned char blue, unsigned char green, unsigned char red) {
				this->blue = blue;
				this->green = green;
				this->red = red;
			}

			unsigned char red;
			unsigned char blue;
			unsigned char green;
			Pixel operator*(Pixel rhs);
		};
	};

	// Behaviors
	Image();
	Image(Header headerObj, vector<Header::Pixel> pixels);
	void ReadHeader(ifstream& inFile, Header& headerObj);
	void ReadTGA(vector<Image::Header::Pixel> &pixels, Image::Header &headerObj, string fileName);
	void WriteTGA(string fileName);
	Image WriteChannel(string fileName, string channel);
	void Flip();
	bool Equal(Image rhs);
	static void Test1();
	static void Test2();
	static void Test3();
	static void Test4();
	static void Test5();
	static void Test6();
	static void Test7();
	static void Test8();
	static void Test9();
	static void Test10();
	static void Test11();

	//Accessors

	Header& GetHeader();
	vector<Header::Pixel>& GetPixels();
	void PrintHeader();

	//Operations
	static Image Multiply(Image image1, Image image2);
	static Image Subtract(Image image1, Image image2);
	static Image Screen(Image image1, Image image2);
	static Image Overlay(Image image1, Image image2);
	void Combine(Image image1, Image image2, Image image3);
	void Add(string channel, int amount);
	void MultiplyPix(int blue, int green, int red);
	void Clamp(float blueChannel, float greenChannel, float redChannel, int i);
	Image& operator=(const Image &rhs);


private:
	Header headerObj = Header();
	vector<Header::Pixel> pixels;
};

