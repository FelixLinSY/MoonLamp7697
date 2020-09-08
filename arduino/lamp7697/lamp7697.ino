

#include <EEPROM.h>
#include <LRemote.h>
#include <Adafruit_NeoPixel.h>

#define EE_ADDR_RED_1       20
#define EE_ADDR_GREEN_1     30
#define EE_ADDR_BLUE_1      40
#define EE_ADDR_RED_2       50
#define EE_ADDR_GREEN_2     60
#define EE_ADDR_BLUE_2      70

#define LED_VALUE_MIN   0
#define LED_VALUE_MAX   255

#define PIN            7
#define NUMPIXELS      4
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

LRemoteButton   btn_random;
LRemoteButton   btn_save1;
LRemoteButton   btn_save2;
LRemoteButton   btn_load1;
LRemoteButton   btn_load2;
LRemoteSlider   slider_red;
LRemoteSlider   slider_green;
LRemoteSlider   slider_blue;
LRemoteLabel    label;
LRemoteLabel    label_red;
LRemoteLabel    label_green;
LRemoteLabel    label_blue;
LRemoteSwitch   switchButton;
LRemoteCircleButton bigButton;

int  red, green, blue;
int  update_led;
void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    red = EEPROM.read(EE_ADDR_RED_1);
    green = EEPROM.read(EE_ADDR_GREEN_1);
    blue = EEPROM.read(EE_ADDR_BLUE_1);
    
    randomSeed(analogRead(0));
    
    pixels.begin(); // This initializ   es the NeoPixel library.
    for(int i=0;i<NUMPIXELS;i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(red, green, blue)); // Moderately bright green color.
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
    
    Serial.println("Start configuring remote");

    // Initialize GPIO
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);

    // Setup the Remote Control's UI canvas
    LRemote.setName("Lamp");
    LRemote.setOrientation(RC_PORTRAIT);
    LRemote.setGrid(4, 6);

    // Add a push button
    btn_random.setText("Random");
    btn_random.setPos(2, 4);
    btn_random.setSize(2, 2);
    btn_random.setColor(RC_YELLOW);
    LRemote.addControl(btn_random);

    // Add a push button
    btn_save1.setText("save1");
    btn_save1.setPos(0, 4);
    btn_save1.setSize(1, 1);
    btn_save1.setColor(RC_YELLOW);
    LRemote.addControl(btn_save1);
    
    // Add a push button
    btn_save2.setText("save2");
    btn_save2.setPos(1, 4);
    btn_save2.setSize(1, 1);
    btn_save2.setColor(RC_YELLOW);
    LRemote.addControl(btn_save2);

    // Add a push button
    btn_load1.setText("load1");
    btn_load1.setPos(0, 5);
    btn_load1.setSize(1, 1);
    btn_load1.setColor(RC_YELLOW);
    LRemote.addControl(btn_load1);
    
    // Add a push button
    btn_load2.setText("load2");
    btn_load2.setPos(1, 5);
    btn_load2.setSize(1, 1);
    btn_load2.setColor(RC_YELLOW);
    LRemote.addControl(btn_load2);
    
    // slider for Red Led
    slider_red.setText("Red LED");
    slider_red.setPos(0, 1);
    slider_red.setSize(3, 1);
    slider_red.setColor(RC_PINK);
    slider_red.setValueRange(LED_VALUE_MIN, LED_VALUE_MAX, red);
    LRemote.addControl(slider_red);
    
    // slider for Green Led
    slider_green.setText("Green LED");
    slider_green.setPos(0, 2);
    slider_green.setSize(3, 1);
    slider_green.setColor(RC_GREEN);
    slider_green.setValueRange(LED_VALUE_MIN, LED_VALUE_MAX, green);
    LRemote.addControl(slider_green);
    
    // slider for Blue Led
    slider_blue.setText("Blue LED");
    slider_blue.setPos(0, 3);
    slider_blue.setSize(3, 1);
    slider_blue.setColor(RC_BLUE);
    slider_blue.setValueRange(LED_VALUE_MIN, LED_VALUE_MAX, blue);
    LRemote.addControl(slider_blue);
    
    // Add a Title label
    label.setText("Moon/Voronoi LED Lamp");
    label.setPos(0, 0);
    label.setSize(4, 1);
    label.setColor(RC_GREY);
    LRemote.addControl(label);
  
    // Add a red value label
    label_red.setText("");
    label_red.setPos(3, 1);
    label_red.setSize(1, 1);
    label_red.setColor(RC_PINK);
    LRemote.addControl(label_red);
    
    // Add a green value label
    label_green.setText("");
    label_green.setPos(3, 2);
    label_green.setSize(1, 1);
    label_green.setColor(RC_GREEN);
    LRemote.addControl(label_green);
    
    // Add a Title label
    label_blue.setText("");
    label_blue.setPos(3, 3);
    label_blue.setSize(1, 1);
    label_blue.setColor(RC_BLUE);
    LRemote.addControl(label_blue);

    update_led = 1;
    
    // Start broadcasting our remote contoller
    // This method implicitly initialized underlying BLE subsystem
    // to create a BLE peripheral, and then
    // start advertisement on it.
    LRemote.begin();
    Serial.println("begin() returned");
}

