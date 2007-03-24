#include "PololuServoController.hpp"
#include <iostream>

namespace all {
	namespace act {

		const double PololuServoController::kdeg2step = 20.8;
		const double PololuServoController::kstep2deg = 1/kdeg2step;

		PololuServoController::PololuServoController(char* iniFile) {
			
			mPolConfig = iniFile;

			if (mPolConfig.Load(iniFile)) {
				
				cout << "Initializing Polulo Servo Controller" << endl;
				cout << "Reading configuration from " << iniFile << endl;

				mPolPort = mPolConfig.GetStringAsChar("polCtrl:port","COM5");
				
				mNumServo = mPolConfig.GetInt("polCtrl:nServo",1);
				
				cout << "Serial port is: " << mPolPort << endl;
				cout << "Assuming " << mNumServo << " servo connected to the controller" << endl; 
				
				initServos();	
			}
			
			else {
				std::cout << "Cannot open configuration file using default values..." << iniFile << endl;
			}

			//fixed protocol command's field
			mCmdBuffer[0]=kSync;
			mCmdBuffer[1]=kDeviceId;

			mConnected = false;
		}

		void PololuServoController::initServos() {
				
			//inizialize the parameters of the servos connected to the controller
				mServos = new servo[mNumServo];

				char iniSection[30] = "servo";

				for (int i=0; i<mNumServo; i++) {
					
					mServos[i].id=byte(i);
					
					iniSection[5]=i+48;
					iniSection[6]='\0';
					
					//read ini file
					strcpy(&iniSection[6],":minPose");
					mServos[i].minPos = mPolConfig.GetInt(iniSection,defMinPose);
					strcpy(&iniSection[6],":maxPose");
					mServos[i].maxPos = mPolConfig.GetInt(iniSection,defMaxPose);
					strcpy(&iniSection[6],":defPose");
					mServos[i].defaultPos = mPolConfig.GetInt(iniSection,defCenterPose);
					strcpy(&iniSection[6],":defSpeed");
					mServos[i].defaultSpeed = mPolConfig.GetInt(iniSection,2);
					strcpy(&iniSection[6], ":maxSpeed");
					mServos[i].maxSpeed = mPolConfig.GetInt(iniSection, defMaxSpeed);

					mServos[i].actualPos = mServos[i].defaultPos;
					mServos[i].actualSpeed = mServos[i].defaultSpeed;
					
					cout << endl << "Servo " << i << endl;
					cout << "Range: " << getDeg(i, mServos[i].minPos) << " % " << getDeg(i, mServos[i].maxPos) << " degrees" << endl;
				}
				cout << endl;
		}

		PololuServoController::~PololuServoController() {
			if (mConnected)
				disconnect();
			delete[] mServos;
		}

		bool PololuServoController::connect() {
			if (!mConnected) {
				cout << "Trying connecting to the controller..." << endl;
				if (mPol.Open(mPolPort) != -1) {
					
					//serial port settings 
					mPol.SetBaudRate(wxBAUD_19200);
					
					mConnected = true;
					cout << "Connected" << endl;
				
					reset();
				
					return true;
				}
				else {
					cout << "Cannot connect to the specified serial port" << endl;
					return false;
				}
			} 
			else {
				cout << "Already connected to the controller on port " << mPolPort << endl;
				return true;
			}
		}
		
		void PololuServoController::disconnect() {
			if (mConnected) {
				
				reset();      //move to the default position
				
				Sleep(100);   //for the inertia!!!
				
				for (int i=0; i<mNumServo; i++) {
					servoOff(i);		//turn off the servos
				}
				
				mPol.Close();
				mConnected = false;
				cout << "Polulo Controller Disconnected" << endl;
			}
		}

		void PololuServoController::servoOn(int servoNumber) {
			
			mCmdBuffer[2] = 0x00;              //set parameters commamd code
			mCmdBuffer[3] = (byte)servoNumber;
			
			byte parameters = 0;
			
			//we are not interested to the other parameters (range and direction) because
			//we use absolute position commands
			parameters |= (1 << 6);
			
			mCmdBuffer[4] = parameters;
			sendCommand(5);
		}

