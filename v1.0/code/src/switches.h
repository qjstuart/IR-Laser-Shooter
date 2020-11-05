/*!
 * \file      switches.h
 * \brief     Generic swicth functions.
 * \copyright ARM University Program &copy; ARM Ltd 2014.
 */
#ifndef SWITCHES_H
#define SWITCHES_H

#include <platform.h>
#include "gpio.h"

/*! \brief Initialise switches
 */
void switches_init(void);
/*! \brief Gets the current logic level of a siwtch pin.
 *  If the pin is high, this function will return a 1,
 *  else it will return 0.
 */
int switch_get(Pin pin);

#endif // SWITCHES_H

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
