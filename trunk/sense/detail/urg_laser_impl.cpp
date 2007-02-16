#include <alcor.extern/wxctb/serport.h>
#include <alcor.extern/wxctb/timer.h>

#include <vector>
#include <boost/function.hpp>

#include <alcor/core/iniWrapper.h>
#include <alcor/core/core.h>

#include "../urg_scan_data_t.hpp"


namespace all {
	namespace sense {
		namespace detail {



class urg_laser_impl {

public:
	static const int READ_WRITE_TIMEOUT = 2000;

	static const std::string URG_CMD_OK;
	static const std::string URG_DATA_OK;

	static const char URG_D_MODE = 'D';
	static const char URG_S_MODE = 'S';

	struct urg_reply_t {
		std::string command;
		std::string status;
		std::string data;
	};

public:
	urg_laser_impl(char*);
	~urg_laser_impl();

public:
	bool connect();
	void disconnect();

	//interface command
	urg_scan_data_ptr do_scan(int, int, int);
	urg_multi_scan_t do_multiple_scan(int, int, int, int, int);
	
	void set_d_mode();
	void set_s_mode();
	void laser_on();
	void laser_off();

	//internal command
	void scip2_mode();
	
	void send_command(std::string);
	void read_reply();

	int decode_sequence(std::string::iterator&, std::size_t);
	void decode_scan(urg_scan_data_ptr);

public:
	std::string m_urg_port;

	wxSerialPort m_urg;

	iniWrapper m_ini_config;

	urg_reply_t m_last_reply;

	timer m_io_timer;
	int m_timer_status;

	urg_multi_scan_t m_last_scan;

	char m_scan_mode;

	bool is_on;

