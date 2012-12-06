#include <string>
#include <vector>
#include "base_sensor.h"

#ifndef compass_H
#define compass_H

class compass : protected base_sensor
{
	public:
		compass(int size = 16, int ID){
			set_all_poll_buffer_size(size);
			set_peripherial_ID(ID);
			set_peripherial_ADDR(0x43,0x42);
			initialize_sensor();
		}
};


#endif // compass_H