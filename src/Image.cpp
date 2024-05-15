#include "Image.h"

Image::Header::Header() {
	idLength = 0;
	colorMapType = 0;
	dataTypeCode = 0;
	colorMapOrigin = 0;
	colorMapLength = 0;
	colorMapDepth = 0;
	xOrigin = 0;
	yOrigin = 0;
	width = 0;
	height = 0;
	bitsPerPixel = 0;
	imageDescriptor = 0;
}

// Header copy assignment operator
Image::Header& Image::Header::operator=(const Header rhs) {
	idLength = rhs.idLength;
	colorMapType = rhs.colorMapType;
	dataTypeCode = rhs.dataTypeCode;
	colorMapOrigin = rhs.colorMapOrigin;
	colorMapLength = rhs.colorMapLength;
	colorMapDepth = rhs.colorMapDepth;
	xOrigin = rhs.xOrigin;
	yOrigin = rhs.yOrigin;
	width = rhs.width;
	height = rhs.height;
	bitsPerPixel = rhs.bitsPerPixel;
	imageDescriptor = rhs.imageDescriptor;

	return *this;
}

Image::Image() {
	
}

Image::Image(Header headerObj, vector<Image::Header::Pixel> pixels) {
	this->headerObj = headerObj;
	this->pixels = pixels;
}

// Image Copy Assignment Operator
Image& Image::operator=(const Image& rhs) {
	headerObj = rhs.headerObj;
	pixels = rhs.pixels;

	return *this;
}

void Image::ReadHeader(ifstream& inFile, Header& headerObj) {


	inFile.read(&headerObj.idLength, sizeof(headerObj.idLength));
	inFile.read(&headerObj.colorMapType, sizeof(headerObj.colorMapType));
	inFile.read(&headerObj.dataTypeCode, sizeof(headerObj.dataTypeCode));
	inFile.read((char*)&headerObj.colorMapOrigin, sizeof(headerObj.colorMapOrigin));
	inFile.read((char*)&headerObj.colorMapLength, sizeof(headerObj.colorMapLength));
	inFile.read(&headerObj.colorMapDepth, sizeof(headerObj.colorMapDepth));
	inFile.read((char*)&headerObj.xOrigin, sizeof(headerObj.xOrigin));
	inFile.read((char*)&headerObj.yOrigin, sizeof(headerObj.yOrigin));
	inFile.read((char*)&headerObj.width, sizeof(headerObj.width));
	inFile.read((char*)&headerObj.height, sizeof(headerObj.height));
	inFile.read(&headerObj.bitsPerPixel, sizeof(headerObj.bitsPerPixel));
	inFile.read(&headerObj.imageDescriptor, sizeof(headerObj.imageDescriptor));


}

Image Image::Multiply(Image image1, Image image2) {
	Image image3(image1.headerObj, image1.pixels);
	float bluIMG3 = 0;
	float greenIMG3 = 0;
	float redIMG3 = 0;
	
	for (unsigned int i = 0; i < image2.pixels.size(); i++) {
		bluIMG3 = (image1.pixels.at(i).blue * image2.pixels.at(i).blue) / 255.0;
		greenIMG3 = (image1.pixels.at(i).green * image2.pixels.at(i).green) / 255.0;
		redIMG3 = (image1.pixels.at(i).red * image2.pixels.at(i).red) / 255.0;

		bluIMG3 += 0.5f;
		greenIMG3 += 0.5f;
		redIMG3 += 0.5f;

		image3.pixels.at(i).blue = (int)bluIMG3;
		image3.pixels.at(i).green = (int)greenIMG3;
		image3.pixels.at(i).red = (int)redIMG3;
	}


	return image3;
}

void Image::Add(string channel, int amount) {
	if (channel =="blue") {
		for (unsigned int i = 0; i < this->pixels.size(); i++) {
			float blue = this->pixels.at(i).blue;
			blue += amount;

			this->Clamp(blue, (float)this->pixels.at(i).green, (float)this->pixels.at(i).red, i);

		}

	}
	else if (channel == "green") {
		for (unsigned int i = 0; i < this->pixels.size(); i++) {
			float green = this->pixels.at(i).green;
			green += amount;

			this->Clamp((float)this->pixels.at(i).blue, green, (float)this->pixels.at(i).red, i);

		}
		
	}
	else if (channel == "red") {
		for (unsigned int i = 0; i < this->pixels.size(); i++) {
			float red = this->pixels.at(i).blue;
			red += amount;

			this->Clamp((float)this->pixels.at(i).blue, (float)this->pixels.at(i).green, red, i);

		}
	}
}

