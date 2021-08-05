#pragma once
#include "net.h"

namespace lzh
{
	namespace nn
	{
		class LZHAPI TrainData
		{
		public:
			typedef vector<NetData*>* iterator;
			typedef const vector<const NetData*>* const_iterator;
			typedef vector<vector<NetData*>>* DataBox;
			typedef const vector<vector<const NetData*>>* ConstDataBox;
			typedef vector<NetData*> DataBoxIter;
			typedef const vector<const NetData*> ConstDataBoxIter;
			TrainData();
			TrainData(string rootpath, string imglist,
				int batch_size, const vector<Mat>(*label_process)(const Mat&) = nullptr);
			TrainData(const vec_mat & data, const vec_mat & label, int batch_size, const vector<Mat>(*label_process)(const Mat&) = nullptr);
			TrainData(const vec_mat & data, const vecs_mat & label, int batch_size, const vector<Mat>(*label_process)(const Mat&) = nullptr);
			TrainData(const vector<NetData> &data, const vector<NetData> &vaild, const vector<NetData> &test, int batch_size);
			void load_train_data(string rootpath, string imglist,
				int batch_size, const vector<Mat>(*label_process)(const Mat&) = nullptr);
			~TrainData();

			void Running(bool run);
			void reset();
			void clear();
			bool isEnable()const;
			int batchSize()const;
			bool all_load()const;
			int len()const;
			iterator batches();
			const vector<NetData>* vaild()const;
			const vector<NetData>* test()const;
			DataBox all_batches();
			ConstDataBox all_batches()const;
			void load_all_data(bool is_show = false, void(*print)(void*, const char *) = nullptr);
			void register_process(void(*mat)(const Mat&, Mat&));
			STLApi<vector<int>> range;
			volatile bool running = true;
			void* pre = nullptr;
		protected:
			void next();
			void get_train_data(string imglist, const vector<Mat>(*label_proces)(const Mat&) = nullptr);
			const vector<Mat> labelProcess(const Mat& l);
		private:
			bool enable = false;
			int index = 0;
			int batch_size;
			int batch_number;
			STLApi<string> rootpath;
			STLApi<vector<Mat>> data;
			STLApi<vector<vector<Mat>>> label;
			STLApi<vector<string>> imgpath;
			STLApi<vector<NetData>> traindata;
			STLApi<vector<NetData>> vailddata;
			STLApi<vector<NetData>> testdata;
			STLApi<vector<vector<NetData*>>> batchdata;
			void(*process_mat)(const Mat&, Mat&) = nullptr;
		};
	}
}
