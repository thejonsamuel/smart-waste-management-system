#define BLYNK_TEMPLATE_ID "your-template-id"
#define BLYNK_TEMPLATE_NAME "your-template-name"
#define BLYNK_AUTH_TOKEN "your-auth-token"

#define VIRTUAL_PIN_FILL_LEVEL V1

#include <BlynkSimpleEsp32.h>
#include <Ultrasonic.h>

// WiFi credentials
char ssid[] = "your-ssid";
char pass[] = "your-password";

// Ultrasonic sensor pins
#define TRIGGER_PIN 12
#define ECHO_PIN 13

// Distance from sensor to bottom of empty bin (in cm)
const int EMPTY_BIN_DISTANCE = 100;   // Adjust based on your setup
const int FULL_BIN_DISTANCE = 5;     // Distance when bin is full

// Event thresholds (percentages)
const int HALF_FULL_PERCENT = 50;
const int NEARLY_FULL_PERCENT = 95;

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

bool halfFullEventSent = false;
bool fullEventSent = false;

void setup() {
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  while (!Blynk.connected()) {
    Serial.println("Connecting to Blynk...");
    delay(500);
  }

  Serial.println("Connected to Blynk.");
}

void loop() {
  Blynk.run();

  float current_distance = ultrasonic.read();

  // Clamp to bounds
  if (current_distance > EMPTY_BIN_DISTANCE) current_distance = EMPTY_BIN_DISTANCE;
  if (current_distance < FULL_BIN_DISTANCE) current_distance = FULL_BIN_DISTANCE;

  // Calculate fill level in %
  int fill_percent = map(current_distance, EMPTY_BIN_DISTANCE, FULL_BIN_DISTANCE, 0, 100);
  fill_percent = constrain(fill_percent, 0, 100);

  Serial.print("Distance: ");
  Serial.print(current_distance);
  Serial.print(" cm | Fill Level: ");
  Serial.print(fill_percent);
  Serial.println(" %");

  // Send to Blynk
  Blynk.virtualWrite(VIRTUAL_PIN_FILL_LEVEL, fill_percent);

  // Trigger Blynk events
  if (fill_percent >= HALF_FULL_PERCENT && !halfFullEventSent) {
    Blynk.logEvent("half_full", "Bin is half full.");
    halfFullEventSent = true;
  }

  if (fill_percent >= NEARLY_FULL_PERCENT && !fullEventSent) {
    Blynk.logEvent("bucket_full", "Bin is almost full. Please empty it.");
    fullEventSent = true;
  }

  // Reset event flags if bin gets emptied
  if (fill_percent < HALF_FULL_PERCENT) {
    halfFullEventSent = false;
    fullEventSent = false;
  }

  delay(2000); // Delay between readings
}
