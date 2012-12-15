#include <string>
#include <vector>
#include "stdData.h"
#include "base_sensor.h"

#ifndef sensor_controller_H
#define sensor_controller_H

class sensor_controller : public base_sensor : protected stdData
{
    private:
        std::vector<us_sensor> US;
        std::vector<accelerometer> ACC;
        std::vector<compass> COMP;
        std::vector<contact_net> CONTACT;
        std::vector<kinect> KINECT;
        std::vector<wisker_matrix> WISKER;



    protected:
        int init_sensors(std::vector<init> initial);

    public:
        sensor_controller(std::vector<init> initial){
            init_sensors(initial);
        }
        int poll_data();
};


#endif // sensor_controller_H
