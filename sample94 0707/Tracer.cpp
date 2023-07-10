#include "Tracer.h" // <1>
#include "util.h"
Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B), colorSensor(PORT_2) { // <2>
}

void Tracer::init() {
}

void Tracer::terminate() {
  
  leftWheel.stop();  // <1>
  rightWheel.stop();
}

void Tracer::run() {
  
  if(colorSensor.getBrightness() >= mThreshold) { // <1>
    leftWheel.setPWM(0);
    rightWheel.setPWM(pwm);
  } else {   // <2>
    leftWheel.setPWM(pwm);
    rightWheel.setPWM(0);
  }
}
