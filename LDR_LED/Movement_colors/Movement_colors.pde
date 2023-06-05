import processing.serial.*;
Serial port;

String value;

float[] colors = new float[3];
//float[] colors_mapped = new float[3];
float[] colors_mapped = {255, 255, 0};

import processing.video.*;

final int CELL_SIZE = 20;
int cols, rows;

Capture cam;
PImage prevFrame;
float contrast = 1.5;
int threshold = 50;

int numPixels;
int[] previousFrame;
int[] differenceFrame;

//Field
int cells = 128; //128
PVector[][] field = new PVector[cells][cells];
PVector pos;
float t = random(3);
float opacity = 120;
int particleCount = 4000;

final PVector temp = new PVector();

Particle[] particles = new Particle[particleCount];

PVector getPosition(float ix, float iy) {
  return temp.set(ix/cells * width, iy/cells * height);
}

PVector getCell(float x, float y) {
  x = constrain(x, 0, width);
  y = constrain(y, 0, height);
  int ix = int(x/width * (field.length - 1));
  int iy = int(y/height * (field.length - 1));
  return field[ix][iy];
}
//

void setup() {
  //size(1280, 960);
  fullScreen();
  String portName = Serial.list()[4];
  port = new Serial(this, portName, 9600);
  port.bufferUntil('\n');
  port.clear();
  
  cam = new Capture(this, width, height, Capture.list()[0], 30);
  cam.start();
  prevFrame = createImage(width, height, RGB);

  numPixels = cam.width * cam.height;
  previousFrame = new int[numPixels];
  differenceFrame = new int[numPixels];

  cols = width/CELL_SIZE;
  rows = height/CELL_SIZE;
  rectMode(CENTER);

  //Field
  for (int i= 0; i<particles.length; i++) {
    particles[i] = new Particle(random(width), random(height));
  }

  pos = new PVector(width/2, height/2);
  //float maxD = sqrt(pow(width/2, 2) + pow(height/2, 2));
  for (int ix = 0; ix<cells; ix++) {
    for (int iy = 0; iy<cells; iy++) {
      field[ix][iy] = new PVector();
    }
  }
  //
}

color cAux;
float hueAux;
PImage frame;

void draw() {
  //background(0);
  fill(0, 16);
  rect(0, 0, width*2, height*2);

  colorMode(RGB, 255, 255, 255, 100);
  cAux = color(255-colors[0], 255-colors[1], 255-colors[2]);

  //Field
  stroke(cAux);
  for (Particle p : particles) {
    p.update();
    p.draw();
  }
  for (int x = 0; x<cells; x++) {
    for (int y = 0; y<cells; y++) {
      PVector p = field[x][y];
      getPosition(x, y);
      float a;
      if (mousePressed) {
        a = atan2(mouseY - temp.y, mouseX - temp.x) - PI * 0.6;
      } else {
        a = noise(x/(float)cells*2, y/(float)cells*2, t + frameCount/100f) * TWO_PI * 1.5;
      }
      p.set(cos(a), sin(a));
    }
  }
  //


  frame = cam.copy();
  frame.loadPixels();
  for (int i = 0; i < numPixels; i++) {
    color currColor = cam.pixels[i];
    color prevColor = previousFrame[i];
    int currR = (currColor >> 16) & 0xFF;
    int currG = (currColor >> 8) & 0xFF;
    int currB = currColor & 0xFF;
    int prevR = (prevColor >> 16) & 0xFF;
    int prevG = (prevColor >> 8) & 0xFF;
    int prevB = prevColor & 0xFF;
    int diffR = abs(currR - prevR);
    int diffG = abs(currG - prevG);
    int diffB = abs(currB - prevB);
    differenceFrame[i] = color(diffR, diffG, diffB);
    previousFrame[i] = currColor;
  }
  colors_mapped[0] = map(colors[0], 0, 255, 255, 0);
  colors_mapped[1] = map(colors[1], 0, 255, 255, 0);
  colors_mapped[2] = map(colors[2], 0, 255, 255, 0);
  fill(colors_mapped[0], colors_mapped[1], colors_mapped[2]);

  for (int row = 0; row < rows; row++) {
    int y = row * CELL_SIZE;
    for (int col = 0; col < cols; col++) {
      int x = col * CELL_SIZE;
      int pixelIndex = y * frame.width + x; // not x-mirrored version
      //int pixelIndex = y * frame.width + frame.width - x - 1; // x-mirrored version
      float diffBrightness = brightness(differenceFrame[pixelIndex]);
      float diffNorm = diffBrightness / 255;
      float rectDim = diffNorm * CELL_SIZE * 3;
      if (rectDim > 5)
        ellipse(x+CELL_SIZE/2, y+CELL_SIZE/2, rectDim, rectDim);
    }
  }
}

void captureEvent(Capture c) {
  c.read();
}

void serialEvent (Serial myPort) {
  // handle data from serial communication
  value = myPort.readString();

  if (value != null) {
    // remove breakline in the end of buffer
    String str = value.substring(0, value.length()-2);

    colors = float(split(str, ","));

    println ("received:", str);
  }
}