void Image::MultiplyPix(int blueVal, int greenVal, int redVal) {
	float bluIMG3 = 0;
	float greenIMG3 = 0;
	float redIMG3 = 0;
	for (unsigned int i = 0; i < this->pixels.size(); i++) {
		bluIMG3 = this->pixels.at(i).blue * blueVal;
		greenIMG3 = this->pixels.at(i).green * greenVal;
		redIMG3 = this->pixels.at(i).red * redVal;

		bluIMG3 += 0.5f;
		greenIMG3 += 0.5f;
		redIMG3 += 0.5f;

		this->Clamp(bluIMG3, greenIMG3, redIMG3, i);
	}

}

// Image 2 - Image 1
Image Image::Subtract(Image image1, Image image2) {
	Image image3(image1.headerObj, image1.pixels);
	float bluIMG3 = 0;
	float greenIMG3 = 0;
	float redIMG3 = 0;
	for (unsigned int i = 0; i < image2.pixels.size(); i++) {

		bluIMG3 = (int)image2.pixels.at(i).blue - (int)image1.pixels.at(i).blue;
		greenIMG3 = (int)image2.pixels.at(i).green - (int)image1.pixels.at(i).green;
		redIMG3 = (int)image2.pixels.at(i).red - (int)image1.pixels.at(i).red;

		image3.Clamp(bluIMG3, greenIMG3, redIMG3, i);
	}
	return image3;
}

void Image::Clamp(float blueChannel, float greenChannel, float redChannel, int i) {
	if (blueChannel > 255) {
		this->pixels.at(i).blue = 255;
	}
	else if (blueChannel < 0) {
		this->pixels.at(i).blue = 0;
	}
	else {
		this->pixels.at(i).blue = blueChannel;
	}
	if (greenChannel > 255) {
		this->pixels.at(i).green = 255;
	}
	else if (greenChannel < 0) {
		this->pixels.at(i).green = 0;
	}
	else {
		this->pixels.at(i).green = greenChannel;
	}
	if (redChannel > 255) {
		this->pixels.at(i).red = 255;
	}
	else if (redChannel < 0) {
		this->pixels.at(i).red = 0;
	}
	else {
		this->pixels.at(i).red = redChannel;
	}
}

Image Image::Screen(Image image1, Image image2) {
	Image image3(image1.headerObj, image1.pixels);
	float bluIMG3 = 0;
	float greenIMG3 = 0;
	float redIMG3 = 0;

	for (unsigned int i = 0; i < image2.pixels.size(); i++) {

		bluIMG3 = (1 - ((1 - (image1.pixels.at(i).blue / 255.0f)) * (1 - (image2.pixels.at(i).blue) / 255.0f))) * 255.0f;
		greenIMG3 = (1 - ((1 - (image1.pixels.at(i).green / 255.0f)) * (1 - (image2.pixels.at(i).green) / 255.0f))) * 255.0f;
		redIMG3 = (1 - ((1 - (image1.pixels.at(i).red / 255.0f)) * (1 - (image2.pixels.at(i).red) / 255.0f))) * 255.0f;

		bluIMG3 += 0.5f;
		greenIMG3 += 0.5f;
		redIMG3 += 0.5f;

		image3.Clamp(bluIMG3, greenIMG3, redIMG3, i);
	}
	return image3;
}

