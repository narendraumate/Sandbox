//
//  Image.h
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#include <string>

void load(const std::string& filepath, int& sizeX, int& sizeY, unsigned char*& pixels);
void unload(unsigned char* const m_pixels);
void save(const std::string& filepath, int& sizeX, int& sizeY, unsigned char*& pixels);
