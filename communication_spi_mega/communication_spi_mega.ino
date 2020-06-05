#include <SPI.h>
#include <MCUFRIEND_kbv.h>
#include <avr/pgmspace.h>
#include <UTFTGLUE.h>
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

#define BACKGROUND_COLOR BLACK

#define CENTER_X (320/2)
#define CENTER_Y (480/2) 
#define DIAL_RADIUS 50 //radius of the dial
#define SINE_TABLE_SIZE 360
#define SINE_TABLE_FACTOR 512

volatile union Float_type{
  volatile uint8_t _byte[4];
  volatile float _float;
  volatile uint32_t _int;
}Float_type;

//uint8_t frame_buffer[320*480];//a simple frame buffer

volatile uint8_t loadFlag;
float oldAngle;
MCUFRIEND_kbv tft;
uint16_t g_id;
float sin_table[SINE_TABLE_SIZE];//sine lut with 2048 precision

/**
 * builds a LUT for a sine function
 * output: sine_table
 */
void buildLUT()
{
  uint16_t i;
  for(i=0;i<SINE_TABLE_SIZE;i++)
  {
    sin_table[i] = (float)(sin(((float)i)*(PI/180)));
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
//  bitSet(SPCR,SPR1);
//  bitSet(SPCR,SPR0);//div by 128
  bitSet(SPCR,SPE);//enable 
  loadFlag=0;
  Float_type._int=0;
  sei();//enable interrupts
}

/**
 * initializes SPI connection to LCD
 */
void initTFT()
{
  tft.reset();
  g_id = tft.readID();//read the driver number from the tft module
  tft.begin(g_id);//begin the tft with specified driver number (should ensure cross- compat)
  tft.fillScreen(BACKGROUND_COLOR);
  tft.drawCircle(CENTER_X,CENTER_Y,50,WHITE);//draws a circle specifying the circle radius
  tft.setCursor(0,0);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Wireless Angle \nMeasurement Tool");
  //tft.println("waiting for data...");
  tft.setTextSize(2);
  oldAngle=0;
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
  for(ty=0;ty<sy;ty++)
  {
    for(tx=0;tx<sx;tx++)
    {
      color = pgm_read_word(data+(ty*sx)+tx);
      newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin_table[(int)((radian/(2*PI))*360)]));
      newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin_table[(int)((radian/(2*PI))*360)]));
      tft.drawPixel(newx,newy,color);
    }
  }
}

void clearBitmap(int x, int y, int sx, int sy, const uint16_t *data, int deg, int rox, int roy)
{
  unsigned int color;
  int tx, ty, newx, newy;
  double radian;
  radian = deg*0.0175;
  for(ty=0;ty<sy;ty++)
  {
    for(tx=0;tx<sx;tx++)
    {
      color = BACKGROUND_COLOR;
      newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin_table[(int)((radian/(2*PI))*360)]));
      newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin_table[(int)((radian/(2*PI))*360)]));
      tft.drawPixel(newx,newy,color);
    }
  }
}

/**
 * writes out the value from the MSP - 
 * should be the transferred angle currently the angle drawing math is a little weird
 */
void writeScreen()
{
  //cli();//disable interrupts
  float local = ((float)(Float_type._int))/1024;//get the data quick
  char number_as_str[50];
  float ratio;
  //Serial.println(local);

  if(local<180&&local!=0)
  {
    clearBitmap(CENTER_X,CENTER_Y,61,6,gauge,oldAngle,0,3);
    oldAngle = local;
    tft.drawCircle(CENTER_X,CENTER_Y,50,WHITE);

    drawBitmap(CENTER_X,CENTER_Y,61,6,gauge,local,0,3);

    tft.setCursor(0,450);

    tft.fillRect(0,450,320,30,BLACK);
    tft.print(local);
  }
  

  //ratio = sin_table[(int)local];

  
  //tft.drawLine(CENTER_X,CENTER_Y,(int)(CENTER_X+(DIAL_RADIUS*ratio)),(int)(CENTER_Y+(DIAL_RADIUS*(ratio))),RED);
  
  //reset values
  Float_type._int=0;
  //sei();//re-enable interrupts
}

/**
 * ISR for spi communication - 
 */
ISR (SPI_STC_vect)
{
  byte data = SPDR;//get the data from the data register
  char str[10];
  //Serial.print(data);
  //sprintf(str,"%c",data);
  
  //Serial.println(str);
  Float_type._byte[loadFlag] = data;
  //loadData.split_val[loadFlag] = data;
  //loadData|=(data<<(loadFlag*sizeof(char)));//fill the global var to the correct data
  loadFlag+=1;
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  buildLUT();
  initSPI();
  initTFT();
  
  //drawBitmap(CENTER_X,CENTER_Y,61,6,gauge,0,0,3);

}

void loop() {
  while(loadFlag<4);
  loadFlag=0;
  writeScreen();
  
  
}
