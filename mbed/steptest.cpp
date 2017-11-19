#include "mbed.h"
#include "stepper.h"

DigitalOut myled(LED1);

Serial pc(USBTX, USBRX); // tx, rx

#define ALT1 p9
#define ALT2 p10
#define ALT3 p11
#define ALT4 p12

#define AZ1 p5
#define AZ2 p6
#define AZ3 p7
#define AZ4 p8

#define AZHOME p13
#define ALTHOME p14

int main() {
    Stepper *azMotor = new Stepper(AZ1, AZ2, AZ3, AZ4, AZHOME);
    azMotor->findHome();
    while(1) {
        myled = 1;
        pc.printf("1\n");
        wait(2);
        myled = 0;
        pc.printf("0\n");
        wait(0.2);
    }
}
