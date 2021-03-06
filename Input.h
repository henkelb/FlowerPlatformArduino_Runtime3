#ifndef Input_h
#define Input_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <Print.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <WString.h>



class Input {
protected:

	int lastValue;

	unsigned long lastTime;

public:
	// TODO CS: TEMP
	bool contributesToState;

	Callback<ValueChangedEvent>* onValueChanged = NULL;

	uint8_t pin;

	unsigned int pollInterval = 50;

	bool internalPullUp = false;

	bool isAnalog = false;

	void setup();

	void loop();

	void printStateAsJson(const __FlashStringHelper* instanceName, Print* print);

};

void Input::setup() {
	pinMode(pin, INPUT);
	if (internalPullUp) {
		digitalWrite(pin, HIGH);
		lastValue = HIGH;
	} else {
		lastValue = LOW;
	}
}

void Input::loop() {
	int value;
	if (isAnalog) {
		value = analogRead(pin);
	} else {
		value = digitalRead(pin);
	}
	if (value == lastValue) {
    	return;
    }
	if (!isAnalog && (millis() - lastTime < pollInterval)) {
		return;
	}

	if (onValueChanged != NULL) {
		ValueChangedEvent event;
		event.previousValue = lastValue;
		event.currentValue = value;
		(*onValueChanged)(&event);
	}

	lastValue = value;

	if (!isAnalog) {
		lastTime = millis();
	}

}

void Input::printStateAsJson(const __FlashStringHelper* instanceName, Print* print) {
	print->print(F("\""));
	print->print(instanceName);
	print->print(F("\":"));
	print->print(lastValue);
}

#endif
