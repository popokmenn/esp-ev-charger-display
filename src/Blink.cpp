#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

#include <Ethernet.h> // Ethernet library v2 is required
#include <ev_register.h>
#include <ModbusEthernet.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 4
#endif

// Define display pins
#define SHARP_SCK 26
#define SHARP_MOSI 27
#define SHARP_SS 14
#define DISPLAY_ON 13

#define BLACK 0
#define WHITE 1

// Define display dimensions
#define SHARP_WIDTH 400
#define SHARP_HEIGHT 240

uint16_t res1;
int yc0 = 50;
int x0 = 10;
int x1 = 220;
int x2 = 240;
int margin_y = 20;
int delaymb = 500;

// Create display object
Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, SHARP_WIDTH, SHARP_HEIGHT);

ModbusEthernet mb;

IPAddress remote(192, 168, 123, 123); // Address of Modbus Slave device
IPAddress ip(192, 168, 123, 110);     // The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE};

void writeLCD();
void writeFloat(float currentval, float lastval, int posx, int posy);
void writeInt(uint16_t currentval, uint16_t lastval, int posx, int posy);

void initializeDisplay(const char *message)
{
    display.begin();
    display.clearDisplay();
    display.setRotation(4);
    display.setTextSize(5);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.print(message);
    display.refresh();
}

void setup()
{
    Serial.begin(115200);
    pinMode(DISPLAY_ON, OUTPUT);
    digitalWrite(DISPLAY_ON, HIGH);

    // start & clear the display
    display.begin();
    display.clearDisplay();
    display.setRotation(4);
    delay(100);
    writeLCD();

    Ethernet.init(5);        // SS pin
    Ethernet.begin(mac, ip); // start the Ethernet connection
    delay(1000);             // give the Ethernet shield a second to initialize
    mb.client();
}

void readAndDisplayInt(uint16_t &currentVal, uint16_t &lastVal, uint16_t reg, int posx, int posy)
{
    lastVal = currentVal;
    mb.readIreg(remote, reg, &currentVal);
    Serial.print("Value:");
    Serial.println(currentVal);
    mb.task();
    writeInt(currentVal, lastVal, posx, posy);
    delay(delaymb);
}

void readAndDisplayFloat(float &currentVal, float &lastVal, uint16_t reg, int posx, int posy)
{
    uint16_t res[2] = {0};
    lastVal = currentVal;
    mb.readIreg(remote, reg, res, 2);
    currentVal = float(res[1]) * 0.1;
    Serial.print("Value:");
    Serial.println(currentVal);
    mb.task();
    writeFloat(currentVal, lastVal, posx, posy);
    delay(delaymb);
}

void loop()
{
    if (mb.isConnected(remote))
    {
        readAndDisplayInt(STATE, lSTATE, REG_STATE, x2, yc0);
        readAndDisplayInt(CHG_STATE, lCHG_STATE, REG_CHG_STATE, x2, yc0 + margin_y);
        readAndDisplayInt(CHG_POW, lCHG_POW, REG_CHG_POW, x2, yc0 + 2 * margin_y);
        readAndDisplayFloat(I_OUT, lI_OUT, REG_I_OUT, x2, yc0 + 3 * margin_y);
        readAndDisplayInt(ACT_CHG_ENR, lACT_CHG_ENR, REG_ACT_CHG_ENR, x2, yc0 + 4 * margin_y);
        readAndDisplayFloat(V_L1, lV_L1, REG_V_L1, x2, yc0 + 5 * margin_y);
        readAndDisplayFloat(V_L2, lV_L2, REG_V_L2, x2, yc0 + 6 * margin_y);
        readAndDisplayFloat(V_L3, lV_L3, REG_V_L3, x2, yc0 + 7 * margin_y);
        display.refresh();
    }
    else
    {
        Serial.println("Try to connect");
        mb.connect(remote);
    }
    delay(500);
    mb.task(); // Common local Modbus task
}

void writeLCD()
{
    display.clearDisplayBuffer();
    display.setTextSize(3);
    display.setTextColor(BLACK);
    display.setCursor(50, 10);
    display.println("EV CHG - AC MAX");

    display.setTextSize(2);
    display.setCursor(x0, yc0);
    display.print("STATE");
    display.setCursor(x1, yc0);
    display.print(":");

    display.setCursor(x0, yc0 + margin_y);
    display.print("CHG STATE");
    display.setCursor(x1, yc0 + margin_y);
    display.print(":");

    display.setCursor(x0, (yc0 + 2 * margin_y));
    display.print("CHG POW (W)");
    display.setCursor(x1, (yc0 + 2 * margin_y));
    display.print(":");

    display.setCursor(x0, (yc0 + 3 * margin_y));
    display.print("I OUT (A)");
    display.setCursor(x1, (yc0 + 3 * margin_y));
    display.print(":");

    display.setCursor(x0, (yc0 + 4 * margin_y));
    display.print("ACT_CHG_ENR (WH)");
    display.setCursor(x1, (yc0 + 4 * margin_y));
    display.print(":");

    display.setCursor(x0, (yc0 + 5 * margin_y));
    display.print("V_L1 (V)");
    display.setCursor(x1, (yc0 + 5 * margin_y));
    display.print(":");

    display.setCursor(x0, (yc0 + 6 * margin_y));
    display.print("V_L2 (V)");
    display.setCursor(x1, (yc0 + 6 * margin_y));
    display.print(":");

    display.setCursor(x0, (yc0 + 7 * margin_y));
    display.print("V_L3 (V)");
    display.setCursor(x1, (yc0 + 7 * margin_y));
    display.print(":");

    display.refresh();
    // display.clearDisplayBuffer();
}

// write new character and erase past character (float)
void writeFloat(float currentval, float lastval, int posx, int posy)
{
    display.setTextSize(2);
    display.setCursor(posx, posy);
    display.setTextColor(WHITE, BLACK);
    display.print(lastval, 2);
    display.setCursor(posx, posy);
    display.setTextColor(BLACK, WHITE);
    display.print(currentval, 2);
}

// write new character and erase past character (int)
void writeInt(uint16_t currentval, uint16_t lastval, int posx, int posy)
{
    display.setTextSize(2);
    display.setCursor(posx, posy);
    display.setTextColor(WHITE, BLACK);
    display.print(lastval);
    display.setCursor(posx, posy);
    display.setTextColor(BLACK, WHITE);
    display.print(currentval);
}