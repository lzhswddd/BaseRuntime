#define LZH_EXPORTS
#include "structure.h"
#include <iostream>
using namespace lzh;
/****************************************************************************
大数类
*****************************************************************************/
LargeNumber::LargeNumber() : std::string()
{
}
LargeNumber::LargeNumber(int8 ch) : std::string()
{
	if (ch >= '0' && ch <= '9')
		push_back(ch - '0');
	else if (ch >= 0 && ch <= 9)
		push_back(ch);
	else {
		while (ch)
		{
			push_back(ch % 10);
			ch /= 10;
		}
		reverse(begin(), end());
	}
}
LargeNumber::LargeNumber(uint8 val) : std::string()
{
	if (val == 0) {
		push_back(0);
	}
	else {
		while (val)
		{
			push_back(val % 10);
			val /= 10;
		}
		reverse(begin(), end());
	}
}
LargeNumber::LargeNumber(int32 val) : std::string()
{
	if (val == 0) {
		push_back(0);
	}
	else {
		while (val)
		{
			push_back(val % 10);
			val /= 10;
		}
		reverse(begin(), end());
	}
}
LargeNumber::LargeNumber(int64 val) : std::string()
{
	if (val == 0) {
		push_back(0);
	}
	else {
		while (val)
		{
			push_back(val % 10);
			val /= 10;
		}
		reverse(begin(), end());
	}
}
LargeNumber::LargeNumber(uint32 val) : std::string()
{
	if (val == 0) {
		push_back(0);
	}
	else {
		while (val)
		{
			push_back(val % 10);
			val /= 10;
		}
		reverse(begin(), end());
	}
}
LargeNumber::LargeNumber(uint64 val) : std::string()
{
	if (val == 0) {
		push_back(0);
	}
	else {
		while (val)
		{
			push_back(val % 10);
			val /= 10;
		}
		reverse(begin(), end());
	}
}
LargeNumber::LargeNumber(const int8* str) : std::string()
{
	uint32 ind = 0;
	while (str[ind] != '\0')
		ind++;
	resize(ind);
	_FOR(i, 0, ind, 1) {
		if ((str[i]<'0' || str[i]>'9') && str[i] != '-')
			continue;
		at(i) = (str[i] == '-' ? str[i] : str[i] - '0');
	}
}
LargeNumber::LargeNumber(std::string str) : std::string()
{
	if (str.empty())return;
	resize(str.size());
	_FOR(i, 0, str.size(), 1) {
		if ((str[i]<'0' || str[i]>'9') && str[i] != '-')
			continue;
		at(i) = (str[i] == '-' ? str[i] : str[i] - '0');
	}
}
LargeNumber::LargeNumber(int32* ptr, int32 n) : std::string()
{
	if (ptr == nullptr)return;
	_FOR(i, 0, n, 1) {
		std::string str = std::to_string(ptr[i]);
		_FOR(j, 0, str.size(), 1) {
			if ((str[j]<'0' || str[j]>'9') && str[j] != '-')
				continue;
			push_back(str[j] == '-' ? str[j] : str[j] - '0');
		}
	}
}
const LargeNumber lzh::operator-(const LargeNumber& ln)
{
	LargeNumber a = ln;
	if (a[0] == '-')
		a.erase(a.begin());
	else
		a.insert(a.begin(), '-');
	return a;
}
bool lzh::operator < (const LargeNumber& a, const LargeNumber& b)
{
	if (b[0] == '-' && a[0] != '-')return false;
	else if (b[0] != '-' && a[0] == '-')return true;
	else if (b[0] == '-' && a[0] == '-') {
		if (b.size() > a.size())return false;
		else if (b.size() < a.size())return true;
		else {
			_FOR(i, 0, a.size(), 1) {
				if (b[i] < a[i])return true;
				else if (b[i] > a[i])return false;
			}
			return false;
		}
	}
	else {
		if (b.size() > a.size())return true;
		else if (b.size() < a.size())return false;
		else {
			_FOR(i, 0, a.size(), 1) {
				if (b[i] < a[i])return false;
				else if (b[i] > a[i])return true;
			}
			return false;
		}
	}
}
bool lzh::operator > (const LargeNumber& a, const LargeNumber& b)
{
	if (b[0] == '-' && a[0] != '-')return true;
	else if (b[0] != '-' && a[0] == '-')return false;
	else if (b[0] == '-' && a[0] == '-') {
		if (b.size() > a.size())return true;
		else if (b.size() < a.size())return false;
		else {
			_FOR(i, 0, a.size(), 1) {
				if (b[i] < a[i])return false;
				else if (b[i] > a[i])return true;
			}
			return false;
		}
	}
	else {
		if (b.size() > a.size())return false;
		else if (b.size() < a.size())return true;
		else {
			_FOR(i, 0, a.size(), 1) {
				if (b[i] < a[i])return true;
				else if (b[i] > a[i])return false;
			}
			return false;
		}
	}
}
bool lzh::operator <= (const LargeNumber& a, const LargeNumber& b)
{
	return !(a > b);
}
bool lzh::operator >= (const LargeNumber& a, const LargeNumber& b)
{
	return !(a < b);
}
bool lzh::operator == (const LargeNumber& a, const LargeNumber& b)
{
	if (b[0] == '-' && a[0] != '-')return false;
	else if (b[0] != '-' && a[0] == '-')return false;
	else if (b[0] == '-' && a[0] == '-') {
		if (b.size() > a.size())return false;
		else if (b.size() < a.size())return false;
		else {
			_FOR(i, 0, a.size(), 1) {
				if (b[i] < a[i])return false;
				else if (b[i] > a[i])return false;
			}
			return true;
		}
	}
	else {
		if (b.size() > a.size())return false;
		else if (b.size() < a.size())return false;
		else {
			_FOR(i, 0, a.size(), 1) {
				if (b[i] < a[i])return false;
				else if (b[i] > a[i])return false;
			}
			return true;
		}
	}
}
void lzh::operator+=(LargeNumber& a, const LargeNumber& b)
{
	a = a + b;
}
void lzh::operator-=(LargeNumber& a, const LargeNumber& b)
{
	a = a - b;
}
void lzh::operator*=(LargeNumber& a, const LargeNumber& b)
{
	a = a * b;
}
void lzh::operator/=(LargeNumber& a, const LargeNumber& b)
{
	a = a / b;
}
const LargeNumber lzh::operator +(const LargeNumber& a_, const LargeNumber& b_)
{
	LargeNumber a = a_;
	LargeNumber b = b_;
	if (a[0] == '-' && b[0] == '-') {
		LargeNumber tempa;
		LargeNumber tempb;
		a.swap(tempa);
		b.swap(tempb);
		tempa.erase(tempa.begin());
		tempb.erase(tempb.begin());
		LargeNumber temp = tempa + tempb;
		temp.insert(temp.begin(), '-');
		return temp;
	}
	else if (a[0] == '-') {
		LargeNumber tempa = a;
		tempa.erase(tempa.begin());
		LargeNumber temp = b - tempa;
		return temp;
	}
	else if (b[0] == '-') {
		LargeNumber tempb = b;
		tempb.erase(tempb.begin());
		LargeNumber temp = a - tempb;
		return temp;
	}
	bool max_object = (a.size() > b.size() ? true : false);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	msize len_max = (a.size() > b.size() ? a.size() : b.size());
	msize len_min = (len_max == a.size() ? b.size() : a.size());
	LargeNumber s_data;
	int32 iscarry = 0;
	_FOR(i, 0, len_min, 1) {
		s_data.push_back((a[i] + b[i] + iscarry) % 10);
		iscarry = (a[i] + b[i] + iscarry) / 10;
	}
	_FOR(i, len_min, len_max, 1) {
		s_data.push_back(((max_object ? a[i] : b[i]) + iscarry) % 10);
		iscarry = ((max_object ? a[i] : b[i]) + iscarry) / 10;
	}
	if (iscarry) {
		s_data.push_back(iscarry);
	}
	reverse(s_data.begin(), s_data.end());
	return s_data;
}
const LargeNumber lzh::operator -(const LargeNumber& a_, const LargeNumber& b_)
{
	LargeNumber a = a_;
	LargeNumber b = b_;
	bool isNegative = false;
	int32 iscarry = 0;
	bool max_object = (a.size() > b.size() ? true : false);
	LargeNumber tempa = a;
	LargeNumber tempb = b;
	if (tempa[0] == '-')
		tempa.erase(tempa.begin());
	if (tempb[0] == '-')
		tempb.erase(tempb.begin());
	if (tempa < tempb) {
		LargeNumber temp = b - a;
		temp.insert(temp.begin(), '-');
		return temp;
	}
	else if (a == b)return LargeNumber("0");
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	msize len_max = (a.size() > b.size() ? a.size() : b.size());
	msize len_min = (len_max == a.size() ? b.size() : a.size());
	LargeNumber s_data;
	_FOR(i, 0, len_min, 1) {
		int8 val = a[i] - iscarry;
		int8 num = (val < 0 ? (10 + val) : val) - b[i];
		if (val > 0 && iscarry != 0)
			iscarry -= 1;
		s_data.push_back(((num < 0 ? (-num / 10 + 1) * 10 : 0) + num) % 10);
		iscarry += (num < 0 ? (-num / 10 + 1) : 0);
		//std::cout << int8(a[i] + '0') << ", " << int8(b[i] + '0') << ", " << s_data << std::endl;
	}
	_FOR(i, len_min, len_min + len_max - len_min, 1) {
		if (iscarry) {
			int8 val = max_object ? a[i] : b[i] - iscarry;
			int8 num = (val < 0 ? (10 + val) : val);
			if (val > 0 && iscarry != 0)
				iscarry -= 1;
			s_data.push_back(((num < 0 ? (-num / 10 + 1) * 10 : 0) + num) % 10);
			iscarry += (num < 0 ? (-num / 10 + 1) : 0);
		}
		else
			s_data.push_back(max_object ? a[i] : b[i]);
	}
	reverse(s_data.begin(), s_data.end());
	return s_data;
}
const LargeNumber lzh::operator *(const LargeNumber& a_, const LargeNumber& b_)
{
	LargeNumber a = a_;
	LargeNumber b = b_;
	bool isan = false;
	bool isbn = false;
	if (a[0] == '-')isan = true;
	if (b[0] == '-')isbn = true;
	if (isan)a.erase(a.begin());
	if (isbn)b.erase(b.begin());
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	LargeNumber sum;
	_FOR(i, isbn ? 1 : 0, b.size(), 1) {
		if (b[i] == 0)continue;
		int8 iscarry = 0;
		LargeNumber s_data;
		_FOR(j, isan ? 1 : 0, a.size(), 1) {
			s_data.push_back((a[j] * b[i] + iscarry) % 10);
			iscarry = (a[j] * b[i] + iscarry) / 10;
		}
		if (iscarry != 0)
			s_data.push_back(iscarry);
		reverse(s_data.begin(), s_data.end());
		_FOR(k, 0, isbn ? i - 1 : i, 1)
			s_data.push_back(0);
		if (sum.size() == 0)
			sum = s_data;
		else
			sum = sum + s_data;
	}
	if ((isan || isbn) && !(isan && isbn))
		sum.insert(sum.begin(), '-');
	return sum;
}
const LargeNumber lzh::operator /(const LargeNumber& a_, const LargeNumber& b_)
{
	int32 i, j;
	int32 nTimes;//两大数相差位数
	int32 nTemp;//Subtract函数返回值
	bool isan = false;
	bool isbn = false;
	LargeNumber a = a_;
	LargeNumber b = b_;
	if (a[0] == '-')isan = true;
	if (b[0] == '-')isbn = true;
	if (isan)a.erase(a.begin());
	if (isbn)b.erase(b.begin());
	int32 len1 = (int32)a.size();//获得大数的位数
	int32 len2 = (int32)b.size();
	int32 len = len1 < len2 ? len2 : len1;
	int32* num_a = new int32[len];//被除数
	int32* num_b = new int32[len];//除数
	int32* num_c = new int32[len];//商
	memset(num_a, 0, sizeof(int32) * len);
	memset(num_b, 0, sizeof(int32) * len);
	memset(num_c, 0, sizeof(int32) * len);
	for (j = 0, i = len1 - 1; i >= 0; j++, i--)
		num_a[j] = a[i];  //将字符串转换成对应的整数,颠倒存储
	for (j = 0, i = len2 - 1; i >= 0; j++, i--)
		num_b[j] = b[i];

	if (len1 < len2)   //如果被除数小于除数，结果为0
	{
		printf("0\n");
		return "0";   //利用continue直接跳出本次循环。 进入下一组测试
	}
	nTimes = len1 - len2;    //相差位数
	for (i = len1 - 1; i >= 0; i--)    //将除数扩大，使得除数和被除数位数相等
	{
		if (i >= nTimes)
			num_b[i] = num_b[i - nTimes];
		else                     //低位置0
			num_b[i] = 0;
	}
	len2 = len1;
	for (j = 0; j <= nTimes; j++)      //重复调用，同时记录减成功的次数，即为商
	{
		while ((nTemp = LargeNumber::SubStract(num_a, num_b + j, len1, len2 - j)) >= 0)
		{
			len1 = nTemp;      //结果长度
			num_c[nTimes - j]++;//每成功减一次，将商的相应位加1
		}
	}
	//输出结果
	for (i = len - 1; num_c[i] == 0 && i >= 0; i--);//跳过高位0
	LargeNumber div(num_c, i + 1);
	if ((isan || isbn) && !(isan && isbn))
		div.insert(div.begin(), '-');
	FREE_ARRAY(num_a);
	FREE_ARRAY(num_b);
	FREE_ARRAY(num_c);
	return div;
}
int32 lzh::LargeNumber::SubStract(int32* p1, int32* p2, int32 len1, int32 len2)

