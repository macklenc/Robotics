#include <string>
#include <vector>
#include "i2c.h"

#ifndef base_sensor_H
#define base_sensor_H

class base_sensor : protected myI2C
{
	private:
		std::vector<int> POLL_BUFFER;	//Contains burst of sensor data
		int SENSOR_ID, BUFFER_SIZE;	//Sensor ID: used for identification, Buffer thing: Determines size of data blurb
		unsigned char DEVICE_ADDR_WRITE, DEVICE_ADDR_READ, REG_ADDR, DATA_OUT, AVERAGED_DATA;
		//In order: Write address, read address, register address, output history, smoothed data

		
	protected:
		void set_peripherial_REG_ADDR(unsigned char addr):REG_ADDR(addr){}	//Set device register address

		virtual int average_filter(){	//This is an averaging filter
			AVERAGED_DATA = 0;
			for (int i = 0; i < BUFFER_SIZE; ++i)
			{
				AVERAGED_DATA += POLL_BUFFER.at(i);
			}
			return AVERAGED_DATA /= BUFFER_SIZE;
		}

		void set_poll_buffer_size(int size): BUFFER_SIZE(size){	//Sets buffer size, and initializes
			for (int i = 0; i < size; ++i)
			{
				POLL_BUFFER.push_back(0);
			}
		}	//set data recording size

		void set_peripherial_ID(int ID):SENSOR_ID(ID){}	//Set the device identification number

		void set_peripherial_ADDR(unsigned char read, unsigned char write):DEVICE_ADDR_READ(read),DEVICE_ADDR_WRITE(write){}	//Set device address
		
		virtual unsigned char poll_data(unsigned char Reg_ADDR){	//Get data from I2C bus
			for (int i = 0; i < BUFFER_SIZE; ++i)
			{
				POLL_BUFFER.at(i) = Read_I2C_Byte(DEVICE_ADDR_READ, Reg_ADDR);
			}
			
			return average_filter();
		}//easter egg! Fred, I am not you're father

		virtual void poll_data_word(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, int bytes){
			Read_Multi_Byte(DEVICE_ADDR,REG_ADDR,bytes);
			word_transfer();
		}
		
		virtual int push_data(unsigned char Reg_ADDR, unsigned char dataOut){	//Send data through I2C bus
			DATA_OUT = Data;
			Send_I2C_Byte(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, unsigned char Data);
		}

		virtual int word_transfer(){	//Copies I2C read buffer for use with words
			for (int i = 0; i < BUFFER_SIZE; ++i)
			{
				POLL_BUFFER.at(i) = I2C_RD_Buf[i];
			}
		}

		virtual int initialize_sensor() = 0;

		virtual int test_sensor() = 0;

		base_sensor(){	//Constructor, not used, DAMMIT JIM!
		}
		~base_sensor();
	public:
		std::vector<int> get_data(){//Get data from data structure
			return POLL_BUFFER;
		} 	
		int get_poll_buffer_size(){	return BUFFER_SIZE;	}//Dumb comment #1
		int get_peripherial_ID(){ return SENSOR_ID;	}
		unsigned char get_peripherial_ADDR_READ(){ return DEVICE_ADDR_READ; }
		unsigned char get_peripherial_ADDR_WRITE(){ return DEVICE_ADDR_WRITE; }
		unsigned char get_peripherial_REG_ADDR(){ return REG_ADDR; }
		unsigned char get_transmit_data(){ return DATA_OUT; }
		unsigned char get_average_data() { return AVERAGED_DATA; }//If you think a comment will help you here... You're wrong :P
};//HE'S DEAD JIM!


#endif // base_sensor_H
