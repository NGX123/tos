/*
    @author = ngx123
    @breif = local header for 8042 PS/2 controller driver
*/


#ifndef PS2_8042_DEFINES_H
#define PS2_8042_DEFINES_H


#include <drivers/x86.h>
#include <types.h>
#include <stdarg.h>
#include <drivers/ps2_8042.h>


struct controller_info
{
    uint8_t channels_present;
    uint8_t channel1_status;
    uint8_t channel2_status;
    uint8_t channel1_device;
    uint8_t channel2_device;
    uint8_t translation;

};


/*
    @brief = Returns the data collected by controller during initialization
    @param datatype = what data is requested(e.g. channels amount)
    @param data = storage for the requested data
    @return = 0 on success, -1 on error
*/
static int ioctl_requestData(uint8_t datatype, uint8_t* data);

/*
    @brief = sends command to the controller and recieves data while also making some checks
    @param command = the command to send to the controller
    @param data = space to store recieved data or to read and send data from
    @return = 0 on success, -1 on error
*/
static int ioctl_controllerDataRecieve(uint8_t command, uint8_t data);

/*
    @brief = sends command and data to the controller while also checking it
    @param command = the command to send to the controller
    @param data = space to store recieved data or to read and send data from
    @return = 0 on success, -1 on error
*/
static int ioctl_controllerDataSend(uint8_t command, uint8_t data);

/*
    @brief = send data directly to chosen controller port
    @param port = the port of the controller to send to
    @param data = the data to send to the port
    @return = 0 on success, -1 on fail
*/
static int ioctl_controllerDirectSend(uint16_t port, uint8_t data);

/*
    @brief = recieve data directly from chosen controller port
    @param port = the port of the controller to recieve from
    @param data = storage for the recieved data
    @return = 0 on success, -1 on fail
*/
static int ioctl_controllerDirectRecieve(uint16_t port, uint8_t* data);
#endif