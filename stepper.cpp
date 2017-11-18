#include "mbed.h"
#include "stepper.h"

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
    degreesPerStep = 360/512;
    findHome();
}

void Stepper::stepCW(){
    *mystep4 = 0;
    *mystep1 = 1;
    wait(0.01);
    *mystep1 = 0;
    *mystep2 = 1;
    wait(0.01);
    *mystep2 = 0;
    *mystep3 = 1;
    wait(0.01);
    *mystep3 = 0;
    *mystep4 = 1;
    wait(0.01);
}

void Stepper::stepCCW(){
    *mystep1 = 0;
    *mystep4 = 1;
    wait(0.01);
    *mystep4 = 0;
    *mystep3 = 1;
    wait(0.01);
    *mystep3 = 0;
    *mystep2 = 1;
    wait(0.01);
    *mystep2 = 0;
    *mystep1 = 1;
    wait(0.01);
}

void Stepper::findHome(){
    while(!home) {
        stepCW();
    }
    currentPosition = 0.0;
}

void Stepper::setAngle(double targetAngle) {
    double difference = targetAngle - currentPosition;
yo finish this;
}

void Stepper::testStepper()
{
    pc.printf("Hello World!\n");
    for(int i = 0; i < 512; i++) {
        *mystep4 = 0;
        *mystep1 = 1;
        wait(0.01);
        *mystep1 = 0;
        *mystep2 = 1;
        wait(0.01);
        *mystep2 = 0;
        *mystep3 = 1;
        wait(0.01);
        *mystep3 = 0;
        *mystep4 = 1;
        wait(0.01);
    }
    wait(0.1);
    pc.printf("turning around!\n");
    for(int i = 0; i < 512; i++) {
        *mystep1 = 0;
        *mystep4 = 1;
        wait(0.01);
        *mystep4 = 0;
        *mystep3 = 1;
        wait(0.01);
        *mystep3 = 0;
        *mystep2 = 1;
        wait(0.01);
        *mystep2 = 0;
        *mystep1 = 1;
        wait(0.01);
    }
}
