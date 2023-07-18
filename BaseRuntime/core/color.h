#ifndef __COLOR_H__
#define __COLOR_H__
#include "vector.h"

namespace lzh
{
	class LZHAPI Color : public Vec4m
	{
	public:
		using value_type = typename Vec4m::value_type;
		enum Spec { Rgb, Bgr, Cmyk, Hsv, Hsl, Xyz, Yxy, Lab, Lch_ab, Lch_uv, Luv };
		enum ColorTab
		{
			Amber = 0,  //����ɫ
			//AntiqueViolet,  //����ɫ
			AntiqueWhite,  //�Ŷ���
			Aqua,  //ǳ��ɫ
			AquaMarine,  //������ʯɫ
			AquaMarineBlue,  //����ɫ
			Azure,  //����ɫ
			AliceBlue, //����˿��
			BabyPink,  //ǳ�ۺ�ɫ
			Beige,  //��ɫ
			Bisque,  //�ٻ�ɫ
			Black,  //��ɫ
			BlancheDalmond,  //����ɫ
			Blue,  //��ɫ
			BlueViolet,  //������ɫ
			Brown,  //��ɫ
			BurlyWood,  //ʵľɫ
			CobaltBlue,  //����ɫ\����ɫ
			CadetBlue,  //����ɫ
			Camel,  //��ɫ
			CharcoalGray,  //̿��ɫ
			Chartreuse,  //����ɫ
			Chocolate,  //�ɿ���ɫ
			CobaltVlue,  //����ɫ
			Coral,  //ɺ��ɫ
			CornflowerBlue,  //����ɫ
			Cornsilk,  //�׳�ɫ
			Crimson,  //�����ɫ
			Cyan,  //��ɫ
			DarkBlue,  //����ɫ
			DarkCyan,  //����ɫ
			DarkGoldenrod,  //�����ɫ
			DarkGray,  //����ɫ
			DarkGreen,  //����ɫ
			DarkKhaki,  //������ɫ
			DarkMagenta,  //�����ɫ
			DarkOliveGreen,  //�������ɫ
			DarkOrange,  //����ɫ
			DarkOrchid,  //����ɫ
			DarkRed,  //����ɫ
			DarkSalmon,  //����ɫ
			DarkSeaGreen,  //������ɫ
			DarkSlateBlue,  //������ɫ
			DarkSlateGray,  //ī��ɫ
			DarkTurquoise,  //����ʯ��
			DarkViolet,  //����ɫ
			DeepPink,  //���ɫ
			DeepSkyBlue,  //������ɫ
			DimGray,  //����ɫ
			DoderBlue,  //������
			DodgerBlue,  //����ɫ
			EmeraldGreen,  //����ɫ
			FireBrick,  //��שɫ
			FloralWhite,  //����ɫ
			ForestGreen,  //ɭ����
			Fuchsia,  //�Ϻ�ɫ
			Gainsboro,  //����ɫ
			GhostWhite,  //�����
			Gold,  //���ɫ
			Goldenrod,  //������ɫ
			Gray,  //��ɫ
			Green,  //��ɫ
			GreenYellow,  //����ɫ
			HoneyDew,  //��ɫ
			HotPink,  //�޷�ɫ
			IndianRed,  //ӡ�ڰ���
			Indigo,  //����ɫ
			Ivory,  //����ɫ
			Khaki,  //����ɫ
			Lavender,  //Ѭ�²�ɫ
			LavenderBlush,  //���Ϻ�
			LawnGreen,  //����ɫ
			Lemonchiffon,  //���ʳ�ɫ
			LightBlue,  //ǳ��ɫ
			LightCoral,  //ǳɺ��ɫ
			LightCyan,  //ǳ��ɫ
			LightGoldenrodYellow,  //ǳ���ɫ
			LightGreen,  //ǳ��ɫ
			LightGrey,  //ǳ��ɫ
			LightPink,  //ǳ��ɫ
			LightSalmon,  //ǳ��ɫ
			LightSeaGreen,  //ǳ����ɫ
			LightSkyBlue,  //ǳ����ɫ
			LightSlateGray,  //ǳ����ɫ
			LightSteelBlue,  //ǳ����ɫ
			LightYellow,  //ǳ��ɫ
			Lilac,  //ǳ��ɫ
			Lime,  //���ɫ
			LimeGreen,  //����ɫ
			Linen,  //����ɫ
			Magenta,  //���ɫ
			Maroon,  //��ɫ
			Mauve,  //�Ϻ�
			MediumAquamarine,  //����ɫ
			MediumBlue,  //����ɫ
			MediumOrchid,  //����ɫ
			MediumPurple,  //����ɫ
			MediumSeaGreen,  //�亣��ɫ
			MediumSlateBlue,  //�䰵��ɫ
			MediumSpringGreen,  //�䴺��ɫ
			MediumTurquoise,  //���̱�ʯɫ
			MediumVioletRed,  //��������ɫ
			MidNightBlue,  //�л���ɫ
			Mintcream,  //����ɫ
			//MistyGray,  //���ɫ
			MistyRose,  //ǳõ��ɫ
			Moccasin,  //¹Ƥɫ
			MossGreen, //̦��ɫ
			NavajoWhite,  //���߰�
			NavyBlue,  //����ɫ\����ɫ\����ɫ
			OffWhite,  //�Ұ�
			Oldlace,  //�ϻ�ɫ
			Olive,  //���ɫ
			Olivedrab,  //���̺�ɫ
			Orange,  //��ɫ
			OrangeRed,  //�Ⱥ�ɫ
			Orchid,  //����ɫ
			OysterWhite,  //���ɫ
			OliveGreen,  //�����
			PaleGoldenrod,  //������ɫ
			PaleGreen,  //����ɫ
			PaleTurquoise,  //����ɫ
			PaleVioletRed,  //��������ɫ
			Pansy,  //������ɫ
			Papayawhip,  //��ľɫ
			Peachpuff,  //��ɫ
			Peru,  //��³ɫ
			Pink,  //�ۺ�
			Plum,  //����ɫ
			PowderBlue,  //����ɫ
			Purple,  //��ɫ
			Red,  //��ɫ
			RosyBrown,  //��õ���
			RoyalBlue,  //����ɫ
			Rubine,  //��ʯ��
			SaddleBrown,  //�غ�ɫ
			Salmon,  //����ɫ
			SalmonPink,  //�Ⱥ�ɫ
			SandyBeige,  //ǳ��ɫ
			SandyBrown,  //ɳ��ɫ
			Sapphire,  //��ʯ��
			Scarlet,  //�ɺ�ɫ
			SeaGreen,  //����ɫ
			Seashell,  //����ɫ
			Sienna,  //��ɫ
			Silver,  //����ɫ
			SkyBlue,  //����ɫ
			SlateBlue,  //ʯ��ɫ
			SlateGray,  //��ʯɫ
			SmokyGray,  //�̻�ɫ
			Snow,  //ѩ��ɫ
			SpringGreen,  //����ɫ
			SteelBlue,  //����ɫ
			Stone,  //ʯɫ
			SnowyWhite,  //ѩ��ɫ
			Tan,  //��ɫ
			Teal,  //ˮѼɫ
			Thistle,  //��ɫ
			Tomato,  //����ɫ
			Turquoise,  //����ɫ
			TurquoiseBlue,  //����ɫ
			Violet,  //��ɫ
			Wheat,  //ǳ��ɫ
			White,  //��ɫ
			SmokeWhite,  //�̰�ɫ
			WineRed,  //���Ѿƺ�
			Yellow,  //��ɫ
			YellowGreen  //����ɫ
		};
		explicit Color() : Vec4m(), spec(Rgb) {}
		explicit Color(std::string css) : Vec4m() { *this = CSS(css); }
		template<typename _Tp> explicit Color(const _Tp v, Spec spec = Rgb) : Vec4m(v), spec(spec) { data[3] = 255; }
		template<typename _T1, typename _T2> explicit Color(const _T1 v1, const _T2 v2, Spec spec = Rgb) : Vec4m(v1, v2), spec(spec) { data[3] = 255; }
		template<typename _T1, typename _T2, typename _T3> explicit Color(const _T1 v1, const _T2 v2, const _T3 v3, Spec spec = Rgb) : Vec4m(v1, v2, v3), spec(spec) { data[3] = 255; }
		template<typename _T1, typename _T2, typename _T3, typename _T4> explicit Color(const _T1 v1, const _T2 v2, const _T3 v3, const _T4 v4, Spec spec = Rgb) : Vec4m(v1, v2, v3, v4), spec(spec) {}
		template<typename _Tp> Color(std::initializer_list<_Tp> &_List) : Vec4m(_List), spec(Rgb) {}
		Color(ColorTab ColorTableIdx) : Vec4m(), spec(Rgb) { *this = Tab(ColorTableIdx); }

