#define LZH_EXPORTS
#include "color.h"
#include "tools.h"
#include "operator.h"
#include <strstream>

using namespace lzh;
using namespace std;

int32 Color::tabLength = int32(YellowGreen) + 1;


const mat_t Color::r_gray = _T(0.299);
const mat_t Color::g_gray = _T(0.587);
const mat_t Color::b_gray = _T(0.114);
const mat_t Color::xyz2rgb[9] =
{
	3.2404548, -1.5371389, -0.49853155,
	-0.96926639, 1.8760109, 0.041556082,
	0.05564342, -0.20402585, 1.0572252
};
const mat_t Color::rgb2xyz[9] =
{
	0.4124564, 0.3575761, 0.1804375,
	0.2126729, 0.7151522, 0.0721750,
	0.0193339, 0.1191920, 0.9503041
};
//const mat_t Color::rgb2xyz[9] =
//{
//	0.436052025, 0.385081593, 0.143087414,
//	0.222491598, 0.716886060, 0.060621486,
//	0.013929122, 0.097097002, 0.714185470
//};
const mat_t Color::Reference_X = (rgb2xyz[0] + rgb2xyz[1] + rgb2xyz[2]) * 100;
const mat_t Color::Reference_Y = (rgb2xyz[3] + rgb2xyz[4] + rgb2xyz[5]) * 100;
const mat_t Color::Reference_Z = (rgb2xyz[6] + rgb2xyz[7] + rgb2xyz[8]) * 100;

Color lzh::Color::anti() const noexcept
{
	return AntiColor(*this);
}

std::string lzh::Color::css() const noexcept
{
	std::string code = "#";
	for (int i = 0; i < 3; i++)
	{
		uint8 v = saturate_cast<uint8>(data[i]);
		uint8 val = v / 16;
		if (val < 10) {
			code += ('0' + val);
		}
		else {
			code += ('A' + (val - 10));
		}
		val = v % 16;
		if (val < 10) {
			code += ('0' + val);
		}
		else {
			code += ('A' + (val - 10));
		}

	}
	return code;
}

int32 lzh::Color::rgb() const noexcept
{
    return int32(red()<<16|green()<<8|blue());
}

void lzh::Color::setRgb(int32 code) noexcept
{
    int8 r = (code >> 16) & 0xFF;
    int8 g = (code >> 8) & 0xFF;
    int8 b = code & 0xFF;
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = 0xFF;
}

int32 lzh::Color::rgba() const noexcept
{
    return int32(red()<<16|green()<<8|blue()|alpha()<<24);
}

void lzh::Color::setRgba(int32 code) noexcept
{
    int8 r = (code >> 16) & 0xFF;
    int8 g = (code >> 8) & 0xFF;
    int8 b = code & 0xFF;
    int8 a = (code >> 24) & 0xFF;
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
}

uint8 lzh::Color::red() const
{
	return saturate_cast<uint8>(data[0]);
}

uint8 lzh::Color::green() const
{
	return saturate_cast<uint8>(data[1]);
}

uint8 lzh::Color::blue() const
{
	return saturate_cast<uint8>(data[2]);
}

void lzh::Color::setTab(ColorTab colorTab)
{
	*this = Tab(colorTab);
}

void lzh::Color::setCSS(std::string code)
{
	*this = CSS(code);
}

uint8 lzh::Color::alpha() const
{
	return saturate_cast<uint8>(data[3]);
}

void lzh::Color::setRed(uint8 r)
{
	data[0] = r;
}

void lzh::Color::setGreen(uint8 g)
{
	data[1] = g;
}

void lzh::Color::setBlue(uint8 b)
{
	data[2] = b;
}

void lzh::Color::setAlpha(uint8 a)
{
	data[3] = a;
}

mat_t& lzh::Color::r()noexcept
{
	return data[0];
}

mat_t& lzh::Color::g()noexcept
{
	return data[1];
}

mat_t& lzh::Color::b()noexcept
{
	return data[2];
}

mat_t& lzh::Color::a()noexcept
{
	return data[3];
}

const mat_t & lzh::Color::r() const noexcept
{
	return data[0];
}

const mat_t & lzh::Color::g() const noexcept
{
	return data[1];
}

const mat_t & lzh::Color::b() const noexcept
{
	return data[2];
}

const mat_t & lzh::Color::a() const noexcept
{
	return data[3];
}

void lzh::Color::setR(mat_t r)noexcept
{
	data[0] = r;
}

void lzh::Color::setG(mat_t g)noexcept
{
	data[1] = g;
}

void lzh::Color::setB(mat_t b)noexcept
{
	data[2] = b;
}

void lzh::Color::setA(mat_t a)noexcept
{
	data[3] = a;
}

Color lzh::Color::Alpha(mat_t a) const noexcept
{
	Color color = *this;
	color[3] = a;
	return color;
}

Color lzh::Color::Uint8() const noexcept
{
	switch (spec)
	{
	case lzh::Color::Cmyk:
	case lzh::Color::Hsv:
	case lzh::Color::Hsl:
		return Color(data[0] * 255, data[1] * 255, data[2] * 255, data[3], spec);
	case lzh::Color::Xyz:
		return Color(data[0] / Reference_X * 255, data[1] / Reference_Y * 255, data[2] / Reference_Z * 255, data[3], spec);
	case lzh::Color::Yxy:
		return Color(data[0] / Reference_Y * 255, data[1] * 255, data[2] * 255, data[3], spec);
	case lzh::Color::Lab:
		return Color(data[0] * 2.55, data[1] + 128, data[2] + 128, data[3], spec);
	case lzh::Color::Luv:
		//return Color(data[0] * 2.55, (data[1] + 134) / 354 * 255, (data[2] + 140) / 262 * 255, data[3], spec);
		return Color(data[0] * 2.55, (data[1] + 100) / 200 * 255, (data[2] + 100) / 200 * 255, data[3], spec);
	case lzh::Color::Lch_ab:
	case lzh::Color::Lch_uv:
		return Color(data[0] * 2.55, data[1] / (128 * sqrt(2)) * 255, data[2] / 360 * 255, data[3], spec);
	case lzh::Color::Rgb:
	case lzh::Color::Bgr:
	default:return *this;
	}
}

