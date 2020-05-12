# solar-controller
Open Source solar hot water system controller

## src structure
* core: the core algorithms used by the controller and common to real embedded hardware, unit tests, and x86 sim
* hw: the hardware abstraction layer (esp32, 8-bit AVR, x86)
* sim: contains a simulator, i.e. runs the core and main program loop on x86
* tests: contains unit tests which exercise the algortihms in core

## Building from source
To build everything: `make`
To build only the AVR hex or only the simulator: `make -f Makefile.avr program`
or `make -f Makefile.sim program`
To flash the AVR hex using avrdude: `make -f Makefile.avr install`

## Test code for Arduino for reading Pt1000 and NTC 10K temperature sensors
This requires the physical setup to use voltage follower opamps and a
differential opamp to offset the ADC input voltage for the PT1000 by 0.455V
(see
https://learn.openenergymonitor.org/electricity-monitoring/temperature/rtd-temperature-sensing).

```
float get_average(int pin)
{
  const int NUMSAMPLES = 5;
  float average = 0;
  for (int i = 0; i < NUMSAMPLES; i++) {
   average += analogRead(pin);
   delay(10);
  }
  return average / NUMSAMPLES;
}

float get_rtd_pt1000_temp(int pin)
{
  analogReference(INTERNAL);
  delay(25);
  analogRead(pin); // fake read after ref change

  float average = get_average(pin);
  average = average / 1023 * 1.1; // voltage at analog input

  // TODO: refine
  #define SLOPE_C 1000
  #define SLOPE 449.24

  average = average * SLOPE + SLOPE_C; // resistance
  return ((average / 1000) - 1.0) / 0.00385;
}

#define NTC_THERMISTORNOMINAL 10000
#define NTC_TEMPERATURENOMINAL 25
#define NTC_BCOEFFICIENT 3950
#define NTC_PULLRESISTOR 10000

float get_ntc_temp(int pin)
{
  analogReference(DEFAULT);
  delay(25);
  analogRead(pin); // fake read after ref change

  float average = get_average(pin);

  // convert the value to resistance
  average = 1023 / average - 1;
  average = NTC_PULLRESISTOR / average;

  float steinhart;
  steinhart = average / NTC_THERMISTORNOMINAL; // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= NTC_BCOEFFICIENT;               // 1/B * ln(R/Ro)
  steinhart += 1.0 / (NTC_TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C

  return steinhart;
}
```