{
	int32 i;
	if (len1 < len2)
		return -1;
	if (len1 == len2)
	{                        //判断p1 > p2
		for (i = len1 - 1; i >= 0; i--)
		{
			if (p1[i] > p2[i])   //若大，则满足条件，可做减法
				break;
			else if (p1[i] < p2[i]) //否则返回-1
				return -1;
		}
	}
	for (i = 0; i <= len1 - 1; i++)  //从低位开始做减法
	{
		p1[i] -= p2[i];
		if (p1[i] < 0)          //若p1<0，则需要借位
		{
			p1[i] += 10;         //借1当10
			p1[i + 1]--;           //高位减1
		}
	}
	for (i = len1 - 1; i >= 0; i--)       //查找结果的最高位
		if (p1[i])                  //最高位第一个不为0
			return (i + 1);       //得到位数并返回
	return 0;                  //两数相等的时候返回0
}

void lzh::LargeNumber::show() const
{
	show(std::cout);
	std::cout << std::endl;
}
void lzh::LargeNumber::show(std::ostream& out)const
{
	std::string str;
	str.resize(size());
	for (int32 i = 0; i < (int32)size(); i++) {
		int8 ch = at(i);
		str[i] = (ch == '-' ? ch : ch + '0');
	}
	out << str;
}

