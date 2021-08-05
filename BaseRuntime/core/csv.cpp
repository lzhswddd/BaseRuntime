#pragma once
#include <fstream>
#define LZH_EXPORTS
#include "csv.h"
#include "tools.h"
#include "convert.h"

namespace lzh
{
	CSV::CSV() : data(true), col(0) {

	}
	CSV::CSV(const std::string & path) : data(true), col(0){
		read(path);
	}
	bool CSV::read(const std::string & path) {
		std::ifstream in(path);
		if (!in.is_open())return false;
		data->clear();
		std::string str;
		while (std::getline(in, str)) {
			data->push_back(lzh::strSplit(str, ','));
			if (col < data->back().size())col = data->back().size();
		}
		in.close(); 
		return true;
	}
	bool CSV::save(const std::string & path)const {
		std::ofstream out(path);
		if (!out.is_open())return false;
		for (size_t i = 0; i < data->size(); i++) {
			for (size_t j = 0; j < data->at(i).size(); j++) {
				if (j == 0)
					out << data->at(i)[j];
				else
					out << ',' << data->at(i)[j];
			}
			if (i + 1 != data->size())out << std::endl;
		}
		out.close(); return true;
	}
	Mat CSV::toMat()const {
		if (data->empty())return Mat();
		Mat mat = Mat::zeros((int)col, (int)data->size());
		int i, j;
		i = 0;
		for (const std::vector<std::string>& vec : *data) {
			j = 0;
			for (const std::string& v : vec) {
				mat(i, j) = Convert::toDouble(v);
				j += 1;
			}
			i += 1;
		}
		return mat;
	}
	void CSV::setMat(IMat m) {
		if (m.empty())return;
		data->clear();
		col = m.cols();
		for (int i = 0; i < m.rows(); i++) {
			lzh::StringList list(m.cols());
			for (int j = 0; j < m.cols(); j++) {
				list[j] = Convert::toString(m(i, j));
			}
			data->push_back(list);
		}
	}
}