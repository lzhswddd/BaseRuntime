#define LZH_EXPORTS
#include "tools.h"
#include <iostream>
#include <fstream>
using namespace lzh;

/****************************************************************************
其他工具
*****************************************************************************/
std::string lzh::getFileName(std::string filename)
{
	auto idx = filename.rfind('\\');
	if (idx != std::string::npos)
		return filename.substr(filename.rfind('\\') + 1);
	idx = filename.rfind('/');
	if (idx != std::string::npos)
		return filename.substr(filename.rfind('/') + 1);
	return "";
}
std::string lzh::getFolder(std::string filename)
{
	auto idx1 = filename.rfind('\\');
	auto idx2 = filename.rfind('/');
	if (idx1 != std::string::npos && idx2 != std::string::npos) {
		if (idx1 > idx2)
			return filename.substr(0, filename.rfind('\\'));
		else
			return filename.substr(0, filename.rfind('/'));
	}
	else if (idx1 != std::string::npos)
		return filename.substr(0, filename.rfind('\\'));
	else if (idx2 != std::string::npos)
		return filename.substr(0, filename.rfind('/'));
	else return "";
}
std::string lzh::getLastFolder(std::string filename)
{
	if (filename.empty())return "";
	if (isFile(filename))filename = getFolder(filename);
	if (filename.empty())return ""; 
	auto idx = filename.rfind('\\');
	if (idx != std::string::npos)
		filename = filename.substr(idx + 1, filename.size() - idx - 1);
	idx = filename.rfind('/'); 
	if (idx != std::string::npos)
		filename = filename.substr(idx + 1, filename.size() - idx - 1);
	return filename;
}
std::string lzh::getFileType(std::string filename)
{
	return filename.substr(filename.rfind('.') + 1);
}
std::string lzh::ReplaceFileSuffix(std::string filename, std::string suffix)
{
	if (!isFile(filename))return filename;
	auto idx = filename.rfind('.');
	if (idx != std::string::npos)
		filename = filename.substr(0, idx + 1) + suffix;
	return filename;
}
bool lzh::isImageFile(std::string filename)
{
	StringList imageType = {
		"bmp", "BMP",
		"jpg", "JPG",
		"jpeg", "JPEG",
		"png", "PNG", 
		"tiff", "TIFF"
	};
	return (std::find(imageType.begin(), imageType.end(), getFileType(filename)) != imageType.end());
}
StringList lzh::getImageFiles(std::string folder_path)
{
	StringList files;
	getFiles(folder_path, files);
	return getImageFiles(files);
}
StringList lzh::getImageFiles(const StringList & files)
{
	StringList ImageFiles;
	for (const std::string &str : files) 
		if (isImageFile(str)) 
			ImageFiles.push_back(str);
	return ImageFiles;
}
void lzh::pause()
{
	fprintf(stderr, "waitting press enter key...\n");
	while (getchar() != '\n');
}
int32 lzh::console(const std::string& str)
{
	return system(str.c_str());
}
void lzh::binaryPrint(int32 value, int32 space)
{
	vec_8b p;
	while (value)
	{
		p.push_back((value & 0x01));
		value >>= 1;
	}if (p.empty()) { printf("0\n"); return; }
	reverse(p.begin(), p.end());
	int32 count = space == 0 ? 0 : p.size() % space == 0 ? space : p.size() % space;
	int32 cnt = 1;
	for (uint8 v : p) {
		printf("%u", v);
		if (space > 0) {
			if (cnt / count <= 1 ? cnt % count == 0 : (cnt - count) % space == 0) {
				printf(" ");
			}
			cnt += 1;
		}
	}
	printf("\n");
}
mat_t lzh::saveDiv(mat_t div, mat_t v, mat_t def)
{
	return isEqual(v, 0) ? def : div / v;
}
void lzh::check(int32 w, int32 h, int32 c)
{
	if (w <= 0 || h <= 0 || c <= 0)
		THROW_INFO(ERR_INFO_VALUE);
}
FILE * lzh::createFile(std::string filename, const char * mode)
{
	return fopen(filename.c_str(), mode);
}
int32 lzh::releaseFile(FILE * file)
{
	return fclose(file);
}
std::string lzh::removeAt(const std::string & str, char ch)
{
	std::string s = str;
	for (std::string::iterator it = s.begin(); it != s.end(); it++) {
		if (*it == ch) {
			s.erase(it);
			it--;
		}
	}
	return str;
}
std::string lzh::_STR(const char * str)
{
	return std::string(str);
}
std::vector<std::string> lzh::strSplit(const std::string& str, int8 ch)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(str);
#endif //LZH_DEBUG
	size_t idx = 0;
	size_t offset = 0;
	std::vector<std::string> spl;
	while (true) {
		offset = str.find(ch, idx);
		if (offset == std::string::npos)break;
		spl.push_back(str.substr(idx, offset - idx));
		idx = offset + 1;
	}
	std::string v = str.substr(idx);
	if (v != "")
		spl.push_back(v);
	return spl;
}
std::vector<std::string> lzh::getFiles(std::string path)
{
	std::vector<std::string> files;
	getFiles(path, files);
	return files;
}
bool lzh::isNumber(std::string str)
{
	if (str.empty())return false;
	for (int i = 0; i < (int)str.length(); i++) {
		char ch = str[i];
		if (!(ch == '.' || ch == '-' || ch == '+' || ch == 'e' || (str[i] >= '0' && str[i] <= '9')))
			return false;
	}
	return true;
}

