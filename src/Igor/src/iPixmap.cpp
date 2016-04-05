// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iPixmap.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	iPixmap::iPixmap(uint32 width, uint32 height, iColorFormat color_format)
	{
	/*	created = false;

		this->color_format = color_format;

		image_width=width;
		image_height=height;
		switch(color_format)
		{
        case iColorFormat::RGB:
			bytes_per_pixel=3;
			format = IL_RGB;
			break;
		case iColorFormat::RGBA:
			bytes_per_pixel=4;
			format = IL_RGBA;
			break;
		case iColorFormat::BGR:
			bytes_per_pixel=3;
			format = IL_BGR;
			break;
		case iColorFormat::BGRA:
			bytes_per_pixel=4;
			format = IL_BGRA;
			break;
		default:
			bytes_per_pixel=0;
			con_err("used an not supportet color format");
		};

		//#define IL_COLOUR_INDEX						0x1900
		//#define IL_COLOR_INDEX						0x1900
		//#define IL_RGB								0x1907
		//#define IL_RGBA								0x1908
		//#define IL_BGR								0x80E0
		//#define IL_BGRA								0x80E1
		//#define IL_LUMINANCE							0x1909
		//#define IL_LUMINANCE_ALPHA					0x190A

		textureID=0;
		data = 0;

		if(bytes_per_pixel)
		{
			data = new uint8[image_width*image_height*bytes_per_pixel];

			for(uint32 x=0;x<image_width*image_height*bytes_per_pixel;x++)
			{
				data[x]=0;
			}

			created = true;
		}*/
	}

	iPixmap::iPixmap()
	{
		data = 0;
		created = false;
		textureID=0;
		bytes_per_pixel = 0;
	}

	bool iPixmap::exportFile(const iaString filename, bool overwrite)
	{
        con_err("not implemented");

		return true;
	}

	void iPixmap::setData(uint8 *data)
	{
		if(!data) return;
		if(!bytes_per_pixel) return;

		if(!created)
		{
			if(!this->data) con_err("inconsistent data pointer");

			this->data = new uint8 [image_width*image_height*bytes_per_pixel];
			created = true;
		}

		memcpy(this->data, data, image_width*image_height*bytes_per_pixel);
	}

	iPixmap::~iPixmap()
	{
		if(data)
		{
			delete [] data;
			data=0;
		}
	}

	uint8* iPixmap::getData()
	{
		return data;
	}

	uint32 iPixmap::getWidth()
	{
		return image_width;
	}

	uint32 iPixmap::getHeight()
	{
		return image_height;
	}

	uint32 iPixmap::getBytesPerPixel()
	{
		return bytes_per_pixel;
	}

	iColorFormat iPixmap::getColorFormat()
	{
		return color_format;
	}

	uint32 iPixmap::getPixel(uint32 x, uint32 y)
	{
		if(x>=image_width) return 0;
		if(y>=image_height) return 0;   
		if(!data) return 0;
		if(bytes_per_pixel!=4)
		{
			con_err("invalid usage");
			return 0;
		}

		return((uint32*)data)[y*image_width+x];
	}

	bool iPixmap::setPixel(uint32 x, uint32 y, uint32 color)
	{
		if(x>=image_width) return 0;
		if(y>=image_height) return 0;   
		if(!data) return 0;
		if(bytes_per_pixel!=4)
		{
			con_err("invalid usage");
			return false;
		}

		((uint32*)data)[y*image_width+x] = color;

		return true;
	}

	uint8 iPixmap::getPixel(uint32 x, uint32 y, uint8 byte)
	{
		if(x>=image_width) return 0;
		if(y>=image_height) return 0;
		if(byte>=bytes_per_pixel) return 0;

		if(!data) return 0;

		return data[(y*image_width+x)*bytes_per_pixel+byte];
	}

	bool iPixmap::setPixel(uint32 x, uint32 y, uint8 byte, uint8 color)
	{
		if(x>=image_width) return false;
		if(y>=image_height) return false;
		if(byte>=bytes_per_pixel) return false;

		if(!data) return false;

		data[(y*image_width+x)*bytes_per_pixel+byte]=color;
		return true;
	}

	bool iPixmap::setPixelRGB(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b)
	{
		if(x>=image_width) return false;
		if(y>=image_height) return false;
		if(2>=bytes_per_pixel) return false;

		if(!data) return false;

		data[(y*image_width+x)*bytes_per_pixel+0]=r;
		data[(y*image_width+x)*bytes_per_pixel+1]=g;
		data[(y*image_width+x)*bytes_per_pixel+2]=b;
		return true;
	}

	bool iPixmap::setPixelRGBA(uint32 x, uint32 y, uint8 r, uint8 g, uint8 b, uint8 a)
	{
		if(x>=image_width) return false;
		if(y>=image_height) return false;
		if(3>=bytes_per_pixel) return false;

		if(!data) return false;

		data[(y*image_width+x)*bytes_per_pixel+0]=r;
		data[(y*image_width+x)*bytes_per_pixel+1]=g;
		data[(y*image_width+x)*bytes_per_pixel+2]=b;
		data[(y*image_width+x)*bytes_per_pixel+3]=a;
		return true;
	}

};