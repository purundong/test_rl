#include "test_rl.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	//auto g_fun = [](double& x) {return 2 * x + 1; };
	//auto a_fun = [](const double& x) {return 1 / x; };
	//rm_algorithm rm;
	//auto xx = rm.fun(g_fun, a_fun, -10000.0, 100);
	//std::vector<double> x = { 1,2,3,4,5,6,7,8,9,10 };
	//double sum{ 0.0 };
	//for(auto& i : x)
	//{
	//	sum += i;
	//}
	//sum=sum/x.size();
	//mbgd_algorithm rm;
	//auto g_fun = [](double& w, double& x) {return w - x; };
	//auto a_fun = [](const double& x) {return 0.05; };
	//auto xx = rm.fun(g_fun, a_fun, x, x.size(), -500, 10000000);
	test_rl w;
	w.show();
	//ui_state w;
	//w.show();
	return a.exec();
}