Array lzh::FromBitmap(int height, int width, int stride, void * data, int type, bool isCopy)
{
	Array dst;
	MatStep step(width, stride / width);
	if (isCopy) {
		dst.create(step[0], height, step[1], type);
		if (step[2] == step)
			memcpy(dst.data, data, dst.memsize());
		else {
			Vector vec(height*step[2], data, type);
			size_t len = calmemsize(type)*step;
			for (int i = 0; i < height; i++) {
				memcpy(dst.data.add(step * i), vec.add(step[2] * i), len);
			}
		}
		dst.rows = height;
		dst.cols = width;
	}
	else {
		dst = Array(width, height, step[1], step, data, type);
	}
	return dst;
}

void lzh::saveString(FILE * file, const std::string & str)
{
	if (!file)return;
	size_t size = str.size();
	fwrite(&size, sizeof(size_t), 1, file);
	fwrite(str.c_str(), sizeof(char)*size, 1, file);
}

void lzh::readString(FILE * file, std::string & str)
{
	if (!file)return;
	size_t size = 0;
	fread(&size, sizeof(size_t), 1, file);
	if (size == 0)return;
	str.resize(size);
	fread(&str[0], sizeof(char)*size, 1, file);
}

const Array lzh::read(std::string filename)
{
	if (filename.empty())return Array();
	std::string suffix = getFileType(filename);
	if (suffix == "csv")return readcsv(filename);
	else if (suffix == "txt")return readtxt(filename);
	else return readbin(filename);
}
void lzh::save(std::string filename, IArray mat)
{
	if (filename.empty())return;
	std::string suffix = getFileType(filename);
	if (suffix == "csv")savecsv(filename, mat);
	else if (suffix == "txt")return savetxt(filename, mat);
	else return savebin(filename, mat);
}
void lzh::save(std::string filename, IMat mat)
{
	if (filename.empty())return;
	std::string suffix = getFileType(filename);
	if (suffix == "csv")savecsv(filename, mat.toArray());
	else if (suffix == "txt")return savetxt(filename, mat.toArray());
	else return savebin(filename, mat.toArray());
}
const Array lzh::readcsv(std::string filename)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(filename);
#endif //LZH_DEBUG
	std::ifstream in(filename);
	if (!in.is_open())
		THROW_INFO(ERR_INFO_FILE);
	std::string str;
	std::getline(in, str);
	std::vector<int32> param = str2Data<int32>(strSplit(str, ','));
	Array mat(param[0], param[1], param[2], param[3], (ColorGamut)param[4]);
	FOR_RANGE(i, 0, mat.rows) {
		std::getline(in, str);
		std::vector<mat_t> data = str2Data<mat_t>(strSplit(str, ','));
		FOR_RANGE(j, 0, mat.cols) {
			FOR_RANGE(k, 0, mat.channels()) {
				mat.set(i, j, k, data[j*mat.channels() + k]);
			}
		}
	}
	in.close();
	return mat;
}
void lzh::savecsv(std::string filename, IArray mat)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(filename);
#endif //LZH_DEBUG
	std::ofstream out(filename);
	if (!out.is_open())
		THROW_INFO(ERR_INFO_FILE);
	out << mat.rows << "," << mat.cols << "," << mat.channels() << "," << mat.depth() << "," << mat.colorGamut() << std::endl;
	FOR_RANGE(i, 0, mat.rows) {
		FOR_RANGE(j, 0, mat.cols) {
			FOR_RANGE(k, 0, mat.channels()) {
				out << std::to_string(mat(i, j, k));
				if (j != mat.cols - 1)
					out << ",";
			}
		}
		out << std::endl;
	}
	out.close();
}
const Array lzh::readbin(std::string filename)
{
	Array mat;
	FILE* in = fopen(filename.c_str(), "rb");
	if (in) {
		int32 param[5] = { 0 };
		fread(param, sizeof(int32) * 5, 1, in);
		mat.create(param[0], param[1], param[2], param[3], (ColorGamut)param[4]);
		fread(mat.data, mat.memsize(), 1, in);
		fclose(in);
	}
	return mat;
}
void lzh::savebin(std::string filename, IArray mat)
{
	FILE* out = fopen(filename.c_str(), "wb");
	if (out) {
		int32 param[5] = { mat.cols, mat.rows, mat.channels(), mat.depth(), mat.colorGamut() };
		fwrite(param, sizeof(int32) * 5, 1, out);
		fwrite(mat.data, mat.memsize(), 1, out);
		fclose(out);
	}
}
const Array lzh::readtxt(std::string filename)
{
	return readcsv(filename);
}
void lzh::savetxt(std::string filename, IArray mat)
{
	savecsv(filename, mat);
}

void lzh::read(std::string filename, vec_array & vec)
{
	if (filename.empty())return;
	std::string suffix = getFileType(filename);
	if (suffix == "csv") readcsv(filename, vec);
	else if (suffix == "txt") readtxt(filename, vec);
	else readbin(filename, vec);
}
void lzh::save(std::string filename, const vec_array & mat)
{
	if (filename.empty())return;
	std::string suffix = getFileType(filename);
	if (suffix == "csv") savecsv(filename, mat);
	else if (suffix == "txt") savetxt(filename, mat);
	else savebin(filename, mat);
}

