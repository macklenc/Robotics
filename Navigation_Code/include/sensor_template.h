#include <string>
#include <vector>
#include "base_sensor.h"

#ifndef sensorName_H
#define sensorName_H

class sensorName : public base_sensor
{
	public:
		void poll_data(){

		}

		int push_data(unsigned char Reg_ADDR, unsigned char dataOut){

		}

		void send_command(std::string command){
			
		}
};


#endif // sensorName_H