Color lzh::Color::inv() const noexcept
{
	switch (spec)
	{
	case lzh::Color::Cmyk:
	case lzh::Color::Hsv:
	case lzh::Color::Hsl:
		return Color(data[0] / 255, data[1] / 255, data[2] / 255, data[3], spec);
	case lzh::Color::Xyz:
		return Color(data[0] / 255 * Reference_X, data[1] / 255 * Reference_Y, data[2] / 255 * Reference_Z, data[3], spec);
	case lzh::Color::Yxy:
		return Color(data[0] / 255 * Reference_Y, data[1] / 255, data[2] / 255, data[3], spec);
	case lzh::Color::Lab:
		return Color(data[0] / 2.55, data[1] - 128, data[2] - 128, data[3], spec);
	case lzh::Color::Luv:
		return Color(data[0] / 2.55, (data[1] / 255 * 200) - 100, (data[2] / 255 * 200) - 100, data[3], spec);
	case lzh::Color::Lch_ab:
	case lzh::Color::Lch_uv:
		return Color(data[0] / 2.55, data[1] / 255 * (128 * sqrt(2)), data[2] / 255 * 360, data[3], spec);
	case lzh::Color::Rgb:
	case lzh::Color::Bgr:
	default:return *this;
	}
}

Color lzh::Color::createSpec(Spec spec) const noexcept
{
	Color color = *this;
	color.spec = spec;
	return color;
}

Color lzh::Color::toSpec(Spec spec) const noexcept
{
	switch (spec)
	{
	case lzh::Color::Rgb:return toRgb();
	case lzh::Color::Bgr:return toBgr();
	case lzh::Color::Cmyk:return toCmyk();
	case lzh::Color::Hsv:return toHsv();
	case lzh::Color::Hsl:return toHsl();
	case lzh::Color::Xyz:return toXyz();
	case lzh::Color::Yxy:return toYxy();
	case lzh::Color::Lab:return toLab();
	case lzh::Color::Luv:return toLuv();
	case lzh::Color::Lch_ab:return toLch_ab();
	case lzh::Color::Lch_uv:return toLch_uv();
	default:return Color();
	}
}

Color lzh::Color::reverse(bool include_alpha) const noexcept
{
	Color color = *this;
	std::reverse(color.begin(), (include_alpha ? color.end() - 1 : color.end()));
	return color;
}

Color lzh::Color::toGray() const noexcept
{
	return Convert(*this, Rgb, Rgb2Gray);
}

Color lzh::Color::toRgb() const noexcept
{
	return Convert(*this, Rgb);
}

Color lzh::Color::toBgr() const noexcept
{
	return Convert(*this, Bgr, Rgb2Bgr);
}

Color lzh::Color::toCmyk() const noexcept
{
	return Convert(*this, Cmyk, Rgb2Cmyk);
}

Color lzh::Color::toHsv() const noexcept
{
	return Convert(*this, Hsv, Rgb2Hsv);
}

Color lzh::Color::toHsl() const noexcept
{
	return Convert(*this, Hsl, Rgb2Hsl);
}

Color lzh::Color::toXyz() const noexcept
{
	return Convert(*this, Xyz, Rgb2Xyz);
}

Color lzh::Color::toYxy() const noexcept
{
	return Convert(*this, Yxy, Rgb2Yxy);
}

Color lzh::Color::toLab() const noexcept
{
	return Convert(*this, Lab, Rgb2Lab);
}

Color lzh::Color::toLuv() const noexcept
{
	return Convert(*this, Luv, Rgb2Luv);
}

Color lzh::Color::toLch_ab() const noexcept
{
	return Convert(*this, Lch_ab, Rgb2Lch_ab);
}

Color lzh::Color::toLch_uv() const noexcept
{
	return Convert(*this, Lch_uv, Rgb2Lch_uv);
}

void lzh::Color::setSpec(Spec spec)
{
	this->spec = spec;
}

lzh::Color::Spec lzh::Color::getSpec() const noexcept
{
	return spec;
}

void lzh::Color::setSpec(ColorGamut gamut)
{
	switch (gamut)
	{
	case lzh::ColorGamutCustom:
	case lzh::ColorGamutGray:
	case lzh::ColorGamutRGB:
	case lzh::ColorGamutRGBA:
	case lzh::ColorGamutMean:
	case lzh::ColorGamutMaxMin:
	case lzh::ColorGamutR:
	case lzh::ColorGamutB:
	case lzh::ColorGamutG:spec = Rgb; break;
	case lzh::ColorGamutBGR:
	case lzh::ColorGamutBGRA:spec = Bgr; break;
	case lzh::ColorGamutHSV:spec = Hsv; break;
	case lzh::ColorGamutHSL:spec = Hsl; break;
	case lzh::ColorGamutXYZ:spec = Xyz; break;
	case lzh::ColorGamutLab:spec = Lab; break;
	default:
		break;
	}
}

void lzh::Color::setVec3(const Vec3m & vec)
{
	memcpy(data, vec.data, sizeof(mat_t) * 3);
}

Vec3m lzh::Color::toVec3() const noexcept
{
	return Vec3m(data[0], data[1], data[2]);
}

void lzh::Color::setVec4(const Vec3m & vec)
{
	memcpy(data, vec.data, sizeof(mat_t) * 4);
}

Vec4m lzh::Color::toVec4() const noexcept
{
	return Vec4m(data[0], data[1], data[2], data[3]);
}

bool lzh::Color::operator!=(ColorTab idx) const
{
	return *this != Color(idx);
}

bool lzh::Color::operator==(ColorTab idx) const
{
	return *this == Color(idx);
}

bool lzh::Color::operator!=(const Color & color) const
{
	return !(*this == color);
}

bool lzh::Color::operator==(const Color & color) const
{
	if (color.data == data)return true;
	return memcmp(data, color.data, sizeof(mat_t)*color.size()) == 0;
}

