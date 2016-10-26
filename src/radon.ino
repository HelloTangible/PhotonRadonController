

#include "photon-thermistor.h"
#include <math.h>

//Relay Pins
int _relayOnePin = D3; //Fan
int _relayTwoPin = D4; //Deicer
int _relayThreePin = D5; //Empty
int _relayFourPin = D6; //Empty

// LED
int _ledOnePin = D0;
int _ledTwoPin = D1;

//Analog Pins
int _outdoorThermistorPin = A0; //OutDoor Temp
int _stackThermistorPin = A1; //Stack Temp
int _pressureSensorPin = A2; //Pressure Sensor

float _currentStackTemp = 0.0;
float _currentOutdoorTemp = 0.0;
float _currentStackPressure = 0.0;

//This is the tempature when we make a decision as to what to do.
float _thresholdFanTemp = 70; //20
float _thresholdDeIcerTemp = 70; //30

bool _isFanOn = false; //variable to hold onto the current state.
bool _isDeIcerOn = false; //variable to hold onto the current state.

Thermistor *stackThermistor;
Thermistor *outdoorThermistor;

void setup() {
  // For an NTC (negative temperature coefficient) thermistor only!
  // CONSTRUCTOR PARAMETERS:
  // 1. pin: Photon pin
  // 2. seriesResistor: The resistance value of the fixed resistor (based on your hardware setup)
  // 3. adcMax: The maximum analog-to-digital convert value returned by analogRead (Photon is 4095 NOT the typical Arduino 1023!)
  // 4. thermistorNominal: Resistance at nominal temperature (will be documented with the thermistor, usually "10k")
  // 5. temperatureNominal: Temperature for nominal resistance in celcius (will be supplied with the thermistor, assume 25 if not stated)
  // 6. bCoef: Beta coefficient of the thermistor; usually 3435 or 3950 (will be documented with the thermistor)
  // 7. samples: Number of analog samples to average (for smoothing)
  // 8. sampleDelay: Milliseconds between analog samples (for smoothing)
  stackThermistor = new Thermistor(_stackThermistorPin, 10000, 4095, 10000, 25, 3950, 5, 20);
  outdoorThermistor = new Thermistor(_outdoorThermistorPin, 10000, 4095, 10000, 25, 3950, 5, 20);

  pinMode(_relayOnePin, OUTPUT);
  pinMode(_relayTwoPin, OUTPUT);
  pinMode(_relayThreePin, OUTPUT);
  pinMode(_relayFourPin, OUTPUT);
  pinMode(_ledOnePin, OUTPUT);
  pinMode(_ledTwoPin, OUTPUT);

  //Init the Relays
  digitalWrite(_relayOnePin, 0);
  digitalWrite(_relayTwoPin, 0);
  digitalWrite(_relayThreePin, 0);
  digitalWrite(_relayFourPin, 0);

  //Init the LEDs
  digitalWrite(_ledOnePin, 0);
  digitalWrite(_ledTwoPin, 0);

/*
  Particle.variable("CurrentStackTemp", &_currentStackTemp, DOUBLE);
  Particle.variable("CurrentOutdoorTemp", &_currentOutdoorTemp, DOUBLE);
  Particle.variable("CurrentStackPressure", &_currentStackPressure, DOUBLE);

  Particle.variable("IsFanOn", &_isFanOn, bool);
  Particle.variable("IsDeicerOn", &_isDeicerOn, bool);
*/
   Particle.function("flipRelay", activateRelay);
  //Particle.function("setThresholdFanTemp", setThresholdFanTemp);
  //Particle.function("setThresholdDeicerTemp", setThresholdDeIcerTemp);
}

void loop() {
  _currentStackTemp = stackThermistor->readTempF();
  _currentOutdoorTemp = outdoorThermistor->readTempF();

  fanTempCheck();
  deIcerTempCheck();
  readPressureSensor();
  publishCurrentState();

  delay(30000);
}

int activateRelay( String relayName ){

  Particle.publish("activateRelay", relayName);

  if (relayName == "Fan") {
    if ( _isFanOn == true )
    {
      activateFan(0);
      _isFanOn == false;
      return 0;
    }
    else
    {
      activateFan(1);
      _isFanOn == true;
      return 1;
    }
  }
  else if (relayName == "DeIcer") {
    if ( _isDeIcerOn == true )
    {
      activateDeIcer(0);
      _isDeIcerOn = false;
      return 0;
    }
    else
    {
      activateDeIcer(1);
      _isDeIcerOn = true;
      return 1;
    }
  }
}


void fanTempCheck () {

  if( _currentOutdoorTemp < _thresholdFanTemp ){
    // if the outdoor temp is lower then turn off the fan
    activateFan(0);

    //If the fan is currently ON publish that we are turning it OFF.
    if ( _isFanOn == true ) {
      Particle.publish("Radon_Fan", "OFF");
    }

    _isFanOn = false;
  }
  else
  {
    activateFan(1);

    //If the fan is currently OFF publish that we are turning it ON.
    if ( _isFanOn == false ){
      Particle.publish("Radon_Fan", "ON");
    }

    _isFanOn = true;
  }

}

void activateFan(int status) {
  digitalWrite(_relayOnePin, status);
  digitalWrite(_ledOnePin, status);
}

void deIcerTempCheck () {

  if( _currentOutdoorTemp < _thresholdDeIcerTemp ){
    // if the temp is lower than the threshold then turn on the deicer
    activateDeIcer(1);

    if ( _isDeIcerOn == false ) {
      Particle.publish("Radon_DeIcer", "ON");
    }

    _isDeIcerOn = true;
  }
  else
  {
    activateDeIcer(0);

    if ( _isDeIcerOn == true ) {
      Particle.publish("Radon_DeIcer", "Off");
    }

    _isDeIcerOn = false;
  }

}

void activateDeIcer(int status) {
  digitalWrite(_relayTwoPin, status);
  digitalWrite(_ledTwoPin, status);
}

void readPressureSensor () {

  int _samples = 5;
  int _sampleDelay = 20;
  float sum = 0;

  for(int i=0; i<_samples; i++) {
    sum += analogRead(_pressureSensorPin);
    delay(_sampleDelay);
  }

  float currentPressure = sum / ((float)_samples);
  _currentStackPressure = currentPressure;

}

void publishCurrentState(){

  String systemStatusPayload = "{";
  systemStatusPayload = systemStatusPayload + "'stackTemp':" + String(_currentStackTemp) + ",";
  systemStatusPayload = systemStatusPayload + "'outdoorTemp':" + String(_currentOutdoorTemp) + ",";
  systemStatusPayload = systemStatusPayload + "'stackPressure':" + String(_currentStackPressure) + ",";
  systemStatusPayload = systemStatusPayload + "'isFanOn':" + String(_isFanOn) + ",";
  systemStatusPayload = systemStatusPayload + "'isDeIcerOn':" + String(_isDeIcerOn);

  systemStatusPayload = systemStatusPayload + "}";

  Particle.publish("Radon_Current_State", systemStatusPayload);

}
