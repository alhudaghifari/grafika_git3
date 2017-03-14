#include "credit.h"
#include <cstdio>

Credit::Credit(char* filename) {
	
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    width = *(int*)&info[18];
    height = *(int*)&info[22];

    data = new int(3*width*height);

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }

}

Credit::~Credit() {
	delete data;
}

int* Credit::getData() {
	return data;
}

int Credit::getHeight() {
	return height;
}

int Credit::getWidth() {
	return width;
}