Color lzh::Color::Random(const std::vector<Color>& except)
{
	if (except.empty()) return Color(rand() % 256, rand() % 256, rand() % 256);
	Color color;
	do {
		color = Color(rand() % 256, rand() % 256, rand() % 256);
	} while (find(except.begin(), except.end(), color) != except.end());
	return color;
}
Color lzh::Color::Tab(ColorTab index)
{
	switch (index)
	{
	case lzh::Color::Amber:return Color(202, 76, 38);
		//case lzh::Color::AntiqueViolet:return Color(?, ?, ?);
	case lzh::Color::AntiqueWhite:return Color(250, 235, 215);
	case lzh::Color::Aqua:return Color(0, 255, 255);
	case lzh::Color::AquaMarine:return Color(127, 255, 170);
	case lzh::Color::AquaMarineBlue:return Color(202, 76, 38);
	case lzh::Color::Azure:return Color(240, 255, 255);
	case lzh::Color::AliceBlue:return Color(240, 248, 255);
	case lzh::Color::BabyPink:return Color(252, 229, 223);
	case lzh::Color::Beige:return Color(107, 142, 35);
	case lzh::Color::Bisque:return Color(255, 228, 196);
	case lzh::Color::Black:return Color(0, 0, 0);
	case lzh::Color::BlancheDalmond:return Color(255, 235, 205);
	case lzh::Color::Blue:return Color(0, 0, 255);
	case lzh::Color::BlueViolet:return Color(138, 43, 226);
	case lzh::Color::Brown:return Color(165, 42, 42);
	case lzh::Color::BurlyWood:return Color(222, 184, 135);
	case lzh::Color::CobaltBlue:return Color(0, 71, 171);
	case lzh::Color::CadetBlue:return Color(95, 158, 160);
	case lzh::Color::Camel:return Color(181, 134, 84);
	case lzh::Color::CharcoalGray:return Color(64, 64, 64);
	case lzh::Color::Chartreuse:return Color(127, 255, 0);
	case lzh::Color::Chocolate:return Color(210, 105, 30);
	case lzh::Color::CobaltVlue:return Color(3, 128, 183);
	case lzh::Color::Coral:return Color(255, 127, 80);
	case lzh::Color::CornflowerBlue:return Color(100, 149, 237);
	case lzh::Color::Cornsilk:return Color(255, 248, 220);
	case lzh::Color::Crimson:return Color(220, 20, 60);
	case lzh::Color::Cyan:return Color(0, 255, 255);
	case lzh::Color::DarkBlue:return Color(0, 0, 139);
	case lzh::Color::DarkCyan:return Color(0, 139, 139);
	case lzh::Color::DarkGoldenrod:return Color(184, 134, 11);
	case lzh::Color::DarkGray:return Color(169, 169, 169);
	case lzh::Color::DarkGreen:return Color(0, 100, 0);
	case lzh::Color::DarkKhaki:return Color(189, 183, 107);
	case lzh::Color::DarkMagenta:return Color(139, 0, 139);
	case lzh::Color::DarkOliveGreen:return Color(85, 107, 47);
	case lzh::Color::DarkOrange:return Color(255, 140, 0);
	case lzh::Color::DarkOrchid:return Color(153, 50, 204);
	case lzh::Color::DarkRed:return Color(139, 0, 0);
	case lzh::Color::DarkSalmon:return Color(233, 150, 122);
	case lzh::Color::DarkSeaGreen:return Color(72, 61, 139);
	case lzh::Color::DarkSlateBlue:return Color(72, 61, 139);
	case lzh::Color::DarkSlateGray:return Color(47, 79, 79);
	case lzh::Color::DarkTurquoise:return Color(0, 206, 209);
	case lzh::Color::DarkViolet:return Color(148, 0, 211);
	case lzh::Color::DeepPink:return Color(255, 20, 147);
	case lzh::Color::DeepSkyBlue:return Color(0, 191, 255);
	case lzh::Color::DimGray:return Color(105, 105, 105);
	case lzh::Color::DoderBlue:return Color(30, 144, 255);
	case lzh::Color::DodgerBlue:return Color(30, 144, 255);
	case lzh::Color::EmeraldGreen:return Color(21, 174, 103);
	case lzh::Color::FireBrick:return Color(178, 34, 34);
	case lzh::Color::FloralWhite:return Color(255, 250, 240);
	case lzh::Color::ForestGreen:return Color(34, 139, 34);
	case lzh::Color::Fuchsia:return Color(255, 0, 255);
	case lzh::Color::Gainsboro:return Color(220, 220, 220);
	case lzh::Color::GhostWhite:return Color(248, 248, 255);
	case lzh::Color::Gold:return Color(255, 215, 0);
	case lzh::Color::Goldenrod:return Color(218, 165, 32);
	case lzh::Color::Gray:return Color(128, 128, 128);
	case lzh::Color::Green:return Color(0, 255, 0);
	case lzh::Color::GreenYellow:return Color(173, 255, 47);
	case lzh::Color::HoneyDew:return Color(240, 255, 240);
	case lzh::Color::HotPink:return Color(255, 105, 180);
	case lzh::Color::IndianRed:return Color(205, 92, 92);
	case lzh::Color::Indigo:return Color(75, 0, 130);
	case lzh::Color::Ivory:return Color(255, 255, 240);
	case lzh::Color::Khaki:return Color(240, 230, 140);
	case lzh::Color::Lavender:return Color(230, 230, 250);
	case lzh::Color::LavenderBlush:return Color(255, 240, 245);
	case lzh::Color::LawnGreen:return Color(124, 252, 0);
	case lzh::Color::Lemonchiffon:return Color(255, 250, 205);
	case lzh::Color::LightBlue:return Color(173, 216, 230);
	case lzh::Color::LightCoral:return Color(240, 128, 128);
	case lzh::Color::LightCyan:return Color(225, 255, 255);
	case lzh::Color::LightGoldenrodYellow:return Color(250, 250, 210);
	case lzh::Color::LightGreen:return Color(144, 238, 144);
	case lzh::Color::LightGrey:return Color(211, 211, 211);
	case lzh::Color::LightPink:return Color(255, 182, 193);
	case lzh::Color::LightSalmon:return Color(255, 160, 122);
	case lzh::Color::LightSeaGreen:return Color(32, 178, 170);
	case lzh::Color::LightSkyBlue:return Color(135, 206, 250);
	case lzh::Color::LightSlateGray:return Color(119, 136, 153);
	case lzh::Color::LightSteelBlue:return Color(176, 196, 222);
	case lzh::Color::LightYellow:return Color(255, 255, 224);
	case lzh::Color::Lilac:return Color(187, 161, 203);
	case lzh::Color::Lime:return Color(0, 255, 0);
	case lzh::Color::LimeGreen:return Color(50, 205, 50);
	case lzh::Color::Linen:return Color(250, 240, 230);
	case lzh::Color::Magenta:return Color(255, 0, 255);
	case lzh::Color::Maroon:return Color(128, 0, 0);
	case lzh::Color::Mauve:return Color(225, 152, 192);
	case lzh::Color::MediumAquamarine:return Color(0, 250, 154);
	case lzh::Color::MediumBlue:return Color(0, 0, 205);
	case lzh::Color::MediumOrchid:return Color(186, 85, 211);
	case lzh::Color::MediumPurple:return Color(147, 112, 219);
	case lzh::Color::MediumSeaGreen:return Color(60, 179, 113);
	case lzh::Color::MediumSlateBlue:return Color(123, 104, 238);
	case lzh::Color::MediumSpringGreen:return Color(245, 255, 250);
	case lzh::Color::MediumTurquoise:return Color(72, 209, 204);
	case lzh::Color::MediumVioletRed:return Color(199, 21, 133);
	case lzh::Color::MidNightBlue:return Color(25, 25, 112);
	case lzh::Color::Mintcream:return Color(0, 255, 127);
		//case lzh::Color::MistyGray:break;
	case lzh::Color::MistyRose:return Color(255, 228, 225);
	case lzh::Color::Moccasin:return Color(255, 228, 181);
	case lzh::Color::MossGreen:return Color(105, 119, 35);
	case lzh::Color::NavajoWhite:return Color(255, 222, 173);
	case lzh::Color::NavyBlue:return Color(0, 0, 128);
	case lzh::Color::OffWhite:return Color(192, 192, 192);
	case lzh::Color::Oldlace:return Color(253, 245, 230);
	case lzh::Color::Olive:return Color(128, 128, 0);
	case lzh::Color::Olivedrab:return Color(85, 107, 47);
	case lzh::Color::Orange:return Color(255, 165, 0);
	case lzh::Color::OrangeRed:return Color(255, 69, 0);
	case lzh::Color::Orchid:return Color(218, 112, 214);
	case lzh::Color::OysterWhite:return Color(255, 251, 240);
	case lzh::Color::OliveGreen:return Color(98, 90, 5);
	case lzh::Color::PaleGoldenrod:return Color(238, 232, 170);
	case lzh::Color::PaleGreen:return Color(152, 251, 152);
	case lzh::Color::PaleTurquoise:return Color(175, 238, 238);
	case lzh::Color::PaleVioletRed:return Color(219, 112, 147);
	case lzh::Color::Pansy:return Color(139, 0, 98);
	case lzh::Color::Papayawhip:return Color(255, 239, 213);
	case lzh::Color::Peachpuff:return Color(255, 218, 185);
	case lzh::Color::Peru:return Color(205, 133, 63);
	case lzh::Color::Pink:return Color(255, 192, 203);
	case lzh::Color::Plum:return Color(221, 160, 221);
	case lzh::Color::PowderBlue:return Color(176, 224, 230);
	case lzh::Color::Purple:return Color(128, 0, 128);
	case lzh::Color::Red:return Color(255, 0, 0);
	case lzh::Color::RosyBrown:return Color(188, 143, 143);
	case lzh::Color::RoyalBlue:return Color(65, 105, 225);
	case lzh::Color::Rubine:return Color(206, 0, 88);
	case lzh::Color::SaddleBrown:return Color(139, 69, 19);
	case lzh::Color::Salmon:return Color(250, 128, 114);
	case lzh::Color::SalmonPink:return Color(242, 155, 135);
	case lzh::Color::SandyBeige:return Color(230, 195, 195);
	case lzh::Color::SandyBrown:return Color(244, 164, 96);
	case lzh::Color::Sapphire:return Color(0, 47, 167);
	case lzh::Color::Scarlet:return Color(196, 76, 85);
	case lzh::Color::SeaGreen:return Color(46, 139, 87);
	case lzh::Color::Seashell:return Color(255, 245, 238);
	case lzh::Color::Sienna:return Color(160, 82, 45);
	case lzh::Color::Silver:return Color(192, 192, 192);
	case lzh::Color::SkyBlue:return Color(135, 206, 235);
	case lzh::Color::SlateBlue:return Color(106, 90, 205);
	case lzh::Color::SlateGray:return Color(112, 128, 144);
	case lzh::Color::SmokyGray:return Color(204, 204, 204);
	case lzh::Color::Snow:return Color(255, 250, 250);
	case lzh::Color::SpringGreen:return Color(60, 179, 113);
	case lzh::Color::SteelBlue:return Color(70, 130, 180);
	case lzh::Color::Stone:return Color(112, 128, 144);
	case lzh::Color::SnowyWhite:return Color(254, 250, 249);
	case lzh::Color::Tan:return Color(210, 180, 140);
	case lzh::Color::Teal:return Color(0, 128, 128);
	case lzh::Color::Thistle:return Color(216, 191, 216);
	case lzh::Color::Tomato:return Color(255, 99, 71);
	case lzh::Color::Turquoise:return Color(64, 224, 208);
	case lzh::Color::TurquoiseBlue:return Color(0, 164, 194);
	case lzh::Color::Violet:return Color(238, 130, 238);
	case lzh::Color::Wheat:return Color(245, 222, 179);
	case lzh::Color::White:return Color(255, 255, 255);
	case lzh::Color::SmokeWhite:return Color(245, 245, 245);
	case lzh::Color::WineRed:return Color(100, 1, 37);
	case lzh::Color::Yellow:return Color(255, 255, 0);
	case lzh::Color::YellowGreen:return Color(154, 205, 50);
	default:
		break;
	}
	return Color();
}

