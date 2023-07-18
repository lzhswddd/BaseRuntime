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
			Amber = 0,  //琥珀色
			//AntiqueViolet,  //古紫色
			AntiqueWhite,  //古董白
			Aqua,  //浅绿色
			AquaMarine,  //海蓝宝石色
			AquaMarineBlue,  //蓝绿色
			Azure,  //天蓝色
			AliceBlue, //爱丽丝蓝
			BabyPink,  //浅粉红色
			Beige,  //米色
			Bisque,  //橘黄色
			Black,  //黑色
			BlancheDalmond,  //白杏色
			Blue,  //蓝色
			BlueViolet,  //紫罗兰色
			Brown,  //棕色
			BurlyWood,  //实木色
			CobaltBlue,  //钴蓝色\艳蓝色
			CadetBlue,  //军蓝色
			Camel,  //驼色
			CharcoalGray,  //炭灰色
			Chartreuse,  //黄绿色
			Chocolate,  //巧克力色
			CobaltVlue,  //艳蓝色
			Coral,  //珊瑚色
			CornflowerBlue,  //菊蓝色
			Cornsilk,  //米绸色
			Crimson,  //暗深红色
			Cyan,  //青色
			DarkBlue,  //暗蓝色
			DarkCyan,  //暗青色
			DarkGoldenrod,  //暗金黄色
			DarkGray,  //暗灰色
			DarkGreen,  //暗绿色
			DarkKhaki,  //暗卡其色
			DarkMagenta,  //暗洋红色
			DarkOliveGreen,  //暗橄榄绿色
			DarkOrange,  //暗桔色
			DarkOrchid,  //暗紫色
			DarkRed,  //暗红色
			DarkSalmon,  //暗肉色
			DarkSeaGreen,  //暗海蓝色
			DarkSlateBlue,  //暗灰蓝色
			DarkSlateGray,  //墨绿色
			DarkTurquoise,  //暗宝石绿
			DarkViolet,  //暗紫色
			DeepPink,  //深粉色
			DeepSkyBlue,  //深天蓝色
			DimGray,  //暗灰色
			DoderBlue,  //道奇蓝
			DodgerBlue,  //闪蓝色
			EmeraldGreen,  //艳绿色
			FireBrick,  //火砖色
			FloralWhite,  //花白色
			ForestGreen,  //森林绿
			Fuchsia,  //紫红色
			Gainsboro,  //淡灰色
			GhostWhite,  //幽灵白
			Gold,  //金黄色
			Goldenrod,  //金麒麟色
			Gray,  //灰色
			Green,  //绿色
			GreenYellow,  //黄绿色
			HoneyDew,  //蜜色
			HotPink,  //艳粉色
			IndianRed,  //印第安红
			Indigo,  //靛蓝色
			Ivory,  //象牙色
			Khaki,  //卡其色
			Lavender,  //熏衣草色
			LavenderBlush,  //淡紫红
			LawnGreen,  //草绿色
			Lemonchiffon,  //柠檬绸色
			LightBlue,  //浅蓝色
			LightCoral,  //浅珊瑚色
			LightCyan,  //浅青色
			LightGoldenrodYellow,  //浅金黄色
			LightGreen,  //浅绿色
			LightGrey,  //浅灰色
			LightPink,  //浅粉色
			LightSalmon,  //浅肉色
			LightSeaGreen,  //浅海蓝色
			LightSkyBlue,  //浅天蓝色
			LightSlateGray,  //浅蓝灰色
			LightSteelBlue,  //浅钢蓝色
			LightYellow,  //浅黄色
			Lilac,  //浅紫色
			Lime,  //酸橙色
			LimeGreen,  //橙绿色
			Linen,  //亚麻色
			Magenta,  //洋红色
			Maroon,  //栗色
			Mauve,  //紫红
			MediumAquamarine,  //间绿色
			MediumBlue,  //间蓝色
			MediumOrchid,  //间紫色
			MediumPurple,  //间紫色
			MediumSeaGreen,  //间海蓝色
			MediumSlateBlue,  //间暗蓝色
			MediumSpringGreen,  //间春绿色
			MediumTurquoise,  //间绿宝石色
			MediumVioletRed,  //间紫罗兰色
			MidNightBlue,  //中灰蓝色
			Mintcream,  //薄荷色
			//MistyGray,  //雾灰色
			MistyRose,  //浅玫瑰色
			Moccasin,  //鹿皮色
			MossGreen, //苔绿色
			NavajoWhite,  //纳瓦白
			NavyBlue,  //藏青色\深蓝色\天蓝色
			OffWhite,  //灰白
			Oldlace,  //老花色
			Olive,  //橄榄色
			Olivedrab,  //深绿褐色
			Orange,  //橙色
			OrangeRed,  //橙红色
			Orchid,  //淡紫色
			OysterWhite,  //乳白色
			OliveGreen,  //橄榄绿
			PaleGoldenrod,  //苍麒麟色
			PaleGreen,  //苍绿色
			PaleTurquoise,  //苍绿色
			PaleVioletRed,  //苍紫罗蓝色
			Pansy,  //紫罗兰色
			Papayawhip,  //番木色
			Peachpuff,  //桃色
			Peru,  //秘鲁色
			Pink,  //粉红
			Plum,  //杨李色
			PowderBlue,  //粉蓝色
			Purple,  //紫色
			Red,  //红色
			RosyBrown,  //褐玫瑰红
			RoyalBlue,  //宝蓝色
			Rubine,  //宝石红
			SaddleBrown,  //重褐色
			Salmon,  //鲜肉色
			SalmonPink,  //橙红色
			SandyBeige,  //浅褐色
			SandyBrown,  //沙褐色
			Sapphire,  //宝石蓝
			Scarlet,  //猩红色
			SeaGreen,  //海绿色
			Seashell,  //海贝色
			Sienna,  //赭色
			Silver,  //银白色
			SkyBlue,  //天蓝色
			SlateBlue,  //石蓝色
			SlateGray,  //灰石色
			SmokyGray,  //烟灰色
			Snow,  //雪白色
			SpringGreen,  //春绿色
			SteelBlue,  //钢蓝色
			Stone,  //石色
			SnowyWhite,  //雪白色
			Tan,  //茶色
			Teal,  //水鸭色
			Thistle,  //蓟色
			Tomato,  //番茄色
			Turquoise,  //青绿色
			TurquoiseBlue,  //翠蓝色
			Violet,  //紫色
			Wheat,  //浅黄色
			White,  //白色
			SmokeWhite,  //烟白色
			WineRed,  //葡萄酒红
			Yellow,  //黄色
			YellowGreen  //黄绿色
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
