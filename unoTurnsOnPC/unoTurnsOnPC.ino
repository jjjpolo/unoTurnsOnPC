#include <Servo.h>

#define irReceiver 12
#define servomotorPinout 11

Servo myservo;  // crea el objeto servo

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting system...");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(irReceiver, INPUT);
  myservo.attach(servomotorPinout);  // vincula el servo al pin digital 9
  Serial.print("* Servomotor attached to pin: "); Serial.println(servomotorPinout);
  Serial.print("* irReceiver attached to pin: "); Serial.println(irReceiver);
}

/*
   The arduino due must has an IR LED connected to 5V pinout
   The arduino uno (this) should has a IR receiver which let us know
   when the Arduio due turns on.
*/
bool checkIfPcTurnedOn ()
{
  Serial.print("IR input state is: "); Serial.println(digitalRead(irReceiver));
  if (digitalRead(irReceiver))
    return true;
  return false;
}

void delayBlinking(int delayValue)
{
  for (int i = 0; i < delayValue; i++) //delay 1s
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500);
  }
}

void moveServo(int from, int to, int speedOfMovement)
{
  for (from; from < to ; from++)
  {
    myservo.write(from);
    delay(speedOfMovement);
  }
}

void powerUpRoutine(int attempt, bool &done)
{
  Serial.print("Running routine for case: "); Serial.println(attempt);
  switch (attempt)
  {
    case 1:
      myservo.write(10);
      delay(1000);
      moveServo(10, 175, 2); //from, to , speed
      delay(300);
      myservo.write(10);
      break;
    case 2:
      myservo.write(10);
      delay(1000);
      moveServo(10, 176, 3);
      delay(325);
      myservo.write(10);
      break;
    default: //3 attempts for now
      myservo.write(10);
      delay(1000);
      moveServo(10, 177, 5);
      delay(350);
      myservo.write(10);
      done = true; /*default case is the last attempt so we are going to break the
                     while loop here (notice that donde is a reference) */
      break;
  }
}

void loop()
{
  bool done = false;
  int attempt = 0;
  delay(1000);
  while (!done) //while PC is power off
  {
    Serial.print("Attempt to power pc up #"); Serial.println(attempt);
    powerUpRoutine(++attempt, done);
    Serial.println("Waiting for a few seconds to let the PC turns on or not");
    delayBlinking(10); //wait a few seconds to let the pc turns on
    if (!done) //checks if poweUpRoutine didn't get to the last attempt (see default case in poweUpRoutine to udenrstand)
      done = checkIfPcTurnedOn(); //Check if pc turned on so it will decide to continue or break this while loop
  }
  Serial.println("Getting out of while loop");
  while (true) delay(1); //If everything went well will loop infinitely here
}