void lzh::readcsv(std::string filename, vec_array& vec)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(filename);
#endif //LZH_DEBUG
	std::ifstream in(filename);
	if (!in.is_open())
		THROW_INFO(ERR_INFO_FILE);
	std::string str;
	std::getline(in, str);
	str = removeAt(str, ',');
	int32 len = std::stoi(str);
	vec_array vec_(len);
	for (int channel = 0; channel < len; channel++) {
		std::getline(in, str);
		std::vector<int32> param = str2Data<int32>(strSplit(str, ','));
		Array mat(param[0], param[1], param[2], param[3], (ColorGamut)param[4]);
		FOR_RANGE(i, 0, mat.rows) {
			std::getline(in, str);
			std::vector<mat_t> data = str2Data<mat_t>(strSplit(str, ','));
			FOR_RANGE(j, 0, mat.cols) {
				FOR_RANGE(k, 0, mat.channels()) {
					mat.set(i, j, k, data[j*mat.channels() + k]);
				}
			}
		}
		vec_[channel] = mat;
	}
	in.close();
	vec_.swap(vec);
}
void lzh::savecsv(std::string filename, const vec_array& vec)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(filename);
#endif //LZH_DEBUG
	std::ofstream out(filename);
	if (!out.is_open())
		THROW_INFO(ERR_INFO_FILE);
	if (!vec.empty()) {
		out << vec.size() << std::endl;
		for (const Array &mat : vec) {
			out << mat.rows << "," << mat.cols << "," << mat.channels() << "," << mat.depth() << "," << mat.colorGamut() << std::endl;
			FOR_RANGE(i, 0, mat.rows) {
				FOR_RANGE(j, 0, mat.cols) {
					out << std::to_string(mat(i, j));
					if (j != mat.cols - 1)
						out << ",";
				}
				out << std::endl;
			}
		}
	}
	out.close();
}
void lzh::readbin(std::string filename, vec_array& mat)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(filename);
#endif //LZH_DEBUG
	FILE* file = fopen(filename.c_str(), "rb");
	if (!file)
		THROW_INFO(ERR_INFO_FILE);
	int32 len;
	fread(&len, sizeof(int32), 1, file);
	vec_array vec(len);
	for (int channel = 0; channel < len; channel++) {
		int32 param[5] = { 0 };
		fread(param, sizeof(int32) * 5, 1, file);
		Array m(param[0], param[1], param[2], param[3], (ColorGamut)param[4]);
		fread(m.data(), m.memsize(), 1, file);
		vec[channel] = m;
	}
	fclose(file);
	vec.swap(mat);
}
void lzh::savebin(std::string filename, const vec_array & mat)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(filename);
#endif //LZH_DEBUG
	FILE* file = fopen(filename.c_str(), "wb");
	if (!file)
		THROW_INFO(ERR_INFO_FILE);
	int32 len = (int32)mat.size();
	fwrite(&len, sizeof(int32), 1, file);
	for (int channel = 0; channel < len; channel++) {
		int32 param[5] = { mat[channel].rows, mat[channel].cols, mat[channel].channels(), mat[channel].depth(), mat[channel].colorGamut() };
		fwrite(param, sizeof(int32) * 5, 1, file);
		fwrite(mat[channel].data(), mat[channel].memsize(), 1, file);
	}
	fclose(file);
}
void lzh::readtxt(std::string filename, vec_array& vec)
{
	readcsv(filename, vec);
}
void lzh::savetxt(std::string filename, const vec_array & mat)
{
	savecsv(filename, mat);
}
vec_mat lzh::tranVec(const vec_array &vec)
{
	vec_mat mat(vec.size());
	FOR_RANGE(i, 0, vec.size())
		mat[i] = vec[i];
	return mat;
}
vec_array lzh::tranVec(const vec_mat &vec)
{
	vec_array mat(vec.size());
	FOR_RANGE(i, 0, vec.size())
		mat[i] = vec[i].toArray();
	return mat;
}
Vector lzh::toVector(const Mat & mat)
{
	Vector ptrmat; 
	ptrmat.set(mat.data(), mat.len(), MAT_T, mat.memCount()); 
	ptrmat.addref();
	return ptrmat;
}
std::string lzh::Enum2String(PrintType type)
{
	std::string str;
	switch (type)
	{
	case lzh::MAT_FIXED:
		str = "FIXED INFO:以小数形式格式化输出";
		break;
	case lzh::MAT_SCIENTIFIC:
		str = "SCIENTIFIC INFO:以科学记数法形式格式化输出";
		break;
	default:
		break;
	}
	return str;
}
std::string lzh::Enum2String(BorderType type)
{
	std::string str;
	switch (type)
	{
	case lzh::BORDER_CONSTANT:
		str = "BORDER_CONSTANT //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`";
		break;
	case lzh::BORDER_REPLICATE:
		str = "BORDER_CONSTANT INFO://!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`";
		break;
	case lzh::BORDER_REFLECT:
		str = "BORDER_REPLICATE INFO://!< `aaaaaa|abcdefgh|hhhhhhh`";
		break;
	case lzh::BORDER_WRAP:
		str = "BORDER_REFLECT INFO://!< `fedcba|abcdefgh|hgfedcb`";
		break;
	case lzh::BORDER_REFLECT_101:
		str = "BORDER_REFLECT_101 INFO://!< `gfedcb|abcdefgh|gfedcba`";
		break;
		//case lzh::BORDER_TRANSPARENT:
		//	str = "BORDER_TRANSPARENT INFO://!< `uvwxyz|abcdefgh|ijklmno`";
		//	break;
		//case lzh::BORDER_ISOLATED:
		//	str = "BORDER_ISOLATED INFO://!< do not look outside of ROI";
		//	break;
	default:
		break;
	}
	return str;
}
std::string lzh::Enum2String(MatErrorInfo type)
{
	return errinfo[type];
}
std::string lzh::Enum2String(EQUATION type)
{
	std::string str;
	switch (type)
	{
	case lzh::SPECIAL_SOLUTION:
		str = "SPECIAL_SOLUTION INFO:方程有特解";
		break;
	case lzh::GENERAL_SOLUTION:
		str = "GENERAL_SOLUTION INFO:方程有通解";
		break;
	case lzh::NO_SOLUTION:
		str = "NO_SOLUTION INFO:方程无解";
		break;
	default:
		break;
	}
	return str;
}
std::string lzh::Enum2String(ORDER type)
{
	std::string str;
	switch (type)
	{
	case lzh::MIN_TO_MAX:
		str = "MIN_TO_MAX INFO:从小到大";
		break;
	case lzh::MAX_TO_MIN:
		str = "MAX_TO_MIN INFO:从大到小";
		break;
	default:
		break;
	}
	return str;
}
std::string lzh::Enum2String(RANK type)
{
	std::string str;
	switch (type)
	{
	case lzh::ROW:
		str = "ROW INFO:行方向";
		break;
	case lzh::COL:
		str = "COL INFO:列方向";
		break;
	case lzh::CHANNEL:
		str = "CHANNEL INFO:通道方向";
		break;
	default:
		break;
	}
	return str;
}
std::string lzh::Enum2String(Dire type)
{
	std::string str;
	switch (type)
	{
	case lzh::LEFT:
		str = "LEFT INFO:左";
		break;
	case lzh::RIGHT:
		str = "RIGHT INFO:右";
		break;
	default:
		break;
	}
	return str;
}
std::string lzh::Enum2String(InterpolationMothed type)
{
	std::string str;
	switch (type)
	{
	case lzh::INTER_SAMPLE:
		str = "INTER_SAMPLE INFO:等间隔采样";
		break;
	case lzh::INTER_MEAN:
		str = "INTER_MEAN INFO:局部均值";
		break;
	default:
		break;
	}
	return str;
}
std::string lzh::Enum2String(RotateAngle type)
{
	std::string str;
	switch (type)
	{
	case lzh::ROTATE_90_ANGLE:
		str = "ROTATE_90_ANGLE INFO:顺时针旋转90度";
		break;
	case lzh::ROTATE_180_ANGLE:
		str = "ROTATE_180_ANGLE INFO:顺时针旋转180度";
		break;
	case lzh::ROTATE_270_ANGLE:
		str = "ROTATE_270_ANGLE INFO:顺时针旋转270度";
		break;
	default:
		break;
	}
	return str;
}
std::string lzh::Enum2String(BSpline::BSplineType type)
{
	std::string str;
	switch (type)
	{
	case BSpline::UNIFORM:
		str = "UNIFORM INFO:均匀B样条曲线90度"; break;
	case BSpline::QUASI_UNIFORM:
		str = "QUASI_UNIFORM INFO:准均匀B样条曲线90度"; break;
	default:
		break;
	}
	return std::string();
}

