/***************************************************
 DFRobot Gravity: Peristaltic Pump
 <https://www.dfrobot.com/wiki/index.php/Gravitry: Peristaltic Pump SKU:DFR0523>

 ***************************************************
 This sample code shows 3 states: clockwise maximum speed rotation; stop; counterclockwise maximum speed rotation

 Created 2017-12-25
 By Jason <jason.ling@dfrobot.com@dfrobot.com>

 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

 /***********Notice and Trouble shooting***************
0   -> clockwise maximum speed rotation
90  -> stop
180 -> counterclockwise maximum speed rotation
 ****************************************************/

#include <ESP32Servo.h>
#include <AceButton.h>
Servo myservo;
#define PUMPPIN 15    //peristaltic pump control pin, connect to arduino digital pin 9
using namespace ace_button;

AceButton button(BUTTON_PIN);

void handleEvent(AceButton* /*button*/, uint8_t eventType,
    uint8_t /*buttonState*/) {
  switch (eventType) {
    case AceButton::kEventPressed:
    myservo.write(90);   //Clockwise maximum speed rotation
    // myservo.write(180); //Counterclockwise maximum speed rotation
      break;
    case AceButton::kEventReleased:
    myservo.write(180);  //Stop
      break;
  }
}
void motor_setup()
{
  	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(PUMPPIN, 500, 2500);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setEventHandler(handleEvent);
}

void motor_loop()
{
  button.check();
}
