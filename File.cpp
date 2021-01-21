#include "File.h"
#include <fstream>
using namespace std;

File::File(const char* filename) : mSize(0),mData(0)
{
	ifstream in(filename, ifstream::binary);

	in.seekg(0, ifstream::end);
	mSize = static_cast<int>(in.tellg());
	in.seekg(0, ifstream::beg);
	mData = new char[mSize];
	in.read(mData, mSize);

}

File::~File()
{
	delete[] mData;
	mData = 0;
}

int File::size() const
{
	return mSize;
}

const char* File::data() const
{
	return mData;
}

unsigned File::getUnsigned(int p) const
{
	const unsigned char* up;
	// char -> unsigend data
	up = reinterpret_cast<const unsigned char*>(mData);

	// endian convert big -> little
	unsigned r = up[p];
	r |= up[p + 1] << 8;
	r |= up[p + 2] << 16;
	r |= up[p + 3] << 24;

	return r;
}