std::string lzh::Enum2String(ColorGamut type)
{
	switch (type)
	{
	case lzh::ColorGamutCustom:return "ColorGamutCustom";
	case lzh::ColorGamutGray:return "ColorGamutGray";
	case lzh::ColorGamutRGB:return "ColorGamutRGB";
	case lzh::ColorGamutBGR:return "ColorGamutBGR";
	case lzh::ColorGamutRGBA:return "ColorGamutRGBA";
	case lzh::ColorGamutBGRA:return "ColorGamutBGRA";
	case lzh::ColorGamutCMYK:return "ColorGamutCMYK";
	case lzh::ColorGamutHSV:return "ColorGamutHSV";
	case lzh::ColorGamutHSL:return "ColorGamutHSL";
	case lzh::ColorGamutXYZ:return "ColorGamutXYZ";
	case lzh::ColorGamutYXY:return "ColorGamutYXY";
	case lzh::ColorGamutLab:return "ColorGamutLab";
	case lzh::ColorGamutLuv:return "ColorGamutLuv";
	case lzh::ColorGamutMean:return "ColorGamutMean";
	case lzh::ColorGamutMaxMin:return "ColorGamutMaxMin";
	case lzh::ColorGamutR:return "ColorGamutR";
	case lzh::ColorGamutB:return "ColorGamutB";
	case lzh::ColorGamutG:return "ColorGamutG";
	default:return "";
	}
	//switch (type)
	//{
	//case lzh::ColorGamutCustom:return "ColorGamutCustom";
	//case lzh::ColorGamutGray:return "ColorGamutGray";
	//case lzh::ColorGamutRGB:return "ColorGamutRGB";
	//case lzh::ColorGamutBGR:return "ColorGamutBGR";
	//case lzh::ColorGamutRGBA:return "ColorGamutRGBA";
	//case lzh::ColorGamutBGRA:return "ColorGamutBGRA";
	//case lzh::ColorGamutHSV:return "ColorGamutHSV";
	//case lzh::ColorGamutXYZ:return "ColorGamutXYZ";
	//case lzh::ColorGamutLab:return "ColorGamutLab";
	//case lzh::ColorGamutLab:return "ColorGamutLab";
	//case lzh::ColorGamutMean:return "ColorGamutMean";
	//case lzh::ColorGamutR:return "ColorGamutR";
	//case lzh::ColorGamutB:return "ColorGamutB";
	//case lzh::ColorGamutG:return "ColorGamutG";
	//default:return "";
	//}
}

