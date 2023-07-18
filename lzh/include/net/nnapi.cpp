#define LZH_EXPORTS
#include "nnapi.h"
#include "nn.h"
using namespace lzh;
using namespace nn;

void nn::SetFunc(string func_name, LF *f, LF *df)
{
	if (func_name == "Quadratic") {
		*f = Quadratic;
		*df = D_Quadratic;
	}
	else if (func_name == "Relative") {
		*f = Relative;
		*df = D_Relative;
	}
	else if (func_name == "CrossEntropy") {
		*f = CrossEntropy;
		*df = D_CrossEntropy;
	}
	else if (func_name == "SoftmaxCrossEntropy") {
		*f = SoftmaxCrossEntropy;
		*df = D_SoftmaxCrossEntropy;
	}
	else if (func_name == "L2") {
		*f = L2;
		*df = D_L2;
	}
}
void nn::SetFunc(string func_name, Act *f, Act *df)
{
	if (func_name == "softmax") {
		*f = lzh::softmax;
		*df = d_softmax;
	}
	else if (func_name == "sigmoid") {
		*f = lzh::sigmoid;
		*df = d_sigmoid;
	}
	else if (func_name == "tanh") {
		*f = lzh::tanh;
		*df = d_tanh;
	}
	else if (func_name == "relu") {
		*f = relu;
		*df = d_relu;
	}
	else if (func_name == "elu") {
		*f = elu;
		*df = d_elu;
	}
	else if (func_name == "selu") {
		*f = selu;
		*df = d_selu;
	}
	else if (func_name == "leaky_relu") {
		*f = leaky_relu;
		*df = d_leaky_relu;
	}
	else if (func_name == "softplus") {
		*f = softplus;
		*df = d_softplus;
	}
}
void nn::SetFunc(LF func, LF * f, LF * df)
{
	if (func == Quadratic) {
		*f = Quadratic;
		*df = D_Quadratic;
	}
	else if (func == Relative) {
		*f = Relative;
		*df = D_Relative;
	}
	else if (func == CrossEntropy) {
		*f = CrossEntropy;
		*df = D_CrossEntropy;
	}
	else if (func == SoftmaxCrossEntropy) {
		*f = SoftmaxCrossEntropy;
		*df = D_SoftmaxCrossEntropy;
	}
	else if (func == L2) {
		*f = L2;
		*df = D_L2;
	}
	else if (func == L1) {
		*f = L1;
		*df = D_L1;
	}
}
void nn::SetFunc(Act func, Act * f, Act * df)
{
	if (func == lzh::softmax) {
		*f = lzh::softmax;
		*df = d_softmax;
	}
	else if (func == lzh::sigmoid) {
		*f = lzh::sigmoid;
		*df = d_sigmoid;
	}
	else if (func == lzh::tanh) {
		*f = lzh::tanh;
		*df = d_tanh;
	}
	else if (func == relu) {
		*f = relu;
		*df = d_relu;
	}
	else if (func == elu) {
		*f = elu;
		*df = d_elu;
	}
	else if (func == selu) {
		*f = selu;
		*df = d_selu;
	}
	else if (func == leaky_relu) {
		*f = leaky_relu;
		*df = d_leaky_relu;
	}
	else if (func == softplus) {
		*f = softplus;
		*df = d_softplus;
	}
}
string nn::Func2String(Act f)
{
	string fun_name = "";
	if (f == lzh::softmax) {
		fun_name = "softmax";
	}
	else if (f == lzh::sigmoid) {
		fun_name = "sigmoid";
	}
	else if (f == lzh::tanh) {
		fun_name = "tanh";
	}
	else if (f == relu) {
		fun_name = "relu";
	}
	else if (f == elu) {
		fun_name = "elu";
	}
	else if (f == selu) {
		fun_name = "selu";
	}
	else if (f == leaky_relu) {
		fun_name = "leaky_relu";
	}
	else if (f == softplus) {
		fun_name = "softplus";
	}
	else {
		fun_name = "user custom";
	}
	return fun_name;
}
string nn::Func2String(LF f)
{
	string fun_name = "";
	if (f == Quadratic) {
		fun_name = "Quadratic";
	}
	if (f == Relative) {
		fun_name = "Relative";
	}
	else if (f == CrossEntropy) {
		fun_name = "CrossEntropy";
	}
	else if (f == SoftmaxCrossEntropy) {
		fun_name = "SoftmaxCrossEntropy";
	}
	else if (f == L2) {
		fun_name = "L2";
	}
	else {
		fun_name = "user custom";
	}
	return fun_name;
}
void nn::generateBbox(const Mat &score, const Mat &location, std::vector<Bbox> &boundingBox_, mat_t scale, mat_t threshold)
{
	const int stride = 2;
	const int cellsize = 12;
	Bbox bbox{};
	const mat_t thres = threshold;
	mat_t inv_scale = 1.0f / scale;
	for (int row = 0; row < score.rows(); row++) {
		bbox.y1 = (int)((stride * row + 1) * inv_scale + 0.5f);
		bbox.y2 = (int)((stride * row + 1 + cellsize) * inv_scale + 0.5f);
		int diff_y = (bbox.y2 - bbox.y1);
		for (int col = 0; col < score.cols(); col++) {
			if (score(row, col, 1) > thres) {
				bbox.score = score(row, col, 1);
				bbox.x1 = (int)((stride * col + 1) * inv_scale + 0.5f);
				bbox.x2 = (int)((stride * col + 1 + cellsize) * inv_scale + 0.5f);
				bbox.area = (mat_t)(bbox.x2 - bbox.x1) * diff_y;
				for (int channel = 0; channel < 4; channel++) {
					bbox.regreCoord[channel] = location(row, col, channel);
				}
				boundingBox_.push_back(bbox);
			}
		}
	}
}
void nn::nms(std::vector<Bbox> &boundingBox_, const mat_t overlap_threshold, string modelname)
{
	if (boundingBox_.empty()) {
		return;
	}
	std::sort(boundingBox_.begin(), boundingBox_.end(), [](const Bbox & lsh, const Bbox & rsh) {
		return lsh.score < rsh.score;
	});
	mat_t IOU = 0;
	mat_t maxX = 0;
	mat_t maxY = 0;
	mat_t minX = 0;
	mat_t minY = 0;
	std::vector<int> vPick;
	int nPick = 0;
	std::multimap<mat_t, int> vScores;
	const int num_boxes = (int)boundingBox_.size();
	vPick.resize(num_boxes);
	for (int i = 0; i < num_boxes; ++i) {
		vScores.insert(std::pair<mat_t, int>(boundingBox_[i].score, i));
	}
	while (vScores.size() > 0) {
		int last = vScores.rbegin()->second;
		vPick[nPick] = last;
		nPick += 1;
		for (std::multimap<mat_t, int>::iterator it = vScores.begin(); it != vScores.end();) {
			int it_idx = it->second;
			maxX = (mat_t)std::max(boundingBox_.at(it_idx).x1, boundingBox_.at(last).x1);
			maxY = (mat_t)std::max(boundingBox_.at(it_idx).y1, boundingBox_.at(last).y1);
			minX = (mat_t)std::min(boundingBox_.at(it_idx).x2, boundingBox_.at(last).x2);
			minY = (mat_t)std::min(boundingBox_.at(it_idx).y2, boundingBox_.at(last).y2);
			//maxX1 and maxY1 reuse 
			maxX = ((minX - maxX + 1) > 0) ? (minX - maxX + 1) : 0;
			maxY = ((minY - maxY + 1) > 0) ? (minY - maxY + 1) : 0;
			//IOU reuse for the area of two bbox
			IOU = maxX * maxY;
			if (!modelname.compare("Union"))
				IOU = IOU / (boundingBox_.at(it_idx).area + boundingBox_.at(last).area - IOU);
			else if (!modelname.compare("Min")) {
				IOU = IOU / ((boundingBox_.at(it_idx).area < boundingBox_.at(last).area) ? boundingBox_.at(it_idx).area
					: boundingBox_.at(last).area);
			}
			if (IOU > overlap_threshold) {
				it = vScores.erase(it);
			}
			else {
				it++;
			}
		}
	}

	vPick.resize(nPick);
	std::vector<Bbox> tmp_;
	tmp_.resize(nPick);
	for (int i = 0; i < nPick; i++) {
		tmp_[i] = boundingBox_[vPick[i]];
	}
	boundingBox_ = tmp_;
}
void nn::refine(vector<Bbox> &vecBbox, const int &height, const int &width, bool square) {
	if (vecBbox.empty()) {
		std::cout << "Bbox is empty!!" << std::endl;
		return;
	}
	mat_t bbw = 0, bbh = 0, maxSide = 0;
	mat_t h = 0, w = 0;
	mat_t x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	for (vector<Bbox>::iterator it = vecBbox.begin(); it != vecBbox.end(); it++) {


		bbw = mat_t((*it).x2 - (*it).x1 + 1);
		bbh = mat_t((*it).y2 - (*it).y1 + 1);
		x1 = (*it).x1 + (*it).regreCoord[0] * bbw;
		y1 = (*it).y1 + (*it).regreCoord[1] * bbh;
		x2 = (*it).x2 + (*it).regreCoord[2] * bbw;
		y2 = (*it).y2 + (*it).regreCoord[3] * bbh;
		if (square) {
			w = x2 - x1 + 1;
			h = y2 - y1 + 1;
			maxSide = (h > w) ? h : w;
			x1 = x1 + w * 0.5f - maxSide * 0.5f;
			y1 = y1 + h * 0.5f - maxSide * 0.5f;
			(*it).x2 = (int)(x1 + maxSide - 1 + 0.5f);
			(*it).y2 = (int)(y1 + maxSide - 1 + 0.5f);
			(*it).x1 = (int)(x1 + 0.5f);
			(*it).y1 = (int)(y1 + 0.5f);
		}
		//boundary check
		if ((*it).x1 < 0)(*it).x1 = 0;
		if ((*it).y1 < 0)(*it).y1 = 0;
		if ((*it).x2 > width)(*it).x2 = width - 1;
		if ((*it).y2 > height)(*it).y2 = height - 1;

		it->area = mat_t((it->x2 - it->x1) * (it->y2 - it->y1));
	}
}
void nn::normalization(const Mat & src, Mat & dst)
{
	dst = src / src.max();
}

void nn::save_mat(FILE * file, const Mat & m)
{
	m.save(file);
}

void nn::load_mat(FILE * file, Mat & m)
{
	m.load(file);
}
