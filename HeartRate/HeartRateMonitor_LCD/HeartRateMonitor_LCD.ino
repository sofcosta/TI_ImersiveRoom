/*
  Heart Rate Monitor controls LCD drawing and projection 
  made in Processing(heartRateMonitor_growingCircles.pde)

  Tecnologias de Interface, 2023
  
  @authors: 
  Sofia Costa
  Carolina Malta
  Alexandra Coelho
  Cátia Pereira
*/


#define samp_siz 4
#define rise_threshold 5
#define rate_values 10
// Pulse Monitor Script

/////incluir biblioteca do ecra
#include <SparkFunColorLCDShield.h>

LCDShield lcd;

int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  ////desenho no ecrã
  lcd.init(PHILIPS);
  lcd.contrast(-64);
  lcd.clear(BLACK);
}
void loop() {
  float reads[samp_siz], sum;
  long int now, ptr;
  float last, reader, start;
  float before, print_value;
  float rates[rate_values];
  bool rising;
  int rise_count;
  int n;
  long int last_beat;

  for (int i = 0; i < samp_siz; i++)
    reads[i] = 0;
  sum = 0;
  ptr = 0;
  while (1) {
    // calculate an average of the sensor
    // during a 20 ms period (this will eliminate
    // the 50 Hz noise caused by electric light
    n = 0;
    start = millis();
    reader = 0.;
    do {
      reader += analogRead(sensorPin);
      //Serial.println(reader);
      n++;
      now = millis();
    } while (now < start + 20);

    reader /= n;  // we got an average
    // Add the newest measurement to an array
    // and subtract the oldest measurement from the array
    // to maintain a sum of last measurements
    sum -= reads[ptr];
    sum += reader;
    reads[ptr] = reader;
    last = sum / samp_siz;
    // now last holds the average of the values in the array
    // check for a rising curve (= a heart beat)
    if (last > before) {
      rise_count++;
      if (!rising && rise_count > rise_threshold) {
        // Ok, we have detected a rising curve, which implies a heartbeat.
        // Record the time since last beat, keep track of the previous
        // times to get a weighed average.
        // The rising flag prevents us from detecting the same rise
        // more than once.
        rising = true;
        rates[0] = millis() - last_beat;
        last_beat = millis();
        // Calculate the weighed average of heartbeat rate
        // according to the three last beats
        print_value = calculateMeanRate(rates);
        Serial.println(print_value);

        lcd.clear(BLACK);
        char buffer[25];
        if (print_value > 30) {
          dtostrf(print_value, 6, 2, buffer);
          lcd.setCircle(64, 64, 25, RED, FILL);
        } else {
          strcpy(buffer, "Please place your finger");
          lcd.setStr(buffer, 50, 20, SLATE, BLACK);
        }
        for (int i = rate_values - 1; i > 0; i--) {
          rates[i] = rates[i - 1];
        }
      }
    } else {
      // Ok, the curve is falling
      rising = false;
      rise_count = 0;
    }
    before = last;
    ptr++;
    ptr %= samp_siz;
  }
}

float calculateMeanRate(float rates[]) {
  float sum = 0;
  for (int i = 0; i < rate_values; i++) {
    sum += rates[i];
  }
  float average = sum / rate_values;
  return 60000. / average;
}