		Color anti()const noexcept;
		std::string css()const noexcept;
		void setTab(ColorTab colorTab);
		void setCSS(std::string code);

		int32 rgb()const noexcept;
        void setRgb(int32 code) noexcept;
		int32 rgba()const noexcept;
        void setRgba(int32 code) noexcept;

		uint8 red()const;
		uint8 green()const;
		uint8 blue()const;
		uint8 alpha()const;
		void setRed(uint8 r);
		void setGreen(uint8 g);
		void setBlue(uint8 b);
		void setAlpha(uint8 a);

		mat_t& r()noexcept;
		mat_t& g()noexcept;
		mat_t& b()noexcept;
		mat_t& a()noexcept;
		const mat_t& r()const noexcept;
		const mat_t& g()const noexcept;
		const mat_t& b()const noexcept;
		const mat_t& a()const noexcept;
		void setR(mat_t r)noexcept;
		void setG(mat_t g)noexcept;
		void setB(mat_t b)noexcept;
		void setA(mat_t a)noexcept;

		Color Alpha(mat_t a)const noexcept;
		Color Uint8()const noexcept;
		Color inv()const noexcept;
		Color createSpec(Spec spec)const noexcept;
		//Color toSpec(Spec spec)const noexcept;
		Color reverse(bool include_alpha = false)const noexcept;
		//operator mat_t &() { return data[0]; }
		//operator const mat_t &()const { return data[0]; }

