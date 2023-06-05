/*
  LDR controls RGB LEDs colors and color of projection 
  made in Processing(movement_colors.pde)

  Tecnologias de Interface, 2023

  @authors: 
  Sofia Costa
  Carolina Malta
  Alexandra Coelho
  Cátia Pereira
*/

// LEDs transparentes
int redPin = 13;   // Red pin of the RGB LED
int greenPin = 12;  // Green pin of the RGB LED
int bluePin = 11;  // Blue pin of the RGB LED

int redPin2 = 10;   // Red pin of the RGB LED
int greenPin2 = 9;  // Green pin of the RGB LED
int bluePin2 = 8;  // Blue pin of the RGB LED

// LEDs (translúcidos -> green e blue estão trocados)
int redPin3 = 7;    // Red pin of the RGB LED 2
int greenPin3 = 5;  // Green pin of the RGB LED 2
int bluePin3 = 6;   // Blue pin of the RGB LED 2

int redPin4 = 4;    // Red pin of the RGB LED 2
int greenPin4 = 2;  // Green pin of the RGB LED 2
int bluePin4 = 3;   // Blue pin of the RGB LED 2

int ldrPin = A7;  // Analog pin for the LDR sensor

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);

  pinMode(redPin3, OUTPUT);
  pinMode(greenPin3, OUTPUT);
  pinMode(bluePin3, OUTPUT);

  pinMode(redPin4, OUTPUT);
  pinMode(greenPin4, OUTPUT);
  pinMode(bluePin4, OUTPUT);

  pinMode(ldrPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  //Serial.println(ldrValue);
  
  int hue = map(ldrValue, 200, 1023, 0, 360);

  float r, g, b;
  hsvToRgb(hue, 1, 1, &r, &g, &b);  // Converte HSV para RGB

  Serial.println((String)(r * 255) + "," + (g * 255) + "," + (b * 255));

  analogWrite(redPin, r * 255);
  analogWrite(greenPin, g * 255);
  analogWrite(bluePin, b * 255);

  analogWrite(redPin2, r * 255);
  analogWrite(greenPin2, g * 255);
  analogWrite(bluePin2, b * 255);

  analogWrite(redPin3, r * 255);
  analogWrite(greenPin3, g * 255);
  analogWrite(bluePin3, b * 255);

  analogWrite(redPin4, r * 255);
  analogWrite(greenPin4, g * 255);
  analogWrite(bluePin4, b * 255);

  delay(100);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Converte HSV para RGB
void hsvToRgb(float h, float s, float v, float* r, float* g, float* b) {
  int i;
  float f, p, q, t;
  if (s == 0) {
    *r = *g = *b = v;
    return;
  }
  h /= 60;
  i = floor(h);
  f = h - i;
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch (i) {
    case 0:
      *r = v;
      *g = t;
      *b = p;
      break;
    case 1:
      *r = q;
      *g = v;
      *b = p;
      break;
    case 2:
      *r = p;
      *g = v;
      *b = t;
      break;
    case 3:
      *r = p;
      *g = q;
      *b = v;
      break;
    case 4:
      *r = t;
      *g = p;
      *b = v;
      break;
    default:
      *r = v;
      *g = p;
      *b = q;
      break;
  }
}
