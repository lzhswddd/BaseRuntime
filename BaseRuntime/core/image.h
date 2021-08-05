#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "interface.h"

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
#endif // !__IMAGE_H__