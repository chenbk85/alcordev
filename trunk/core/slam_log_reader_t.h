#ifndef slam_log_reader_t_H_INCLUDED
#define slam_log_reader_t_H_INCLUDED
// --------------------------------
#include <iostream>
#include <fstream>
#include "alcor/core/slam_data_log_t.h"
#include "alcor/sense/urg_scan_data_t.hpp"
//---------------------------------

namespace all{
	namespace core{

class slam_log_reader_t{

public :
	slam_log_reader_t(const slam_data_log_t& data, const std::string& fname = "general_data_log.bin"): //ctor
	   logFileName_(fname),
	   loggedData_(data)
	{
		chunkSize_ = sizeof(loggedData_); // Save the chunk size as slam_data_log_t struct size
		headerSize_ = sizeof(numSamples_) + 
					  sizeof(chunkSize_) +
					  sizeof(laserPresent_) +
					  sizeof(odoPresent_) +
					  sizeof(inertialPresent_) +
					  sizeof(numLaserScans_); // Save header size
		logFile_.open(logFileName_.c_str(),std::ios::in|std::ios::binary);
		logFile_.read((char *) &numSamples_,sizeof(numSamples_)); // read the number of samples recorded into the logfile
		std::cout << "Il numero dei campioni memorizzati e' : " << numSamples_ << std::endl;
		logFile_.read((char *) &chunkSize_,sizeof(chunkSize_)); // read the number of samples recorded into the logfile
		std::cout << "La dimensione di ogni chunk memorizzato e' : " << chunkSize_ << std::endl;
		logFile_.read((char *) &laserPresent_,sizeof(laserPresent_)); // read is laser was logged into the logfile
		std::cout << "Laser loggato : " << laserPresent_ << std::endl;
		logFile_.read((char *) &odoPresent_,sizeof(odoPresent_)); // read is odometry was logged into the logfile
		std::cout << "Odometria loggata : " << odoPresent_ << std::endl;
		logFile_.read((char *) &inertialPresent_,sizeof(inertialPresent_)); // read is inertial sensor was logged into the logfile
		std::cout << "Piattaforma inerziale loggata : " << inertialPresent_ << std::endl;
		logFile_.read((char *) &numLaserScans_,sizeof(numLaserScans_)); // read number of the laser Scan
		std::cout << "Numero di Scan : " << numLaserScans_ << std::endl;
	
		loggedData_.laserData.reset(new all::sense::urg_scan_data_t());
		loggedData_.laserData->scan_points.resize(numLaserScans_);
	}

	~slam_log_reader_t() //dtor
	{ 
		std::cout << "Sto chiudendo il file binario." << std::endl;
		logFile_.close();
		std::cout << "File binario chiuso." << std::endl;
	}
	
	slam_data_log_t next_sample(){
		/* 
			Values are recorded in this sequence :
			1) timestamp
			2) laser datas
			3) odometry datas
			4) inertial datas
		*/
		logFile_.read((char *) &loggedData_.timestamp, sizeof(loggedData_.timestamp)); // Timestamp of the sample read
		std::cout << "Timestamp sample : " << loggedData_.timestamp << std::endl;
		logFile_.read((char *) &loggedData_.laserData->scan_points[0], sizeof(sense::urg_scan_data_t::value_type)*numLaserScans_); // Scan points of the sample read
		for(int i=0;i<numLaserScans_ ;i++){
			std::cout << "Scan points sample : " << loggedData_.laserData->scan_points[i] << std::endl;
		}
		logFile_.read((char *) &loggedData_.odo, sizeof(loggedData_.odo)); // Odometry of the sample read
		std::cout << "Odometry sample : " << loggedData_.odo << std::endl;
		logFile_.read((char *) &loggedData_.inertialSensor, sizeof(loggedData_.inertialSensor)); // Inertial values of the sample read
		std::cout << "Inertial sensor sample : " << loggedData_.inertialSensor << std::endl;

		return loggedData_;
	}

	bool isLaserLogged(){
		// true if laser was logged
		return laserPresent_;
	}

	bool isOdometryLogged(){
		// true if odometry was logged
		return odoPresent_;
	}

	bool isInertialLogged(){
		// true if inertial sensor was logged
		return inertialPresent_;
	}

	slam_data_log_t last_sample_read(){
		return loggedData_;
	}

	size_t numSamplesRecorded(){
		return numSamples_;
	}


private:

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


	} //core
}//all
#endif