void loop() {
    String        temp_str;

    // check if we are connect by some
    // BLE central device, e.g. an mobile app
    if (!LRemote.connected()) {
        Serial.println("waiting for connection");
        delay(1000);
    } else {
        // The interval between button down/up
        // can be very short - e.g. a quick tap
        // on the screen.
        // We could lose some event if we
        // delay something like 100ms.
        delay(15);
    }

    // Process the incoming BLE write request
    // and translate them to control events
    LRemote.process();

    // Now we poll each control's status

    if ((btn_random.isValueChanged()) && (btn_random.getValue())) {
        Serial.print("btn_random new value =");
        Serial.println(btn_random.getValue());
        red = random(LED_VALUE_MAX);
        green = random(LED_VALUE_MAX);
        blue = random(LED_VALUE_MAX);
        update_led = 1;
    }
    
    if ((btn_save1.isValueChanged()) && (btn_save1.getValue())) {
        EEPROM.write(EE_ADDR_RED_1, red);
        EEPROM.write(EE_ADDR_GREEN_1, green);
        EEPROM.write(EE_ADDR_BLUE_1, blue);
        Serial.println("save profile 1 to EEPROM");
    }

    if ((btn_save2.isValueChanged()) && (btn_save2.getValue())) {
        EEPROM.write(EE_ADDR_RED_2, red);
        EEPROM.write(EE_ADDR_GREEN_2, green);
        EEPROM.write(EE_ADDR_BLUE_2, blue);
        Serial.println("save profile 2 to EEPROM");
    }
    
    if ((btn_load1.isValueChanged()) && (btn_load1.getValue())) {
        red = EEPROM.read(EE_ADDR_RED_1);
        green = EEPROM.read(EE_ADDR_GREEN_1);
        blue = EEPROM.read(EE_ADDR_BLUE_1);
        update_led = 1;
    }
    
    if ((btn_load2.isValueChanged()) && (btn_load2.getValue())) {
        red = EEPROM.read(EE_ADDR_RED_2);
        green = EEPROM.read(EE_ADDR_GREEN_2);
        blue = EEPROM.read(EE_ADDR_BLUE_2);
        update_led = 1;
    }

    if (slider_red.isValueChanged()) {
        red = slider_red.getValue();
        Serial.print("slider to new value = ");
        Serial.println(slider_red.getValue());
        update_led = 1;
    }
    if (slider_green.isValueChanged()) {
        green = slider_green.getValue();
        Serial.print("slider to new value = ");
        Serial.println(slider_green.getValue());
        update_led = 1;
    }
    if (slider_blue.isValueChanged()) {
        blue = slider_blue.getValue();
        Serial.print("slider to new value = ");
        Serial.println(slider_blue.getValue());
        update_led = 1;
    }
    if (update_led) {
        update_led = 0;
        temp_str = String(red, DEC);
        label_red.updateText(temp_str);
        temp_str = String(green, DEC);
        label_green.updateText(temp_str);
        temp_str = String(blue, DEC);
        label_blue.updateText(temp_str);
        
        for(int i=0;i<NUMPIXELS;i++){
            // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
            pixels.setPixelColor(i, pixels.Color(red, green, blue)); // Moderately bright green color.
        }
        pixels.show(); // This sends the updated pixel color to the hardware.
    }
}
