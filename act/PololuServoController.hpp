#ifndef PololuServoControllerHPP
#define PololuServoControllerHPP

#include <wx/ctb/serport.h>
#include <wx/ctb/timer.h>

#include <alcor/core/iniWrapper.h>
#include <alcor/core/core.h>

using namespace std;

namespace all {
	namespace act {
		
		class PololuServoController {
		
		//const
		public:
			
			static const int SZ_POLBUF = 6;

			static const byte kSync			= 0x80;
			static const byte kDeviceId     = 0x01;

			//default servo configuration
			static const int defMinPose = 500;
			static const int defMaxPose = 5500;
			static const int defCenterPose = 3000;

			static const double kdeg2step;
			static const double kstep2deg;

			//speed = 0 -> instantly change position
			//speed = 1 -> 100 steps per second (5 degrees per second)
			//speed = 127 -> 12700 steps per second (635 degrees per second) (exaggerated! see below...)
			static const int kMinSpeed = 0;		
			static const int kMaxSpeed = 127;

			//max phisical speed of the servo motor
			static const int defMaxSpeed = 45;

		
		//c'tor
		public:
			PololuServoController(char* iniFile="config/polServoConf.ini");
			~PololuServoController();

		//connection
		public:
			bool connect();
			void disconnect();

			void servoOn(int);
			void servoOff(int);

		//move
		public:
			void setPose (int, double);
			
			//blocking: wait until servo reach the desired position 
			//before returning control to the caller
			void setPoseBlk (int, double);
			
			void reset();

		//setters
		public:
			void setSpeed (int, int);

		//getters
		public:
			double getPose (int);
			
			//get servo speed in degrees per second
			double getSpeed(int);

		//service function
		protected:
			//send a command to the controller (number of byte to send)
			void sendCommand(int);

			//implements the corrispondent protocol's command
			void setAbsPose(byte, byte, byte);

			//initialize the servos struct from ini file
			void initServos();

			//compute step corresponding to the specified angle
			int getStep(int, double);
			
			//compute angle corresponding to the specified step
			double getDeg(int, int);

			//compute the number of milliseconds necessary to make the specified movement
			//from the current position
			int getMoveTime(int, double);

		//typedef
		protected:
			typedef struct {
				byte id;
				int minPos;
				int maxPos;
				int defaultPos;
				int actualPos;
				int defaultSpeed;
				int actualSpeed;
				int maxSpeed;
			} servo;

		//class data
		protected:
			wxSerialPort mPol;     //serial connection to the controller
			bool mConnected;

			char* mPolPort;              //serial port of the controller
			
			iniWrapper mPolConfig;		 //ini configuration

			core::uint8_t mCmdBuffer[SZ_POLBUF];	 //command buffer

			servo* mServos;				 //parameters of servos connected to the controller
			int mNumServo;
		};

	} //namespace act
} //namespace all

#endif
