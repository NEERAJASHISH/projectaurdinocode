#define SAMPLE_RATE 75
#define BAUD_RATE 9600
#define INPUT_PIN A0

const int MAX_INPUTS = 5; // maximum number of inputs to process
const int TIMEOUT_MS = 3000;// timeout value in milliseconds
const int numInputs = 5;
String inputs[numInputs];
int inputCount = 0;

void setup() {
  // initialize the serial communication
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
}

void loop() {
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  static long timer = 0;
  timer -= interval;

  // Sample
  if (timer < 0) {
    timer += 1000000 / SAMPLE_RATE;
    float sensor_value = analogRead(INPUT_PIN);
    float signal = EOGFilter(sensor_value);
    //Serial.println(signal);
    
    if (signal >= 350) {
      //Serial.println(signal);
      //Serial.println("r");
      inputs[inputCount++] = "l";
    } else if(signal>=(260) && signal<=(310))
    {
      //Serial.println(signal);
      //Serial.println("l");
      inputs[inputCount++] = "r";
    }

    // Check if we have collected enough inputs
    if (inputCount == numInputs) {
      String majorityInput = findMajorityInput(inputs);
      Serial.println("\n " + majorityInput);
      inputCount = 0;

      // Wait for TIMEOUT_MS milliseconds
      delay(TIMEOUT_MS);
    }
  }
}

float EOGFilter(float input) {
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
