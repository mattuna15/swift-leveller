
#define EEPROM_HEIGHT_ADDR      910
#define EEPROM_HEIGHT_ADDR1     1200
#define EEPROM_HEIGHT_ADDR2     1210
#define EEPROM_HEIGHT_ADDR3     1220
#define EEPROM_HEIGHT_ADDR4     1230
#define EEPROM_HEIGHT_ADDR5     1240
#define EEPROM_HEIGHT_ADDR6     1250
#define EEPROM_HEIGHT_ADDR7     1260
#define EEPROM_HEIGHT_ADDR8     1270
#define EEPROM_HEIGHT_ADDR9     1280

#define DATA_TYPE_FLOAT         4
// e2prom device
#define EEPROM_ON_CHIP          0

//capacitive touch values
#define CAP_OFF                 1190             
#define CAP_ON                  1300   


//serial data
const byte numChars = 32;
char receivedChars[numChars]; 
char tempChars[numChars]; 
boolean newData = false;

      // variables to hold the parsed data
char ok[2] = {0};
float height = 0.0;

//column positions

int x[3] = {-100, 0, 100};
int y[3] = {50, 100, 150 };

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial1.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:


 // G0 X100 Y100 Z100 F1000\n

 //  M2212 N0 A200 T1 V10\n
//Write EEPROM N(0~2,0 is
//internal EEPROM,1 is
//USR_E2PROM, 2 is
//SYS_E2PROM), A is address, T is
//type (1 char,2 int,4 float)V is the
//input data

  Serial1.println("G0 X" + String(x[0]) + " Y" + String(y[0]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1200 T4 V" + String(height) );
  
  Serial1.println("G0 X" + String(x[1]) + " Y" + String(y[0]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1210 T4 V" + String(height) );
  
  Serial1.println("G0 X" + String(x[2]) + " Y" + String(y[0]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1220 T4 V" + String(height) );

  Serial1.println("G0 X" + String(x[0]) + " Y" + String(y[1]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1230 T4 V" + String(height) );

  Serial1.println("G0 X" + String(x[1]) + " Y" + String(y[1]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1240 T4 V" + String(height) );

  Serial1.println("G0 X" + String(x[2]) + " Y" + String(y[1]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1250 T4 V" + String(height) );
  
  Serial1.println("G0 X" + String(x[0]) + " Y" + String(y[2]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1260 T4 V" + String(height) );

  Serial1.println("G0 X" + String(x[1]) + " Y" + String(y[2]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1270 T4 V" + String(height) );

  Serial1.println("G0 X" + String(x[2]) + " Y" + String(y[2]) + "Z50 F1000");
  recvWithEndMarker();
  moveDown();
  Serial1.println("M2212 N0 A1280 T4 V" + String(height) );
}

void moveDown() {
  
  digitalWrite(LED_BUILTIN, LOW);
  int val = touchRead(A9);
  while (val < CAP_ON) {
      Serial1.println("G2205 H0.1 F100");
      recvWithEndMarker();
      val = touchRead(A9);
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial1.println("P2221");
  recvWithEndMarker();
  parseP2221Data();
  
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    if (Serial1.available() > 0) {
        rc = Serial1.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\n'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }

}

void parseP2221Data() {      // split the data into its parts
//  ok S100 R90 H80

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars," ");      // get the first part - the string
    strcpy(ok, strtokIndx);
 
    strtokIndx = strtok(NULL, " "); 
    strtokIndx = strtok(NULL, " "); 

    //only want the height
    strtokIndx = strtok(NULL, ",");
    height = atof(strtokIndx);     // convert this part to a float

}
