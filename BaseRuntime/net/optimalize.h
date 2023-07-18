#ifndef __OPTIMALIZE_H__
#define __OPTIMALIZE_H__

#include "net_core.h"
#include "traindata.h"

namespace lzh {
	namespace nn
	{
		/**
		@brief Optimizer�Ż�������
		ע��ģ�͵���ʧ����loss
		�ṩ�Ż�����
		*/
		class LZHAPI Optimizer
		{
		public:
			typedef void(*Jacobi)(Object, TrainData::iterator, vector<Mat>&, vector<mat_t>&);
			explicit Optimizer();
			Optimizer(mat_t step);
			~Optimizer();
			/**
			@brief ��ʼ���Ż�������
			@param size �Ż�����ߴ�
			*/
			virtual void init(vector<Size3>& size) = 0;
			virtual void save(string file)const = 0;
			virtual void load(string file) = 0;
			/**
			@brief �����Ż�������1��
			@param x ��������
			@param y �������
			@param da �仯��
			@param size
			@param size
			*/
			virtual void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error) = 0;
			void Register(Jacobi jacobi, const void* obj = nullptr);
			bool Enable(const Mat &x, const vector<Mat> &y)const;
			void RegisterMethod(OptimizerMethod method);
			OptimizerMethod OpMethod()const;
			virtual void copyTo(Optimizer* op)const = 0;
			virtual Mat Params()const = 0;
			mat_t& Step() { return step; }

