int ch=4; // number of relaysint 
int relay[]={2,3,4,5}; // pin number of relays
int wait=2000;int i=0;


void setup() {
 for(int i=0; i<ch; i++){
   pinMode(relay[i],OUTPUT); // set relay pins as output
   digitalWrite(relay[i],HIGH); // turn relay off
  }
} 

void loop() {
  for(int i=0; i<ch; i++){
    digitalWrite(relay[i],LOW); // turn relay ON
    delay(wait);
  }
  for(int i=0; i<ch; i++){
    digitalWrite(relay[i],HIGH); // turn relay OFF
    delay(wait);
  }
}