ColorGamut lzh::String2ColorGamut(const char * str)
{
	if (strcmp(str, "ColorGamutCustom") == 0) return ColorGamutCustom;
	else if (strcmp(str, "ColorGamutGray") == 0) return ColorGamutGray;
	else if (strcmp(str, "ColorGamutRGB") == 0) return ColorGamutRGB;
	else if (strcmp(str, "ColorGamutBGR") == 0) return ColorGamutBGR;
	else if (strcmp(str, "ColorGamutRGBA") == 0) return ColorGamutRGBA;
	else if (strcmp(str, "ColorGamutBGRA") == 0) return ColorGamutBGRA;
	else if (strcmp(str, "ColorGamutCMYK") == 0) return ColorGamutCMYK;
	else if (strcmp(str, "ColorGamutHSV") == 0) return ColorGamutHSV;
	else if (strcmp(str, "ColorGamutHSL") == 0) return ColorGamutHSL;
	else if (strcmp(str, "ColorGamutXYZ") == 0) return ColorGamutXYZ;
	else if (strcmp(str, "ColorGamutYXY") == 0) return ColorGamutYXY;
	else if (strcmp(str, "ColorGamutLab") == 0) return ColorGamutLab;
	else if (strcmp(str, "ColorGamutLuv") == 0) return ColorGamutLuv;
	else if (strcmp(str, "ColorGamutMean") == 0) return ColorGamutMean;
	else if (strcmp(str, "ColorGamutMaxMin") == 0) return ColorGamutMaxMin;
	else if (strcmp(str, "ColorGamutR") == 0) return ColorGamutR;
	else if (strcmp(str, "ColorGamutB") == 0) return ColorGamutB;
	else if (strcmp(str, "ColorGamutG") == 0) return ColorGamutG;
	return ColorGamutCustom;
}

BorderType lzh::String2BorderType(const char * str)
{
	if (strcmp(str, "BORDER_CONSTANT") == 0) return BORDER_CONSTANT;
	else if (strcmp(str, "BORDER_REPLICATE") == 0) return BORDER_REPLICATE;
	else if (strcmp(str, "BORDER_REFLECT") == 0) return BORDER_REFLECT;
	else if (strcmp(str, "BORDER_WRAP") == 0) return BORDER_WRAP;
	else if (strcmp(str, "BORDER_REFLECT_101") == 0) return BORDER_REFLECT_101;
	else if (strcmp(str, "BORDER_DEFAULT") == 0) return BORDER_DEFAULT;
	else return BORDER_DEFAULT;
}

int32 lzh::String2Type(const char * str)
{
	for (int32 type : Range(TP_64D, true))
		if (strcmp(str, TypeString[type++]) == 0) return type;
	return NONE;
}

