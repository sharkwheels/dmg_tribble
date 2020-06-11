/********************
Tribble Tamagochi
June 2020
DMG Controller Jam
Author: Nadine Lessio
Description: A fluffy ball that screams if its not being held. 
**********************/

#include <Adafruit_CircuitPlayground.h>
#include <millisDelay.h>

#define CAP_TOP 35      // top of the cap sense reading
#define CAP_BOTTOM 19   // bottom of the cap sense reading

// old top: 25, old bottom: 10
// fiddle w/ these values to deal with response issues. 
// Might have to revisit serial write situations. re: need a rest state? 

int motor_pin = 6; 
bool motorOn = false;
bool okToPrint = true;

bool isPurring= false;
bool isScreaming = false;

int motor_interval = 400;
millisDelay motorDelay;

long distressInts[3] = {30000,60000,180000}; // 30 seconds, 1 minute, 3 minutes
long previousMillis = 0; 
long DistressInterval = 60000; // start at 1 minute

//int held_count = 0;

void setup() {
	CircuitPlayground.begin();
	Serial.begin(9600); 
	pinMode(motor_pin, OUTPUT);  
	digitalWrite(motor_pin, LOW); 
	motorDelay.start(motor_interval); 
}

void loop(){
	
	int reading = CircuitPlayground.readCap(2);

	// little test for cap read setting
	//Serial.println(reading);
	/*if(reading >= CAP_TOP){
		Serial.println("touching");
	}else if(reading <= CAP_BOTTOM){
		Serial.println("off");
	}*/
	//digitalWrite(motor_pin,HIGH);

	// Rest State
	if(!isPurring && !isScreaming){
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis > DistressInterval) {
			previousMillis = currentMillis;
			isScreaming = true;
			isPurring = false;
			Serial.println(DistressInterval);
			Serial.println("screaming");
		}	
	}

	if(reading >= CAP_TOP) {
		isPurring = true;
		isScreaming = false;
		if(isPurring && !isScreaming){
			if(okToPrint){
				// keeping for later functionality
				//held_count++;		
				//Serial.println(held_count);
				Serial.println("purring");
				okToPrint = false;
			}
			if (motorDelay.justFinished()) {
				motorDelay.repeat(); // start delay again without drift
				motorOn = !motorOn;
				if (motorOn) {
					digitalWrite(motor_pin, HIGH); // turn motor on
				}else{
					digitalWrite(motor_pin, LOW); // turn motor off
				}
			}
		}	
	}else if(reading <= CAP_BOTTOM && isPurring && !isScreaming){
		DistressInterval = distressInts[random(3)]; //pick a random interval time to next scream state
		//DistressInterval = 30000; // test
		digitalWrite(motor_pin, LOW);
		motorDelay.restart();
		if(reading <= CAP_BOTTOM && !okToPrint){
			okToPrint = true;
		}
		isPurring = false;
	}

	if(isScreaming && !isPurring){
		digitalWrite(motor_pin,HIGH);
	}

}