// Image 2 is background
Image Image::Overlay(Image image1, Image image2) {
	Image image3(image1.headerObj, image1.pixels);
	float bluIMG3 = 0;
	float greenIMG3 = 0;
	float redIMG3 = 0;
	
	for (unsigned int i = 0; i < image2.pixels.size(); i++) {

		// Greater than because 0 is black (127 is the value of gray)
		if ((double)image2.pixels.at(i).blue < 127.5 && (double)image2.pixels.at(i).green < 127.5 && (double)image2.pixels.at(i).red < 127.5) {
			bluIMG3 = (2 * (int)image1.pixels.at(i).blue * (int)image2.pixels.at(i).blue) / 255.0;
			greenIMG3 = (2 * (int)image1.pixels.at(i).green * (int)image2.pixels.at(i).green) / 255.0;
			redIMG3 =  (2 * (int)image1.pixels.at(i).red * (int)image2.pixels.at(i).red) / 255.0;
			
			bluIMG3 += 0.5f;
			greenIMG3 += 0.5f;
			redIMG3 += 0.5f;
		}
		else {
			bluIMG3 = (1-(2 * (1 - image1.pixels.at(i).blue / 255.0f) * (1 - image2.pixels.at(i).blue / 255.0f))) * 255.0f;
			greenIMG3 = (1 - (2 * (1 - image1.pixels.at(i).green / 255.0f) * (1 - image2.pixels.at(i).green / 255.0f))) * 255.0f;
			redIMG3 = (1 - (2 * (1 - image1.pixels.at(i).red / 255.0f) * (1 - image2.pixels.at(i).red / 255.0f))) * 255.0f;

			bluIMG3 += 0.5f;
			greenIMG3 += 0.5f;
			redIMG3 += 0.5f;
		}
		

		image3.Clamp(bluIMG3, greenIMG3, redIMG3, i);
	}
	return image3;
}

void Image::Combine(Image bluIMG, Image greenIMG, Image redIMG) {
	unsigned char blue = 0;
	unsigned char green = 0;
	unsigned char red = 0;
	for (unsigned int i = 0; i < bluIMG.pixels.size(); i++) {

		blue = bluIMG.pixels.at(i).blue;
		green = greenIMG.pixels.at(i).green;
		red = redIMG.pixels.at(i).red;

		pixels.push_back(Image::Header::Pixel(blue, green, red));
	}

}

// Accessors
Image::Header& Image::GetHeader() {
	return headerObj;
}

vector<Image::Header::Pixel>& Image::GetPixels() {
	return pixels;
}

//Behaviors
void Image::ReadTGA(vector<Image::Header::Pixel>& pixels, Image::Header& headerObj, string fileName) {

	ifstream inFile(fileName, ios_base::binary);

	inFile.read((char*)&headerObj.idLength, sizeof(headerObj.idLength));
	inFile.read((char*)&headerObj.colorMapType, sizeof(headerObj.colorMapType));
	inFile.read((char*)&headerObj.dataTypeCode, sizeof(headerObj.dataTypeCode));
	inFile.read((char*)&headerObj.colorMapOrigin, sizeof(headerObj.colorMapOrigin));
	inFile.read((char*)&headerObj.colorMapLength, sizeof(headerObj.colorMapLength));
	inFile.read((char*)&headerObj.colorMapDepth, sizeof(headerObj.colorMapDepth));
	inFile.read((char*)&headerObj.xOrigin, sizeof(headerObj.xOrigin));
	inFile.read((char*)&headerObj.yOrigin, sizeof(headerObj.yOrigin));
	inFile.read((char*)&headerObj.width, sizeof(headerObj.width));
	inFile.read((char*)&headerObj.height, sizeof(headerObj.height));
	inFile.read((char*)&headerObj.bitsPerPixel, sizeof(headerObj.bitsPerPixel));
	inFile.read((char*)&headerObj.imageDescriptor, sizeof(headerObj.imageDescriptor));


	for (unsigned int i = 0; i < headerObj.width * headerObj.height; i++) {
		Header::Pixel pixel;
		inFile.read((char*)&pixel.blue, sizeof(pixel.blue));
		inFile.read((char*)&pixel.green, sizeof(pixel.green));
		inFile.read((char*)&pixel.red, sizeof(pixel.red));
		pixels.push_back(pixel);
	}
	inFile.close();

}

