#ifndef Stepper_h
#define Stepper_h


class Stepper
{
  public:
    Stepper(PinName pin1, PinName pin2, PinName pin3, PinName pin4, PinName home);
    void stepperOff();
    void stepCW();
    void stepCCW();
    void findHome();
    void setAngle(double);
    void stepperLoop();
    void testStepper();

  private:
    int currentPosition;
    int targetPosition;
    double degreesPerStep;
    DigitalIn* home;
    DigitalOut* mystep1;
    DigitalOut* mystep2;
    DigitalOut* mystep3;
    DigitalOut* mystep4;
};


#endif
