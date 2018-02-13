#include <Adafruit_NeoPixel.h>

// set to pin connected to data input of WS8212 (NeoPixel) strip
#define PIN         0

// any pin with analog input (used to initialize random number generator)
#define RNDPIN      2

// number of LEDs (NeoPixels) in your strip
// (please note that you need 3 bytes of RAM available for each pixel)
#define NUMPIXELS   8

// max LED brightness (1 to 255) – start with low values!
// (please note that high brightness requires a LOT of power)
#define BRIGHTNESS  64

// increase to get narrow spots, decrease to get wider spots
#define FOCUS       65

// decrease to speed up, increase to slow down (it's not a delay actually)
#define DELAY       4000

// set to 1 to display FPS rate
#define DEBUG       1


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// we have 3 color spots (reg, green, blue) oscillating along the strip with different speeds
float spdr, spdg, spdb;
float offset;
unsigned char colors2[] = {
  0,0,0,
  0,0,0,
  100,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0,
  0,0,0
};

unsigned char colors[] = {
  255,0,100,
  148,0,211,
  75,0,130,
  0,0,255,
  0,255,0,
  255,255,0,
  255,127,0,
  255,0,0,
  255,0,100
};


int steps = 50;
int step = 0;
long time = 0;
int colorStep=1;

void setup() {
  // initialize pseudo-random number generator with some random value
  randomSeed(analogRead(RNDPIN));

  // initialize LED strip
  strip.begin();
  strip.show();

  time = millis();
}

void loop() {
  // use real time to recalculate position of each color spot
  long ms = millis();

  if(time+10 < ms)
  {
    time = ms;
    step++;
  }

  if(step>=steps)
  {
    step=0;
    colorStep = (colorStep+1)%NUMPIXELS;
  }

  // now iterate over each pixel and calculate it's color
  for (int i=0; i<NUMPIXELS; i++) {
      int color = ((colorStep+i)*3) % 24;
      int scale = 10;
      int color1 = color+3;

      float fr = colors[color+0]+(colors[color1+0]-colors[color+0])*step/steps;
      float fg = colors[color+1]+(colors[color1+1]-colors[color+1])*step/steps;
      float fb = colors[color+2]+(colors[color1+2]-colors[color+2])*step/steps;


      unsigned char cr = fr/scale;
      unsigned char cg = fg/scale;
      unsigned char cb = fb/scale;
       
      strip.setPixelColor(i,
        cr,
        cg,
        cb
      );
  }

  // send data to LED strip
  strip.show();
}


