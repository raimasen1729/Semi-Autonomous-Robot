/*
 * header for MEAM510 hacks for vive interface
 * May 2021
 * Use at your own risk
 * 
 */

#ifndef VIVE510
#define VIVE510

#include <arduino.h>

// vive status errors
#define VIVE_BACKWARDSERROR -1
#define VIVE_LOCKEDON 20 
// less than 20 is an error.

class Vive510
{
private:

  volatile uint32_t m_usRising ; // updated by interrupts 
  volatile uint32_t m_usFalling ;
  uint32_t m_currentJPulse;
  uint32_t m_currentKPulse;
  uint32_t m_currentRising; // base edge used for sync
  int m_xCoord;
  int m_yCoord;
  int m_vivestatus = 0;
  int m_pin;               // signal input pin
  int m_sweepWidth=100;
  int m_syncWeight = 10;   // filter weight for determining X Y axis (J,K pulse)
  
  uint32_t m_oldKPulse;
  uint32_t m_oldJPulse;
  uint32_t m_lastFalling;
  int m_spurious;

  int isJPulse();
  int isKPulse();
  uint32_t alignment(uint32_t timeus, uint32_t interval);
  void  updateKPulse(uint32_t pulsewidth);
  void  updateJPulse(uint32_t pulsewidth);
  void processPulse();
  
public:
  Vive510(int pin);
  int xCoord();
  int yCoord();
  void swapXY();
  uint32_t sync(int);
  int status();
  void stop();
  void start();
  void begin();
  void begin(int);
  
  void IRAM_ATTR pulseISR(uint32_t); // need public for global interrupt Arduino hack
};


#endif