LargeN::LargeN() : ln(new LargeNumber())
{

}
LargeN::LargeN(int8 ch) : ln(new LargeNumber(ch))
{
}
LargeN::LargeN(uint8 val) : ln(new LargeNumber(val))
{
}
lzh::LargeN::LargeN(int32 val) : ln(new LargeNumber(val))
{
}
LargeN::LargeN(int64 val) : ln(new LargeNumber(val))
{
}
LargeN::LargeN(uint32 val) : ln(new LargeNumber(val))
{
}
LargeN::LargeN(uint64 val) : ln(new LargeNumber(val))
{
}
LargeN::LargeN(const int8 * str) : ln(new LargeNumber(str))
{
}
LargeN::LargeN(std::string str) : ln(new LargeNumber(str))
{
}
LargeN::LargeN(int32 * ptr, int32 n) : ln(new LargeNumber(ptr, n))
{
}

LargeN::LargeN(const LargeN & largeNumber)
{
	*this = largeNumber;
}

LargeN::~LargeN()
{
	FREE_PTR(ln);
}

LargeNumber & lzh::LargeN::operator=(const LargeNumber & largenumber)
{
	if (ln == &largenumber)return *this;
	*ln = largenumber;
	return *this;
}

LargeNumber & lzh::LargeN::operator=(const LargeN & largenumber)
{
	if (ln == largenumber.ln)return *this;
	*ln = *largenumber.ln;
	return *this;
}
