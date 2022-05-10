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
//#include <AceButton.h>
Servo myservo;
#define PUMPPIN 15   //peristaltic pump control pin, connect to arduino digital pin 9
// using namespace ace_button;
#define PUMP_TIME 3000

// AceButton button(BUTTON_PIN);

// void handleEvent(AceButton* /*button*/, uint8_t eventType,
//     uint8_t /*buttonState*/) {
//   switch (eventType) {
//     case AceButton::kEventPressed:
//        //Clockwise maximum speed rotation
//     // myservo.write(180); //Counterclockwise maximum speed rotation
//       break;
//     case AceButton::kEventReleased:
//     myservo.write(180);  //Stop
//       break;
//   }
// }
void motor_setup()
{
  pinMode(PUMPPIN, OUTPUT); 
}

void motor_loop()
{
  //button.check();

}
