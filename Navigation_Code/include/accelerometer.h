#include <string>
#include <vector>
#include <iostream>
#include "base_sensor.h"

#ifndef accelerometer_H
#define accelerometer_H

class accelerometer : public base_sensor
{
	/**
		It is recommended to configure the device in 
		standby mode and then to enable measurement mode.
	**/

	//Registers

	/**
		All except DEVID reset to 0, DEVID resets to 11100101
		Write address 0x3A, Read address 0x3B
	**/

	const int DEVID;	//Device ID, read only

	//Tap control
	const int THRESH_TAP;	//tap threshold
	const int OFSX;	//X-axis offset
	const int OFSY;	//Y-axis offset
	const int OFSZ;	//Z-axis offset
	const int DUR;	//Tap duration
	const int Latent;	//Tap latency
	const int Window;	//Tap window
	const int TAP_AXES;	//Axis control for tap/double tap
	const int ACT_TAP_STATUS;	//Source of tap/double tap

	//Power options
	const int THRESH_ACT;	//Activity threshold
	const int THRESH_INACT;	//Inactivity threshold
	const int TIME_INACT;	//Inactivity time
	const int ACT_INACT_CTL;	//Axis enable control for activity and inactivity detection, read only
	const int BW_RATE;	//Data rate and power mode control
	const int POWER_CTL	//Power-saving features control

	//Free Fall control
	const int THRESH_FF;	//Free-fall threshold
	const int TIME_FF;	//Free-fall time
	
	//Interrupt control
	const int INT_ENABLE;	//Interrupt enable control
	const int INT_MAP;	//Interrupt mapping control
	const int INT_SOURCE;	//Source of interrupts, read only

	//Data
	const int DATA_FORMAT;	//Data format control
	const int DATAX0;	//X-Axis Data 0, read only
	const int DATAX1;	//X-Axis Data 1, read only
	const int DATAY0;	//Y-Axis Data 0, read only
	const int DATAY1;	//Y-Axis Data 1, read only
	const int DATAZ0;	//Z-Axis Data 0, read only
	const int DATAZ1;	//Z-Axis Data 1, read only
	const int FIFO_CTL;	//FIFO control
	const int FIFO_STATUS;	//FIFO status, read only

	std::vector<int> X_POLL_BUFFER;	//X-Axis buffer
	std::vector<int> Y_POLL_BUFFER;	//Y-Axis buffer
	std::vector<int> Z_POLL_BUFFER;	//Z-Axis buffer

	int X_AVERAGED_DATA;	//Fried Chicken
	int Y_AVERAGED_DATA;	//Pulled Pork
	int Z_AVERAGED_DATA;	//Chicken Fried Steak

	void poll_data_word(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, int bytes){	//read multiple bytes
		Read_Multi_Byte(DEVICE_ADDR,REG_ADDR,bytes);
		word_transfer();
	}

	void set_all_poll_buffer_size(int size){	//Initializes Buffers
		set_poll_buffer_size(size);
		for (int i = 0; i < size; ++i)
		{
			X_POLL_BUFFER.push_back(0);
			Y_POLL_BUFFER.push_back(0);
			Z_POLL_BUFFER.push_back(0);
		}
	}

	public:
		accelerometer(int size = 16, int ID){	//Initialize device
			DEVID = 0x00;
			THRESH_TAP = 0x1D;
			OFSX = 0x1E;
			OFSY = 0x1F;
			OFSZ = 0x20;
			DUR = 0x21;
			Latent = 0x22;
			Window = 0x23;
			THRESH_ACT = 0x24;
			THRESH_INACT = 0x25;
			TIME_INACT = 0x26;
			ACT_INACT_CTL = 0x27;
			THRESH_FF = 0x28;
			TIME_FF = 0x29;
			TAP_AXES = 0x2A;
			ACT_TAP_STATUS = 0x2B;
			BW_RATE = 0x2C;
			POWER_CTL = 0x2D;
			INT_ENABLE = 0x2E;
			INT_MAP = 0x2F;
			INT_SOURCE = 0x30;
			DATA_FORMAT = 0x31;
			DATAX0 = 0x32;
			DATAX1 = 0x33;
			DATAY0 = 0x34;
			DATAY1 = 0x35;
			DATAZ0 = 0x36;
			DATAZ1 = 0x37;
			FIFO_CTL = 0x38;
			FIFO_STATUS = 0x39;

			set_all_poll_buffer_size(size);
			set_peripherial_ID(ID);
			set_peripherial_ADDR(0x3A,0x3B);
			initialize_sensor();
		}

		int initialize_sensor(){
			//Take sensor out of standby
			unsigned char POWER_CTL_holder;
			POWER_CTL_holder = poll_data(POWER_CTL);
			POWER_CTL_holder |= 0x08;
			push_data(POWER_CTL,POWER_CTL_holder);
			return 1;
		}

		int enter_measurement_mode(){
			unsigned char POWER_CTL_holder;
			POWER_CTL_holder = poll_data(POWER_CTL);
			POWER_CTL_holder |= 0x08;
			push_data(POWER_CTL,POWER_CTL_holder);
			return 1;
		}

		int enter_standby_mode(){
			unsigned char POWER_CTL_holder;
			POWER_CTL_holder = poll_data(POWER_CTL);
			POWER_CTL_holder &= 0xF7;
			push_data(POWER_CTL,POWER_CTL_holder);
			return 1;
		}

