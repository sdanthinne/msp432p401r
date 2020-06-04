#include <SPI.h>
#include <MCUFRIEND_kbv.h>
#include <avr/pgmspace.h>
#include <UTFTGLUE.h>
#include "math.h"
#include "string.h"
#include "gauge-needle-png-4.c"
/**
   ss = 53
   sck = 52
   MOSI = 51
   MISO = 50
*/
/**
 * Maybe to transfer data - use 5 bytes bytes 1,2,3 show location, byte 4,5 is color?
 * Or should the graphics processing be handled by the arduino - maybe faster? (just send an angle value)
 */
#define SS 53
#define SCK 52
#define MOSI 51
#define MISO 50

/**
 * color macros
 */
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define CENTER_X (320/2)
#define CENTER_Y (480/2) 
#define DIAL_RADIUS 50 //radius of the dial
#define SINE_TABLE_SIZE 1024

volatile union Float_type{
  uint8_t _byte[4];
  float _float;
  uint32_t _int;
}Float_type;

volatile uint8_t loadFlag;
MCUFRIEND_kbv tft;
uint16_t g_id;
int32_t sine_table[SINE_TABLE_SIZE];//sine lut with 2048 precision

/**
 * builds a LUT for a sine function
 * output: sine_table
 */
void buildLUT()
{
  uint16_t i;
  for(i=0;i<SINE_TABLE_SIZE;i++)
  {
    sine_table[i] = 128*sin((360/2048)*((double)i));
  }
}

/**
 * initializes SPI connection to the MSP 
 */
void initSPI()
{
  SPI.begin();
  pinMode(SS, INPUT);
  bitClear(SPCR, 4); //set as slave
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);//ckpl=1, cphase = 0 output edge = rising, data capture = falling
  bitSet(SPCR, 7); //enable SPI interrupts
  bitSet(SPCR,SPR1);
  bitSet(SPCR,SPR0);//div by 128
  bitSet(SPCR,SPE);//enable 
  loadFlag=0;
  Float_type._int=0;
}

/**
 * initializes SPI connection to LCD
 */
void initTFT()
{
  tft.reset();
  g_id = tft.readID();//read the driver number from the tft module
  tft.begin(g_id);//begin the tft with specified driver number (should ensure cross- compat)
  tft.fillScreen(BLACK);
  //tft.drawCircle(CENTER_X,CENTER_Y,50);//draws a circle specifying the circle radius
  tft.setCursor(30,0);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Wireless Angle Measurement Tool");
  tft.println("waiting for data...");
}




/**
 * drawBitmap adapted from UTFT library 
 * 
 */
void drawBitmap(int x, int y, int sx, int sy, const uint16_t *data, int deg, int rox, int roy)
{
  unsigned int color;
  int tx, ty, newx, newy;
  double radian;
  radian = deg*0.0175;
  
  
}

/**
 * writes out the value from the MSP - 
 * should be the transferred angle currently the angle drawing math is a little weird
 */
void writeScreen()
{
  cli();//disable interrupts
  float local = Float_type._float;//get the data quick
  char number_as_str[50];
  uint16_t ratio;
  
  //drawBitmap(CENTER_X,CENTER_Y,163,15,gauge,data,0,7);
  
  //sprintf(number_as_str,"%f",local);
  //Serial.println(local); 
  tft.print(number_as_str);
  //ratio = sine_table[(int)local];
  //tft.drawLine(CENTER_X,CENTER_Y,(DIAL_RADIUS*ratio)>>7+CENTER_X,(1-((DIAL_RADIUS*ratio)>>7))+CENTER_Y,RED);
  
  //reset values
  Float_type._int=0;
  sei();//re-enable interrupts
}

/**
 * ISR for spi communication - 
 */
ISR (SPI_STC_vect)
{
  byte data = SPDR;//get the data from the data register
  char str[10];
  //Serial.print(data);
  sprintf(str,"%c",data);
  
  Serial.println(str);
  Float_type._byte[loadFlag] = data;
  //loadData.split_val[loadFlag] = data;
  //loadData|=(data<<(loadFlag*sizeof(char)));//fill the global var to the correct data
  loadFlag+=1;
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initSPI();
  initTFT();
  buildLUT();
}

void loop() {
  while(loadFlag<4);
  loadFlag=0;
  writeScreen();
  
  
}
