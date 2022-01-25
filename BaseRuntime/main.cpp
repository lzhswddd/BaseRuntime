#include <iostream>
#define LZH_EXPORTS
#include "lzh.h"
#include <fstream>
using namespace std;
using namespace lzh;

class A
{
public:
	bool f() 
	{
		output("a");
		return true; 
	}
};
int main()
{
	lzh::Srandom();
	
	Container<int> dict;
	dict[1][1] = "sdd";
	dict[1][2] = 32;
	dict[1][3] = Point(1, 2);
	dict[1][4] = Rect(1,1,3,4);
	dict[2] = 15;
	dict[3] = PointF(1,2);
	cout << dict[1] << endl;
	cout << dict[2] << endl;
	cout << dict[3] << endl;
	//lzh::MultThread<std::function<void(void)>> distribute(8);
	//distribute.Start();
	//distribute.Process = [](std::function<void(void)> f) {f(); };
	//for (int i : range(10))
	//	distribute.Append([]() {
	//	output("a");
	//		});

	//distribute.Stop();
	//distribute.Join();

	pause();
	//Action<int> a;
	//a += [](int v) {output(v); };
	//a += [](int v) {output(v); };
	//a += [](int v) {output(v); };
	//a(2);

	/*A a;
	Delegate<bool()> signal = Connect(&a, &A::f);
	AutoResetEvent ev(false);
	thread th([&](){
		output("wait");
		signal();
		Wait(1000);
		output("switch on");
		ev.Set();
		output("switch off");
		});

	output("wait one");
	ev.WaitOne(-1);
	output("pass");

	if(th.joinable())th.join();*/


	//Image img = Image::zeros(Size3(1270, 720, 3), TP_8U);

	//imshow(imread("C:\\Users\\lzh\\Desktop\\CE00647-11-B2-PX25PY177-102.bmp"));
	//waitKey();

	//Action<void> act;

	////MultThread<int> thread(16);
	////thread.Process = [&](int i) {
	////	lzh::circle(img, Point((rand() % (1200)) + 70, (rand() % (720 - 70)) + 70), 60, Color::Red, 1, true);
	////};
	////thread.Start();

	//Timer time;
	//while (true) 
	//{
	//	time.Start();

	//	//MultThread<int>::ForEach(0, 1089, [&](int i) {
	//	//	lzh::circle(img, Point((rand() % (1200)) + 70, (rand() % (720 - 70)) + 70), 60, Color::Red, 1, true);
	//	//	}, 8);
	//		//lzh::circle(img, Point((rand() % (1200)) + 70, (rand() % (720 - 70)) + 70), 60, Color::Red, 1, true);
	//	output(time.End());
	//}
	//imwrite("C:\\Users\\lzh\\Desktop\\test.jpg", img);
	//int* a;
	//int row, col;
	//FILE* file;
	//fopen_s(&file, "D:\\Project\\MapDoNetToC\\MapDoNetToC\\bin\\Debug\\netcoreapp3.1\\map.dat", "rb");
	//if (file != NULL) {
	//	fread_s(&row, sizeof(int), sizeof(int), 1, file);
	//	fread_s(&col, sizeof(int), sizeof(int), 1, file);
	//	a = new int[row * col];
	//	fread_s(a, sizeof(int) * row * col, sizeof(int) * row * col, 1, file);

	//	ofstream out("map.txt");
	//	out << row << endl;
	//	out << col << endl;
	//	for (int i = 0; i < row; i++) {
	//		for (int j = 0; j < col; j++) {
	//			out << a[i * col + j];
	//		}
	//		out << endl;
	//	}
	//	out.close();
	//}
	//ElemMat em({ { 4, 4 }, {"12", "52"} });
	//for (Element& elem : em)
	//	cout << elem << endl;
	//Element v, s, v1, s2;
	//zip(v, s, v1, s2) = zip("12", "52", "fa", "nd");
	//cout << v << ", " << s << ", " << v1 << ", " << s2 << endl;

	//Element iter; Element value={}
	////for(iter : )

	//Element m = Element::Create(Element::STD_MAP);
	//m["int"] = 1;
	//m["double"] = 1.5887;
	//m[1] = 427;
	//m[1.674] = "double";
	//for (Element::iterator iter = m.begin(); iter != m.end(); iter++) {
	//	cout << iter.key() << ", " << iter.value() << endl;
	//}

	//Element elem8 = { elem1, elem2, elem3, elem4, elem5, elem6, elem7 };
	//for (Element elem : elem8.toList()) {
	//	cout  << elem  << ",  type: " << elem.typeName() << ",  size: " << sizeof(elem) << endl;
	//}
	//A a;
	//Element elem(a);
	//cout << elem << ",  type: " << elem.typeName() << ",  size: " << sizeof(elem) << endl;
	return 0;
}
