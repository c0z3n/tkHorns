//     tkHorns.ino
//     Gordon Clement 2012
//     
//     valid OSC addresses and functionality described in setup()
//
#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>
#include <Servo.h> 

//replace with MAC address of the ethernet shield being used
byte myMac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x18, 0x24 }; 

//IP address and port to assign to the OSC server
byte myIp[]  = { 192, 168, 1, 201 };
int  serverPort  = 10000;

//int horn_1_state, horn_2_state, horn_3_state, horn_4_state, horn_5_state = 0;

int horn_state[5] = {0,0,0,0,0};

Servo servos[5];

//define openstate and closestate for the valve pins.
int OPENSTATE = 1;
int CLOSESTATE = 0;

//define default servo position, 0-180
int SERVO_DEFAULT_POS = 90;


int HORNPINS[5] = {A5,A4,A3,A2,A1};  //the pins the horn relays are connected to
int SERVOPINS[5] = {5,6,7,8,9};      //the pins the wiper servos are connected to
int SERVOPOS[5] = {SERVO_DEFAULT_POS,SERVO_DEFAULT_POS,SERVO_DEFAULT_POS,SERVO_DEFAULT_POS,SERVO_DEFAULT_POS};

OSCServer osc;

void setup(){
 //Serial.begin(19200); 
 Ethernet.begin(myMac ,myIp); 
 osc.begin(serverPort);

//   #===== reset procedures==============#
//   any osc message sent to these addresses will trigger 
//   the corresponding reset procedure
 osc.addCallback("/tk/reset", &reset);               //reset all valves to closed state
 osc.addCallback("/tk/reset_servos", &servoreset);   //reset all servos to default position
 
//   #===== toggle valves ================#
//   any OSC command sent to these addresses will toggle 
//   the state of the corresponding horn.
 osc.addCallback("/tk/valve1", &horn1);
 osc.addCallback("/tk/valve2", &horn2);
 osc.addCallback("/tk/valve3", &horn3);
 osc.addCallback("/tk/valve4", &horn4);
 osc.addCallback("/tk/valve5", &horn5);
 
//   #===== open valves ==================#
//   any OSC command sent to these addresses will 
//   OPEN the corresponding valve
 osc.addCallback("/tk/valve1/open", &open1);
 osc.addCallback("/tk/valve2/open", &open2);
 osc.addCallback("/tk/valve3/open", &open3);
 osc.addCallback("/tk/valve4/open", &open4);
 osc.addCallback("/tk/valve5/open", &open5);
 
//   #===== close valves =================#
//   any OSC command sent to these addresses will 
//   CLOSE the corresponding valve
 osc.addCallback("/tk/valve1/close", &close1);
 osc.addCallback("/tk/valve2/close", &close2);
 osc.addCallback("/tk/valve3/close", &close3);
 osc.addCallback("/tk/valve4/close", &close4);
 osc.addCallback("/tk/valve5/close", &close5);
 
//   #===== set servo target positions ===#
//   send OSC messages containing a FLOAT of between 0 and 180
//   to these addresses to set the corresponding wiper position
 osc.addCallback("/tk/wiper1", &wipe1);
 osc.addCallback("/tk/wiper2", &wipe2);
 osc.addCallback("/tk/wiper3", &wipe3);
 osc.addCallback("/tk/wiper4", &wipe4);
 osc.addCallback("/tk/wiper5", &wipe5);
 
//initialize pins
 for(int i=0;i<5;i++){
   pinMode(HORNPINS[i], OUTPUT);
   pinMode(SERVOPINS[i], OUTPUT);
   servos[i].attach(SERVOPINS[i]);
 }
}

void loop(){
  if(osc.aviableCheck()>0){
    //Serial.println("alive!");
  }
  for(int i=0; i<5; i++){
    if(horn_state[i] > 0){digitalWrite(HORNPINS[i], HIGH);}
    else{digitalWrite(HORNPINS[i], LOW);}
    servos[i].write(SERVOPOS[i]);
  }
}
//toggle horn states
void horn1(OSCMessage *_mes){ horn_toggle(horn_state[0]); }
void horn2(OSCMessage *_mes){ horn_toggle(horn_state[1]); }
void horn3(OSCMessage *_mes){ horn_toggle(horn_state[2]); }
void horn4(OSCMessage *_mes){ horn_toggle(horn_state[3]); }
void horn5(OSCMessage *_mes){ horn_toggle(horn_state[4]); }
//open horns
void open1(OSCMessage *_mes){ horn_state[0] = OPENSTATE; }
void open2(OSCMessage *_mes){ horn_state[1] = OPENSTATE; }
void open3(OSCMessage *_mes){ horn_state[2] = OPENSTATE; }
void open4(OSCMessage *_mes){ horn_state[3] = OPENSTATE; }
void open5(OSCMessage *_mes){ horn_state[4] = OPENSTATE; }
//close horns
void close1(OSCMessage *_mes){ horn_state[0] = CLOSESTATE; }
void close2(OSCMessage *_mes){ horn_state[1] = CLOSESTATE; }
void close3(OSCMessage *_mes){ horn_state[2] = CLOSESTATE; }
void close4(OSCMessage *_mes){ horn_state[3] = CLOSESTATE; }
void close5(OSCMessage *_mes){ horn_state[4] = CLOSESTATE; }
//set wiper servos
void wipe1(OSCMessage *_mes){ SERVOPOS[0]= int(_mes->getArgFloat(0)); }
void wipe2(OSCMessage *_mes){ SERVOPOS[1]= int(_mes->getArgFloat(0)); }
void wipe3(OSCMessage *_mes){ SERVOPOS[2]= int(_mes->getArgFloat(0)); }
void wipe4(OSCMessage *_mes){ SERVOPOS[3]= int(_mes->getArgFloat(0)); }
void wipe5(OSCMessage *_mes){ SERVOPOS[4]= int(_mes->getArgFloat(0)); }

void horn_toggle(int &state){
  if (state == 0){state = 1;}
  else { state = 0; }
}

//reset all horns to closed
void reset(OSCMessage *_mes){
  for(int i=0; i<5; i++){
    horn_state[i] = CLOSESTATE;
  }
}
//reset all servos to default position
void servoreset(OSCMessage *_mes){
  for(int i=0; i<5; i++){
    SERVOPOS[i] = SERVO_DEFAULT_POS;
  }
}
