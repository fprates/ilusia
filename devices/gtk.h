/*
 * gtk.h
 *
 *  Created on: Oct 2, 2011
 *      Author: francisco.prates
 */

#ifndef GTK_H_
#define GTK_H_

#include "../ilusia.h"

#ifdef __linux__
#define ILS_GTK "libgtk-3.so"
#endif

extern int _ini_gtk(struct ils_config *config);

#endif /* GTK_H_ */
