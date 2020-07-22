#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <stdint.h>
#include "lodepng.h"

class Texture {
private:

	struct TextureInfo
	{
		std::vector<uint8_t>* data;
		unsigned width;
		unsigned height;
	};

	static std::map<std::string, TextureInfo> _cache;
	std::vector<uint8_t>* _imageData;
	unsigned _width;
	unsigned _height;

public:

	// Only supports png currently
	Texture(std::string filepath);

	unsigned width(void) const;
	unsigned height(void) const;
	uint8_t* data(void) const;
};


#endif // TEXTURE_H_
