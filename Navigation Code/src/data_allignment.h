#include <string>
#include <vector>
#include "stdData.h"

#ifndef data_allignment_H
#define data_allignment_H

class data_allignment : protected stdData
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
        data_allignment(std::vector<init> initial){
            init_sensors(initial);
        }
        int poll_data();
};


#endif // data_allignment_H
