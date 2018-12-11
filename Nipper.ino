#include <Servo.h> 

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 5;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
const int servoPin = 20;  //digital pin used to control the servo
const int shearsOpen = 120;
const int shearsClosed = 50;

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonReading = HIGH;   // the previous reading from the input pin
int lastButtonState = HIGH;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

Servo myServo;

void setup() {
	pinMode(buttonPin, INPUT);
	pinMode(ledPin, OUTPUT);

	// set initial LED state
	digitalWrite(ledPin, ledState);

	Serial.begin(115200);

	setShearsOpen(true);
}

void loop() {
	// read the state of the switch into a local variable:
	int reading = digitalRead(buttonPin);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != lastButtonReading) {
		// reset the debouncing timer
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != buttonState) {
			buttonState = reading;

			// only toggle the LED if the new button state is HIGH
			if (buttonState == LOW) {
				ledState = !ledState;
				Serial.println("switch off");
				lastButtonState = LOW;
			}
			else {
				Serial.println("switch on");
				Serial.print("lastButtonState ");
				Serial.println(lastButtonState);
				if (lastButtonState == LOW) {
					setShearsOpen(false);
					delay(500);
					setShearsOpen(true);
				}
				lastButtonState = HIGH;
			}
		}
	}

	// set the LED:
	digitalWrite(ledPin, ledState);

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonReading = reading;
}

void setShearsOpen(bool openShears)
{
	myServo.attach(servoPin);
	delay(50);
	if (openShears)
		myServo.write(shearsOpen);
	else
		myServo.write(shearsClosed);
	delay(500);
	myServo.detach();
}