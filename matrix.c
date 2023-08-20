/*
Matrix NES Demo - MIT License
Copyright (c) 2021-2022 Ninja Dynamics

*edited 2023 Rani Timekey to add music and other effects*

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*Rani Timekey Notes Aug 2023*

Few things:
*the original Ninja Dynamics version wasn't actually random because 
it doesn't seed the random number generator. It plays out exactly
the same every time you run it. Added a timer (which counts off 
cycles before you hit start) which then seeds the random number 
generator with that. I would recommend setting aside a timer byte
in your project for this to ensure truly random results.
*Canonically, all the characters in the MATRIX "Digital Rain"
effect are backwards, rather than both forwards and backwards. 
Changed the graphics to represent that. Doing so freed up a ton of space.
*Added sound and music as well as the ability to turn the 
"Digital Rain" state on and off to better illustrate how one could
implement this into their own projects. Also included code to
speed up or slow down the rain with the left and right buttons.
That part is not some of my better coding work, just wanted to
throw in one more bell and whistle before I made this public.
*The background music is a remix of Theodore Kerr's "Battle" theme,
under Creative Commons licensing. Sounds are Shiru's default 
Famitone sound effects.
*/

#include "array.h"

#include "vram.h"
#include "neslib.h"

//#link "vram.c"
//#link "chr_matrix.s"

//#link "famitone2.s"
//#link "matrix_music.s"
//#link "demosounds.s"

//#define USE_OPAQUE_CHAR 

#define CHR_MASK   (0x0F)
#define CHR_START  (0xA0)
#define CHR_AMOUNT (0xFF - CHR_START)

static word addr;

static char x;
static char y;
static char r;

static char i;

static char start[32];
static char chars[32];

static char sprId;
static char tileY, pixelY;

static char density;
static signed char dir;

// Additional variables added by Rani Timekey
extern const void sound_data[];
extern const void music_data[];
int j;          //for loops
char randseed;  // seed value for random number generator
char rainspeed = 32; // speed control for digital rain
char pad;       // controller input
bool start_pressed = false;  // Only allows one input from Start Button at a time.
bool left_pressed = false;   // Only allows one input from Select Button at a time.
bool right_pressed = false;  // Only allows one input from Select Button at a time.
char title_select = 0; // 0 = Title Screen, 1 = Digital Rain
char speed_select = 9; // moves between speed values
char speed_values[10] = 
     {
     1,2,4,6,8,10,12,16,20,32
     }; // value string for digital rain slowdown


// Yeah this is an empty Attribute Table, but including it anyway.
// Will make it easier for you to add your own custom screen.
const char ATTRIBUTE_TABLE_1[0x40] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 0-3
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 4-7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 8-11
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 12-15
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 16-19
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 20-23
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 24-27
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // rows 28-29
                                   };
const char PALETTE[32] = {
    0x0F, // screen color
          0x0B, 0x1A, 0x3C, // background palette 0
    0x0F, 0x0F, 0x0F, 0x0F, // background palette 1
    0x0F, 0x0F, 0x0F, 0x0F, // background palette 2
    0x0F, 0x0F, 0x0F, 0x0F, // background palette 3

    0x0F, 0x0F, 0x0F, 0x0F, // sprite palette 0
    0x0F, 0x0F, 0x0F, 0x0F, // sprite palette 1
    0x0F, 0x0F, 0x0F, 0x0F, // sprite palette 2
    0x0F, 0x0F, 0x3C, 0x0F  // sprite palette 3
                         }; //Screen Pallette

const char TITLE_SCREEN[960] = 
   {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x67, 0x68, 0x80, 0x81, 0x82, 
   0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x77, 0x78, 0x90, 0x91, 0x92, 
   0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x00, 0x09, 
   0x0A, 0x0B, 0x0C, 0x00, 0x00, 0x74, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 
   0x1A, 0x1B, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 
   0x2A, 0x2B, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 
   0x3A, 0x3B, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x48, 0x49, 
   0x4A, 0x4B, 0x4C, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x56, 0x57, 0x58, 0x59, 
   0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x6E, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7B, 0x7C, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x7E, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x8C, 0x50, 0x52, 0x45, 
   0x53, 0x53, 0x00, 0x00, 0x8E, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x9B, 0x9C, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6A, 0x00, 0x00, 0x00, 0x53, 0x54, 
   0x41, 0x52, 0x54, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x8A, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x8D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x27, 0x28, 0x9D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x3E, 0x3F, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x37, 0x1E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x4F, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x30, 0x32, 0x33, 0x00, 0x00, 0x52, 0x41, 0x51, 
   0x44, 0x00, 0x54, 0x44, 0x43, 0x45, 0x42, 0x45, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
   }; //Title Screen Tiles

