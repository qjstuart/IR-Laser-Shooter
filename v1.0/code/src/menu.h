/*!
 * \file      menu.h
 * \brief     Controls the menu
 */
#ifndef MENU_H
#define MENU_H

#include "switches.h"
#include "game_modes.h"
#include <stdlib.h> 
#include <stdio.h>

/*! 
 *	\brief Initialises menu
 */
void menu_init(void);

/*! 
 *	\brief Shows the menu and reads input
 */
void show_menu(void);

#endif // MENU_H