Color lzh::Color::CSS(std::string code)
{
	if (code.empty())return Color();
	int8 count = 0;
	Color color;
	color.setSpec(Rgb);
	uint16 v;
	mat_t* data = color.data;
	for (char ch : code) {
		if (__RANGE__('0', ch, '9'))
			v |= saturate_cast<uint8>(ch - '0') << count * 4;
		else if (__RANGE__('A', ch, 'F'))
			v |= saturate_cast<uint8>(ch - 'A' + 10) << count * 4;
		else if (__RANGE__('a', ch, 'f'))
			v |= saturate_cast<uint8>(ch - 'a' + 10) << count * 4;
		else return color;
		count = (count + 1) % 2;
		if (count == 0) {
			if (data == color.end())break;
			v = 0;
			*(data++) = v;
		}
	}
	return color;
}

Color lzh::Color::AntiColor(const Color & color)
{
	Color c;
	Abs(255 - color, c);
	return c;
}


Color lzh::Color::Empty(const Color & color)
{
	return color;
}

Color lzh::Color::Reverse(const Color & color)
{
	Color c = color;
	std::reverse(c.begin(), c.end() - 1);
	return color;
}

Color lzh::Color::Convert(const Color & color, Spec spec, Color(*f)(const Color &color))
{
	if (!f)return Color();
	if (color.spec == spec)return color;
	switch (color.spec)
	{
	case lzh::Color::Rgb:return f(color);
	case lzh::Color::Bgr:return f(Bgr2Rgb(color));
	case lzh::Color::Cmyk:return f(Cmyk2Rgb(color));
	case lzh::Color::Hsv:return f(Hsv2Rgb(color));
	case lzh::Color::Hsl:return f(Hsl2Rgb(color));
	case lzh::Color::Xyz:
		if (spec == Yxy)return Xyz2Yxy(color);
		else if (spec == Lab)return Xyz2Lab(color);
		else if (spec == Lch_ab)return Convert(Xyz2Lab(color), spec);
		else if (spec == Luv)return Xyz2Luv(color);
		else if (spec == Lch_uv)return Convert(Xyz2Luv(color), spec);
		else if (spec == Yxy)return Xyz2Yxy(color);
		else return f(Xyz2Rgb(color));
	case lzh::Color::Yxy:
		if (spec == Xyz)return Yxy2Xyz(color);
		else return f(Yxy2Rgb(color));
	case lzh::Color::Lab:
		if (spec == Luv)return Xyz2Luv(Lab2Xyz(color));
		else if (spec == Xyz)return Lab2Xyz(color);
		else if (spec == Lch_ab)return Lab2Lch(color);
		else return f(Lab2Rgb(color));
	case lzh::Color::Luv:
		if (spec == Lab)return Xyz2Lab(Luv2Xyz(color));
		else if (spec == Xyz)return Luv2Xyz(color);
		else if (spec == Lch_uv)return Luv2Lch(color);
		else return f(Luv2Rgb(color));
	case lzh::Color::Lch_ab:
		if (spec == Lab)return Lch2Lab(color);
		else if (spec == Luv || spec == Xyz)return Convert(Lch2Lab(color), spec);
		else return f(Lch_ab2Rgb(color));
	case lzh::Color::Lch_uv:
		if (spec == Luv)return Lch2Luv(color);
		else if (spec == Lab || spec == Xyz)return Convert(Lch2Luv(color), spec);
		else return f(Lch_uv2Rgb(color));
	default: return Color();
	}
}