void draw_background()
  {
  ppu_off();
  pal_all(PALETTE);
  vram_adr(0x23c0);
  vram_write(ATTRIBUTE_TABLE_1, sizeof(ATTRIBUTE_TABLE_1));
  vram_adr(NTADR_A(0, 0));
  for (j = 0; j < sizeof(TITLE_SCREEN); j++)
    { //Draw_Title Screen
    vram_put(TITLE_SCREEN[j]);
    }
  ppu_on_all();
  }

void putChar(char _i, char _x, char _y, char _c) 
  {
  addr = NTADR(NAMETABLE_A, _x, _y);
  vram_buffer[VRB_TILES_MSB(_i) ] = MSB(addr);
  vram_buffer[VRB_TILES_LSB(_i) ] = LSB(addr);
  vram_buffer[VRB_TILES_DATA(_i)] = _c; 
  }

void initDigitalRain(void) 
  {

  // Set the VRAM buffer
  set_vram_update(vram_buffer);  
  
  // Define the Y starting positions for each column
  for (i = 0; i < 32; ++i) 
    {
    start[i] = rand8() % 30;
    }  
        
  // Set the code density
  density = 248;
  dir = -1; 
  }



void DigitalRain(void) 
  {
  // Make the density oscilate
  density += dir;
  if (density == 224 || density == 248) dir = -dir;
  // Don't ask me, it just works
  for (x = 0; x < 32; ++x) 
     { 
     r = rand8();
     if (r < 192) continue;
     chars[x] = (r >= density);      
     }       
 
  // Put the characters on screen    
  for (x = 0; x < 32; ++x) 
     {           
     if (title_select == 1)
        {
        // Set the random character in the buffer
        if (chars[x]) chars[x] = CHR_START + (rand8() % CHR_AMOUNT);   
        // Calculate the Y tile and pixel positions
        tileY = (start[x] + y) % 30; pixelY = (tileY * 8);          
        // Draw the character in white using a sprite
        sprId = oam_spr(x * 8, pixelY - 1, chars[x], 0x03, sprId);
   
        #ifdef USE_OPAQUE_CHAR      
        // Draw an opaque blank character in order to "erase" what's below
        sprId = oam_spr(x * 8, pixelY - 1, 0x0F, 0x02, sprId); // Optional    
        #endif
      
        // Draw the character in green using a background tile
        putChar(x, x, tileY, chars[x]);
        }
     else
        {//erase stray VRAM noise by nulling tiles and sprites out
        sprId = oam_spr(x * 8, pixelY - 1, 0x00, 0x02, sprId); //Makes sprites transparent
        putChar(x, 0, 30, 0x00);
        }
     }   
  // Clean-up
  oam_hide_rest(sprId); sprId = 0;
  vram_buffer[LAST_INDEX_OF(vram_buffer)] = NT_UPD_EOF;
  ppu_wait_nmi(); 
  // Increase Y position and loop
  ++y;
  }

// main function, run after console reset
void main(void) 
  {  
  famitone_init(&music_data);
  sfx_init(&sound_data);
  nmi_set_callback(famitone_update);  
  music_play(0);
  draw_background();  
  // Enter the Matrix
  while (1) 
     {    
     pad = pad_poll(0);
     if (title_select == 0)
        {
        randseed++;
        if (pad & PAD_START)
           {
           if (!start_pressed)
              {
              start_pressed = true;
              music_play(1);
              sfx_play(2,0);
              set_rand(randseed); // Seed random number generator
              initDigitalRain();
              title_select = 1; //Set to Digital Rain loop
              }
           }
        else
           {
           start_pressed = false;
           }
        }
     if (title_select == 1)
        {
        randseed++; // Used as an animation timer here
        if (pad & PAD_LEFT)
           {
           if (!left_pressed)
              {
              left_pressed = true;
              if (speed_select > 0)
                 {
                 sfx_play(2,0);
                 speed_select--;                 
                 }
              }
           }
        else
          {
          left_pressed = false;
          }
        if (pad & PAD_RIGHT)
           {
           if (!right_pressed)
              {
              right_pressed = true;
              if (speed_select < 9)
                 {
                 sfx_play(2,0);
                 speed_select++;
                 }
              }
           }
        else
          {
          right_pressed = false;
          }
        rainspeed = speed_values[speed_select];
        if (pad & PAD_START)
           {
           if (!start_pressed)
              {
              start_pressed = true;
              title_select = 0; // Set to Title Screen loop
              music_play(0);
              sfx_play(2,0);
              for (j = 0; j < 2; ++j) 
                 { 
                 DigitalRain(); //Run loop a couple times with null values to clear VRAM noise
                 }
              draw_background();               
              }
           }
        else
           {
           start_pressed = false;
           if (rainspeed >= 32)
              {          
              DigitalRain(); // Not my best work here, but this gets rid of the hiccup at full speed
              }
           else
              {//compares the timers to slow down the digital rain
              if ((randseed + 1) / rainspeed == 1)
                 {          
                 DigitalRain();
                 }
              }
           }
        }
   
     };
}
