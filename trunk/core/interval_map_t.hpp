#ifndef interval_map_t_H_INCLUDED
#define interval_map_t_H_INCLUDED

namespace all {
	namespace core {

class interval_map_t {

public:
	void set_range(double xmin, double xmax, double ymin, double ymax);

	double x_to_y(double);
	double y_to_x(double);

private:

	double m_xmin;
	double m_xmax;
	double m_ymin;
	double m_ymax;

	double m_xy_ratio;
	double m_yx_ratio;
};



inline void interval_map_t::set_range(double xmin, double xmax, double ymin, double ymax) {
	m_xmin = xmin;
	m_xmax = xmax;
	m_ymin = ymin;
	m_ymax = ymax;

	m_xy_ratio = (xmax - xmin)/(ymax - ymin);
	m_yx_ratio = (ymax - ymin)/(xmax - xmin);

}

inline double interval_map_t::x_to_y(double x) {
	double y = ((x - m_xmin) * m_yx_ratio) + m_ymin;
	return y;
}

inline double interval_map_t::y_to_x(double y) {
	double x = ((y - m_ymin) * m_yx_ratio) + m_xmin;
	return x;
}


}}  //namespaces

#endif