Color lzh::Color::Rgb2Bgr(const Color & color)
{
	return Reverse(color).createSpec(Bgr);
}

Color lzh::Color::Bgr2Rgb(const Color & color)
{
	return Reverse(color).createSpec(Rgb);
}

Color lzh::Color::Rgb2Gray(const Color & color)
{
	return Color(color[0] * r_gray + color[1] * g_gray + color[2] * b_gray, Rgb).Alpha(color[3]);
}

Color lzh::Color::Gray2Rgb(const Color & color)
{
	return Color(color[0], Rgb).Alpha(color[3]);
}

Color lzh::Color::Rgb2Cmyk(const Color & color)
{
	Color rgb = color;
	Color cmyk, cmy;
	cmy[0] = 1 - (rgb[0] / 255);
	cmy[1] = 1 - (rgb[1] / 255);
	cmy[2] = 1 - (rgb[2] / 255);

	mat_t var_K = 1;

	if (cmy[0] < var_K)   var_K = cmy[0];
	if (cmy[1] < var_K)   var_K = cmy[1];
	if (cmy[2] < var_K)   var_K = cmy[2];
	if (var_K == 1) {
		cmyk[0] = 0;      //Black only
		cmyk[1] = 0;
		cmyk[2] = 0;
	}
	else {
		cmyk[0] = (cmy[0] - var_K) / (1 - var_K);
		cmyk[1] = (cmy[1] - var_K) / (1 - var_K);
		cmyk[2] = (cmy[2] - var_K) / (1 - var_K);
	}
	cmyk[3] = var_K;
	cmyk.setSpec(Cmyk);
	return cmyk;
}

Color lzh::Color::Cmyk2Rgb(const Color & color)
{
	Color cmyk = color;
	Color rgb, cmy;
	cmy[0] = (cmyk[0] * (1 - cmyk[3]) + cmyk[3]);
	cmy[1] = (cmyk[1] * (1 - cmyk[3]) + cmyk[3]);
	cmy[2] = (cmyk[2] * (1 - cmyk[3]) + cmyk[3]);

	rgb[0] = (1 - cmy[0]) * 255;
	rgb[1] = (1 - cmy[1]) * 255;
	rgb[2] = (1 - cmy[2]) * 255;

	rgb.setSpec(Rgb);
	return rgb;
}

Color lzh::Color::Rgb2Xyz(const Color & color)
{
	Color xyz;
	mat_t var_R = (color[0] / 255);
	mat_t var_G = (color[1] / 255);
	mat_t var_B = (color[2] / 255);

	if (var_R > 0.04045) var_R = pow((var_R + 0.055) / 1.055, 2.4);
	else var_R = var_R / 12.92;
	if (var_G > 0.04045) var_G = pow((var_G + 0.055) / 1.055, 2.4);
	else var_G = var_G / 12.92;
	if (var_B > 0.04045) var_B = pow((var_B + 0.055) / 1.055, 2.4);
	else var_B = var_B / 12.92;

	var_R = var_R * 100;
	var_G = var_G * 100;
	var_B = var_B * 100;

	xyz[0] = var_R * rgb2xyz[0] + var_G * rgb2xyz[1] + var_B * rgb2xyz[2];
	xyz[1] = var_R * rgb2xyz[3] + var_G * rgb2xyz[4] + var_B * rgb2xyz[5];
	xyz[2] = var_R * rgb2xyz[6] + var_G * rgb2xyz[7] + var_B * rgb2xyz[8];

	xyz.setA(color.a());
	xyz.setSpec(Xyz);
	return xyz;
}

Color lzh::Color::Xyz2Rgb(const Color & color)
{
	Color rgb;
	mat_t var_X = color[0] / 100;
	mat_t var_Y = color[1] / 100;
	mat_t var_Z = color[2] / 100;

	mat_t var_R = var_X * xyz2rgb[0] + var_Y * xyz2rgb[1] + var_Z * xyz2rgb[2];
	mat_t var_G = var_X * xyz2rgb[3] + var_Y * xyz2rgb[4] + var_Z * xyz2rgb[5];
	mat_t var_B = var_X * xyz2rgb[6] + var_Y * xyz2rgb[7] + var_Z * xyz2rgb[8];

	if (var_R > 0.0031308) var_R = 1.055 * pow(var_R, (1 / 2.4)) - 0.055;
	else var_R = 12.92 * var_R;
	if (var_G > 0.0031308) var_G = 1.055 * pow(var_G, (1 / 2.4)) - 0.055;
	else var_G = 12.92 * var_G;
	if (var_B > 0.0031308) var_B = 1.055 * pow(var_B, (1 / 2.4)) - 0.055;
	else var_B = 12.92 * var_B;

	rgb[0] = var_R * 255;
	rgb[1] = var_G * 255;
	rgb[2] = var_B * 255;

	rgb.setA(color.a());
	rgb.setSpec(Rgb);
	return rgb;
}

Color lzh::Color::Rgb2Yxy(const Color & color)
{
	return Xyz2Yxy(Rgb2Xyz(color));
}