		Color toGray() const noexcept;
		Color toRgb() const noexcept;
		Color toBgr() const noexcept;
		Color toCmyk() const noexcept;
		Color toHsv() const noexcept;
		Color toHsl() const noexcept;
		Color toXyz() const noexcept;
		Color toYxy() const noexcept;
		Color toLab() const noexcept;
		Color toLuv() const noexcept;
		Color toLch_ab() const noexcept;
		Color toLch_uv() const noexcept;
		Color toSpec(Spec spec) const noexcept;

		void setSpec(Spec spec);
		Spec getSpec() const noexcept;
		void setSpec(ColorGamut gamut);

		void setVec3(const Vec3m &vec);
		Vec3m toVec3()const noexcept;
		void setVec4(const Vec3m &vec);
		Vec4m toVec4()const noexcept;
		bool operator != (ColorTab idx)const;
		bool operator == (ColorTab idx)const;
		bool operator != (const Color &color)const;
		bool operator == (const Color &color)const;

		static int32 tabLength;
		static Color Random(const std::vector<Color>& except = std::vector<Color>());
		static Color Tab(ColorTab index);
		static Color CSS(std::string code);
		static Color AntiColor(const Color & color);

		static Color Empty(const Color &color);
		static Color Reverse(const Color &color);
		static Color Convert(const Color &color, Spec spec, Color(*f)(const Color &color) = Empty);

		static Color Rgb2Bgr(const Color &color);
		static Color Bgr2Rgb(const Color &color);
		static Color Rgb2Gray(const Color &color);
		static Color Gray2Rgb(const Color &color);
		static Color Rgb2Cmyk(const Color &color);
		static Color Cmyk2Rgb(const Color &color);
		static Color Rgb2Xyz(const Color &color);
		static Color Xyz2Rgb(const Color &color);
		static Color Rgb2Yxy(const Color &color);
		static Color Yxy2Rgb(const Color &color);
		static Color Rgb2Hsv(const Color &color);
		static Color Hsv2Rgb(const Color &color);
		static Color Rgb2Hsl(const Color &color);
		static Color Hsl2Rgb(const Color &color);
		static Color Rgb2Lab(const Color &color);
		static Color Lab2Rgb(const Color &color);
		static Color Rgb2Luv(const Color &color);
		static Color Luv2Rgb(const Color &color);
		static Color Rgb2Lch_ab(const Color &color);
		static Color Lch_ab2Rgb(const Color &color);
		static Color Rgb2Lch_uv(const Color &color);
		static Color Lch_uv2Rgb(const Color &color);

		static Color Lab2Lch(const Color &color);
		static Color Lch2Lab(const Color &color);
		static Color Luv2Lch(const Color &color);
		static Color Lch2Luv(const Color &color);

		static Color Xyz2Yxy(const Color &color);
		static Color Yxy2Xyz(const Color &color);
		static Color Xyz2Lab(const Color &color);
		static Color Lab2Xyz(const Color &color);
		static Color Xyz2Luv(const Color &color);
		static Color Luv2Xyz(const Color &color);

		static const mat_t r_gray;
		static const mat_t g_gray;
		static const mat_t b_gray;
		static const mat_t Reference_X;
		static const mat_t Reference_Y;
		static const mat_t Reference_Z;
		static const mat_t xyz2rgb[9];
		static const mat_t rgb2xyz[9];
	private:
		Spec spec;
	};
	Color operator + (Val val, const Color & color);
	Color operator + (const Color & c1, const Color & c2);
	Color operator + (const Color & color, Val val);

	Color operator - (Val val, const Color & color);
	Color operator - (const Color & c1, const Color & c2);
	Color operator - (const Color & color, Val val);

	Color operator * (Val val, const Color & color);
	Color operator * (const Color & c1, const Color & c2);
	Color operator * (const Color & color, Val val);

	Color operator / (Val val, const Color & color);
	Color operator / (const Color & c1, const Color & c2);
	Color operator / (const Color & color, Val val);

	Color operator += (Color & color, Val val);
	Color operator += (Color & color, Color & c);

	Color operator -= (Color & color, Val val);
	Color operator -= (Color & color, Color & c);

	Color operator *= (Color & color, Val val);
	Color operator *= (Color & color, Color & c);

	Color operator /= (Color & color, Val val);
	Color operator /= (Color & color, Color & c);
}
#endif // ! __COLOR_H__
