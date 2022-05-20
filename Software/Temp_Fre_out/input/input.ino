char input;
 
void setup() {
    Serial.begin(9600); 
    delay(2000);  
 
    Serial.println("Type something!");
}
 
void loop() {
  
    if(Serial.available()){
        input = Serial.read();
        int valA = input - '0';
        valA = valA*100;
        //String stringOne = String(input);// converting a constant char into a String
        Serial.print("You typed: " );
        Serial.println(valA);
    }
}
