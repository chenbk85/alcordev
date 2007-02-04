#ifndef i_serializable_H_INLCUDED
#define i_serializable_H_INLCUDED
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
namespace all{
	namespace core{
//---------------------------------------------------------------------------
class  i_serializable
{
public:
	virtual void import(void*)= 0;
	virtual void pack(void* )  = 0; 
};
//---------------------------------------------------------------------------
	}//namespace
}//namespace
//---------------------------------------------------------------------------
#endif //i_serializable_H_INLCUDED