/*
 * ap.cpp
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "ap.h"
#include "thread/thread.h"


using namespace ap;





void apInit(void)
{
  threadInit();  
}

void apMain(void)
{
  while(1)
  {
    esp32Update();
    delay(2);
  }
}


