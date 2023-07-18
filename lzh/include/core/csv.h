#pragma once
#include "mat.h"
#include "pointer.h"
namespace lzh
{
	class LZHAPI CSV
	{
	public:
		CSV();
		CSV(const std::string & path);

		bool read(const std::string & path);
		bool save(const std::string & path)const;

		Mat toMat()const;
		void setMat(IMat m);


	protected:
		size_t col;
		Pointer<std::vector<std::vector<std::string>>> data;
	};
}