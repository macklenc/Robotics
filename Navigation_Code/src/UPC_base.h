/*********************************************************************
**********************************************************************
**** File Name: UPC_base.h     					    				**
**** Developer: Christopher Macklen				    				**
****                                                                **
**** Revision : 1.0 - New model created                             **
**** Model    : Universal Peripherial Communication Base Object	    **
****                                                                **
**** Queries  : Institute for Electrical and Electronics Engineers  **
****            Webiste: http://www.uccs.edu/ieee/index.html        **
****            Developer email: cmacklen@uccs.edu                  **
**********************************************************************
****************************Function List*****************************
void set_peripherial_REG_ADDR(unsigned char addr);						//Set device register address
virtual int average_filter();											//This is an averaging filter
void set_poll_buffer_size(int size);									//Sets buffer size, and initializes
void set_peripherial_ID(int ID);										//Set the device identification number
void set_peripherial_ADDR(unsigned char read, unsigned char write);		//Set device address
virtual unsigned char poll_data(unsigned char Reg_ADDR);				//Get data from I2C bus
virtual void poll_data_word(unsigned char Reg_ADDR, int bytes);
virtual int push_data(unsigned char Reg_ADDR, unsigned char dataOut);	//Send data through I2C bus
virtual int word_transfer();											//Copies I2C read buffer for use with words
virtual int initialize_sensor() = 0;
virtual int test_sensor() = 0;
UPC_base();																//Constructor, not used, DAMMIT JIM!
~UPC_base();
std::vector<int> get_data();											//Get data from data structure
int get_poll_buffer_size();												//Dumb comment #1
int get_peripherial_ID();
unsigned char get_peripherial_ADDR_READ();
unsigned char get_peripherial_ADDR_WRITE();
unsigned char get_peripherial_REG_ADDR();
unsigned char get_transmit_data();
unsigned char get_average_data();										//If you think a comment will help you here... You're wrong :P
**********************************************************************
*********************************************************************/

#include <string>
#include <vector>
#include <exception>
#include "i2c.h"
#include <iostream>

#ifndef UPC_base_H
#define UPC_base_H

class UPC_exception_buffer_size: public std::exception
{
	virtual const char* what() const throw(){
		return "Buffer size too large";
	}
}buffer_size;

class UPC_exception_byte_size: public std::exception
{
	virtual const char* what() const throw(){
		return "Cannot accept negative number";
	}
}byte_size;

class UPC_base : protected myI2C
{
		
	protected:
		std::vector<int> POLL_BUFFER;	//Contains burst of sensor data
		int SENSOR_ID, BUFFER_SIZE;	//Sensor ID: used for identification, Buffer thing: Determines size of data blurb
		unsigned char DEVICE_ADDR_WRITE, DEVICE_ADDR_READ, REG_ADDR, DATA_OUT, AVERAGED_DATA;
		//In order: Write address, read address, register address, output history, smoothed data

		void set_peripherial_REG_ADDR(unsigned char addr){ REG_ADDR = addr; }	//Set device register address

		virtual int average_filter(){	//This is an averaging filter
			AVERAGED_DATA = 0;
			unsigned averaging = 0;
			try{
				for (int i = 0; i < BUFFER_SIZE; ++i)
				{
					averaging += POLL_BUFFER.at(i);
				}
			}
			catch(std::exception& e){
				std::cerr << "Exception caught in UPC_base:\n";
				std::cerr << e.what() << std::endl;
			}
			return AVERAGED_DATA = averaging / BUFFER_SIZE;
		}

		void set_poll_buffer_size(int size){	//Sets buffer size, and initializes
			BUFFER_SIZE = size;
			try {
				if (size > 32)
				{
					throw (buffer_size);
				} else 
				for (int i = 0; i < size; ++i)
				{
					POLL_BUFFER.push_back(0);
				}
			}
			catch(std::exception& e){
				std::cerr << "Exception caught in UPC_base:\n";
				std::cerr << e.what() << std::endl;
			}
		}	//set data recording size

		void set_peripherial_ID(int ID){ SENSOR_ID = ID; }	//Set the device identification number

		void set_peripherial_ADDR(unsigned char read, unsigned char write){ DEVICE_ADDR_READ = read; DEVICE_ADDR_WRITE = write; }	//Set device address
		
		virtual unsigned char poll_data(unsigned char Reg_ADDR){	//Get data from I2C bus
			try{
				for (int i = 0; i < BUFFER_SIZE; ++i)
				{
					POLL_BUFFER.at(i) = Read_I2C_Byte(DEVICE_ADDR_READ, Reg_ADDR);
				}	
			}
			catch(std::exception& e){
				std::cerr << "Exception caught in UPC_base:\n";
				std::cerr << e.what() << std::endl;
			}			
			
			return average_filter();
		}//easter egg! Fred, I am not you're father

		virtual void poll_data_word(unsigned char Reg_ADDR, int bytes){
			try{
				if (bytes < 0)
				{
					throw(byte_size);
				}
				Read_Multi_Byte(DEVICE_ADDR_READ,REG_ADDR,bytes);
				word_transfer();
			}
			catch(std::exception& e){
				std::cerr << "Exception caught in UPC_base:\n";
				std::cerr << e.what() << std::endl;
			}
		}
		
		virtual int push_data(unsigned char Reg_ADDR, unsigned char dataOut){	//Send data through I2C bus
			try{
				DATA_OUT = dataOut;
				Send_I2C_Byte(DEVICE_ADDR_WRITE, Reg_ADDR, dataOut);
			}
			catch(std::exception& e){
				std::cerr << "Exception caught in UPC_base:\n";
				std::cerr << e.what() << std::endl;
			}
			return 1;
		}

		virtual int word_transfer(){	//Copies I2C read buffer for use with words
			try{
				for (int i = 0; i < BUFFER_SIZE; ++i)
				{
					POLL_BUFFER.at(i) = I2C_RD_Buf[i];
				}
			}
			catch(std::exception& e){
				std::cerr << "Exception caught in UPC_base:\n";
				std::cerr << e.what() << std::endl;
			}
			return 1;
		}

		virtual int initialize_sensor() = 0;

		virtual int test_sensor() = 0;

		UPC_base(){	//Constructor, not used, DAMMIT JIM!
		}
		~UPC_base();
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


#endif // UPC_base_H