		void poll_coordinate_data(){	//Retrieve X,Y,Z data and add to buffers
			for (int i = 0; i < BUFFER_SIZE; ++i)
			{
				poll_data_word(DEVICE_ADDR_READ, DATAX0, 6);
				X_POLL_BUFFER.at(i)=((int)POLL_BUFFER.at(1)<<8)|(int)POLL_BUFFER.at(0);
				Y_POLL_BUFFER.at(i)=((int)POLL_BUFFER.at(3)<<8)|(int)POLL_BUFFER.at(2);
				Z_POLL_BUFFER.at(i)=((int)POLL_BUFFER.at(5)<<8)|(int)POLL_BUFFER.at(4);
			}
			
			average_filter();
		}

		int average_filter(){	//This is an averaging filter
			X_AVERAGED_DATA = 0;
			Y_AVERAGED_DATA = 0;
			Z_AVERAGED_DATA = 0;
			for (int i = 0; i < BUFFER_SIZE; ++i)
			{
				X_AVERAGED_DATA += X_POLL_BUFFER.at(i);
				Y_AVERAGED_DATA += Y_POLL_BUFFER.at(i);
				Z_AVERAGED_DATA += Z_POLL_BUFFER.at(i);
			}

			X_AVERAGED_DATA /= BUFFER_SIZE;
			Y_AVERAGED_DATA /= BUFFER_SIZE;
			Z_AVERAGED_DATA /= BUFFER_SIZE;

			return 1;
		}

		int test_sensor(){	//Forces sensor self test, and checks to see if values are within specs.
			int X_CORRECT = 0, Y_CORRECT = 0, Z_CORRECT = 0, X_INCORRECT = 0, Y_INCORRECT = 0, Z_INCORRECT = 0;
			unsigned char CURRENT_DATA_FORMAT;
			//Enter standby
			enter_standby_mode();
			//Set self test bit
			unsigned char TEST_BIT, DATA;
			signed int16_t SIGNED_DATA;
			TEST_BIT = poll_data(DATA_FORMAT);
			TEST_BIT |= 0x80;
			push_data(DATA_FORMAT,TEST_BIT);
			enter_measurement_mode();

			poll_coordinate_data();

			CURRENT_DATA_FORMAT = poll_data(DATA_FORMAT);
			CURRENT_DATA_FORMAT &= 0x0C;

			for (int i = 0; i < BUFFER_SIZE; ++i)
			{
				if(CURRENT_DATA_FORMAT == 0){	//+-2G
					if (X_POLL_BUFFER.at(i) >= 50 && X_POLL_BUFFER.at(i) <= 540)
						X_CORRECT++;
					else
						X_INCORRECT++;

					if(Y_POLL_BUFFER.at(i) >= -540 && Y_POLL_BUFFER.at(i) <= -50)
						Y_CORRECT++;
					else
						Y_INCORRECT++;

					if(Z_POLL_BUFFER.at(i) >= -75 && Z_POLL_BUFFER.at(i) <= 875)
						Z_CORRECT++;
					else
						Z_INCORRECT++;
				} else if (CURRENT_DATA_FORMAT == 1){	//+-4G
					if (X_POLL_BUFFER.at(i) >= 25 && X_POLL_BUFFER.at(i) <= 270)
						X_CORRECT++;
					else
						X_INCORRECT++;

					if(Y_POLL_BUFFER.at(i) >= -270 && Y_POLL_BUFFER.at(i) <= -25)
						Y_CORRECT++;
					else
						Y_INCORRECT++;

					if(Z_POLL_BUFFER.at(i) >= 38 && Z_POLL_BUFFER.at(i) <= 438)
						Z_CORRECT++;
					else
						Z_INCORRECT++;
				} else if (CURRENT_DATA_FORMAT == 2){	//+-8G
					if (X_POLL_BUFFER.at(i) >= 12 && X_POLL_BUFFER.at(i) <= 135)
						X_CORRECT++;
					else
						X_INCORRECT++;

					if(Y_POLL_BUFFER.at(i) >= -135 && Y_POLL_BUFFER.at(i) <= -12)
						Y_CORRECT++;
					else
						Y_INCORRECT++;

					if(Z_POLL_BUFFER.at(i) >= 19 && Z_POLL_BUFFER.at(i) <= 219)
						Z_CORRECT++;
					else
						Z_INCORRECT++;
				} else if (CURRENT_DATA_FORMAT == 3){	//+-16G
					if (X_POLL_BUFFER.at(i) >= 6 && X_POLL_BUFFER.at(i) <= 67)
						X_CORRECT++;
					else
						X_INCORRECT++;

					if(Y_POLL_BUFFER.at(i) >= -67 && Y_POLL_BUFFER.at(i) <= -6)
						Y_CORRECT++;
					else
						Y_INCORRECT++;

					if(Z_POLL_BUFFER.at(i) >= 10 && Z_POLL_BUFFER.at(i) <= 110)
						Z_CORRECT++;
					else
						Z_INCORRECT++;
				}
				
			}

			enter_standby_mode();
			//Set self test bit
			TEST_BIT = poll_data(DATA_FORMAT);
			TEST_BIT &= 0x7F;
			push_data(DATA_FORMAT,TEST_BIT);
			enter_measurement_mode();

			if (X_INCORRECT > 0 || Y_INCORRECT > 0 || Z_INCORRECT > 0)
			{
				std::cerr << "The accelerometer has failed with " << X_INCORRECT + Y_INCORRECT + Z_INCORRECT << "failures.\n";
				return 2;
			} else {
				std::cerr << "All tests have passed.\n";
				return 1;
			}
		}
};


#endif // accelerometer_H