			static string Method2String(OptimizerMethod method);
			static OptimizerMethod String2Method(string name);
			static Optimizer* CreateOptimizer(Optimizer* op);
			static Optimizer* CreateOptimizer(OptimizerInfo optimizer_info);
			/**
			@brief CreateOptimizer �����Ż���
			�����Ż���
			@param opm �Ż�������
			@param step ѧϰ��
			@param loss_f ��ʧ����
			@param value ����
			*/
			static Optimizer* CreateOptimizer(OptimizerMethod opm, mat_t step = 1e-2f, const Mat &value = Mat(Size(3), 0.9f, 0.999e-3f, 1e-7f));
			static void SaveParam(FILE * file, const vector<Mat> *vec);
			static void LoadParam(FILE * file, vector<Mat> *vec);
		protected:
			//����ʵ��
			void* obj;
			Jacobi jacobi;
			//ѧϰ��
			mat_t step;
			//�̳л��������
			OptimizerMethod method;
		};
		/**
		@brief Method����ĺ�������
		ע��ģ�͵���ʧ����loss
		���ṩ�Ż�����
		*/
		class LZHAPI Method :public Optimizer
		{
		public:
			explicit Method();
			Method(mat_t step);
            void init(vector<Size3>& ) {}
            void save(string )const {}
            void load(string ) {}
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			Optimizer* minimize()const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		};
		/**
		@brief GradientDescentOptimizer����ĺ�������
		ע��ģ�͵���ʧ����loss
		�ṩ�Ż�����GradientDescent
		a = a - step * df(a, x)
		*/
		class LZHAPI GradientDescentOptimizer :public Optimizer
		{
		public:
			/**
			@brief �Ż������캯��
			@param step ѧϰ��
			*/
			explicit GradientDescentOptimizer(mat_t step = 1e-2);
            void init(vector<Size3>& ) {}
            void save(string )const {}
            void load(string ) {}
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief ����ģ�ͺ���������ע����Ż���Optimizer
			*/
			Optimizer* minimize()const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		};
		/**
		@brief MomentumOptimizer����ĺ�������
		ע��ģ�͵���ʧ����loss
		�ṩ�Ż�����MomentumGradientDescent
		ma = momentum*ma + step * df(a, x)
		a = a - ma
		*/
		class LZHAPI MomentumOptimizer :public Optimizer
		{
		public:
			/**
			@brief �Ż������캯��
			@param step ѧϰ��
			@param gama ����ϵ��
			*/
			explicit MomentumOptimizer(mat_t step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief ����ģ�ͺ���������ע����Ż���Optimizer
			@param loss_ ��ʧ����
			@param output_ �������
			@param activation_ �����
			*/
			Optimizer* minimize(mat_t momentum = 0.9f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> ma;
			mat_t momentum;
		};
		/**
		@brief NesterovMomentumOptimizer����ĺ�������
		ע��ģ�͵���ʧ����loss
		�ṩ�Ż�����NesterovMomentumGradientDescent
		ma = momentum*ma + step * df(a - momentum*ma, x)
		a = a - ma
		*/
		class LZHAPI NesterovMomentumOptimizer :public Optimizer
		{
		public:
			/**
			@brief �Ż������캯��
			@param step ѧϰ��
			@param gama ����ϵ��
			*/
			explicit NesterovMomentumOptimizer(mat_t step = 1e-2);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief ����ģ�ͺ���������ע����Ż���Optimizer
			@param loss_ ��ʧ����
			@param output_ �������
			@param activation_ �����
			*/
			Optimizer* minimize(mat_t momentum = 0.9f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> ma;
			mat_t momentum;
		};
		/**
		@brief AdagradOptimizer����ĺ�������
		ע��ģ�͵���ʧ����loss
		�ṩ�Ż�����AdagradGradientDescent
		alpha = alpha + df(a, x)^2
		a = a - step/sqrt(alpha + epsilon)*df(a, x)
		*/
		class LZHAPI AdagradOptimizer :public Optimizer
		{
		public:
			/**
			@brief �Ż������캯��
			@param step ѧϰ��
			@param epsilon ƫ��(�����ĸ=0)
			*/
			explicit AdagradOptimizer(mat_t step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief ����ģ�ͺ���������ע����Ż���Optimizer
			@param loss_ ��ʧ����
			@param output_ �������
			@param activation_ �����
			*/
			Optimizer* minimize(mat_t epsilon = 1e-7f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> alpha;
			mat_t epsilon;
		};
		/**
		@brief RMSPropOptimizer����ĺ�������
		ע��ģ�͵���ʧ����loss
		�ṩ�Ż�����RMSPropGradientDescent
		alpha = beta*alpha + (1 - beta)*df(a, x)^2
		a = a - step/sqrt(alpha + epsilon)*df(a, x)
		*/
		class LZHAPI RMSPropOptimizer :public Optimizer
		{
		public:
			/**
			@brief �Ż������캯��
			@param step ѧϰ��
			@param decay ����Ӧ����
			@param epsilon ƫ��(�����ĸ=0)
			*/
			explicit RMSPropOptimizer(mat_t step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief ����ģ�ͺ���������ע����Ż���Optimizer
			@param loss_ ��ʧ����
			@param output_ �������
			@param activation_ �����
			*/
			Optimizer* minimize(mat_t decay = 0.9f, mat_t epsilon = 1e-7f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> alpha;
			mat_t decay;
			mat_t epsilon;
		};
		/**
		@brief AdamOptimizer����ĺ�������
		ע��ģ�͵���ʧ����loss
		�ṩ�Ż�����AdamGradientDescent
		ma = beta1*ma + (1 - beta1)*df(a, x)
		alpha = beta2*alpha + (1 - beta2)*df(a, x)^2
		a = a - step/sqrt(alpha + epsilon)*ma
		*/
		class LZHAPI AdamOptimizer :public Optimizer
		{
		public:
			/**
			@brief �Ż������캯��
			@param step ѧϰ��
			@param beta1 ����ϵ��
			@param beta2 ����Ӧϵ��
			@param epsilon ƫ��(�����ĸ=0)
			*/
			explicit AdamOptimizer(Val step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief ����ģ�ͺ���������ע����Ż���Optimizer
			@param loss_ ��ʧ����
			@param output_ �������
			@param activation_ �����
			*/
			Optimizer* minimize(mat_t beta1 = 0.9f, mat_t beta2 = 0.999f, mat_t epsilon = 1e-7f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> ma;
			STLApi<vector<Mat>> alpha;
			mat_t epsilon;
			mat_t beta1;
			mat_t beta2;
		};
		/**
		@brief NesterovAdamOptimizer����ĺ�������
		ע��ģ�͵���ʧ����loss
		�ṩ�Ż�����NesterovAdamGradientDescent
		ma = beta1*ma + (1 - beta1)*df(a - step/sqrt(alpha + epsilon)*ma, x)
		alpha = beta2*alpha + (1 - beta2)*df(a - step/sqrt(alpha + epsilon)*ma, x)^2
		a = a - step/sqrt(alpha + epsilon)*ma
		*/
		class LZHAPI NesterovAdamOptimizer :public Optimizer
		{
		public:
			/**
			@brief �Ż������캯��
			@param step ѧϰ��
			@param beta1 ����ϵ��
			@param beta2 ����Ӧϵ��
			@param epsilon ƫ��(�����ĸ=0)
			*/
			explicit NesterovAdamOptimizer(mat_t step = 1e-2f);
			void init(vector<Size3>& size);
			void save(string file)const;
			void load(string file);
			void Run(vector<Mat> &dlayer, TrainData::iterator x, vector<mat_t> &error);
			/**
			@brief ����ģ�ͺ���������ע����Ż���Optimizer
			@param loss_ ��ʧ����
			@param output_ �������
			@param activation_ �����
			*/
			Optimizer* minimize(mat_t beta1 = 0.9f, mat_t beta2 = 0.999f, mat_t epsilon = 1e-7f)const;
			Mat data(vector<string> &value_name)const;
			void copyTo(Optimizer* op)const;
			Mat Params()const;
		private:
			STLApi<vector<Mat>> ma;
			STLApi<vector<Mat>> alpha;
			mat_t epsilon;
			mat_t beta1;
			mat_t beta2;
		};
		typedef Optimizer* OptimizerP;

	}
}

#endif // !__NET_CORE_H__
