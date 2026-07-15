#include <ESP8266WiFi.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>

void initCC1101(float mhz);
void sendSamples(int samples[], int samplesLenght, float mhz);
void enterDeepSleep();

// CC1101 to NodeMCU ESP8266 has the following connections:
// CS pin:    15
// GDO0 pin:  5
// RST pin:   unused
// GDO2 pin:  4 (optional)


// M1101
#define PIN_GDO0 5
#define PIN_GDO2 4


// LED

#define PIN_LED_ONBOARD 2

#define LENGTH_SAMPLES_MY_SIGNAL 617
int samples_my_signal[LENGTH_SAMPLES_MY_SIGNAL] = {2993,-98,18655,-100,829,-164,623,-198,1279,-98,393,-98,165,-132,531,-166,197,-134,365,-592,133,-794,99,-132,67,-798,231,-296,65,-166,133,-900,165,-134,2389,-232,1491,-66,131,-268,365,-166,1193,-66,269,-134,825,-66,18301,-726,353,-740,359,-720,363,-722,399,-722,339,-746,373,-712,377,-712,377,-740,349,-726,377,-726,335,-750,371,-322,751,-758,347,-310,765,-324,783,-322,781,-328,745,-336,745,-738,353,-744,365,-358,723,-756,337,-746,337,-8124,751,-740,347,-754,323,-748,361,-716,399,-724,337,-758,337,-748,337,-746,373,-734,355,-728,355,-726,355,-756,341,-344,745,-754,349,-340,731,-352,763,-356,723,-354,751,-356,751,-728,355,-724,373,-310,765,-752,349,-738,325,-8132,743,-740,349,-740,351,-744,361,-720,365,-746,377,-712,377,-724,337,-750,371,-726,339,-748,375,-712,375,-714,377,-320,753,-732,373,-310,767,-322,783,-334,743,-374,739,-346,735,-746,355,-754,333,-358,721,-756,337,-758,335,-8128,747,-738,347,-752,323,-748,361,-722,363,-756,337,-758,335,-748,339,-746,373,-726,375,-726,337,-748,337,-746,373,-310,769,-742,347,-342,753,-332,779,-344,725,-338,757,-336,757,-730,351,-744,365,-354,729,-758,347,-726,343,-8120,751,-740,323,-746,353,-744,361,-720,365,-756,337,-746,339,-746,375,-714,375,-726,375,-726,335,-746,339,-748,375,-340,733,-736,347,-348,743,-372,739,-350,757,-344,723,-338,777,-742,351,-718,363,-354,731,-736,353,-732,341,-8112,765,-720,365,-748,337,-748,337,-746,373,-734,355,-724,341,-766,317,-780,353,-748,333,-754,333,-748,337,-748,373,-322,751,-732,341,-344,767,-324,777,-336,741,-374,739,-346,721,-770,355,-716,363,-358,721,-756,337,-746,339,-8112,753,-744,323,-778,323,-742,361,-720,365,-756,337,-748,339,-748,375,-712,375,-736,355,-726,355,-758,339,-734,347,-362,735,-746,359,-356,723,-356,755,-356,715,-346,769,-320,761,-748,349,-738,361,-356,727,-752,333,-754,335,-8090,781,-742,325,-738,351,-742,359,-720,365,-756,337,-746,339,-746,377,-734,355,-726,355,-728,353,-758,351,-720,375,-344,753,-728,351,-342,729,-352,763,-356,755,-322,753,-352,753,-722,385,-726,349,-354,719,-756,347,-734,351,-168822,97,-4318,265,-164,99,-332,131,-66,797,-330,259,-200,1565,-130,365,-366,331,-400,369,-136,197,-334,301,-66,199,-198,469,-68,597,-66,25107,-102,335,-300,14627,-756,131,-464,65,-132,329,-66,97,-66,65,-266,269,-100,3049,-132,557,-196,261,-132,261,-196,327,-66,227,-196,9393,-100,10359,-132,929,-66,167,-66,267,-232,165,-164,291,-166,65,-202,14043,-1052,229,-2334,999,-270,333,-298,163,-460,1475,-298,65,-132,233,-298,97,-132,161,-132,227,-66,56895,-66,825,-132,12023,-196,95,-1246,97,-2420,131,-198,133,-66,497,-132,165,-526,131,-68,393,-132,291,-66,687,-100,689,-100,131,-234,231,-166,629,-264,229,-164,195,-100,14527,-98,129,-262,67,-896,65,-630,67,-1096,65,-166,2377,-294,427,-64,459,-296,65,-132,163,-724,99,-296,99,-100,9747,-734,65,-296,325,-132,97};

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(PIN_LED_ONBOARD, OUTPUT);
  digitalWrite(PIN_LED_ONBOARD, LOW);

  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);

  //CC1101 SETUP
  initCC1101(304.25);

  if (ELECHOUSE_cc1101.getCC1101()) {
    Serial.println("Connection OK");
  }

  Serial.println("Setup done.");

  sendSamples(samples_my_signal, LENGTH_SAMPLES_MY_SIGNAL, 304.25);

  digitalWrite(PIN_GDO0, LOW);
  digitalWrite(PIN_GDO2, LOW);

  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setGDO(PIN_GDO0, PIN_GDO2);
  ELECHOUSE_cc1101.setModulation(2);
  ELECHOUSE_cc1101.setDRate(512);
  ELECHOUSE_cc1101.setPktFormat(3);

  enterDeepSleep();
}

void loop() {
  // Intentionally empty. Deep sleep should never return.
}

void initCC1101(float mhz){
    ELECHOUSE_cc1101.Init();
    ELECHOUSE_cc1101.setGDO(PIN_GDO0, PIN_GDO2);
    ELECHOUSE_cc1101.setMHZ(mhz);        // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.SetTx();               // set Transmit on
    ELECHOUSE_cc1101.setModulation(2);      // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setDRate(512);         // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
    ELECHOUSE_cc1101.setPktFormat(3);       // Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 
                                            // 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 
                                            // 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 
                                            // 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
  
    if(!ELECHOUSE_cc1101.getCC1101()){       // Check the CC1101 Spi connection.
      Serial.println("CC1101 Connection Error");
    }
}

void sendSamples(int samples[], int samplesLenght, float mhz) {
  initCC1101(mhz);
  Serial.print("Transmitting ");
  Serial.print(samplesLenght);
  Serial.println(" Samples");

  int delay = 0;
  byte n = 0;

  for (int i = 0; i < samplesLenght; i++) {
    n = 1;
    delay = samples[i];
    if (delay < 0) {
      delay = delay * -1;
      n = 0;
    }

    digitalWrite(PIN_GDO0, n);
    delayMicroseconds(delay);
  }

  digitalWrite(PIN_GDO0, LOW);
  Serial.println("Transmission completed.");
}

void enterDeepSleep() {
  Serial.println("Entering deep sleep...");
  Serial.flush();
  delay(50);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  ESP.deepSleep(0, RF_DISABLED);
}