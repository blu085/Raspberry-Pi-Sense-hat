/** @brief sensors header file code
 *  @author Huy Bui
 *  @file sensors.h
 *  @since 2025-04-02
 */

//#include
#include<stdio.h>
#include<stdlib.h>
#include"hts221.h"
#include"led2472g.h"
#include"lps25h.h"
#include"lsm9ds1.h"
#include"curses.h"
#include"time.h"
#include"unistd.h"

//globle variable
double xl[2] = {0.0, 0.0};