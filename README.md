# Car Simulator
We did system that control the electrical functions of car.

## Features
- Seat belt control according to engine running status.
- Air conditioning control according to temperature.
- Headlight control according to light level.
- Other system running control according to engine running status.
  
## Circuit Diagram
![Circuit](/images/image.png)

### Elements
- Arduino MEGA 2560
- LCD screen
- Light sensor
- Tempperature sensor
- Button
- Led
- Buzzer
- Engine
- Resistance

### Operating
- If seat belt is not fastened, engine wont't run so other sensors won't run.
- If the seat belt is removed while the engine is running, the warning will be triggered and the red LED and audible warning will be activated so other sensors won't run.
- If the temperature is below 25 degrees, the air conditioner works.
- If the light is below 250, the LED will turn on and the headlight will work.
- If the fuel level drops below 10% the LED will turn on. If the fuel level drops below 5% the LED will flash. At 0% the LED will turn off.
- If the door is open, the engine will not start and will give a warning.

## Setup
- Install Arduino IDE and Proteus.
- Open the circuit with Proteus.
- Select Arduino Mega 2560 and upload the code, then compile.
- Upload the resulting "/.hex" file to the Arduino board in the circuit.

## Communication
If you encounter a problem, you can contact us.
-> alperenplk.28@gmail.com -> talhakkmz@icloud.fr
  
