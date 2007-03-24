#ifndef	point2d_H
#define	point2d_H
//-----------------------------------------------------------------------------------------------
#include <iostream>
#include <cmath>
#include "angle.h"
//-----------------------------------------------------------------------------------------------
namespace all{
namespace math{

//fwd declarations
class	point2d;
const	point2d operator+(const point2d&, const point2d&);
const	point2d	operator-(const point2d&, const point2d&);
const	point2d	operator*(const point2d&, double);
const	point2d	operator*(double, const point2d&);

/**
 *	class point2d:
 *	no copy assignment, copy constructor e destructor... cause this is a value-like class
 *	no automatic conversion FROM and TO any builtin...
 *	internal data are double with meaning METRES
 *	operator += and -= are members
 *	operator + and - are not members
 *	operator /= and *= are members and we intend point /= (or *=) scalar value
 *	operator * is not member cause you can have point2d*double or vice versa
 *	operator / is member because you could have only point2d/double...
 */


class point2d
{

public:	//ctors
	point2d()
		:x_(0.0)
		,y_(0.0)
	{
	}
	
	point2d(double a, double b)
		:x_(a)
		,y_(b)
	{
	}
	
	point2d( double mod, angle a)
		:x_(mod*a.cos())
		,y_(mod*a.sin())
	{
	}

public:	//setters and getters
	double	getX() const {return x_;}
	double	getY() const {return y_;}
	
	double	get_x1() const {return x_;}
	double	get_x2() const {return y_;}

	void	setX(double temp){x_ = temp;}
	void	setY(double temp){y_ = temp;}

	void	set_x1(double val){x_ = val;}
	void	set_x2(double val){y_ = val;}

	void	reset(){x_=0.0; y_=0.0;}

  //(re-set the point. modulo e angolo)
  void  set(double mod, angle dir)
    {
      x_ = mod*dir.cos();
      y_ = mod*dir.sin();
    }

public:	//unary operators
	const point2d	operator-() const {return point2d(-x_,-y_);}

public:	//binary operators
	point2d&	operator+=(const point2d& temp){x_+=temp.x_;y_+=temp.y_;return *this;}
	point2d&	operator-=(const point2d& temp){x_-=temp.x_;y_-=temp.y_;return *this;}
	point2d&	operator*=(double temp){x_*=temp;y_*=temp;return *this;}

	point2d&	operator/=(double temp)
	{
		if(!temp)
			throw std::runtime_error("division by ZERO in all::math::point2d::operator/=(double) *_*");
		x_/=temp;
		y_/=temp;
		return *this;
	}

	const point2d	operator/(double temp) const 
	{
		if(!temp)
			throw std::runtime_error("division by ZERO in all::math::point2d::operator/(double) *_*");
		return point2d(x_/temp , y_/temp);
	}

public:	//tools
	double			magnitude()		const	{return double(sqrt((x_*x_ + y_*y_)));}
	const	angle	orientation()	const	{return angle(atan2(y_,x_), rad_tag);}
	const	point2d	unit()	const			
	{
		double temp = magnitude();
		if(temp == 0)
			throw std::runtime_error("division by ZERO in all::math::point2d::unit() *_*");
		return *this/temp;
	}
	point2d&		resize(double temp)
	{
		*this = unit()*temp;
		return *this;
	}
	const	point2d	rotate(const angle& temp) const
	{
		//unoptimized but "clear"
		//return point2d( magnitude() , temp + orientation() );

		//optimized 
		return point2d( x_*temp.cos() - y_*temp.sin() , x_*temp.sin() + y_*temp.cos() );
	}
	point2d&		rotateApply(const angle& temp)	{*this = rotate(temp); return *this;}

public:	//friends
	friend	std::ostream&	operator<<(std::ostream& os, const point2d& temp);
	friend	std::istream&	operator>>(std::istream& is, point2d& temp);
	friend	double			dotproduct(const point2d&, const point2d&);

private:	//data stored as a pair of double
	double	x_;
	double	y_;

};//class point2d

inline std::ostream&	operator<<(std::ostream& os, const point2d& temp)
{
	os<< temp.x_ << " "<< temp.y_;
	return os;
}

inline std::istream&	operator>>(std::istream& is, point2d& temp)
{
	is >> temp.x_ >> temp.y_; 
	return is;
}
	
const	inline point2d operator-(const point2d& first, const point2d& second)
{
	point2d ret(first);
	ret -= second;
	return ret;
}
const	inline point2d operator+(const point2d& first, const point2d& second)
{
	point2d ret(first);
	ret += second;
	return ret;
}

const	inline point2d	operator*(const point2d& p, double temp)
{
	point2d ret(p);
	ret *= temp;
	return ret;
}

const	inline point2d	operator*(double temp, const point2d& p)
{
	point2d ret(p);
	ret *= temp;
	return ret;
}

inline	double	dotproduct(const point2d& a, const point2d& b)
{
	return a.x_*b.x_ + a.y_*b.y_;
}

//to do *_*
	//bool	InPoly(std::vector<point2d> *Vect, bool circle=true) const; ///<Controllo se il punto è nel poligono
	///**<Il parametro \a circle serve per dichiarare se il poligono è chiuso o bisogna aggiungere l'origine (0,0).
	//*/
	//double	Dist(std::list<pose2d> *l) const;///<minima distanza da un'insieme di punti
	//double	Dist(std::vector<point2d> *p, bool circle=true, bool open=false) const;///<Distanza del punto da un poligono
	///**<Il poligono è definito dal vettore di vertici. Il parametro \a circle serve per dichiarare
	//	se il poligono è chiuso oppure bisogna aggiungere l'origine (0,0)
	//	Il parametro \a open serve per dichiarare se si tratta di una spezzata.
	//	Ovviamente non ha senso che entrambi valgano \a true ma le tre altre possibili configurazioni
	//	hanno senso:
	//	- \a false \a false è un poligono con un vertice aggiunto nell'origine
	//	- \a false \a true è una spezzata
	//	- \a true \a false è un poligono chiuso e definito dai soli vertici (default)