Color lzh::Color::Yxy2Rgb(const Color & color)
{
	return Xyz2Rgb(Yxy2Xyz(color));
}

Color lzh::Color::Rgb2Hsv(const Color & color)
{
	Color hsv;
	hsv.setSpec(Hsv);
	Color rgb = color;
	rgb[0] = rgb[0] / 255;
	rgb[1] = rgb[1] / 255;
	rgb[2] = rgb[2] / 255;
	mat_t Cmax = Max(rgb.toVec3());
	mat_t Cmin = Min(rgb.toVec3());
	mat_t Delta = Cmax - Cmin;
	hsv[2] = Cmax;
	if (Delta == 0) {
		hsv[0] = 0;
		hsv[1] = 0;
	}
	else {
		hsv[1] = Delta / Cmax;
		mat_t del_R = (((Cmax - rgb[0]) / 6) + (Delta / 2)) / Delta;
		mat_t del_G = (((Cmax - rgb[1]) / 6) + (Delta / 2)) / Delta;
		mat_t del_B = (((Cmax - rgb[2]) / 6) + (Delta / 2)) / Delta;

		if (rgb[0] == Cmax) hsv[0] = del_B - del_G;
		else if (rgb[1] == Cmax) hsv[0] = (1 / 3.0) + del_R - del_B;
		else if (rgb[2] == Cmax) hsv[0] = (2 / 3.0) + del_G - del_R;
		if (hsv[0] < 0) hsv[0] += 1;
		if (hsv[0] > 1) hsv[0] -= 1;
		//hsv[0] *= 360;
	}
	return hsv.Alpha(color[3]);
}

Color lzh::Color::Hsv2Rgb(const Color & color)
{
	Color rgb;
	rgb.setSpec(Rgb);
	Color hsv = color;
	if (hsv[1] == 0) {
		rgb[0] = hsv[0] * 255;
		rgb[1] = hsv[1] * 255;
		rgb[2] = hsv[2] * 255;
	}
	else {
		Color pqt;
		hsv[0] = hsv[0] * 6;
		if (hsv[0] == 6)
		{
			hsv[0] = 0;
		}
		int32 hi = int32(hsv[0]);
		mat_t f = hsv[0] - hi;
		pqt[0] = hsv[2] * (1 - hsv[1]);
		pqt[1] = hsv[2] * (1 - f * hsv[1]);
		pqt[2] = hsv[2] * (1 - (1 - f) * hsv[1]);
		switch (hi)
		{
		case 0:rgb = Color(hsv[2], pqt[2], pqt[0]); break;
		case 1:rgb = Color(pqt[1], hsv[2], pqt[0]); break;
		case 2:rgb = Color(pqt[0], hsv[2], pqt[2]); break;
		case 3:rgb = Color(pqt[0], pqt[1], hsv[2]); break;
		case 4:rgb = Color(pqt[2], pqt[0], hsv[2]); break;
		default:rgb = Color(hsv[2], pqt[0], pqt[1]); break;
		}
		rgb[0] *= 255;
		rgb[1] *= 255;
		rgb[2] *= 255;
	}
	return rgb.Alpha(color[3]);
}

Color lzh::Color::Rgb2Hsl(const Color & color)
{
	Color hsl;
	hsl.setSpec(Hsl);
	Color rgb = color;

	rgb[0] = rgb[0] / 255;
	rgb[1] = rgb[1] / 255;
	rgb[2] = rgb[2] / 255;

	mat_t Cmax = Max(rgb.toVec3());
	mat_t Cmin = Min(rgb.toVec3());
	mat_t Delta = Cmax - Cmin;

	hsl[2] = (Cmax + Cmin) * 0.5;
	if (isEqual(Delta, 0)) {
		hsl[1] = 0;
		hsl[0] = 0;
	}
	else {
		if (__RANGE(0, hsl[2], 0.5))
			hsl[1] = Delta / (Cmax + Cmin);
		else
			hsl[1] = Delta / (2 - Cmax - Cmin);

		mat_t del_R = (((Cmax - rgb[0]) / 6) + (Delta / 2)) / Delta;
		mat_t del_G = (((Cmax - rgb[1]) / 6) + (Delta / 2)) / Delta;
		mat_t del_B = (((Cmax - rgb[2]) / 6) + (Delta / 2)) / Delta;

		if (Cmax == rgb[0])hsl[0] = del_B - del_G;
		else if (Cmax == rgb[1])hsl[0] = (1.0 / 3.0) + del_R - del_B;
		else if (Cmax == rgb[2])hsl[0] = (2.0 / 3.0) + del_G - del_R;
		if (hsl[0] < 0)hsl[0] += 1;
		if (hsl[0] > 1)hsl[0] -= 1;
	}
	return hsl.Alpha(color[3]);
}

Color lzh::Color::Hsl2Rgb(const Color & color)
{
	Color rgb;
	rgb.setSpec(Rgb);
	Color hsl = color;
	if (isEqual(hsl[1], 0)) {
		rgb[0] = hsl[0] * 255;
		rgb[1] = hsl[1] * 255;
		rgb[2] = hsl[2] * 255;
	}
	else {
		Color t;
		mat_t p, q;

		if (hsl[2] < 0.5) q = hsl[2] * (1 + hsl[1]);
		else q = hsl[2] + hsl[1] - hsl[2] * hsl[1];

		p = 2 * hsl[2] - q;
		t[0] = hsl[0] + 1 / 3.0;
		t[1] = hsl[0];
		t[2] = hsl[0] - 1 / 3.0;
		mat_t th[3] = { 1 / 6.0 , 0.5, 2 / 3.0 };

		for (int i = 0; i < 3; i++) {
			if (t[i] < 0)t[i] += 1;
			else if (t[i] > 1)t[i] -= 1;
			if (t[i] < th[0])rgb[i] = p + ((q - p) * 6 * t[i]);
			else if (__RANGE(th[0], t[i], th[1]))rgb[i] = q;
			else if (__RANGE(th[1], t[i], th[2]))rgb[i] = p + ((q - p) * 6 * (th[2] - t[i]));
			else rgb[i] = p;
		}

		rgb[0] *= 255;
		rgb[1] *= 255;
		rgb[2] *= 255;
	}
	return rgb.Alpha(color[3]);
}

Color lzh::Color::Rgb2Lab(const Color & color)
{
	return Xyz2Lab(Rgb2Xyz(color));
}

Color lzh::Color::Lab2Rgb(const Color & color)
{
	return Xyz2Rgb(Lab2Xyz(color));
}

