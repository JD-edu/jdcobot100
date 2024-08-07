/*MIT License

Copyright (c) 2024 JD edu. http://jdedu.kr author: conner.jeong@gmail.com
     
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
     
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
     
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN TH
SOFTWARE.*/
#include<Servo.h>

Servo base;
Servo shoulder;
Servo forearm;
Servo upperarm;
int baseAngle = 90;
int shoulderAngle = 90;
int forearmAngle = 90;
int upperarmAngle = 90;


int servoParallelControl (int thePos, Servo theServo, int speed){
    int startPos = theServo.read();       //read the current position of the servo we are working with.
    int newPos = startPos;                // newPos holds the position of the servo as it moves
    
    //define where the pos is with respect to the command
    // if the current position is less that the desired move the position up
    if (startPos < (thePos)){
       newPos = newPos + 1;               
       theServo.write(newPos);
       delay(speed);
       return 0;                          // Tell primary program that servo has not reached its position     
    }
    // Else if the current position is greater than the desired move the servo down
    else if (newPos > (thePos)){
      newPos = newPos - 1;
      theServo.write(newPos);
      delay(speed);
      return 0;  
    }  
    // If the servo is +-5 within the desired range then tell the main program that the servo has reached the desired position.
    else {
        return 1;
    }  
}

void setup() {
  Serial.begin(115200);
  base.attach(3);
  base.write(baseAngle);
  shoulder.attach(5);
  shoulder.write(shoulderAngle);
  forearm.attach(6);
  forearm.write(forearmAngle);
  upperarm.attach(9);
  upperarm.write(upperarmAngle);

}
int status1 = 0;  // base status
  int status2 = 0;
  int status3 = 0;
  int status4 = 0; 
  int done = 0 ;
void loop() {
      // this value tells when all the joints have reached thier positions
  while(done == 0){     // Loop until all joints have reached thier positions                      && ready == 1
    //move the servo to the desired position
    //This block of code uses "Functions" to make is more condensed.
    status1 = servoParallelControl(120, base, 20);
    status2 = servoParallelControl(120, shoulder, 20);
    status3 = servoParallelControl(120, forearm, 20);
    status4 = servoParallelControl(120, upperarm, 20);         
    // Check whether all the joints have reached their positions
    if (status1 == 1 && status1 == 2 && status1 == 3 && status1 == 4  ){
      done = 1; //When done =1 then the loop will stop
    }   
  }// end of while
}
