#include <Adafruit_NeoPixel.h>

// set to pin connected to data input of WS8212 (NeoPixel) strip
#define PIN         0

// any pin with analog input (used to initialize random number generator)
#define RNDPIN      2

// number of LEDs (NeoPixels) in your strip
// (please note that you need 3 bytes of RAM available for each pixel)
#define NUMPIXELS   8

// decrease to speed up, increase to slow down (it's not a delay actually)

#define DELAY 0
#define STEPS 70
#define SCALE 16

#define PI 3.141593

float fr, fg, fb; // float for temporary values
unsigned char cr, cg, cb; // uint_8 for rgb
int step = 0;
int direction = 1;
long time = 0;
int colorStep = 0;
int loopTime = 100;
int loopStep = 0;

unsigned char screen[256];
unsigned char pixels[NUMPIXELS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // initialize pseudo-random number generator with some random value
  randomSeed(analogRead(RNDPIN));

  clearScreen();
  clearPixels();
  drawLineH(6);
  drawLineH(10);
  drawLineV(6);
  drawLineV(10);
  clearPixels();

  // initialize LED strip
  strip.begin();
  strip.show();

  time = millis();
}

void loop() {
  // use real time to recalculate position of each color spot
  long ms = millis();

//  if (time + DELAY < ms)
  {
    time = ms;

    loopStep = (loopStep+1) % loopTime;
    
    calcPixels(loopStep);
    showPixels();
  }
}

void drawLineH(int line)
{
  for(int i=0;i<16;i++)
    screen[line*16+i] = 255;
}

void drawLineV(int col)
{
  for(int i=0;i<16;i++)
    screen[i*16+col] = 255;
}

void calcPixels(int angle)
{
  float da = PI*2*angle/loopTime;

  float fx = sin(da)*16;
  float fy = cos(da)*16;

  for(int r=0;r<8;r++)
  {
    int ix = 7-fx*r/16;
    int iy = 7-fy*r/16;

    pixels[r*3+0] = screen[iy*16+ix];
  }
}

void clearScreen()
{
  for(int i=0;i<256;i++)
    screen[i]=0;
}
void clearPixels()
{
  for(int i=0;i<NUMPIXELS*3;i++)
    pixels[i]=0;
}

void showPixels()
{
    // now iterate over each pixel and calculate it's color
  for (int i = 0; i < NUMPIXELS; i++) {
    fr = pixels[i*3 + 0];
    fg = pixels[i*3 + 1];
    fb = pixels[i*3 + 2];

    cr = fr / SCALE;
    cg = fg / SCALE;
    cb = fb / SCALE;

    strip.setPixelColor(i,
                        cr,
                        cg,
                        cb
                       );
  }

  // send data to LED strip
  strip.show();
}




