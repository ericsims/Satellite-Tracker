#include "mbed.h"
#include "stepper.h"
#include "math.h"

#define STEP_DELAY 0.01

extern Serial pc;


Stepper::Stepper(PinName pin1, PinName pin2, PinName pin3, PinName pin4, PinName homepin, int stepsPerRev) {
    mystep1 = new DigitalOut(pin1);
    mystep2 = new DigitalOut(pin2);
    mystep3 = new DigitalOut(pin3);
    mystep4 = new DigitalOut(pin4);
    home = new DigitalIn(homepin);
    home->mode(PullUp);
    stepsPerRevolution = stepsPerRev;
    stepsPerDegree = stepsPerRevolution/360.0;
    degreesPerStep = 1/stepsPerDegree;
    findHome();
}

double Stepper::getStepsPerDegree() {
  return stepsPerDegree;
}

double Stepper::getDegreesPerStep() {
  return degreesPerStep;
}

void Stepper::stepperOff() {
  mystep1->write(0);
  mystep2->write(0);
  mystep3->write(0);
  mystep4->write(0);
}

void Stepper::stepCW(){
  mystep1->write(0);
  mystep2->write(0);
  mystep3->write(1);
  mystep4->write(1);
  wait(STEP_DELAY);
  mystep1->write(0);
  mystep2->write(1);
  mystep3->write(1);
  mystep4->write(0);
  wait(STEP_DELAY);
  mystep1->write(1);
  mystep2->write(1);
  mystep3->write(0);
  mystep4->write(0);
  wait(STEP_DELAY);
  mystep1->write(1);
  mystep2->write(0);
  mystep3->write(0);
  mystep4->write(1);
  wait(STEP_DELAY);
  currentPosition--;
  currentPosition += stepsPerRevolution;
  currentPosition = currentPosition % stepsPerRevolution;
}

void Stepper::stepCCW(){
  mystep1->write(1);
  mystep2->write(1);
  mystep3->write(0);
  mystep4->write(0);
  wait(STEP_DELAY);
  mystep1->write(0);
  mystep2->write(1);
  mystep3->write(1);
  mystep4->write(0);
  wait(STEP_DELAY);
  mystep1->write(0);
  mystep2->write(0);
  mystep3->write(1);
  mystep4->write(1);
  wait(STEP_DELAY);
  mystep1->write(1);
  mystep2->write(0);
  mystep3->write(0);
  mystep4->write(1);
  wait(STEP_DELAY);
  currentPosition++;
  currentPosition = currentPosition % stepsPerRevolution;
}

void Stepper::findHome(){
  while(!*home) {
    stepCCW();
  }
  stepperOff();
  currentPosition = 0;
}

//takes angle in degrees
void Stepper::setAngle(double targetAngle) {
  pc.printf("set Angle\n");
  targetPosition = ((int)floor(targetAngle+360)%360) / degreesPerStep;
  // this makes sure your angle is between 0 and 360 degrees.
}

bool Stepper::stepperLoop(){
  printf("currentPos: %i TargetPos: %i\n", currentPosition, targetPosition);
  if(currentPosition == targetPosition) {
    stepperOff();
    return true;
  } else {
    if(targetPosition > currentPosition) {
      if(targetPosition - currentPosition > stepsPerDegree * 360 / 2)
        stepCW();
      else
        stepCCW();
    } else { //if(targetPosition < currentPosition)
      if(currentPosition - targetPosition > stepsPerDegree * 360 / 2)
        stepCCW();
      else
        stepCW();
    }
    stepperOff();
    return false;
  }
}

void Stepper::testStepper()
{
  pc.printf("Hello World!\n");
  for(int i = 0; i < 512; i++) {
    mystep1->write(0);
    mystep2->write(0);
    mystep3->write(1);
    mystep4->write(1);
    wait(STEP_DELAY);
    mystep1->write(0);
    mystep2->write(1);
    mystep3->write(1);
    mystep4->write(0);
    wait(STEP_DELAY);
    mystep1->write(1);
    mystep2->write(1);
    mystep3->write(0);
    mystep4->write(0);
    wait(STEP_DELAY);
    mystep1->write(1);
    mystep2->write(0);
    mystep3->write(0);
    mystep4->write(1);
    wait(STEP_DELAY);
  }
  wait(0.1);
  pc.printf("turning around!\n");
  for(int i = 0; i < 512; i++) {
    mystep1->write(1);
    mystep2->write(1);
    mystep3->write(0);
    mystep4->write(0);
    wait(STEP_DELAY);
    mystep1->write(0);
    mystep2->write(1);
    mystep3->write(1);
    mystep4->write(0);
    wait(STEP_DELAY);
    mystep1->write(0);
    mystep2->write(0);
    mystep3->write(1);
    mystep4->write(1);
    wait(STEP_DELAY);
    mystep1->write(1);
    mystep2->write(0);
    mystep3->write(0);
    mystep4->write(1);
    wait(STEP_DELAY);
  }
}
