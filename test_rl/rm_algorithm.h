#pragma once
#include <vector>

class rm_algorithm
{
public:
	template<typename fun_type, typename a_type>
	double fun(fun_type& f, a_type a, double x, int iter);
};

template<typename fun_type, typename a_type>
inline double rm_algorithm::fun(fun_type& f, a_type a, double w, int iter)
{
	for (int i = 1; i <= iter; ++i)
		w = w - (a(double(i)) * f(w));
	return w;
}

class mbgd_algorithm
{
public:
	template<typename fun_type, typename a_type>
	double fun(fun_type& f, a_type& a, const std::vector<double>& x, int batch_size, double w, int iter);
};

template<typename fun_type, typename a_type>
inline double mbgd_algorithm::fun(fun_type& f, a_type& a, const std::vector<double>& x, int batch_size, double w, int iter)
{
	for (int i = 1; i <= iter; ++i) {
		double sum = 0;
		for (int j = 0; j < batch_size; ++j)
			sum += x[j];
		sum /= batch_size;
		w = w - (a(double(i)) * f(w, sum));
	}
	return w;
}