import processing.serial.*;
Serial port;

ArrayList<Ball> circles = new ArrayList<Ball>();

String value;

float pulseValue;

void setup() {
  //size(1280, 960);
  fullScreen();
  String portName = Serial.list()[4];
  port = new Serial(this, portName, 9600);
  port.bufferUntil('\n');
  port.clear();
}

void draw() {
  background(0);

  for (int i=0; i<circles.size(); i++) {
    circles.get(i).grow();
    circles.get(i).display();
  }
  println(circles.size());
}

void serialEvent (Serial myPort) {
  // handle data from serial communication
  value = myPort.readString();

  if (value != null) {
    // remove breakline in the end of buffer
    String str = value.substring(0, value.length()-2);

    pulseValue = float(str);

    if (pulseValue > 40) circles.add(new Ball());
    println ("received:", str);
  }
}
