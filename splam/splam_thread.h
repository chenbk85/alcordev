#ifndef SPLAM_THREAD_H
#define SPLAM_THREAD_H
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

//fwd
class splam_thread_impl;
  
/**
 *  executes splam activities.
 *  sample main:
 *
 *  int main()
 *  {
 *    all::splam::splam_thread    splam;
 *    splam.run();
 *    splam.run_server();
 *    
 *    ...
 *    
 *    splam.stop_server();
 *    splam.stop();
 *  }
 */

class splam_thread
{
public:	
	explicit splam_thread(char* name = "./config/splam.ini");
	~splam_thread();
	void	run();
	void	stop();
	void	start_server();
	void	stop_server();

private:
	splam_thread_impl* pImpl_;
};

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
#endif