//---------------------------------------------------------------------------
#include <algorithm>
#include "ValueIteration.h"
using namespace std;
//---------------------------------------------------------------------------
#define min(x, y)	((x<y)?x:y)
#define max(x, y)	((x>y)?x:y)
//---------------------------------------------------------------------------
#define	INFINITO			999999999
#define	MENODIINFINITO		999999990
#define	POCO				0.00001
#define	RADICEDIDUE			1.41421356
#define	GRAD_TO_RAD			0.0174532925
#define	RAD_TO_GRAD			57.29577957855
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
#define LOOP_UPDATE(xx,yy)	{\
								change = true;\
								_startx = Umed(0,(xx)-1,_startx);\
								_endx = Umed(_endx,(xx)+1,sizexx);\
								_starty = Umed(0,(yy)-1,_starty);\
								_endy = Umed(_endy,(yy)+1,sizeyy);\
							}
							/*	non capisco perchè non posso sostituire la:
								_startx = Umed(0,(xx)-1,_startx);\
								
								con:
								if((xx)>0)\
									_startx = min((xx)-1,_startx);\
								else\
									_startx = 0;\
							*/
//---------------------------------------------------------------------------
const ValueIterationNode errorNode={INFINITO,INFINITO};
//---------------------------------------------------------------------------
ValueIteration::ValueIteration()
{
	data = 0;
	loopDone = false;
	size.first = 0;
	size.second = 0;
	distCell=0;
}
//---------------------------------------------------------------------------
ValueIteration::ValueIteration(int xx, int yy, int cost)
{
	data = 0;
	loopDone = false;
	size.first = xx;
	size.second = yy;
	distCell = cost;
	Create(xx,yy,cost);
}
//---------------------------------------------------------------------------
ValueIteration::~ValueIteration()
{
	if(data)
		Kill();
}
//---------------------------------------------------------------------------
bool ValueIteration::Create(int xx, int yy, int dist)
{
	size2_t temp;
	temp.first = xx;
	temp.second = yy;
	return Create(temp,dist);
}
//---------------------------------------------------------------------------
bool ValueIteration::Create(size2_t dim, int dist)
{
	Kill();

	if((data = new ValueIterationNode[dim.first * dim.second])==NULL)
		return false;
	
	size = dim;
	distCell = dist;
	loopDone = false;

	return true;
}
//---------------------------------------------------------------------------
bool ValueIteration::Kill()
{
	if(!data)
		return false;

	delete[] data;
	data = 0;
	loopDone = false;
	return true;
}
//---------------------------------------------------------------------------
bool ValueIteration::CorrectCoord(int xx, int yy)
{
	if(xx<0)return false;
	if(yy<0)return false;
	if(xx>=size.first)return false;
	if(yy>=size.second)return false;
	return true;
}
//---------------------------------------------------------------------------
ValueIterationNode ValueIteration::GetXY(int xx, int yy)
{
	return data[MapXY(xx,yy)];
}
//---------------------------------------------------------------------------
void ValueIteration::SetXY(int xx, int yy, int val, int cost)
{
	data[MapXY(xx,yy)].value = val;
	data[MapXY(xx,yy)].cost = cost;
}
//---------------------------------------------------------------------------
void ValueIteration::SetUntouchArea(int xx, int yy, int radius, bool alsoZeroes)
{
	for(int i= max(0,xx-radius);i<min(size.first-1,xx+radius);i++)
		for(int j= max(0,yy-radius);j<min(size.second-1,yy+radius);j++)
			if(GetXY(i,j).value!=0)
				SetXY(i,j,INFINITO,INFINITO);
}
//---------------------------------------------------------------------------
bool ValueIteration::Update(int xx, int yy)
{
	ValueIterationNode node = GetXY(xx,yy);

	if(node.value>MENODIINFINITO)
		return false;

	if(node.value == 0)
		return false;

	int newx,newy;
	bool returnValue=false;
	int occval = node.cost;
	for(newx=max(xx-1,0); newx<min(xx+2,size.first);++newx)
		for(newy=max(yy-1,0);newy<min(yy+2,size.second);++newy)
		{
			int temp = GetXY(newx,newy).value + occval;
			if((newx!=xx)&&(newy!=yy))
				temp+=distCell*RADICEDIDUE;
			else
				temp+=distCell;
			if(temp<node.value)
			{
				SetXY(xx,yy,temp,node.cost);
				node = GetXY(xx,yy);
				returnValue=true;
			}
		}
	return returnValue;
}
//---------------------------------------------------------------------------
ValueIterationInfo ValueIteration::Loop()
{
	ValueIterationInfo info;
	info.noProblems = false;
	if(!data)
		return info;
	bool change = true;
	int startx=0;
	int starty=0;
	int endx = size.first-1;
	int endy = size.second-1;
	int sizexx = size.first-1;
	int sizeyy = size.second-1;
	int _startx=endx;
	int _starty=endy;
	int _endx=0;
	int _endy=0;
	info.nStep=0;
	int i,j;
	int iter=0;
	while(change)
	{
		//logg.log<<"  num:"<<iter++<<"  start:"<<startx<<","<<starty<<"  end:"<<endx<<","<<endy<<"  dimension:"<<(endx-startx+1)*(endy-starty+1) <<std::endl;
		change = false;
		switch(info.nStep%8)
		{
		case 0://X+ Y+
			for(i=startx;i<=endx;++i)
				for(j=starty;j<=endy;++j)
					if(Update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 1://X- Y-
			for(i=endx;i>=startx;--i)
				for(j=endy;j>=starty;--j)
					if(Update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 2://Y- X+
			for(j=endy;j>=starty;--j)
				for(i=startx;i<=endx;++i)
					if(Update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 3://Y+ X-
			for(j=starty;j<=endy;++j)
				for(i=endx;i>=startx;--i)
					if(Update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 4://Y+ X+
			for(j=starty;j<=endy;++j)
				for(i=startx;i<=endx;++i)
					if(Update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 5://Y- X-
			for(j=endy;j>=starty;--j)
				for(i=endx;i>=startx;--i)
					if(Update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 6://X+ Y-
			for(i=startx;i<=endx;++i)
				for(j=endy;j>=starty;--j)
					if(Update(i,j))
						LOOP_UPDATE(i,j)
			break;

		case 7://X- Y+
			for(i=endx;i>=startx;--i)
				for(j=starty;j<=endy;++j)
					if(Update(i,j))
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
	loopDone = true;
	return info;
}
//---------------------------------------------------------------------------
bool ValueIteration::GetPath(size2_t start, ValueIterationPath &path)
{
	int aaa=0;

	if(!data)
		return false;
	if(!loopDone)
		return false;

	path.clear();
	//cout << "path pulito!"<<endl;
	path.push_back(start);
	//cout << "pushato l'inizio!"<<endl;
	size2_t actualCoord = start;
	while(GetXY(actualCoord.first,actualCoord.second).value != 0)
	{
		//cout << "inizio ciclo: "<<aaa++<<endl;
		int minim=GetXY(actualCoord.first,actualCoord.second).value;
		size2_t minicoord = actualCoord;
		for(int i=actualCoord.first-1;i<=actualCoord.first+1;i++)
			for(int j=actualCoord.second-1;j<=actualCoord.second+1;j++)
				if(CorrectCoord(i,j))
					if(GetXY(i,j).value<minim)
					{
						minim = GetXY(i,j).value;
						minicoord.first = i;
						minicoord.second = j;
					}
		actualCoord = minicoord;
		path.push_back(actualCoord);
		//cout << "aggiunto un altro elemento al goalpath!"<<endl;
		if(path.size() > size.first+size.second)
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------
