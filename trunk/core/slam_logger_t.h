#ifndef slam_data_log_t_H_INCLUDED
#define slam_data_log_H_INCLUDED
// --------------------------------
#include <iostream>
#include <fstream>
#include "alcor/core/slam_data_log_t.h"
//---------------------------------

namespace all{
	namespace core{
class slam_logger_t
{
public:
	slam_logger_t(const std::string& fname = "general_data_log.bin",
					bool laserP = false, 
					bool odoP = false, 
					bool inertialP = false,
					const int maxLaserScan = all::sense::urg_scan_data_t::MAX_NUM_POINTS): // ctor
		logFileName_(fname),
		numSamples_(0),
		laserPresent_(laserP),
		odoPresent_(odoP),
		inertialPresent_(inertialP),
		numLaserScans_(maxLaserScan)
	{
		logFile_.open(logFileName_.c_str(),std::ios::out|std::ios::binary);
		chunkSize_ = sizeof(loggedData_.timestamp)+
					 sizeof(loggedData_.odo) +
					 sizeof(loggedData_.inertialSensor) +
					 sizeof(loggedData_.laserData) * maxLaserScan;
					 ; // Save the chunk size as slam_data_log_t struct size
		headerSize_ = sizeof(numSamples_) + 
					  sizeof(chunkSize_) +
					  sizeof(laserPresent_) +
					  sizeof(odoPresent_) +
					  sizeof(inertialPresent_) +
					  sizeof(numLaserScans_); // Save header size
		logFile_.seekp((std::streamoff) headerSize_);
	}

	~slam_logger_t() // dtor
	{
		printf("writing down the header\n");
		write_header_();
		printf("closing the binary log\n");
		logFile_.close(); 
	}

	void add_data_(const slam_data_log_t& data){
		/* 
			The add function insert this sequence of values :
			1) timestamp
			2) laser datas
			3) odometry datas
			4) inertial datas
		*/
		logFile_.write((char *) &data.timestamp, sizeof(data.timestamp));
		logFile_.write((char *) &data.laserData->scan_points[0], sizeof(sense::urg_scan_data_t::value_type)*numLaserScans_);
		logFile_.write((char *) &data.odo, sizeof(data.odo));
		logFile_.write((char *) &data.inertialSensor, sizeof(data.inertialSensor));
		numSamples_ ++;
	}

private:

	void write_header_(){
		logFile_.seekp(std::ios::beg); // Go to the beginning of the file
		// Writing the header
		logFile_.write( (char*) &numSamples_, sizeof(numSamples_)); // writing the number of chunk recorded
		logFile_.write( (char*) &chunkSize_, sizeof(chunkSize_)); // writing the size of each chunk
		logFile_.write( (char*) &laserPresent_, sizeof(laserPresent_)); // writing laser presence boolean
		logFile_.write( (char*) &odoPresent_, sizeof(odoPresent_)); // writing odometry presence boolean
		logFile_.write( (char*) &inertialPresent_, sizeof(inertialPresent_)); // writing inertial sensor presence boolean
		logFile_.write( (char*) &numLaserScans_, sizeof(numLaserScans_)); // writing number of laser scans 

		std::cout << "-- Header scritto --" << std::endl;
		std::cout << "-- Numero di campioni : " << numSamples_ << std::endl;

	}

	std::fstream logFile_; // pointer to stream log file
	std::string logFileName_; // name of log file

	size_t numSamples_; // Samples recorded into log file
	size_t headerSize_; // Size of the header
	size_t chunkSize_; // Chunk size

	bool laserPresent_; // true if laser is logged, false otherwise
	bool odoPresent_; // true if robot odometry is logged, false otherwise
	bool inertialPresent_; // true if inertial sensor is logged, false otherwise

	int numLaserScans_; // number of laser scans

	slam_data_log_t loggedData_; // struct keeping data values
	
};
	}} // closing all::core
#endif