void Image::WriteTGA(string fileName) {
	ofstream outFile(fileName, ios_base::binary);

	outFile.write((char*)&headerObj.idLength, sizeof(headerObj.idLength));
	outFile.write((char*)&headerObj.colorMapType, sizeof(headerObj.colorMapType));
	outFile.write((char*)&headerObj.dataTypeCode, sizeof(headerObj.dataTypeCode));
	outFile.write((char*)&headerObj.colorMapOrigin, sizeof(headerObj.colorMapOrigin));
	outFile.write((char*)&headerObj.colorMapLength, sizeof(headerObj.colorMapLength));
	outFile.write((char*)&headerObj.colorMapDepth, sizeof(headerObj.colorMapDepth));
	outFile.write((char*)&headerObj.xOrigin, sizeof(headerObj.xOrigin));
	outFile.write((char*)&headerObj.yOrigin, sizeof(headerObj.yOrigin));
	outFile.write((char*)&headerObj.width, sizeof(headerObj.width));
	outFile.write((char*)&headerObj.height, sizeof(headerObj.height));
	outFile.write((char*)&headerObj.bitsPerPixel, sizeof(headerObj.bitsPerPixel));
	outFile.write((char*)&headerObj.imageDescriptor, sizeof(headerObj.imageDescriptor));

	for (unsigned int i = 0; i < pixels.size(); i++) {
		outFile.write((char*)&pixels.at(i).blue, sizeof(pixels.at(i).blue));
		outFile.write((char*)&pixels.at(i).green, sizeof(pixels.at(i).green));
		outFile.write((char*)&pixels.at(i).red, sizeof(pixels.at(i).red));
	}
	outFile.close();
}

Image Image::WriteChannel(string fileName, string channel) {
	ofstream outFile(fileName, ios_base::binary);
	Image image2(headerObj, pixels);

	image2.pixels.clear();

	unsigned char bluIMG = 0;
	unsigned char greenIMG = 0;
	unsigned char redIMG = 0; 

	outFile.write((char*)&headerObj.idLength, sizeof(headerObj.idLength));
	outFile.write((char*)&headerObj.colorMapType, sizeof(headerObj.colorMapType));
	outFile.write((char*)&headerObj.dataTypeCode, sizeof(headerObj.dataTypeCode));
	outFile.write((char*)&headerObj.colorMapOrigin, sizeof(headerObj.colorMapOrigin));
	outFile.write((char*)&headerObj.colorMapLength, sizeof(headerObj.colorMapLength));
	outFile.write((char*)&headerObj.colorMapDepth, sizeof(headerObj.colorMapDepth));
	outFile.write((char*)&headerObj.xOrigin, sizeof(headerObj.xOrigin));
	outFile.write((char*)&headerObj.yOrigin, sizeof(headerObj.yOrigin));
	outFile.write((char*)&headerObj.width, sizeof(headerObj.width));
	outFile.write((char*)&headerObj.height, sizeof(headerObj.height));
	outFile.write((char*)&headerObj.bitsPerPixel, sizeof(headerObj.bitsPerPixel));
	outFile.write((char*)&headerObj.imageDescriptor, sizeof(headerObj.imageDescriptor));

	if (channel == "blue") {
		for (unsigned int i = 0; i < pixels.size(); i++) {
			greenIMG = this->pixels.at(i).blue;
			redIMG = this->pixels.at(i).blue;

			outFile.write((char*)&pixels.at(i).blue, sizeof(pixels.at(i).blue));
			outFile.write((char*)&greenIMG, sizeof(greenIMG));
			outFile.write((char*)&redIMG, sizeof(redIMG));

			image2.pixels.push_back(Image::Header::Pixel(pixels.at(i).blue, greenIMG, redIMG));
		}
	}
	else if (channel == "green") {
		for (unsigned int i = 0; i < pixels.size(); i++) {
			bluIMG = this->pixels.at(i).green;
			redIMG = this->pixels.at(i).green;

			outFile.write((char*)&bluIMG, sizeof(bluIMG));
			outFile.write((char*)&pixels.at(i).green, sizeof(pixels.at(i).green));
			outFile.write((char*)&redIMG, sizeof(redIMG));

			image2.pixels.push_back(Image::Header::Pixel(bluIMG, pixels.at(i).green, redIMG));
		}
	}
	else if (channel == "red") {
		for (unsigned int i = 0; i < pixels.size(); i++) {
			bluIMG = this->pixels.at(i).red;
			greenIMG = this->pixels.at(i).red;

			outFile.write((char*)&bluIMG, sizeof(bluIMG));
			outFile.write((char*)&greenIMG, sizeof(greenIMG));
			outFile.write((char*)&pixels.at(i).red, sizeof(pixels.at(i).red));

			image2.pixels.push_back(Image::Header::Pixel(bluIMG, greenIMG, pixels.at(i).red));
		}
	}
	
	outFile.close();

	return image2;
}

