#include <algorithm>
#include "value_iteration.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
#define min(x, y)	(((x)<(y)) ? (x) : (y))
#define max(x, y)	(((x)>(y)) ? (x) : (y))
//---------------------------------------------------------------------------
#define	INFINITO			999999999
#define	MENODIINFINITO		999999990
#define	POCO				0.00001
#define	RADICEDIDUE			1.41421356
#define	GRAD_TO_RAD			0.0174532925
#define	RAD_TO_GRAD			57.29577957855
//---------------------------------------------------------------------------
#define MapXY(row,col) ((row)*size_.col_ + (col))
//---------------------------------------------------------------------------
#define Umed(a,b,c)	(\
						(((a)>(b)) && ((a)>(c))) ? \
						max((b),(c)) : \
						( \
							(((a)<(b)) && ((a)<(c))) ? \
							min((b),(c)) : \
							a\
						)\
					)
//---------------------------------------------------------------------------
#define LOOP_UPDATE(row,col)	{\
								change = true;\
								_startx = Umed(0,(row)-1,_startx);\
								_endx = Umed(_endx,(row)+1,sizexx);\
								_starty = Umed(0,(col)-1,_starty);\
								_endy = Umed(_endy,(col)+1,sizeyy);\
							}
							/*	non capisco perchè non posso sostituire la:
								_startx = Umed(0,(row)-1,_startx);\
								
								con:
								if((row)>0)\
									_startx = min((row)-1,_startx);\
								else\
									_startx = 0;\
							*/