Color lzh::Color::Rgb2Luv(const Color & color)
{
	return Xyz2Luv(Rgb2Xyz(color));
}

Color lzh::Color::Luv2Rgb(const Color & color)
{
	return Xyz2Rgb(Luv2Xyz(color));
}

Color lzh::Color::Rgb2Lch_ab(const Color & color)
{
	return Lab2Lch(Rgb2Lab(color));
}

Color lzh::Color::Lch_ab2Rgb(const Color & color)
{
	return Lab2Rgb(Lch2Lab(color));
}

Color lzh::Color::Rgb2Lch_uv(const Color & color)
{
	return Luv2Lch(Rgb2Luv(color));
}

Color lzh::Color::Lch_uv2Rgb(const Color & color)
{
	return Luv2Rgb(Lch2Luv(color));
}

Color lzh::Color::Lab2Lch(const Color & color)
{
	Color lab = color;
	Color lch;
	mat_t var_H = atan2(lab[2], lab[1]);

	if (var_H > 0) var_H = (var_H / PI) * 180;
	else var_H = 360 - (abs(var_H) / PI) * 180;

	lch[0] = lab[0];
	lch[1] = sqrt(lab[1] * lab[1] + lab[2] * lab[2]);
	lch[2] = var_H;
	lch.setA(lab.a());
	lch.setSpec(Lch_ab);
	return lch;
}

Color lzh::Color::Lch2Lab(const Color & color)
{
	Color lch = color;
	Color lab;

	lab[0] = lch[0];
	lab[1] = cos(ang2rad*lch[2]) * lch[1];
	lab[2] = sin(ang2rad*lch[2]) * lch[1];

	lab.setA(lch.a());
	lab.setSpec(Lab);
	return lab;
}

Color lzh::Color::Luv2Lch(const Color & color)
{
	Color luv = color;
	Color lch;
	mat_t var_H = atan2(luv[2], luv[1]);

	if (var_H > 0) var_H = (var_H / PI) * 180;
	else var_H = 360 - (abs(var_H) / PI) * 180;

	lch[0] = luv[0];
	lch[1] = sqrt(luv[1] * luv[1] + luv[2] * luv[2]);
	lch[2] = var_H;
	lch.setA(luv.a());
	lch.setSpec(Lch_uv);
	return lch;
}

Color lzh::Color::Lch2Luv(const Color & color)
{
	Color lch = color;
	Color luv;

	luv[0] = lch[0];
	luv[1] = cos(ang2rad*lch[2]) * lch[1];
	luv[2] = sin(ang2rad*lch[2]) * lch[1];

	luv.setA(lch.a());
	luv.setSpec(Luv);
	return luv;
}

Color lzh::Color::Xyz2Yxy(const Color & color)
{
	Color yxy;
	yxy.setSpec(Yxy);
	yxy[0] = color[1];
	mat_t v = color[0] + color[1] + color[2];
	if (isEqual(v, 0)) {
		yxy[1] = 0; yxy[2] = 0;
	}
	else {
		if (isEqual(color[0], 0))yxy[1] = 0;
		else yxy[1] = color[0] / v;
		if (isEqual(color[1], 0))yxy[2] = 0;
		else yxy[2] = color[1] / v;
	}
	return yxy.Alpha(color[3]);
}

Color lzh::Color::Yxy2Xyz(const Color & color)
{
	Color xyz;
	xyz.setSpec(Xyz);
	xyz[1] = color[0];
	if (isEqual(color[2], 0)) {
		xyz[0] = 0;
		xyz[2] = 0;
	}
	else {
		xyz[0] = color[0] / color[2] * color[1];
		xyz[2] = (1 - color[1] - color[2]) * (color[0] / color[2]);
	}
	return xyz.Alpha(color[3]);
}

Color lzh::Color::Xyz2Lab(const Color & color)
{
	Color lab, xyz, fxyz;
	lab.setSpec(Lab);
	Color param = Color(Reference_X, Reference_Y, Reference_Z, 1);
	mat_t param_13 = _T(1.0 / 3.0), param_16116 = _T(16.0 / 116.0);
	xyz = color / param;

	if (xyz[0] > 0.008856) xyz[0] = pow(xyz[0], param_13);
	else xyz[0] = (7.787 * xyz[0]) + param_16116;
	if (xyz[1] > 0.008856) xyz[1] = pow(xyz[1], param_13);
	else xyz[1] = (7.787 * xyz[1]) + param_16116;
	if (xyz[2] > 0.008856) xyz[2] = pow(xyz[2], param_13);
	else xyz[2] = (7.787 * xyz[2]) + param_16116;

	lab[0] = 116 * xyz[1] - 16;
	lab[1] = 500 * (xyz[0] - xyz[1]);
	lab[2] = 200 * (xyz[1] - xyz[2]);
	return lab.Alpha(color[3]);
}

Color lzh::Color::Lab2Xyz(const Color & color)
{
	Color lab = color;
	Color xyz, fxyz;
	Color param = Color(Reference_X, Reference_Y, Reference_Z, _T(1.0));
	mat_t param_16116 = _T(16.0 / 116.0), param_div = _T(7.787);

	mat_t var_Y = (lab[0] + 16) / 116;
	mat_t var_X = lab[1] / 500 + var_Y;
	mat_t var_Z = var_Y - lab[2] / 200;

	mat_t var_xxx = pow(var_X, 3);
	mat_t var_yyy = pow(var_Y, 3);
	mat_t var_zzz = pow(var_Z, 3);

	if (var_yyy > 0.008856) fxyz[1] = var_yyy;
	else fxyz[1] = (var_Y - param_16116) / param_div;
	if (var_xxx > 0.008856) fxyz[0] = var_xxx;
	else fxyz[0] = (var_X - param_16116) / param_div;
	if (var_zzz > 0.008856) fxyz[2] = var_zzz;
	else fxyz[2] = (var_Z - param_16116) / param_div;

	xyz = fxyz * param;
	xyz.setSpec(Xyz);
	return xyz.Alpha(color[3]);
}

Color lzh::Color::Xyz2Luv(const Color & color)
{
	Color luv;
	Color xyz = color;
	mat_t X = xyz[0], Y = xyz[1], Z = xyz[2];
	mat_t var_U = (4 * X) / (X + (15 * Y) + (3 * Z));
	mat_t var_V = (9 * Y) / (X + (15 * Y) + (3 * Z));

	mat_t ref_U = (4 * Reference_X) / (Reference_X + (15 * Reference_Y) + (3 * Reference_Z));
	mat_t ref_V = (9 * Reference_Y) / (Reference_X + (15 * Reference_Y) + (3 * Reference_Z));

	mat_t var_Y = Y / 100;
	if (var_Y > 0.0088564516790356) var_Y = pow(var_Y, (1 / 3.0));
	else var_Y = (7.787 * var_Y) + (16.0 / 116.0);

	luv[0] = (116 * var_Y) - 16;
	luv[1] = 13 * luv[0] * (var_U - ref_U);
	luv[2] = 13 * luv[0] * (var_V - ref_V);
	luv.setA(xyz.a());
	luv.setSpec(Luv);
	return luv;
}

