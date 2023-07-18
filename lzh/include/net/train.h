#ifndef __TRAIN_H__
#define __TRAIN_H__

#include "net_core.h"
#include "optimalize.h"

namespace lzh {
	namespace nn {

		/**
		TrainOption ���ò�����
		@param TrainData *trainData				ѵ����
		@param uint32 epoch_number				ѵ������
		@param int show_epoch					�������(ȡ��)
		@param bool op_init = true				�Ƿ��ʼ���Ż���
		@param bool everytime_show = false		�Ƿ�ÿ���ζ����
		@param LOSS_INFO loss_info = nullptr	�ص���ʧ��Ϣ
		@param bool save = false				�Ƿ񱣴�����
		@param int save_epoch					������������(ȡ��)
		@param string savemodel					����ģ��·��
		@param string savemethod				�����Ż���·��
		*/
		class LZHAPI TrainOption
		{
		public:
			typedef void(*LOSS_INFO)(void*, int, vector<mat_t>*, vector<mat_t>*);
			TrainData *trainData;
			uint32 epoch_number;
			int show_epoch;
			bool op_init = true;
			bool everytime_show = false;
			STLApi<vector<bool(*)(const Mat &, const Mat &)>> evaluate = vector<bool(*)(const Mat &, const Mat &)>();
			LOSS_INFO loss_info = nullptr;
			bool logloss = false;
			STLApi<string> logpath;
			bool save = false;
			int save_epoch;
			STLApi<string> savemodel;
			STLApi<string> savemethod;
		};
		class LZHAPI Train
		{
		public:
			Train(Net *net = nullptr);
			~Train();
			void Running(bool run);
			size_t outputSize()const;
			void RegisterNet(Net *net);
			void RegisterOptimizer(Optimizer *optimizer);
			/**
			@brief FutureJacobi Ԥ���ſɱȾ���
			ģ�Ͳ����Ƚ��е���������ݶȾ���
			@param input ����
			@param output ���
			@param d_layer ����ݶȾ���
			*/
			void FutureJacobi(TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t> &error);
			/**
			@brief FutureJacobi �ſɱȾ���
			�����ݶȾ���
			@param input ����
			@param output ���
			@param d_layer ����ݶȾ���
			*/
			void Jacobi(TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t> &error);
			/**
			@brief TrainModel ������ѵ��ģ��
			@param input ����
			@param label ��ǩ
			@param error ��ʧ�������
			*/
			void Fit(TrainData::iterator data, vector<mat_t> *error = nullptr);

			void Fit(TrainOption *option);
			void Fit(OptimizerInfo op_info, TrainOption *option);
			void Fit(Optimizer *op, TrainOption *option);
			void Fit(Net *net, Optimizer *op, TrainOption *option);
			void Fit(Net *net, OptimizerInfo op_info, TrainOption *option);
			void initialize();

			mat_t lambda = 1.0f;
			bool regularization = false;
			void *pre = nullptr;
			volatile bool running = true;
		protected:
			/**
			@brief BackPropagation ���򴫲�
			@param input ����
			@param output ���
			@param d_layer ����ݶ�ֵ
			*/
			void BackPropagation(TrainData::iterator data, vector<Mat> & d_layer, vector<mat_t> & error);
			void JacobiMat(TrainData::iterator label, vector<Mat> &d_layer, vector<vector<Mat>> &output);
			static void JacobiCallBack(Object obj, TrainData::iterator data, vector<Mat>& d_layer, vector<mat_t> &error);
		private:
			Net *net;
			Optimizer* op;
			STLApi<vector<Mat>> dlayer;
			STLApi<vector<Size3>> size;
			STLApi<vector<NetNode<Layer*>*>> loss;
		};
	}
}

#endif // __TRAIN_H__