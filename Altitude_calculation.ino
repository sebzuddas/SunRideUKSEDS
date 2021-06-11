// Hypsometric formula found here: https://physics.stackexchange.com/questions/333475/how-to-calculate-altitude-from-current-temperature-and-pressure

float P0 = 101325; //Atmospheric pressure at sea level
float P; //Atmospheric pressure where the rocket is
float T; // Temperature
float H; // Height above sea level (Max altitude = highest value of H - initial value of H)

void setup() {
Serial.begin(9600);
}

void loop() {
H = ((pow((P0/P), (1/5.257))-1)*(T + 273.15))/(0.0065);
//H = 44330*(1-pow((P/P0), (1/5.257))) Alternative that uses a constant temperature of 15C, barometric formula
Serial.println(H);
delay(1000);
}
