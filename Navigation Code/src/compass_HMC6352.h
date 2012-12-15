/*********************************************************************
**********************************************************************
**** File Name: compass_HMC6352.h 									**
**** Developer: Christopher Macklen						            **
****                                                                **
**** Revision : 1.0 - New model created                             **
**** Model    : Universal Peripherial Communication - Compass 		**
****                                                                **
**** Queries  : Institute for Electrical and Electronics Engineers  **
****            Webiste: http://www.uccs.edu/ieee/index.html        **
****            Developer email: cmacklen@uccs.edu                  **
**********************************************************************
*********************************************************************/ 
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