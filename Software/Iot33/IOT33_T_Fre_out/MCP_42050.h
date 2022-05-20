/*-------------------------------------------
  MCP42050 Read
  IOT33
  Trung Nguyen 2021
  version 1.00 - 18/11/2021

  --------------------------------------------*/

#ifndef __MCP42050_h
#define __MCP42050_h
//#include <SPI.h>


void MCP42050_init();

void MCP42050_Resistor_out(float x);

void setPotWiper(int addr, int pos);


#endif // 
