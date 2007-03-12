//--------------------------------------------------------------------------------
#ifndef ValueIterationH
#define ValueIterationH
//--------------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include "alcor/extern/pmap/slap.h"
#include "alcor/types/defs.h"
//--------------------------------------------------------------------------------
//	DEFINE
//--------------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	DICHIARAZIONE CLASSI
//---------------------------------------------------------------------------
class	ValueIteration;
struct	ValueIterationInfo;
struct	ValueIterationNode;
typedef	std::vector<size2_t>			ValueIterationPath;
typedef	ValueIterationPath::iterator	ValueIterationPathIt;
//---------------------------------------------------------------------------
//	DEFINIZIONE CLASSI:	STRUCT ValueIterationInfo
//---------------------------------------------------------------------------
struct	ValueIterationInfo
{
	//da completare con le informazioni che ritengo utili 
	int		nStep;///<numero di passi impiegati
	long	mSecElapsed;///<tempo impiegato
	bool	noProblems;///<indica se la procedura di Loop è andata a buon fine
};
//---------------------------------------------------------------------------
//	DEFINIZIONE CLASSI:	STRUCT ValueIterationNode
//---------------------------------------------------------------------------
struct	ValueIterationNode
{
	int	value;
	int	cost;
};
//---------------------------------------------------------------------------
//	DEFINIZIONE CLASSI:	CLASSE ValueIteration
//---------------------------------------------------------------------------
/**	\image html USTlogo.jpg*/
class	ValueIteration
{
public:	
	ValueIteration();
	ValueIteration(int xx, int yy, int cost);
	~ValueIteration();///<	Distruttore
	bool					Create(int xx, int yy, int dist);
	bool					Create(size2_t dim, int dist);
	bool					Kill();
	bool					CorrectCoord(int xx, int yy);///<Restituisce true se la coppia (xx,yy) può essere indirizzo di cella
	inline int					MapXY(int xx, int yy){return (yy*size.first + xx);}///<Funzione che restituisce lo spiazzamento fisico per accedere alla cella (x,y).
	inline ValueIterationNode	GetXY(int xx, int yy);///<	Funzione Rapidissima d'accesso alla mappa in lettura
	inline void				SetXY(int xx, int yy, int val, int cost=0);///<	Funzione Rapidissima d'accesso alla mappa in scrittura
	void					SetUntouchArea(int xx, int yy, int radius, bool alsoZeroes=false);
	ValueIterationInfo		Loop();
	inline size2_t			GetSize(){return size;}
	bool					GetPath(size2_t start, ValueIterationPath& path);

protected:
	bool					Update(int xx, int yy);///<	Funzione per l'aggiornamento delle singole celle

protected:
	ValueIterationNode*		data;
	int						distCell;
	size2_t					size;
	bool					loopDone;
};
//--------------------------------------------------------------------------------
#endif