Color lzh::Color::Luv2Xyz(const Color & color)
{
	Color luv = color;
	Color xyz;
	mat_t var_Y = (luv[0] + 16) / 116.0;
	mat_t var_yyy = var_Y * var_Y * var_Y;
	if (var_yyy > 0.0088564516790356) var_Y = var_yyy;
	else var_Y = (var_Y - 16.0 / 116.0) / 7.787;

	mat_t ref_U = (4 * Reference_X) / (Reference_X + (15 * Reference_Y) + (3 * Reference_Z));
	mat_t ref_V = (9 * Reference_Y) / (Reference_X + (15 * Reference_Y) + (3 * Reference_Z));

	mat_t var_U = luv[1] / (13 * luv[0]) + ref_U;
	mat_t var_V = luv[2] / (13 * luv[0]) + ref_V;

	xyz[1] = var_Y * 100;
	xyz[0] = -(9 * xyz[1] * var_U) / ((var_U - 4) * var_V - var_U * var_V);
	xyz[2] = (9 * xyz[1] - (15 * var_V * xyz[1]) - (var_V * xyz[0])) / (3 * var_V);
	xyz.setA(xyz.a());
	xyz.setSpec(Xyz);
	return xyz;
}

Color lzh::operator+(Val val, const Color & color)
{
	Color v;
	v[0] = val + color[0];
	v[1] = val + color[1];
	v[2] = val + color[2];
	v[3] = val + color[3];
	v.setSpec(color.getSpec());
	return v;
}

Color lzh::operator+(const Color & c1, const Color & c2)
{
	Color v;
	v[0] = c1[0] + c2[0];
	v[1] = c1[1] + c2[1];
	v[2] = c1[2] + c2[2];
	v[3] = c1[3] + c2[3];
	v.setSpec(c1.getSpec());
	return v;
}

Color lzh::operator+(const Color & color, Val val)
{
	Color v;
	v[0] = val + color[0];
	v[1] = val + color[1];
	v[2] = val + color[2];
	v[3] = val + color[3];
	v.setSpec(color.getSpec());
	return v;
}

Color lzh::operator-(Val val, const Color & color)
{
	Color v;
	v[0] = val - color[0];
	v[1] = val - color[1];
	v[2] = val - color[2];
	v[3] = val - color[3];
	v.setSpec(color.getSpec());
	return v;
}

Color lzh::operator-(const Color & c1, const Color & c2)
{
	Color v;
	v[0] = c1[0] - c2[0];
	v[1] = c1[1] - c2[1];
	v[2] = c1[2] - c2[2];
	v[3] = c1[3] - c2[3];
	v.setSpec(c1.getSpec());
	return v;
}

Color lzh::operator-(const Color & color, Val val)
{
	Color v;
	v[0] = color[0] - val;
	v[1] = color[1] - val;
	v[2] = color[2] - val;
	v[3] = color[3] - val;
	v.setSpec(color.getSpec());
	return v;
}

Color lzh::operator*(Val val, const Color & color)
{
	Color v;
	v[0] = val * color[0];
	v[1] = val * color[1];
	v[2] = val * color[2];
	v[3] = val * color[3];
	v.setSpec(color.getSpec());
	return v;
}

Color lzh::operator*(const Color & c1, const Color & c2)
{
	Color v;
	v[0] = c1[0] * c2[0];
	v[1] = c1[1] * c2[1];
	v[2] = c1[2] * c2[2];
	v[3] = c1[3] * c2[3];
	v.setSpec(c1.getSpec());
	return v;
}

Color lzh::operator*(const Color & color, Val val)
{
	Color v;
	v[0] = color[0] * val;
	v[1] = color[1] * val;
	v[2] = color[2] * val;
	v[3] = color[3] * val;
	v.setSpec(color.getSpec());
	return v;
}

Color lzh::operator/(Val val, const Color & color)
{
	Color v;
	v[0] = val / color[0];
	v[1] = val / color[1];
	v[2] = val / color[2];
	v[3] = val / color[3];
	v.setSpec(color.getSpec());
	return v;
}

Color lzh::operator/(const Color & c1, const Color & c2)
{
	Color v;
	v[0] = c1[0] / c2[0];
	v[1] = c1[1] / c2[1];
	v[2] = c1[2] / c2[2];
	v[3] = c1[3] / c2[3];
	v.setSpec(c1.getSpec());
	return v;
}

Color lzh::operator/(const Color & color, Val val)
{
	Color v;
	v[0] = color[0] / val;
	v[1] = color[1] / val;
	v[2] = color[2] / val;
	v[3] = color[3] / val;
	v.setSpec(color.getSpec());
	return v;
}

Color lzh::operator+=(Color & color, Val val)
{
	color[0] += val;
	color[1] += val;
	color[2] += val;
	color[3] += val;
	return color;
}

Color lzh::operator+=(Color & color, Color & c)
{
	color[0] += c[0];
	color[1] += c[1];
	color[2] += c[2];
	color[3] += c[3];
	return color;
}

Color lzh::operator-=(Color & color, Val val)
{
	color[0] -= val;
	color[1] -= val;
	color[2] -= val;
	color[3] -= val;
	return color;
}

Color lzh::operator-=(Color & color, Color & c)
{
	color[0] -= c[0];
	color[1] -= c[1];
	color[2] -= c[2];
	color[3] -= c[3];
	return color;
}

Color lzh::operator*=(Color & color, Val val)
{
	color[0] *= val;
	color[1] *= val;
	color[2] *= val;
	color[3] *= val;
	return color;
}

Color lzh::operator*=(Color & color, Color & c)
{
	color[0] *= c[0];
	color[1] *= c[1];
	color[2] *= c[2];
	color[3] *= c[3];
	return color;
}

Color lzh::operator/=(Color & color, Val val)
{
	color[0] /= val;
	color[1] /= val;
	color[2] /= val;
	color[3] /= val;
	return color;
}

Color lzh::operator/=(Color & color, Color & c)
{
	color[0] /= c[0];
	color[1] /= c[1];
	color[2] /= c[2];
	color[3] /= c[3];
	return color;
}