void Image::PrintHeader() {
	cout << (int)headerObj.idLength << endl;
	cout << (int)headerObj.colorMapType << endl;
	cout << (int)headerObj.dataTypeCode << endl;
	cout << headerObj.colorMapOrigin << endl;
	cout << headerObj.colorMapLength << endl;
	cout << (int)headerObj.colorMapDepth << endl;
	cout << headerObj.xOrigin << endl;
	cout << headerObj.yOrigin << endl;
	cout << headerObj.width << endl;
	cout << headerObj.height << endl;
	cout << (int)headerObj.bitsPerPixel << endl;
	cout << (int)headerObj.imageDescriptor << endl;
}

bool Image::Equal(Image rhs) {
	bool equality = false;

	for (unsigned int i = 0; i < this->pixels.size(); i++) {
		if ((int)this->pixels.at(i).blue == (int)rhs.pixels.at(i).blue) {
			if ((int)this->pixels.at(i).green == (int)rhs.pixels.at(i).green) {
				if ((int)this->pixels.at(i).red == (int)rhs.pixels.at(i).red) {
					equality = true;
				}
				else {
					equality = false;
					break;
				}
			}
			else {
				equality = false;
				break;
			}
		}
		else {
			equality = false;
			break;
		}
	}

	return equality;
}

void Image::Flip() {
	vector<Image::Header::Pixel> pixels2 = pixels;

	for (unsigned int i = 0; i < pixels.size(); i++) {
		pixels.at(i) = pixels2.at(pixels2.size() - i - 1);
	}
}

void Image::Test1() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	vector<Image::Header::Pixel> pixels3;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);
	Image image3(headerObj, pixels3);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/layer1.tga");
	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "input/pattern1.tga");

	image3 = Image::Multiply(image1, image2);

	image1.GetPixels().clear();

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "examples/EXAMPLE_part1.tga");

	image3.WriteTGA("output/part1.tga");
	
	if (image3.Equal(image1)) {
		cout << "Test #1: Passed!" << endl;
	}
	else {
		cout << "Test #1: Failed!" << endl;
	}
}

void Image::Test2() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	vector<Image::Header::Pixel> pixels3;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);
	Image image3(headerObj, pixels3);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/layer2.tga");
	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "input/car.tga");

	image3 = Image::Subtract(image1, image2);

	image1.GetPixels().clear();

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "examples/EXAMPLE_part2.tga");
	
	image3.WriteTGA("output/part2.tga");

	if (image3.Equal(image1)) {
		cout << "Test #2: Passed!" << endl;
	}
	else {
		cout << "Test #2: Failed!" << endl;
	}
}

void Image::Test3() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	vector<Image::Header::Pixel> pixels3;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);
	Image image3(headerObj, pixels3);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/layer1.tga");
	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "input/pattern2.tga");
	



	image3 = Image::Multiply(image1, image2);

	image1.GetPixels().clear();
	image2.GetPixels().clear();

	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "input/text.tga");

	image1 = Image::Screen(image3, image2);

	image3.GetPixels().clear();

	image3.ReadTGA(image3.GetPixels(), image3.GetHeader(), "examples/EXAMPLE_part3.tga");


	if (image1.Equal(image3)) {
		cout << "Test #3: Passed!" << endl;
	}
	else {
		cout << "Test #3: Failed!" << endl;
	}
	image1.WriteTGA("output/part3.tga");
}

void Image::Test4() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	vector<Image::Header::Pixel> pixels3;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);
	Image image3(headerObj, pixels3);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/layer2.tga");
	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "input/circles.tga");
	

	image3 = Image::Multiply(image1, image2);

	image1.GetPixels().clear();
	image2.GetPixels().clear();
	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "input/pattern2.tga");

	image1 = Image::Subtract(image2, image3);

	image3.GetPixels().clear();
	image3.ReadTGA(image3.GetPixels(), image3.GetHeader(), "examples/EXAMPLE_part4.tga");

	if (image1.Equal(image3)) {
		cout << "Test #4: Passed!" << endl;
	}
	else {
		cout << "Test #4: Failed!" << endl;
	}

	image1.WriteTGA("output/part4.tga");
}

