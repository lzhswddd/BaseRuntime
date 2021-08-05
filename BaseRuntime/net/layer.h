#ifndef __LAYER_H__
#define __LAYER_H__

#include "net_core.h"
#include "../json/json.h"
using json = nlohmann::json;

namespace lzh {
	namespace nn
	{
		class LZHAPI Layer
		{
		public:
			explicit Layer(string name = "") : name(name), type(LayerType::NONE), layer_index(0) {}
			virtual void forword(const Mat &in, Mat &out)const = 0;
			virtual void forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> &variable) = 0;
			virtual void back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number)const = 0;
			virtual Size3 initialize(Size3 param_size) = 0;
			virtual void save(json * jarray, FILE* file)const = 0;
			virtual void load(json & info, FILE* file) = 0;
			virtual void show(std::ostream &out)const = 0;

			STLApi<string> name;
			LayerType type;
			int layer_index;
			bool last = false;
			bool start = false;
			bool isinit;
			bool isforword;
			bool isback;
			bool isupdate;
			bool isparam;

			static string Type2String(LayerType type);
			static LayerType String2Type(string str);
		};
		extern LZHAPI std::ostream& operator << (std::ostream& out, const Layer* layer);	
		class LZHAPI parametrics : public Layer
		{
		public:
			explicit parametrics(string name = "") : Layer(name)
			{
				isinit = true;
				isforword = true;
				isback = true;
				isupdate = true;
				isparam = true;
			}
			explicit parametrics(bool freeze, string name = "") 
				: Layer(name), freeze(freeze)
			{
				isinit = true;
				isforword = true;
				isback = true;
				isupdate = true;
				isparam = true;
			}
			void freeze_enable(bool freeze);
			bool isfreeze()const;
			virtual void updateregular() = 0;
			virtual void update(const vector<Mat> &d, int *idx) = 0;
			virtual mat_t norm(int num)const = 0;
			virtual void save_param(FILE* file)const = 0;
			virtual void load_param(FILE* file) = 0;
			virtual void append_size(vector<Size3> *size) = 0;
			bool freeze = false;
			bool regularization = false;
			mat_t lambda = 0.5f;
			Mat regular;
		};
		class LZHAPI Node : public Layer
		{
		public:
			explicit Node(string name = "") : Layer(name) {
				isinit = false;
				isforword = false;
				isback = false;
				isupdate = false;
				isparam = false;
			}
			void forword(const Mat &in, Mat &out)const { out = in; }
			void forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> &variable) { out = in; }
			void back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number)const { out = in; }
			Size3 initialize(Size3 size) { return size; }
			void save(json * jarray, FILE* file)const {}
			void load(json & info, FILE* file) {}
			void show(std::ostream &out)const {}
		};

		class LZHAPI Activate :
			public parametrics
		{
		public:
			Activate(bool freeze = true, string name = "");
			Activate(bool freeze, Mat alpha);
			~Activate();

			virtual void updateregular();
			virtual void update(const vector<Mat> &d, int *idx);
			virtual void forword(const Mat &in, Mat &out)const;
			virtual void forword_train(const vector<Mat> & in, vector<Mat> & out, vector<Mat> &variable);
			virtual void back(const vector<Mat> &in, vector<Mat> & out, vector<Mat> *dlayer, int *number)const;
			virtual Size3 initialize(Size3 param_size);
			virtual void append_size(vector<Size3> *size);
			virtual void save(json * jarray, FILE* file)const;
			virtual void load(json & info, FILE* file);
			virtual void save_param(FILE* file)const;
			virtual void load_param(FILE* file);
			virtual void show(std::ostream &out)const;
			virtual mat_t norm(int num)const;

			virtual Mat f(const Mat & x)const;
			virtual Mat df(const Mat & x)const;
			virtual void f(mat_t *p, const mat_t *mat, const mat_t *ai)const = 0;
			virtual void df(mat_t *p, const mat_t *mat, const mat_t *ai)const = 0;

			Mat a;
			STLApi<vector<Mat>> variable;
		};

		extern LZHAPI Layer* CreateLayer(json &info, FILE* file);
		extern LZHAPI Layer* PRelu(string name = "");
		extern LZHAPI Layer* Sigmoid(bool freeze = true, string name = "");
		extern LZHAPI Layer* Tanh(bool freeze = true, string name = "");
		extern LZHAPI Layer* Softmax(bool freeze = true, string name = "");
		extern LZHAPI Layer* BatchNorm(string name = "");
		extern LZHAPI Layer* Reshape(Size3 size, string name = "");
		extern LZHAPI Layer* Loss(LF loss_f, mat_t weight = 1, string name = "");
		extern LZHAPI Layer* Loss(ReduceType loss_f, mat_t weight = 1, string name = "");
		extern LZHAPI Layer* Loss(LF f, LF df, bool ignore_activate, mat_t weight = 1, string name = "");
		extern LZHAPI Layer* Dropout(mat_t dropout, string name = "");
		extern LZHAPI Layer* Activation(Act acti, string name = "");
		extern LZHAPI Layer* Activation(ActivationType acti, string name = "");
		extern LZHAPI Layer* Activation(Act f, Act df, string name = "");
		extern LZHAPI Layer* MaxPool(Size poolsize, int strides, string name = "");
		extern LZHAPI Layer* AveragePool(Size poolsize, int strides, string name = "");
		extern LZHAPI Layer* Dense(int layer_size, Act act_f = nullptr, string name = "");
		extern LZHAPI Layer* FullConnect(int layer_size, Act act_f = nullptr, string name = "");
		extern LZHAPI Layer* Conv2D(ConvInfo cinfo, Act act_f = nullptr, string name = "");
		extern LZHAPI Layer* Conv2D(int output_channel, int kern_size, bool is_copy_border = true, Act act_f = nullptr, string name = "", Size strides = Size(1, 1), Point anchor = Point(-1, -1));
	}
}

#endif // !__LAYER_H__
