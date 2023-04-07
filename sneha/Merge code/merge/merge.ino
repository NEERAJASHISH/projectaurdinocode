#define SAMPLE_RATE 75
#define BAUD_RATE 115200
#define INPUT_PIN A0
const int MAX_INPUTS = 5; // maximum number of inputs to process
const int TIMEOUT_MS = 10000;// timeout value in milliseconds
const int numInputs = 5;
String inputs[numInputs];
int inputCount1 = 0;
float EOGFilter;

void setup() {
	// Serial connection begin
	Serial.begin(BAUD_RATE);
  while (!Serial){
    ;
  }
}

void loop() {
	// Calculate elapsed time
	static unsigned long past = 0;
	unsigned long present = micros();
  char result;
	unsigned long interval = present - past;
	past = present;

	// Run timer
	static long timer = 0;
	timer -= interval;

	// Sample
	if(timer < 0){
		timer += 1000000 / SAMPLE_RATE;
		float sensor_value = analogRead(INPUT_PIN);
		float signal = EOGFilter(sensor_value);
		Serial.println(signal);
    // Initialize counts for "left" and "right" signals
        static int left_count = 0;
        static int right_count = 0;
    
    if(signal>=(200))
    {
      right_count++;
    }
    else if(signal>=(140) && signal<=(175))
    {
       left_count++;     
    }
	}
   // Check if we have enough samples (5) and determine the majority
        static int sample_count = 0;
        sample_count++;

        if(sample_count == 5){
            if(left_count > right_count){
                Serial.println("Majority: left");
            } else (right_count > left_count) {
                Serial.println("Majority: right");
            }
}
// Reset counts and sample count
            left_count = 0;
            right_count = 0;
            sample_count = 0;
      }
   }
}

float EOGFilter(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.02977423*z1 - 0.04296318*z2;
    output = 0.09797471*x + 0.19594942*z1 + 0.09797471*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.08383952*z1 - 0.46067709*z2;
    output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.92167271*z1 - 0.92347975*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.96758891*z1 - 0.96933514*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}