		void PololuServoController::servoOff(int servoNumber) {
			
			mCmdBuffer[2] = 0x00;
			mCmdBuffer[3] = (byte)servoNumber;
			
			byte parameters = 0;
			//parameters &= ~(1 << 6);
			mCmdBuffer[4] = parameters;
			sendCommand(5);
		}

		void PololuServoController::sendCommand(int dataSize) {
			mPol.Writev((char*) mCmdBuffer, dataSize, NULL);
		}

		int PololuServoController::getStep(int servoNumber, double degree) {
			return mServos[servoNumber].defaultPos + (int)(degree*kdeg2step);
		}

		double PololuServoController::getDeg(int servoNumber, int step) {
			return (step - mServos[servoNumber].defaultPos)*kstep2deg;
		}


		void PololuServoController::setAbsPose(core::uint8_t servoId, core::uint8_t data1, core::uint8_t data2) {
			mCmdBuffer[2]=0x04;      //absolute position command code
			mCmdBuffer[3]=servoId;
			mCmdBuffer[4]=data1;
			mCmdBuffer[5]=data2;
			sendCommand(6);
		}

		void PololuServoController::setPose(int servoNumber, double angle) {
			
			byte servoId=(byte)servoNumber;
			int step;
			
			step = getStep(servoNumber, angle);
			
			//check if step is out of range
			if ((step <= mServos[servoNumber].maxPos) && (step >= mServos[servoNumber].minPos)) {
				
				byte data2 = step; //least significant 8 bits of step
	
				data2 &=~(1 << 7); //clear most signicant bit (see protocol)
			
				byte data1 = step >> 7; //most significant 8 bits of step
			
				setAbsPose(servoId, data1, data2); //set the desired pose

				mServos[servoNumber].actualPos = step;  //update the actualPos field
			}
			else
				cout << "Angle out of range\n" << endl;


		}

		int PololuServoController::getMoveTime(int servoNumber, double nextPose) {
			
			int actStep = mServos[servoNumber].actualPos;
			int nextStep = getStep(servoNumber, nextPose);
			double deltaStep = abs(actStep - nextStep);
			
			double speed = mServos[servoNumber].actualSpeed;
			
			speed*=100;   //speed in steps per second
			
			double movTime=(deltaStep/speed);   //time to complete moving in seconds
			
			int mSecWait = (movTime * 1000);
			return mSecWait;
		}


		void PololuServoController::setPoseBlk(int servoNumber, double angle) {
		
			int mSecWait = getMoveTime(servoNumber, angle);

			setPose(servoNumber, angle);
			
			Sleep(mSecWait);
			cout << "Done" << endl;
		}


		double PololuServoController::getPose(int servoNumber) {
			return getDeg(servoNumber, mServos[servoNumber].actualPos);
		}

		double PololuServoController::getSpeed(int servoNumber) {
			return mServos[servoNumber].actualSpeed*100*kstep2deg;
		}

		void PololuServoController::setSpeed(int servoNumber, int speed) {
			if((speed <= kMaxSpeed) && (speed >= kMinSpeed) && (speed <= mServos[servoNumber].maxSpeed)) {
				mCmdBuffer[2]=0x01;
				mCmdBuffer[3]=(byte)servoNumber;
				mCmdBuffer[4]=(byte)(speed);
				sendCommand(5);
				mServos[servoNumber].actualSpeed = speed;
			}
		}

		void PololuServoController::setSpeedDeg(int servoNumber, double speed) {

			int step_speed = (speed * kdeg2step) / 100;

			setSpeed(servoNumber, step_speed);
		}



		void PololuServoController::reset() {
			for (int i=0; i< mNumServo; i++) {
				
				setSpeed(i,mServos[i].defaultSpeed);
				setPoseBlk(i,0);
			}
		}

	} //namespace act
} //namespace all