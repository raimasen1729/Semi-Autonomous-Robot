/*
 * MEAM510 hacks for web interface
 * Jan 2021
 * Use at your own risk
 * 
 * Mark Yim
 * University of Pennsylvania
 * copyright (c) 2021 All Rights Reserved
 */

#include "vive510.h"

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// need global interrupt Arduino attachInterrupt won't take member function as interrupt
// need to pass THIS as arg since all vive object interrupts call same global interrupt 
void IRAM_ATTR m_pulseISR(void *v) {
  portENTER_CRITICAL_ISR(&mux);
  static_cast<Vive510 *>(v)->pulseISR(micros());
  portEXIT_CRITICAL_ISR(&mux);  
}

Vive510::Vive510(int pin) {
  m_pin = pin;
}

void IRAM_ATTR Vive510::pulseISR(uint32_t us) {
    if (digitalRead(m_pin)==HIGH) {
      m_usRising = us;
    }
    else { 
      m_usFalling = us;
    }
    if (m_vivestatus == VIVE_LOCKEDON)   processPulse();
}

uint32_t mymod(uint32_t a, uint32_t bnum, uint32_t bdenom) { //could use fmod here...
  while (a > (uint32_t)0xffffffff/bdenom) a -= bnum;           // wrap case, know that bnum is divisible
  return a-(bnum*(a*bdenom/bnum)+bdenom/2)/bdenom;  
}

uint32_t Vive510::alignment(uint32_t timeus, uint32_t freq) { //  positive modulo  closest to either end
  if (mymod(timeus, 1000000, freq) < 1000000/freq/2) {
    return mymod(timeus, 1000000, freq);
  } else {
    return 1000000/freq - mymod(timeus, 1000000, freq) ;
  }
}

int Vive510::xCoord(){
  return m_xCoord;
}

int Vive510::yCoord(){
  return m_yCoord;
}

int Vive510::isKPulse(){
  if (alignment(m_usRising - m_currentRising, 60) < 300)    return true;
  else return false;
}

int Vive510::isJPulse(){
  if (alignment(m_usRising - m_currentRising - 1000000/120, 60) < 300) return true;
  else return false;
}

uint32_t med3filt(uint32_t a, uint32_t b, uint32_t c) {
  uint32_t middle;
  if ((a <= b) && (a <= c))
    middle = (b <= c) ? b : c;  
  else if ((b <= a) && (b <= c))
    middle = (a <= c) ? a : c;
  else    middle = (a <= b) ? a : b;
  return middle;
}

void  Vive510::updateKPulse(uint32_t pulsewidth){
  uint32_t newvalue = med3filt(m_currentKPulse, m_oldKPulse, pulsewidth);
  m_currentKPulse = (m_currentKPulse*m_syncWeight + newvalue)/(m_syncWeight+1);
  m_oldKPulse = pulsewidth;
}

void  Vive510::updateJPulse(uint32_t pulsewidth){
  uint32_t newvalue = med3filt(m_currentJPulse, m_oldJPulse, pulsewidth);
  m_currentJPulse = (m_currentJPulse*m_syncWeight + newvalue)/(m_syncWeight+1);
  m_oldJPulse = pulsewidth;
}

void Vive510::swapXY(){
      uint32_t tmpSwap;
#ifdef DEBUG2
      Serial.printf("BACKWARD" );
#endif
      m_usRising += 1000000/120; // swap J and K
      m_currentRising += 1000000/120;
      tmpSwap = m_currentKPulse;
      m_currentKPulse = m_currentJPulse;
      m_currentJPulse = tmpSwap;
      m_xCoord = VIVE_BACKWARDSERROR;
}

