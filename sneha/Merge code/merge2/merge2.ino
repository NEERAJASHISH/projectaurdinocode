#define SAMPLE_RATE 75
#define BAUD_RATE 9600
#define INPUT_PIN A0

const int NUM_SIGNALS = 5; // Number of signals to store
int signals[NUM_SIGNALS]; // Array to store signals
int num_right = 0; // Counter for 'right' signals
int num_left = 0; // Counter for 'left' signals

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
    //Serial.println(signal);

    if(signal>=(180))
    {
      signals[NUM_SIGNALS - 1] = 1; // Store 'right' signal
    }
    else if(signal>=(140) && signal<=(175))
    {
      signals[NUM_SIGNALS - 1] = -1; // Store 'left' signal
    }
    else
    {
      signals[NUM_SIGNALS - 1] = 0; // Store 'no signal'
    }

    // Count occurrences of each signal
    num_right = 0;
    num_left = 0;
    for(int i = 0; i < NUM_SIGNALS; i++)
    {
      if(signals[i] == 1)
      {
        num_right++;
      }
      else if(signals[i] == -1)
      {
        num_left++;
      }
    }

    // Print the majority signal
    if(num_right > num_left)
    {
      Serial.println("r");
    }
    else if(num_left > num_right)
    {
      Serial.println("l");
    }
    else
    {
      //Serial.println("no majority");
    }

    // Shift signals array to make room for the next signal
    for(int i = 0; i < NUM_SIGNALS - 1; i++)
    {
      signals[i] = signals[i + 1];
    }
    static int num_inputs = 0;
    num_inputs++;
    if(num_inputs == 5){
      
      delay(2000);
      num_inputs = 0;
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