	boost::function <void (urg_scan_data_ptr)> m_line_ready_cb;

};

const std::string urg_laser_impl::URG_CMD_OK = "00";
const std::string urg_laser_impl::URG_DATA_OK = "99";


urg_laser_impl::urg_laser_impl(char* ini_file):m_io_timer(READ_WRITE_TIMEOUT, &m_timer_status, NULL) {
	if (m_ini_config.Load(ini_file)) {
		m_urg_port = m_ini_config.GetStringAsChar("urg:port", "COM5");
	}
	else {
		printf("Unable to open configuration file %s\n", ini_file);
		m_urg_port = wxCOM6;
	}
	m_scan_mode = URG_D_MODE;
	is_on = false;

}

urg_laser_impl::~urg_laser_impl() {
	disconnect();
}

bool urg_laser_impl::connect() {
	if ( m_urg.Open(m_urg_port.c_str()) != -1) {
		printf("URG connected on port %s\n", m_urg_port.c_str());
		scip2_mode();
		laser_on();
		return true;
	}
	else {
		printf ("Unable to open port %s\n", m_urg_port.c_str());
		return false;
	}
}

void urg_laser_impl::disconnect() {
	m_urg.Close();
}

void urg_laser_impl::scip2_mode() {
	send_command("SCIP2.0");
	read_reply();
}

void urg_laser_impl::set_d_mode() {
	m_scan_mode = URG_D_MODE;
}

void urg_laser_impl::set_s_mode() {
	m_scan_mode = URG_S_MODE;
}

void urg_laser_impl::laser_on() {
	if (!is_on) {
		send_command("BM");
		read_reply();
		if (m_last_reply.status == URG_CMD_OK || m_last_reply.status == "02") {
			is_on=true;
		}
	}
}

void urg_laser_impl::laser_off() {
	if (is_on) {
		send_command("QT");
		read_reply();
		if (m_last_reply.status == URG_CMD_OK || m_last_reply.status == "02") {
			is_on=false;
		}
	}
}

urg_scan_data_ptr urg_laser_impl::do_scan(int start_step, int end_step, int cc) {
	
	if (!is_on) 
		laser_on();

	std::string command = "G" + all::core::make_string(m_scan_mode,1) 
						  + all::core::make_string(start_step, 4) 
						  + all::core::make_string(end_step, 4)
						  + all::core::make_string(cc, 2);

	send_command(command);

	read_reply();

	if (m_last_reply.status == URG_CMD_OK) {

		m_last_scan.scan_vec.reset(new urg_scan_data_ptr(new urg_scan_data_t()));
		m_last_scan.n_scan = 1;

		m_last_scan.scan_vec[0]->start_step = start_step;
		m_last_scan.scan_vec[0]->end_step = end_step;
		m_last_scan.scan_vec[0]->cc = cc;
		decode_scan(m_last_scan.scan_vec[0]);

		return m_last_scan.scan_vec[0];
	}
}

urg_multi_scan_t urg_laser_impl::do_multiple_scan(int start_step, int end_step, int cc, int n_scan, int interval) {
	
	if (!is_on)
		laser_on();

	std::string command = "M" + all::core::make_string(m_scan_mode,1) 
						  + all::core::make_string(start_step, 4) 
						  + all::core::make_string(end_step, 4)
						  + all::core::make_string(cc, 2) 
						  + all::core::make_string(interval, 1)
						  + all::core::make_string(n_scan, 2);

	send_command(command);

	read_reply();

	if (m_last_reply.status == URG_CMD_OK) {
        
		m_last_scan.scan_vec.reset (new urg_scan_data_ptr[n_scan]);
		m_last_scan.n_scan = n_scan;

		for (int i=0; i < n_scan; i++) {
			read_reply();

			if (m_last_reply.status == URG_DATA_OK) {

				m_last_scan.scan_vec[i].reset(new urg_scan_data_t());

				m_last_scan.scan_vec[i]->start_step = start_step;
				m_last_scan.scan_vec[i]->end_step = end_step;
				m_last_scan.scan_vec[i]->cc = cc;

				decode_scan(m_last_scan.scan_vec[i]);

				if (m_line_ready_cb)
					m_line_ready_cb(m_last_scan.scan_vec[i]);

			}
			else
				m_last_scan.scan_vec[i].reset();
		}
		return m_last_scan;
	}
}
	

void urg_laser_impl::send_command(std::string command) {
	
	if (m_urg.IsOpen()) {
		
		command+='\n';
		char* cmd = const_cast <char*> (command.c_str());
	
		m_timer_status = 0;

		m_io_timer.start();
		int wd = m_urg.Writev(cmd, command.length(), &m_timer_status);

	}
}

void urg_laser_impl::read_reply() {
		
	if (m_urg.IsOpen()) {
		
		char* reply_buf = NULL;
		int rd = m_urg.ReadUntilRegEx(&reply_buf, "\n\n", READ_WRITE_TIMEOUT);

		std::string reply = reply_buf;

		delete[] reply_buf;

		std::cout << reply;

		//divide reply in command, status and data (see protocol)
		std::string::size_type pos = reply.find('\n',0);
		m_last_reply.command = reply.substr(0,pos);
		m_last_reply.status = (reply.substr(pos+1,2));
		pos = reply.find('\n',pos+1);
		m_last_reply.data = reply.substr(pos+1, reply.length());

	}

}

int urg_laser_impl::decode_sequence(std::string::iterator& sequence_iterator, std::size_t length) {
	
	int value = 0;
	for (length; length>0; length--) {
		
		//jump sum and end of line fields
		if (*(sequence_iterator + 1) == '\n') 
			sequence_iterator+=2;

		//
		char tmp = *sequence_iterator;
		tmp-=0x30;
        value += (tmp << (6*(length-1)));
		sequence_iterator++;
	}
	return value;

}

void urg_laser_impl::decode_scan(urg_scan_data_ptr scan) {
	
	//number of char per range value
	int n_char;
	if (m_scan_mode == URG_D_MODE)
		n_char = 3;
	else
		n_char = 2;

	std::string::iterator data_iterator = m_last_reply.data.begin();
	scan->time_stamp = decode_sequence(data_iterator, 4);

	data_iterator+=2;
	
	std::string::iterator end_iterator = m_last_reply.data.end() - 3;

	int i = 128;
	while (data_iterator < end_iterator) {
		scan->scan_points.push_back(decode_sequence(data_iterator, n_char));
	}

}

}}} //namespaces