// move checkflag to be backwards checking...think about whether tu link w/spuriuos
void Vive510::processPulse() {
  static int checkflag=0;

  if (m_lastFalling != m_usFalling) {
    int pulsewidth = m_usFalling-m_usRising;
    if (pulsewidth > m_sweepWidth) {
      if (isKPulse()) {
        if (m_usFalling - m_lastFalling > 8500) { // missed a 120Hz sync pulse
          checkflag = (checkflag > 60) ? 60 : checkflag+10 ;
          m_spurious+=10;
        }
        if (checkflag > 0)  {
          updateKPulse(pulsewidth);
          if (m_currentKPulse - m_currentJPulse < 0)  swapXY();
        }   
        if (--checkflag < 0 ) checkflag = 0;   
        m_xCoord = m_usRising-m_lastFalling;        
        m_currentRising += mymod(m_usRising-m_currentRising,1000000,60); // PLL lock phase 
        m_spurious = 0;
#ifdef DEBUG
  Serial.printf("\nKPin%d width=%d align=%u x=%d ", m_pin, pulsewidth,alignment(m_usRising - m_currentRising, 60),m_xCoord);
#endif
      } 
      else if (isJPulse()) {
        if (checkflag>0) {
          updateJPulse(pulsewidth);
        }
        m_yCoord = m_usRising-m_lastFalling;
#ifdef DEBUG
  Serial.printf("\tJPin%d width=%d%d align=%u y=%d ", m_pin, pulsewidth,alignment(m_usRising - m_currentRising, 60),m_yCoord);
#endif
      }
      else {
#ifdef DEBUG2
        Serial.printf("P%dSpurious%d width:%d %u \n",m_pin,m_spurious,pulsewidth,alignment(m_usRising - m_currentRising, 60));
#endif
        if (m_spurious++ > 100) m_vivestatus=0;
      }
    }
    else { // x sweep or y sweep
#ifdef DEBUG
      Serial.printf("Pin%d:%d  r=%d", m_pin,pulsewidth,m_usRising-m_lastFalling);
#endif
    }
    m_lastFalling = m_usFalling;
  }
}

void Vive510::start() {
  // use ESP32 version of attachInterrupt to allow THIS argument
  attachInterruptArg(digitalPinToInterrupt(m_pin), m_pulseISR,  this,  CHANGE);
}

void Vive510::begin() {
  pinMode(m_pin, INPUT);  
  start();
}
  
void Vive510::begin(int pin) {
  m_pin = pin;
  pinMode(m_pin, INPUT);  
  start();
}
  
void Vive510::stop() {
  detachInterrupt(digitalPinToInterrupt(m_pin));
}

int Vive510::status() {
  return m_vivestatus;
}
/*
void Vive510::setSyncWeight(int weight) {
   m_syncWeight = weight;
}

int Vive510::getSyncWeight() {
  return m_syncWeight;
}

*/
uint32_t  Vive510::sync(int reps){
  int i=0;
  int maxpulse =0;
  uint32_t m_lastFalling;
  uint32_t currentKRising;
  uint32_t pulsewidth;
  uint32_t startms = millis();
  m_lastFalling = m_usFalling;
  
  while (millis()-startms < 1000/60) { // look at 1st 2 JorK pulses
    if (m_lastFalling != m_usFalling) {
      m_lastFalling = m_usFalling;
      pulsewidth = m_usFalling-m_usRising;
      if ( pulsewidth > m_sweepWidth) {
        i++;
        if (pulsewidth > maxpulse) {
          maxpulse = pulsewidth;
          currentKRising = m_usRising;
        }
      }
    }
    yield();
  }

  startms = millis();
  while (millis()-startms < reps*1000/60) {
    if (m_lastFalling != m_usFalling) {
      m_lastFalling = m_usFalling;
      pulsewidth = m_usFalling-m_usRising;
      if (pulsewidth > m_sweepWidth) {
        i++;
        if (alignment(m_usRising - currentKRising, 60) < 200)  { // aligns with currentK
          updateKPulse(pulsewidth);
        }
        else if (alignment(m_usRising - currentKRising - 1000000/120, 60) < 200)  { // aligns with currentK
          updateJPulse(pulsewidth);
        } 
        else { 
          if (m_currentKPulse - m_currentJPulse < 0)  swapXY(); 
        } 
#ifdef DEBUG2
        Serial.print("+");
#endif
      }
    }
    yield();
  }
#ifdef DEBUG2
  Serial.printf("%d/%d ",i,2*reps+2);
#endif
  if (i < 2*reps+2) {
    m_vivestatus = 20*i/(reps+2); // just for debugging info
  } 
  else {
    if (m_currentJPulse > m_currentKPulse) {
      m_currentRising =  currentKRising + 1000000/120;
    }
    else  {
      m_currentRising = currentKRising;
    }
#ifdef DEBUG2
    Serial.printf("Pin%dJ %d K %d\n",m_pin,m_currentJPulse, m_currentKPulse);
#endif
    m_vivestatus = VIVE_LOCKEDON;
  }
  return m_vivestatus;
}
