#ifndef SPLAMDATANET_H
#define	SPLAMDATANET_H
//---------------------------------------------------------------------------
#include <Aria.h>
#include <Arnetworking.h>
#include <boost/shared_ptr.hpp>
#include "alcor/splam/splam_data.h"
#include "alcor.extern/compression/Codec/RLE.h"
#include "alcor.extern/compression/Generic.h"
//---------------------------------------------------------------------------
namespace all{
namespace splam{

/**	La classe splam_data_net è nota al Server ed al Client dello SLAM per trasmettere lo splam_data via TCP
	offre i metodi di Serializzazione import e pack ed un header di controllo.

	L'header AL MOMENTO indica semplicemente se il dato è valido o no (0 = non valido... heart beat)
	
	Lo splam_data DEVE ESSERE ISTANZIATO DALL'ESTERNO... per cui non servono distruttori.
	*/
class	splam_data_net
{
public:
	enum
	{
		broken_data	=	0,
		valid_data	=	1,
	};

public:
	splam_data_net(size_t);
	~splam_data_net();

public:
	TGeneric<CRLE>*		runLenght_;
	splam_data_ptr		data_;
	BYTE				header_;
	BYTE*				target_;
	long				target_lenght;



public:
	void	import_og_map(ArNetPacket*);
	void	pack_og_map(ArNetPacket*);
	void	import_others(ArNetPacket*);
	void	pack_others(ArNetPacket*);
};

typedef boost::shared_ptr<splam_data_net>	splam_data_net_ptr;

}//namespace splam
}//namespace all
//---------------------------------------------------------------------------
#endif