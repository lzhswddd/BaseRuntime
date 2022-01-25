#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "interface.h"
#include "imageio.h"
#include "tools.h"

#define OPENGL_

namespace lzh
{
	using Image = lzh::Array;
	//¶ÁÈ¡Í¼Ïñ
	extern LZHAPI const Array imread(std::string image_path, ColorGamut colorgamut = ColorGamutRGB);
	extern LZHAPI const Array imread(std::string image_path, int32 colorgamut);
	extern LZHAPI uint8* mat2img(IMat src);
	extern LZHAPI OMat img2mat(uint8* img, int width, int height, int channel);
	//±£´æÍ¼Ïñ
	extern LZHAPI void imwrite(std::string image_path, InputArray image, const char* format = nullptr);
	extern LZHAPI void imshow(InputArray image, std::string name = " ");
	extern LZHAPI int waitKey(uint32 delay_ms = 0);
	extern LZHAPI void show();
	extern LZHAPI void destroyAllWindows();
}

template<>
class ImageIODevice<lzh::Image> : public ImageIOSupprt<lzh::Image>
{
public:
	using ImageType = lzh::Image;
	ImageIODevice() :ImageIOSupprt<ImageType>() {}
	ImageIODevice(ImageType* image) :ImageIOSupprt<ImageType>(image) {}
	void GetSize(int& width, int& height) {
		width = image->cols;
		height = image->rows;
	}
	void GetChannel(int& channel) { channel = image->channels(); }
	void GenSize(int width, int height, int channel) { image->create(width, height, channel, lzh::TP_8U); }
	void CopyPart(const ImageType& part, int x, int y, int width, int height) {
		part.setTo(image->range(lzh::Rect(x, y, width, height)));
	}
	void CropPart(ImageType& part, int x, int y, int width, int height) {
		part = image->range(lzh::Rect(x, y, width, height)).clone();
	}
	static bool deserialize(const std::string& path, ImageType& image) {
		image = lzh::imread(path);
		return !image.empty();
	}
	static bool serialize(const std::string& path, const ImageType& image) {
		if (image.empty())return false;
		lzh::imwrite(path, image);
		return lzh::isExists(path);
	}
};
#endif // !__IMAGE_H__