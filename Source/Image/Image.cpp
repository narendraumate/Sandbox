//
//  Image.cpp
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"

void load(const std::string& filepath, int& sizeX, int& sizeY, unsigned char*& pixels) {
	int channels;
	int count;

	int forceChannels = 4;
	pixels = stbi_load(filepath.c_str(), &sizeX, &sizeY, &channels, forceChannels);
	if (channels != forceChannels) {
		channels = forceChannels;
	}
	count = sizeY * sizeX * channels;

	// Flip Y for OpenGL. Not sure if other API need this.
	unsigned char* newPixels = new unsigned char[count];
	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX * channels; ++x) {
			newPixels[sizeX * (sizeY - 1 - y) * channels + x] = pixels[sizeX * y * channels + x];
		}
	}
	delete pixels;
	pixels = newPixels;
}

void unload(unsigned char* const m_pixels) {
	stbi_image_free(m_pixels);
}

void save(const std::string& filepath, int& sizeX, int& sizeY, unsigned char*& pixels) {
	int channels = 4;
#if 1
	// Flip Y for OpenGL. Not sure if other API need this.
	unsigned char* newPixels = new unsigned char[sizeX * sizeY * channels];
	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX * 4; ++x) {
			newPixels[sizeX * (sizeY - 1 - y) * channels + x] = pixels[sizeX * y * channels + x];
		}
	}
	stbi_write_bmp(filepath.c_str(), sizeX, sizeY, channels, newPixels);
	delete[] newPixels;
#else
	stbi_write_tga(filepath.c_str(), sizeX, sizeY, channels, pixels);
#endif
}