std::string lzh::Enum2String(Color::ColorTab type)
{
	switch (type)
	{
	case lzh::Color::Amber:return "琥珀色";
		//case lzh::Color::AntiqueViolet:return Color(?, ?, ?, index);
	case lzh::Color::AntiqueWhite:return "古董白";
	case lzh::Color::Aqua:return "浅绿色";
	case lzh::Color::AquaMarine:return "海蓝宝石色";
	case lzh::Color::AquaMarineBlue:return "蓝绿色";
	case lzh::Color::Azure:return "天蓝色";
	case lzh::Color::AliceBlue:return "爱丽丝蓝";
	case lzh::Color::BabyPink:return "浅粉红色";
	case lzh::Color::Beige:return "米色";
	case lzh::Color::Bisque:return "橘黄色";
	case lzh::Color::Black:return "黑色";
	case lzh::Color::BlancheDalmond:return "白杏色";
	case lzh::Color::Blue:return "蓝色";
	case lzh::Color::BlueViolet:return "紫罗兰色";
	case lzh::Color::Brown:return "棕色";
	case lzh::Color::BurlyWood:return "实木色";
	case lzh::Color::CobaltBlue:return "钴蓝色";
	case lzh::Color::CadetBlue:return "军蓝色";
	case lzh::Color::Camel:return "驼色";
	case lzh::Color::CharcoalGray:return "炭灰色";
	case lzh::Color::Chartreuse:return "黄绿色";
	case lzh::Color::Chocolate:return "巧克力色";
	case lzh::Color::CobaltVlue:return "艳蓝色";
	case lzh::Color::Coral:return "珊瑚色";
	case lzh::Color::CornflowerBlue:return "菊蓝色";
	case lzh::Color::Cornsilk:return "米绸色";
	case lzh::Color::Crimson:return "暗深红色";
	case lzh::Color::Cyan:return "青色";
	case lzh::Color::DarkBlue:return "暗蓝色";
	case lzh::Color::DarkCyan:return "暗青色";
	case lzh::Color::DarkGoldenrod:return "暗金黄色";
	case lzh::Color::DarkGray:return "暗灰色";
	case lzh::Color::DarkGreen:return "暗绿色";
	case lzh::Color::DarkKhaki:return "暗卡其色";
	case lzh::Color::DarkMagenta:return "暗洋红色";
	case lzh::Color::DarkOliveGreen:return "暗橄榄绿色";
	case lzh::Color::DarkOrange:return "暗桔色";
	case lzh::Color::DarkOrchid:return "暗紫色";
	case lzh::Color::DarkRed:return "暗红色";
	case lzh::Color::DarkSalmon:return "暗肉色";
	case lzh::Color::DarkSeaGreen:return "暗海蓝色";
	case lzh::Color::DarkSlateBlue:return "暗灰蓝色";
	case lzh::Color::DarkSlateGray:return "墨绿色";
	case lzh::Color::DarkTurquoise:return "暗宝石绿";
	case lzh::Color::DarkViolet:return "暗紫色";
	case lzh::Color::DeepPink:return "深粉色";
	case lzh::Color::DeepSkyBlue:return "深天蓝色";
	case lzh::Color::DimGray:return "暗灰色";
	case lzh::Color::DoderBlue:return "道奇蓝";
	case lzh::Color::DodgerBlue:return "闪蓝色";
	case lzh::Color::EmeraldGreen:return "艳绿色";
	case lzh::Color::FireBrick:return "火砖色";
	case lzh::Color::FloralWhite:return "花白色";
	case lzh::Color::ForestGreen:return "森林绿";
	case lzh::Color::Fuchsia:return "紫红色";
	case lzh::Color::Gainsboro:return "淡灰色";
	case lzh::Color::GhostWhite:return "幽灵白";
	case lzh::Color::Gold:return "金黄色";
	case lzh::Color::Goldenrod:return "金麒麟色";
	case lzh::Color::Gray:return "灰色";
	case lzh::Color::Green:return "绿色";
	case lzh::Color::GreenYellow:return "黄绿色";
	case lzh::Color::HoneyDew:return "蜜色";
	case lzh::Color::HotPink:return "艳粉色";
	case lzh::Color::IndianRed:return "印第安红色";
	case lzh::Color::Indigo:return "靛蓝色";
	case lzh::Color::Ivory:return "象牙色";
	case lzh::Color::Khaki:return "卡其色";
	case lzh::Color::Lavender:return "熏衣草色";
	case lzh::Color::LavenderBlush:return "淡紫红色";
	case lzh::Color::LawnGreen:return "草绿色";
	case lzh::Color::Lemonchiffon:return "柠檬绸色";
	case lzh::Color::LightBlue:return "浅蓝色";
	case lzh::Color::LightCoral:return "浅珊瑚色";
	case lzh::Color::LightCyan:return "浅青色";
	case lzh::Color::LightGoldenrodYellow:return "浅金黄色";
	case lzh::Color::LightGreen:return "浅绿色";
	case lzh::Color::LightGrey:return "浅灰色";
	case lzh::Color::LightPink:return "浅粉色";
	case lzh::Color::LightSalmon:return "浅肉色";
	case lzh::Color::LightSeaGreen:return "浅海蓝色";
	case lzh::Color::LightSkyBlue:return "浅天蓝色";
	case lzh::Color::LightSlateGray:return "浅蓝灰色";
	case lzh::Color::LightSteelBlue:return "浅钢蓝色";
	case lzh::Color::LightYellow:return "浅黄色";
	case lzh::Color::Lilac:return "浅紫色";
	case lzh::Color::Lime:return "酸橙色";
	case lzh::Color::LimeGreen:return "橙绿色";
	case lzh::Color::Linen:return "亚麻色";
	case lzh::Color::Magenta:return "洋红色";
	case lzh::Color::Maroon:return "栗色";
	case lzh::Color::Mauve:return "紫红";
	case lzh::Color::MediumAquamarine:return "间绿色";
	case lzh::Color::MediumBlue:return "间蓝色";
	case lzh::Color::MediumOrchid:return "间紫色";
	case lzh::Color::MediumPurple:return "间紫色";
	case lzh::Color::MediumSeaGreen:return "间海蓝色";
	case lzh::Color::MediumSlateBlue:return "间暗蓝色";
	case lzh::Color::MediumSpringGreen:return "间春绿色";
	case lzh::Color::MediumTurquoise:return "间绿宝石色";
	case lzh::Color::MediumVioletRed:return "间紫罗兰色";
	case lzh::Color::MidNightBlue:return "中灰蓝色";
	case lzh::Color::Mintcream:return "薄荷色";
	//case lzh::Color::MistyGray:return "雾灰色";
	case lzh::Color::MistyRose:return "浅玫瑰色";
	case lzh::Color::Moccasin:return "鹿皮色";
	case lzh::Color::MossGreen:return "苔绿色";
	case lzh::Color::NavajoWhite:return "纳瓦白";
	case lzh::Color::NavyBlue:return "藏青色";
	case lzh::Color::OffWhite:return "灰白";
	case lzh::Color::Oldlace:return "老花色";
	case lzh::Color::Olive:return "橄榄色";
	case lzh::Color::Olivedrab:return "深绿褐色";
	case lzh::Color::Orange:return "橙色";
	case lzh::Color::OrangeRed:return "橙红色";
	case lzh::Color::Orchid:return "淡紫色";
	case lzh::Color::OysterWhite:return "乳白色";
	case lzh::Color::OliveGreen:return "橄榄绿";
	case lzh::Color::PaleGoldenrod:return "苍麒麟色";
	case lzh::Color::PaleGreen:return "苍绿色";
	case lzh::Color::PaleTurquoise:return "苍绿色";
	case lzh::Color::PaleVioletRed:return "苍紫罗蓝色";
	case lzh::Color::Pansy:return "紫罗兰色";
	case lzh::Color::Papayawhip:return "番木色";
	case lzh::Color::Peachpuff:return "桃色";
	case lzh::Color::Peru:return "秘鲁色";
	case lzh::Color::Pink:return "粉红";
	case lzh::Color::Plum:return "杨李色";
	case lzh::Color::PowderBlue:return "粉蓝色";
	case lzh::Color::Purple:return "紫色";
	case lzh::Color::Red:return "红色";
	case lzh::Color::RosyBrown:return "褐玫瑰红";
	case lzh::Color::RoyalBlue:return "宝蓝色";
	case lzh::Color::Rubine:return "宝石红";
	case lzh::Color::SaddleBrown:return "重褐色";
	case lzh::Color::Salmon:return "鲜肉色";
	case lzh::Color::SalmonPink:return "橙红色";
	case lzh::Color::SandyBeige:return "浅褐色";
	case lzh::Color::SandyBrown:return "沙褐色";
	case lzh::Color::Sapphire:return "宝石蓝";
	case lzh::Color::Scarlet:return "猩红色";
	case lzh::Color::SeaGreen:return "海绿色";
	case lzh::Color::Seashell:return "海贝色";
	case lzh::Color::Sienna:return "赭色";
	case lzh::Color::Silver:return "银白色";
	case lzh::Color::SkyBlue:return "天蓝色";
	case lzh::Color::SlateBlue:return "石蓝色";
	case lzh::Color::SlateGray:return "灰石色";
	case lzh::Color::SmokyGray:return "烟灰色";
	case lzh::Color::Snow:return "雪白色";
	case lzh::Color::SpringGreen:return "春绿色";
	case lzh::Color::SteelBlue:return "钢蓝色";
	case lzh::Color::Stone:return "石色";
	case lzh::Color::SnowyWhite:return "雪白色";
	case lzh::Color::Tan:return "茶色";
	case lzh::Color::Teal:return "水鸭色";
	case lzh::Color::Thistle:return "蓟色";
	case lzh::Color::Tomato:return "番茄色";
	case lzh::Color::Turquoise:return "青绿色";
	case lzh::Color::TurquoiseBlue:return "翠蓝色";
	case lzh::Color::Violet:return "紫色";
	case lzh::Color::Wheat:return "浅黄色";
	case lzh::Color::White:return "白色";
	case lzh::Color::SmokeWhite:return "烟白色";
	case lzh::Color::WineRed:return "葡萄酒红";
	case lzh::Color::Yellow:return "黄色";
	case lzh::Color::YellowGreen:return "黄绿色";
	default:return "";
	}

}