	//	per completezza, il caso \a true \a true lo trattiamo come di default.*/

/*
	const	point2d	point2d::RelToAss(const pose2d& sRif) const
	//  trasforma le coordinate di un punto da un sistema relativo ad uno assoluto
	{
	    return  sRif.getP() + point2d( modulo() , sRif.getTh() + angolo());
	}
	double	point2d::Dist(std::list<pose2d> *l) const
	{
		double minimo = INFINITO;
		double temp;
		std::list<pose2d>::iterator it;
		for(it=l->begin(); it!=l->end(); ++it)
		{
			temp = Dist(it->getP());
			if(temp<minimo)
				minimo = temp;
		}
		return minimo;
	}
	double	point2d::Dist(std::vector<point2d> *p, bool circle, bool open) const
	//	la distanza di un punto P da un poligono e' la minima distanza tra il
	//	punto ed uno qualsiasi dei suoi lati
	{
		double dmin,temp;
		if(circle)
			dmin=Dist(line2d((*p)[p->size()-1],(*p)[0]));
		else
		{
			if(!open)
				dmin = std::min( Dist(line2d((*p)[p->size()-1],point2d(0,0))) , Dist(line2d((*p)[0],point2d(0,0))) );
			else
				dmin = INFINITO;
		}
		for(unsigned int i=0;i<p->size()-1;++i)
			if((temp=Dist(line2d((*p)[i],(*p)[i+1])))< dmin)
				dmin=temp;
			return dmin;
	}
	bool	point2d::InPoly(std::vector<point2d> *Vect, bool circle) const
	//	ritorna true se il punto e' interno al poligono,false se e' esterno
	//	da vedere anche sta soluzione proposta da un tale Randolph Franklin (da convertire in c++ e usare bool invece di int)
	//	http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	//	int pnpoly(int npol, float *xp, float *yp, float x, float y)
	//  {
	//		int i, j, c = 0;
	//		for (i = 0, j = npol-1; i < npol; j = i++) 
	//		{
	//			if	((((yp[i]<=y) && (y<yp[j])) ||
	//				((yp[j]<=y) && (y<yp[i]))) &&
	//				(x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))
	//				c = !c;
	//		}
	//		return c;
	//  }
	{
		int count=0;
		line2d rif(*this , point2d( INFINITO , y_+1)); //punto fuori dell'area di interesse
		for(unsigned int i=0;i< Vect->size()-1;++i)
			if( line2d((*Vect)[i],(*Vect)[i+1]) / rif )
				count++;
	
		if(circle)
		{
			if( line2d((*Vect)[Vect->size()-1],(*Vect)[0]) / rif )
				count++;
		}
		else
		{
			if( line2d((*Vect)[Vect->size()-1],point2d(0,0)) / rif )
				count++;
			if( line2d(point2d(0,0),(*Vect)[0]) / rif )
				count++;
		}
		
		if((count%2) == 0)return false;
		return true;
	}
*/


}//namespace math
}//namespace all
//-----------------------------------------------------------------------------------------------
#endif //point2d_H