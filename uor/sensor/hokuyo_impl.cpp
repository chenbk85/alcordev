/*
 *  hokuyo.cpp
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 08/10/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#include <boost/config.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <vector>

using namespace boost;


namespace  uor {
	
	struct hokuyo_impl {
		
		//BOOST_STATIC_CONSTANT(std::size_t, buffer_size = 512);
		
		enum {buffer_size = 512};
		
		///ctor
		hokuyo_impl() 
		: port_(io_service_)
		, timer_(io_service_) 
		, inbuffer_(buffer_size)      
		{
			//port_.set_option(boost::asio::serial_port_base::baud_rate::baud_rate(115200));
		}
		
		///CONNECT
		bool connect(const std::string& sport) {
			try {
				port_.open(sport.c_str()); 
				return true;
			}
			catch (std::exception& error) {
				
				std::cout 
				<< std::endl
				<< "OUCH! hokuyo_impl CTOR" << std::endl
				<< error.what() << std::endl
				<< sport
				<< std::endl << std::endl;
				return false;
			}
		}
		
		///
		bool initialize () {
			
			//boost::asio::async_read(port_,
			//						boost::asio::buffer(inbuffer_, 1),
			//						boost::bind(&hokuyo_impl::handle_read, this
			//									, boost::asio::placeholders::error
			//						));
			
			//
			boost::asio::async_read_until(  port_
										  , rbuffer_
										  , "\n\n"
										  , boost::bind(&hokuyo_impl::handle_read_until
														, this
														, boost::asio::placeholders::error
														)
										  );

			//port_.async_read_some(boost::asio::buffer(inbuffer_, 128)
			//						, bind(&hokuyo_impl::handle_read_some
			//							, this
			//							, asio::placeholders::error
			//							, asio::placeholders::bytes_transferred)
			//							);

			//
			ios_thread_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
			//
			write("SCIP2.0\n");	
			Sleep(100);
			write("BM\n");
			return true;
		}
		
		///
		void close()
		{
			io_service_.post(boost::bind(&hokuyo_impl::do_close, this));
		}
		
		//[]
		///
		void write (const std::string& msg)
		{
			io_service_.post(boost::bind(&hokuyo_impl::do_write, this, msg));
		}
		
		///
		void do_write(const std::string & msg)
		{
			std::cout
			<< "do_write: "
			<< msg << std::endl
			<< "length: " << msg.length() << std::endl
			<< std::endl;	
			
			boost::asio::async_write
			(
			 port_,
			 boost::asio::buffer(msg, msg.length()),
			 boost::bind(&hokuyo_impl::handle_write, this,
						 boost::asio::placeholders::error)
			 );
		}
		//[HANDLES] ---------------------------------------------------------
		///
		void handle_read(const boost::system::error_code& error) 
		{
			if (!error)
			{
				std::cout
				<< "HANDLE READ OK" << std::endl
				<< std::endl;
				boost::asio::async_read(
					port_,
					boost::asio::buffer(inbuffer_, 1),
						boost::bind(&hokuyo_impl::handle_read
						, this
						, boost::asio::placeholders::error)
				);
			}
			else {
				
				std::cout
				<< "[ERROR] HANDLE READ" << std::endl
				<< error.message() << std::endl
				<< "value: " << error.value() << std::endl
				<< std::endl;

				do_close();
			}
			
		}

		void handle_read_until(const boost::system::error_code& error) 
		{
			if (!error)
			{
				std::cout
				<< "HANDLE READ UNTIL OK" << std::endl
				<< &rbuffer_ 
				<< std::endl;

				boost::asio::async_read_until(  port_
							  , rbuffer_
							  , "\n\n"
							  , boost::bind(&hokuyo_impl::handle_read_until
											, this
											, boost::asio::placeholders::error
											)
							  );
			}
			else {
				
				std::cout
				<< "[ERROR] HANDLE READ UNTIL" << std::endl
				<< error.message() << std::endl
				<< "value: " << error.value() << std::endl
				<< &rbuffer_ 
				<< std::endl;

				do_close();
			}
			
		}
		///
				///
		void handle_read_some(const boost::system::error_code& error, std::size_t bytes_transferred)
		{
			if (!error)
			{
				std::cout
				<< "HANDLE READ SOME OK" << std::endl 
				<< "Bytes Transferred: " << bytes_transferred << std::endl
				<< std::endl;

			port_.async_read_some(boost::asio::buffer(inbuffer_, 128)
				, bind(&hokuyo_impl::handle_read_some
					, this
					, asio::placeholders::error
					, asio::placeholders::bytes_transferred)
					);
			}
			else
			{
				std::cout
				<< "[ERROR] HANDLE READ SOME" << std::endl
				<< error.message() << std::endl
				<< "value: " << error.value() << std::endl
				<< "Bytes Transferred: " << bytes_transferred << std::endl
				<< std::endl;

				do_close();
			}
		}

		///
		void handle_write(const boost::system::error_code& error)
		{
			if (!error)
			{
				std::cout << "HANDLE WRITE OK" << std::endl
				<< std::endl;		
			}
			else
			{
				std::cout << "[ERROR] HANDLE WRITE" << std::endl;
				do_close();
			}
		}
		
		//
		void sync_read()
		{
			std::cout << "sync_read" << std::endl;
			size_t	len = boost::asio::read_until(port_, rbuffer_, "\n\n");
			std::cout 
			<< "READ: " << len <<   std::endl
			<< std::endl;
			
		}
		
		///CLOSES SERIAL CONNECTION
		void do_close()
		{
			try{
				port_.cancel();
				
				port_.close();
				
				ios_thread_->join();
			}
			catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}
		
		
		//[MEMBER DATA] ---------------------------------------------------------
		///ioservice
		boost::asio::io_service io_service_;
		///timer
		boost::asio::deadline_timer timer_;
		///serial port
		boost::asio::serial_port port_;
		//		///read buffer
		boost::asio::streambuf rbuffer_;
		///ios thread
		boost::shared_ptr <boost::thread> ios_thread_;
		
		///
		//unsigned char inbuffer_ [buffer_size];
		std::vector <unsigned char> inbuffer_;
		
	};
	
}//close namespace