std::string lzh::Type2String(int32 type)
{
	switch (type)
	{
	case lzh::TP_8S:return "signed char";
	case lzh::TP_8U:return "unsigned char";
	case lzh::TP_16S:return "signed short";
	case lzh::TP_16U:return "unsigned short";
	case lzh::TP_32S:return "signed int";
	case lzh::TP_32U:return "unsigned int";
	case lzh::TP_64S:return "signed long long int";
	case lzh::TP_64U:return "unsigned long long int";
	case lzh::TP_32F:return "float";
	case lzh::TP_64F:return "double";
	case lzh::TP_64D:return "long double";
	default:return "other type";
	}
}


/****************************************************************************
计时器
*****************************************************************************/
#if defined(__linux__)
#include <sys/time.h>
#include <unistd.h>
Timer::Timer()
{
	start = (struct timeval*)malloc(sizeof(struct timeval));
	end = (struct timeval*)malloc(sizeof(struct timeval));
}
Timer::~Timer()
{
	FREE_PTR(start);
	FREE_PTR(end);
}
void Timer::Start()
{
	gettimeofday((struct timeval*)start, NULL);
}
mat_t Timer::End()
{
	gettimeofday((struct timeval*)end, NULL);
	return _T(((struct timeval*)end->tv_sec - (struct timeval*)start->tv_sec) * 1000.0 + (struct timeval*)end->tv_usec - (struct timeval*)start->tv_usec);
}
TimeData lzh::Timer::EndTime()
{
	mat_t v = End();
	return TimeData();
}
static struct timeval t1, t2;
void lzh::StartCounter()
{
	gettimeofday(&t1, NULL);
}
mat_t lzh::EndCounter()
{
	gettimeofday(&t2, NULL);
	return _T((t2.tv_sec - t1.tv_sec) * 1000.0 + t2.tv_usec - t1.tv_usec);
}
void lzh::Wait(uint ms)
{
	sleep(ms);
}
#elif defined(_WIN32)
#include <windows.h>  
#include <io.h>
#include <direct.h>  
Timer::Timer()
{
	start = new LARGE_INTEGER();
	end = new LARGE_INTEGER();
	fc = new LARGE_INTEGER();
	QueryPerformanceFrequency((LARGE_INTEGER*)fc);
}
Timer::~Timer()
{
	FREE_PTR(start);
	FREE_PTR(end);
	FREE_PTR(fc);
}
void Timer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)start);
}
mat_t Timer::End()
{
	QueryPerformanceCounter((LARGE_INTEGER*)end);
	return _T(((((LARGE_INTEGER*)end)->QuadPart - ((LARGE_INTEGER*)start)->QuadPart) * 1000.0) / ((LARGE_INTEGER*)fc)->QuadPart);
}
TimeData Timer::EndTime()
{
	return TimeData(End());
}
static LARGE_INTEGER cpuFreq;
static LARGE_INTEGER startTime;
static LARGE_INTEGER endTime;