void Image::Test5() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	vector<Image::Header::Pixel> pixels3;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);
	Image image3(headerObj, pixels3);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/layer1.tga");
	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "input/pattern1.tga");

	image3 = Image::Overlay(image1, image2);

	image1.GetPixels().clear();
	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "examples/EXAMPLE_part5.tga");

	if (image1.Equal(image3)) {
		cout << "Test #5: Passed!" << endl;
	}
	else {
		cout << "Test #5: Failed!" << endl;
	}

	image3.WriteTGA("output/part5.tga");
}

void Image::Test6() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);
	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/car.tga");

	image1.Add("green", 200);

	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "examples/EXAMPLE_part6.tga");

	if (image1.Equal(image2)) {
		cout << "Test #6: Passed!" << endl;
	}
	else {
		cout << "Test #6: Failed!" << endl;
	}

	image1.WriteTGA("output/part6.tga");
}

void Image::Test7() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/car.tga");

	image1.MultiplyPix(0, 1, 4);

	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "examples/EXAMPLE_part7.tga");

	if (image1.Equal(image2)) {
		cout << "Test #7: Passed!" << endl;
	}
	else {
		cout << "Test #7: Failed!" << endl;
	}

	image1.WriteTGA("output/part7.tga");
}

void Image::Test8() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	vector<Image::Header::Pixel> pixels3;
	vector<Image::Header::Pixel> pixels4;
	vector<Image::Header::Pixel> pixels5;
	vector<Image::Header::Pixel> pixels6;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);
	Image image3(headerObj, pixels3);
	Image image4(headerObj, pixels4);
	Image image5(headerObj, pixels5);
	Image image6(headerObj, pixels6);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/car.tga");

	image2 = image1.WriteChannel("output/part8_b.tga", "blue");
	image3 = image1.WriteChannel("output/part8_g.tga", "green");
	image4 = image1.WriteChannel("output/part8_r.tga", "red");

	image1.pixels.clear();

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "examples/EXAMPLE_part8_b.tga");
	image5.ReadTGA(image5.GetPixels(), image5.GetHeader(), "examples/EXAMPLE_part8_g.tga");
	image6.ReadTGA(image6.GetPixels(), image6.GetHeader(), "examples/EXAMPLE_part8_r.tga");

	if (image2.Equal(image1) && image3.Equal(image5) && image4.Equal(image6)) {
		cout << "Test #8: Passed!" << endl;
	}
	else {
		cout << "Test #8: Failed!" << endl;
	}
}

void Image::Test9() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	vector<Image::Header::Pixel> pixels3;
	vector<Image::Header::Pixel> pixels4;
	vector<Image::Header::Pixel> pixels5;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);
	Image image3(headerObj, pixels3);
	Image image4(headerObj, pixels4);
	Image image5(headerObj, pixels5);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/layer_blue.tga");
	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "input/layer_green.tga");
	image3.ReadTGA(image3.GetPixels(), image3.GetHeader(), "input/layer_red.tga");
	
	image4.ReadTGA(image4.GetPixels(), image4.GetHeader(), "examples/EXAMPLE_part9.tga");

	image5.Combine(image1, image2, image3);

	if (image4.Equal(image5)) {
		cout << "Test #9: Passed!" << endl;
	}
	else {
		cout << "Test #9: Failed!" << endl;
	}

	image5.WriteTGA("output/part9.tga");
}

void Image::Test10() {
	Header headerObj;
	vector<Image::Header::Pixel> pixels1;
	vector<Image::Header::Pixel> pixels2;
	Image image1(headerObj, pixels1);
	Image image2(headerObj, pixels2);

	image1.ReadTGA(image1.GetPixels(), image1.GetHeader(), "input/text2.tga");
	image2.ReadTGA(image2.GetPixels(), image2.GetHeader(), "examples/EXAMPLE_part10.tga");

	image1.Flip();

	if (image1.Equal(image2)) {
		cout << "Test #10: Passed!" << endl;
	}
	else {
		cout << "Test #10: Failed!" << endl;
	}

	image1.WriteTGA("output/part10.tga");
}