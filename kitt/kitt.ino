#include <Adafruit_NeoPixel.h>

// set to pin connected to data input of WS8212 (NeoPixel) strip
#define PIN         0

// any pin with analog input (used to initialize random number generator)
#define RNDPIN      2

// number of LEDs (NeoPixels) in your strip
// (please note that you need 3 bytes of RAM available for each pixel)
#define NUMPIXELS   8

// decrease to speed up, increase to slow down (it's not a delay actually)

#define DELAY 3
#define STEPS 70
#define SCALE 10

float fr, fg, fb; // float for temporary values
unsigned char cr, cg, cb; // uint_8 for rgb
int step = 0;
int direction = 1;
long time = 0;
int colorStep = 0;

unsigned char colors[] = {
  0, 0, 0,
  0, 0, 0,
  0, 0, 0,
  0, 0, 0,
  0, 0, 0,
  0, 0, 0,
  0, 0, 0,
  0, 0, 0,
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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

  if (time + DELAY < ms)
  {
    time = ms;
    step++;

    if(step>STEPS)
    {
      step=0;
      colorStep += direction;
      
      if (colorStep >= NUMPIXELS-1)
      {
        direction = -1;
      }
      if (colorStep < 1)
      {
        direction = 1;
      }

      colors[colorStep*3] = 255;
    }

    for(int i=0;i<NUMPIXELS;i++)
    {
      if(colors[i*3]>0)
      {
        colors[i*3]--;
      }
    }
  }


  // now iterate over each pixel and calculate it's color
  for (int i = 0; i < NUMPIXELS; i++) {
    fr = colors[i*3 + 0];
    fg = colors[i*3 + 1];
    fb = colors[i*3 + 2];

    unsigned char cr = fr / SCALE;
    unsigned char cg = fg / SCALE;
    unsigned char cb = fb / SCALE;

    strip.setPixelColor(i,
                        cr,
                        cg,
                        cb
                       );
  }

  // send data to LED strip
  strip.show();
}


