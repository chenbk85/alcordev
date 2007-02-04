#include "alcor/act/p3dx_server_data_t.h"

#include "ArNetPacket.h"
//TODO:aggiustare ... mm -> mt
void all::act::p3dx_position_t::import(void* _pkt)
				{
          ArNetPacket* pkt = static_cast<ArNetPacket*>(_pkt);
          //this->setX(math::metric(pkt->bufToDouble(), math::metric::Millimeters()));
          this->set_x1( pkt->bufToDouble() );
          //this->setY(math::metric(pkt->bufToDouble(), math::metric::Millimeters()));
          this->set_x2( pkt->bufToDouble() );
          //this->setTh(math::angle(pkt->bufToDouble(), math::deg_t) );
          this->set_th(pkt->bufToDouble(), math::deg_tag);
				};

  void all::act::p3dx_position_t::pack(void* _pkt) 
				{
          ArNetPacket* pkt = static_cast<ArNetPacket*>(_pkt);

          pkt->doubleToBuf( get_x1() );
          pkt->doubleToBuf( get_x2() );
          pkt->doubleToBuf( get_th(math::deg_tag) );

					pkt->finalizePacket();
				}; 

    //void all::act::p3dx_position_t::pack(void* _pkt) 
				//{
    //      ArNetPacket* pkt = static_cast<ArNetPacket*>(_pkt);

    //      pkt->doubleToBuf( get_x1(math::mm_t)  );
    //      pkt->doubleToBuf( get_x2(math::mm_t)  );
    //      pkt->doubleToBuf( get_th(math::deg_t) );

				//	pkt->finalizePacket();
				//}; 