#ifndef i_observer_H_INLCUDED
#define i_observer_H_INLCUDED
//--------------------------------------------------------------------------+
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
//#include <boost\foreach.hpp>
#include <algorithm>
#include <boost\bind.hpp>
//--------------------------------------------------------------------------+
namespace all {
	namespace core {
//--------------------------------------------------------------------------+
class i_observer;
//--------------------------------------------------------------------------+
class i_observable
{
public:
	i_observable(){};
  typedef vector<i_observer*>  observers_vect_t;
public:
	virtual void subscribe(i_observer*);

public:
	virtual void notify(void* data = NULL);

private:
	observers_vect_t observers;
};
//--------------------------------------------------------------------------+
class i_observer
{
public:
	i_observer(){};

public:
	virtual void get_notified(void*)=0;

};
//--------------------------------------------------------------------------+
inline void i_observable::notify(void* msg)
{

  std::for_each(
    observers.begin()
    , observers.end()
    , boost::bind(&i_observer::get_notified, _1, msg)
    );
  
}
//--------------------------------------------------------------------------+
//--------------------------------------------------------------------------+
inline void i_observable::subscribe(i_observer* ob)
{
	cout << "Listener Subscribed" << endl;
	observers.push_back(ob);
}
//--------------------------------------------------------------------------+
		}
	}
//--------------------------------------------------------------------------+
#endif //i_observer_H_INLCUDED