void lzh::Frequency()
{
	if (cpuFreq.QuadPart == 0)
		QueryPerformanceFrequency(&cpuFreq);
}
void lzh::StartCounter()
{
	QueryPerformanceCounter(&startTime);
}
mat_t lzh::EndCounter()
{
	QueryPerformanceCounter(&endTime);
	return _T(((endTime.QuadPart - startTime.QuadPart) * 1000.0) / cpuFreq.QuadPart);
}
/**
@brief getFiles 得到路径下所有文件的路径
@param path 文件夹路径
@param files 保存path下的所有文件路径
*/
void lzh::getFiles(std::string path, std::vector<std::string>& files)
{
	//文件句柄  
	intptr_t hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
std::string lzh::show_path()
{
	int8 buffer[260];
	char* ch = _getcwd(buffer, 260);
	return std::string(buffer);
}
size_t lzh::getFileSize(std::string file_path)
{
	size_t size = 0;
	std::ifstream fin(file_path);
	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		size = fin.tellg();
		fin.close();
	}
	return size;
}
size_t lzh::getFileSize(std::ifstream & file)
{
	if (!file.is_open())return 0;
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	return size;
}
bool lzh::removeFile(std::string path)
{
	return (remove(path.c_str()) == 0);
}

bool lzh::createDirectory(std::string path)
{
	if (GetFileAttributesA(path.c_str()) != FILE_ATTRIBUTE_DIRECTORY) {
		bool flag = CreateDirectory(path.c_str(), NULL);
		return flag;
	}
	else return true;
}
std::string lzh::appendPath(const std::initializer_list<std::string>& path)
{
	if (path.size() == 0)
		return std::string();
	std::string res;
	for (const std::string & str : path)
		res += '\\' + str;
	res = res.substr(1, res.size() - 1);
	return res;
}
bool lzh::isFolder(std::string path)
{
	if (path.empty())return false;
	WIN32_FIND_DATAA FindFileData;
	FindFirstFileA(path.c_str(), &FindFileData);
	return (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}
bool lzh::isFile(std::string path)
{
	if (path.empty())return false;
	WIN32_FIND_DATAA FindFileData;
	FindFirstFileA(path.c_str(), &FindFileData);
	return !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}
bool lzh::isExists(std::string path)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(path.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	else {
		FindClose(hFind);
		return true;
	}
}
void lzh::Wait(uint32 ms)
{
	Sleep(ms);
}
void print(int colorCode, const char* info, ...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
	va_list _ArgList;
	__crt_va_start(_ArgList, info);
	_vfprintf_l(stderr, info, NULL, _ArgList);
	__crt_va_end(_ArgList);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
std::string lzh::getModuleFileName()
{
	char buffer[256];
	_getcwd(buffer, 256);
	return std::string(buffer);
}
void lzh::Command(const char * cmd)
{
	HWND hwnd = GetForegroundWindow();
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return;
	}
	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_SHOWNORMAL;
	si.dwFlags = STARTF_USESHOWWINDOW;
	//关键步骤，CreateProcess函数参数意义请查阅MSDN   
	if (CreateProcess(NULL, (char*)cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		ResumeThread(pi.hThread); //唤醒线程
		WaitForSingleObject(pi.hProcess, INFINITE);//等待进程结束，时间：无限
		CloseHandle(hWrite);
	}
}
char* lzh::WideCharToANSI(const wchar_t* szWidechar)
{
	int widecharLen = ::WideCharToMultiByte(CP_ACP, 0, szWidechar, -1, NULL, 0, NULL, NULL);
	char* pAnsi = new char[widecharLen + 1];
	memset(pAnsi, 0x00, widecharLen + 1);
	::WideCharToMultiByte(CP_ACP, 0, szWidechar, -1, pAnsi, widecharLen, NULL, NULL);

	return pAnsi;
}
wchar_t* lzh::ANSIToWideChar(const char* szWidechar)
{
	int widecharLen = ::MultiByteToWideChar(CP_ACP, 0, szWidechar, -1, NULL, 0);
	wchar_t* pAnsi = new wchar_t[widecharLen + 1];
	memset(pAnsi, 0x00, widecharLen + 1);
	::MultiByteToWideChar(CP_ACP, 0, szWidechar, -1, pAnsi, widecharLen);

	return pAnsi;
}
std::string lzh::WStr2Str(const std::wstring& str) {
	char* ptr = WideCharToANSI(str.c_str());
	std::string s = ptr;
	delete ptr;
	return s;
}
std::wstring lzh::Str2WStr(const std::string& str) {
	wchar_t* ptr = ANSIToWideChar(str.c_str());
	std::wstring s = ptr;
	delete ptr;
	return s;
}
std::vector<std::string> lzh::WStr2Str(const std::vector<std::wstring>& str)
{
	std::vector<std::string> vec(str.size());
	for (size_t i = 0; i != str.size(); i++)
		vec[i] = WStr2Str(str[i]);
	return vec;
}
std::vector<std::wstring> lzh::Str2WStr(const std::vector<std::string>& str)
{
	std::vector<std::wstring> vec(str.size());
	for (size_t i = 0; i != str.size(); i++)
		vec[i] = Str2WStr(str[i]);
	return vec;
}
std::string lzh::UTF8ToString(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}
std::string lzh::StringToUTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
#endif