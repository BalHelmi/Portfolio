void setup() {

  Serial.begin(9600);
  
}


int  i = 0;

void loop() {
  
//  while(!(i == 20)) {
//    if (i == 3 ) break;
//    Serial.println(i);
//     i++;
//    delay(1000);
//  }

while(i != 5) {
  Serial.println("azerty");
  i++;
}

  

  
}
