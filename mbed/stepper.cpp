#include "mbed.h"
#include "stepper.h"
#include "math.h"

#define STEP_DELAY .01

extern Serial pc;


/*DigitalOut mystep1(p17);
DigitalOut mystep2(p18);
DigitalOut mystep3(p19);
DigitalOut mystep4(p20);
*/


Stepper::Stepper(PinName pin1, PinName pin2, PinName pin3, PinName pin4, PinName homepin) {
    mystep1 = new DigitalOut(pin1);
    mystep2 = new DigitalOut(pin2);
    mystep3 = new DigitalOut(pin3);
    mystep4 = new DigitalOut(pin4);
    home = new DigitalIn(homepin);
    home->mode(PullUp);
    stepsPerDegree = 512/360;
    degreesPerStep = 360/512;
    findHome();
}

double Stepper::getStepsPerDegree() {
  return stepsPerDegree;
}

double Stepper::getDegreesPerStep() {
  return degreesPerStep;
}

void Stepper::stepperOff() {
  *mystep1 = 0;
  *mystep2 = 0;
  *mystep3 = 0;
  *mystep4 = 0;
}

void Stepper::stepCW(){
  *mystep1 = 0;
  *mystep2 = 0;
  *mystep3 = 1;
  *mystep4 = 1;
  wait(STEP_DELAY);
  *mystep1 = 0;
  *mystep2 = 1;
  *mystep3 = 1;
  *mystep4 = 0;
  wait(STEP_DELAY);
  *mystep1 = 1;
  *mystep2 = 1;
  *mystep3 = 0;
  *mystep4 = 0;
  wait(STEP_DELAY);
  *mystep1 = 1;
  *mystep2 = 0;
  *mystep3 = 0;
  *mystep4 = 1;
  wait(STEP_DELAY);
  currentPosition--;
}

void Stepper::stepCCW(){
  *mystep1 = 1;
  *mystep2 = 1;
  *mystep3 = 0;
  *mystep4 = 0;
  wait(STEP_DELAY);
  *mystep1 = 0;
  *mystep2 = 1;
  *mystep3 = 1;
  *mystep4 = 0;
  wait(STEP_DELAY);
  *mystep1 = 0;
  *mystep2 = 0;
  *mystep3 = 1;
  *mystep4 = 1;
  wait(STEP_DELAY);
  *mystep1 = 1;
  *mystep2 = 0;
  *mystep3 = 0;
  *mystep4 = 1;
  wait(STEP_DELAY);
  currentPosition++;
}

void Stepper::findHome(){
  pc.printf("finding home %u\n", !*home);
  while(!*home) {
  pc.printf("finding home %u\n", !*home);
    stepCW();
  }
  currentPosition = 0;
}

//takes angle in degrees
void Stepper::setAngle(double targetAngle) {
  targetPosition = ((int)floor(targetAngle+360)%360) / degreesPerStep;
  // this makes sure your angle is between 0 and 360 degrees.
}

void Stepper::stepperLoop(){
  if(currentPosition == targetPosition) {
    stepperOff();
  } else {
    if(targetPosition > currentPosition) {
      stepCCW();
    } else { //if(targetPosition < currentPosition)
      stepCW();
    }
  }
}

void Stepper::testStepper()
{
  pc.printf("Hello World!\n");
  for(int i = 0; i < 512; i++) {
    *mystep1 = 0;
    *mystep2 = 0;
    *mystep3 = 1;
    *mystep4 = 1;
    wait(STEP_DELAY);
    *mystep1 = 0;
    *mystep2 = 1;
    *mystep3 = 1;
    *mystep4 = 0;
    wait(STEP_DELAY);
    *mystep1 = 1;
    *mystep2 = 1;
    *mystep3 = 0;
    *mystep4 = 0;
    wait(STEP_DELAY);
    *mystep1 = 1;
    *mystep2 = 0;
    *mystep3 = 0;
    *mystep4 = 1;
    wait(STEP_DELAY);
  }
  wait(0.1);
  pc.printf("turning around!\n");
  for(int i = 0; i < 512; i++) {
    *mystep1 = 1;
    *mystep2 = 1;
    *mystep3 = 0;
    *mystep4 = 0;
    wait(STEP_DELAY);
    *mystep1 = 0;
    *mystep2 = 1;
    *mystep3 = 1;
    *mystep4 = 0;
    wait(STEP_DELAY);
    *mystep1 = 0;
    *mystep2 = 0;
    *mystep3 = 1;
    *mystep4 = 1;
    wait(STEP_DELAY);
    *mystep1 = 1;
    *mystep2 = 0;
    *mystep3 = 0;
    *mystep4 = 1;
    wait(STEP_DELAY);
  }
}
