#define SAMPLE_RATE 75
#define BAUD_RATE 115200
#define INPUT_PIN A0
const int MAX_INPUTS = 5; // maximum number of inputs to process
const int TIMEOUT_MS = 10000;// timeout value in milliseconds
const int numInputs = 5;
String inputs[numInputs];
int inputCount1 = 0;
char result;


void setup() {
	// Serial connection begin
	Serial.begin(BAUD_RATE);
}

void loop() {
	// Calculate elapsed time
	static unsigned long past = 0;
	unsigned long present = micros();
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
    
    if(signal<=(-130))
    {
      result=Serial.println("r");
    }
    else if(signal<=(-50) && signal>=(-100))
    {
      result=Serial.println("l");      
    }
	}
}
void setup() {
  // initialize the serial communication
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
}

void loop() {
  static int inputCount = 0; // initialize the input counter
  static unsigned long timeoutStartTime = 0; 
  static bool isTimedOut = false; // initialize the timeout state

  // process up to MAX_INPUTS inputs
  while (inputCount < MAX_INPUTS && !isTimedOut) {
    if (Serial.available()) {
      // read the input from the user
      char input = result;
      if (input == '\n') { // ignore newline characters
        continue;
      }
      String strInput(input); // convert char to String
      inputs[inputCount1++] = strInput;
      Serial.print('\n');
      Serial.print(input); // echo the input back to the user
      if (inputCount1 == numInputs) {
        String majorityInput = findMajorityInput(inputs);
        Serial.println("\nThe majority input is: " + majorityInput);
        inputCount1 = 0;
      }
      inputCount++;
    }
  }

  // check if we have reached the maximum number of inputs
  if (inputCount == MAX_INPUTS || isTimedOut) {
    // check if we need to start the timeout
    if (timeoutStartTime == 0) {
      timeoutStartTime = millis();
    }

    // check if the timeout has elapsed
    if ((millis() - timeoutStartTime) >= TIMEOUT_MS) {
      while (Serial.available()) {
        Serial.read();
      }
      // reset the input counter and timeout start time
      inputCount = 0;
      timeoutStartTime = 0;
      isTimedOut = true; // set the timeout state
    } else {
      // do nothing and continue waiting for input
      return;
    }
  }

  // reset the timeout state and resume input processing
  isTimedOut = false;
}

String findMajorityInput(String inputs[]) {
  String majorityInput;
  int maxCount = 0;
  for (int i = 0; i < numInputs; i++) {
    int count = 1;
    for (int j = i + 1; j < numInputs; j++) {
      if (inputs[i] == inputs[j]) {
        count++;
      }
    }
    if (count > maxCount) {
      majorityInput = inputs[i];
      maxCount = count;
    }
  }
  return majorityInput;
}

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 75.0 Hz, frequency: [0.5, 19.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: 
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
// https://courses.ideate.cmu.edu/16-223/f2020/Arduino/FilterDemos/filter_gen.py
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