//---------------------------------------------------------------------------
namespace all{
namespace util{

const value_iteration_node errorNode={INFINITO,INFINITO};

value_iteration::value_iteration()
	:data_(0)
	,loop_done_(false)
	,dist_(0)
{
}

value_iteration::value_iteration(size_t rows, size_t cols, int cost)
	:data_(0)
	,loop_done_(false)
	,dist_(cost)
	,size_(rows,cols)
{
	create(rows,cols,cost);
}

value_iteration::~value_iteration()
{
	if(data_)
		kill();
}

bool value_iteration::create(size_t rows, size_t cols, int cost)
{
	return create(size2d(rows,cols),cost);
}

bool value_iteration::create(size2d dim, int cost)
{
	kill();

	if((data_ = new value_iteration_node[dim.row_*dim.col_])==NULL)
		return false;
	
	size_ = dim;
	dist_ = cost;
	loop_done_ = false;

	return true;
}

bool value_iteration::kill()
{
	if(!data_)
		return false;

	delete[] data_;
	data_ = 0;
	loop_done_ = false;
	return true;
}

bool value_iteration::in_map(size_t row, size_t col) const
{
	return (row<size_.row_) && (col<size_.col_);
}

value_iteration_node value_iteration::get_row_col(size_t row, size_t col) const 
{
	return data_[MapXY(row,col)];
}

void value_iteration::set_row_col(size_t row, size_t col, int val, int cost)
{
	size_t index = MapXY(row,col);
	data_[index].value = val;
	data_[index].cost = cost;
}

void value_iteration::set_untouch_area(size_t row, size_t col, int radius, bool alsoZeroes)
{
	for(size_t i= max(0,row-radius);i<min(size_.row_-1,row+radius);i++)
		for(size_t j= max(0,col-radius);j<min(size_.col_-1,col+radius);j++)
			if(get_row_col(i,j).value!=0)
				set_row_col(i,j,INFINITO,INFINITO);
}

bool value_iteration::update(size_t row, size_t col)
{
	value_iteration_node node = get_row_col(row,col);

	if(node.value>MENODIINFINITO)
		return false;

	if(node.value == 0)
		return false;

	size_t newx,newy;
	bool returnValue=false;
	int occval = node.cost;

	for(newx=max(row-1,0); newx<min(row+2,size_.row_);++newx)
		for(newy=max(col-1,0);newy<min(col+2,size_.col_);++newy)
		{
			int temp = get_row_col(newx,newy).value + occval;
			if((newx!=row)&&(newy!=col))
				temp+=static_cast<int>(static_cast<double>(dist_)*RADICEDIDUE);
			else
				temp+=dist_;
			if(temp<node.value)
			{
				set_row_col(row,col,temp,node.cost);
				node = get_row_col(row,col);
				returnValue=true;
			}
		}
	return returnValue;
}

value_iteration_info value_iteration::loop()
{
	value_iteration_info info;
	info.noProblems = false;
	if(!data_)
		return info;

	bool change = true;
	size_t startx=0;
	size_t starty=0;
	size_t endx = size_.row_-1;
	size_t endy = size_.col_-1;
	size_t sizexx = size_.row_-1;
	size_t sizeyy = size_.col_-1;
	size_t _startx=endx;
	size_t _starty=endy;
	size_t _endx=0;
	size_t _endy=0;
	size_t i,j;
	size_t iter=0;
	info.nStep=0;

	while(change)
	{
		//logg.log<<"  num:"<<iter++<<"  start:"<<startx<<","<<starty<<"  end:"<<endx<<","<<endy<<"  dimension:"<<(endx-startx+1)*(endy-starty+1) <<std::endl;
		change = false;
		switch(info.nStep%8)
		{
		case 0://X+ Y+
			for(i=startx;i<=endx;++i)
				for(j=starty;j<=endy;++j)
					if(update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 1://X- Y-
			for(i=endx;i>=startx;--i)
				for(j=endy;j>=starty;--j)
					if(update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 2://Y- X+
			for(j=endy;j>=starty;--j)
				for(i=startx;i<=endx;++i)
					if(update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 3://Y+ X-
			for(j=starty;j<=endy;++j)
				for(i=endx;i>=startx;--i)
					if(update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 4://Y+ X+
			for(j=starty;j<=endy;++j)
				for(i=startx;i<=endx;++i)
					if(update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 5://Y- X-
			for(j=endy;j>=starty;--j)
				for(i=endx;i>=startx;--i)
					if(update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 6://X+ Y-
			for(i=startx;i<=endx;++i)
				for(j=endy;j>=starty;--j)
					if(update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 7://X- Y+
			for(i=endx;i>=startx;--i)
				for(j=starty;j<=endy;++j)
					if(update(i,j))
						LOOP_UPDATE(i,j)
			break;
		}
		++info.nStep;
		startx=_startx;
		starty=_starty;
		endx=_endx;
		endy=_endy;
		_startx=sizexx;
		_starty=sizeyy;
		_endx=0;
		_endy=0;
	}
	info.noProblems = true;
	loop_done_ = true;
	return info;
}

bool value_iteration::get_path(size2d start, value_iteration_path &path)
{
	int aaa=0;

	if(!data_)
		return false;
	if(!loop_done_)
		return false;

	path.clear();
	//cout << "path pulito!"<<endl;
	path.push_back(start);
	//cout << "pushato l'inizio!"<<endl;
	size2d actualCoord = start;
	while(get_row_col(actualCoord.row_, actualCoord.col_).value != 0)
	{
		//cout << "inizio ciclo: "<<aaa++<<endl;
		int minim=get_row_col(actualCoord.row_, actualCoord.col_).value;
		size2d minicoord = actualCoord;
		for(size_t i=actualCoord.row_-1; i<=actualCoord.row_+1; ++i)
			for(size_t j=actualCoord.col_-1; j<=actualCoord.col_+1; ++j)
				if(in_map(i,j))
					if(get_row_col(i,j).value<minim)
					{
						minim = get_row_col(i,j).value;
						minicoord.row_ = i;
						minicoord.col_ = j;
					}
		actualCoord = minicoord;
		path.push_back(actualCoord);
		//cout << "aggiunto un altro elemento al goalpath!"<<endl;
		if(path.size() > size_.row_+size_.col_)
			return false;
	}

	return true;
}

}//namespace util
}//namespace all
//--------------------------------------------------------------------------------
