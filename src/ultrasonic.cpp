#include "ultrasonic.h"


uint8_t dist(uint16_t dly) {
  uint8_t d = 0;
  digitalWrite(trigP, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay 
  
  digitalWrite(trigP, HIGH);  // tigPin high
  delayMicroseconds(15);      // trigPin high for 10 micro seconds
  digitalWrite(trigP, LOW);   // trigPin low
  
  duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
  d = duration*0.0343/2;        //Calculating actual/real distance
  if(serial) Serial.print("Temp Distance = ");        //Output distance on arduino serial monitor 
  if(serial) Serial.println(d);
  delay(dly);
  return d;
}

// uint16_t avgDist(uint16_t dly, uint8_t sample) {
//   unsigned long total = 0;
//   uint16_t avg = 0;
//   for(uint8_t i = 0; i < sample; i++) total += dist(dly);
//   avg = total / sample;
//   return avg;
// }

uint16_t filteredAvgDist(uint16_t dly, uint8_t sample, uint8_t tolerance = 5, uint8_t minClusterSize = 3) {
  uint16_t readings[MAX_SAMPLES];

  if (sample > MAX_SAMPLES) sample = MAX_SAMPLES; // Limit to max samples

  // Step 1: Collect samples
  for (uint8_t i = 0; i < sample; i++) {
    readings[i] = dist(dly);
    delay(1); // Small delay between readings
  }

  uint8_t bestCount = 0;
  uint16_t bestSum = 0;

  for (uint8_t i = 0; i < sample; i++) {
    uint8_t count = 1;
    uint16_t sum = readings[i];

    for (uint8_t j = 0; j < sample; j++) {
      if (i != j && abs(readings[i] - readings[j]) <= tolerance) {
        count++;
        sum += readings[j];
      }
    }
    
    if (count > bestCount) {
      bestCount = count;
      bestSum = sum;
    }
  }

  // Step 4: Return average of the best cluster
  if (bestCount >= minClusterSize) {
    return bestSum / bestCount;
  } else {
    // If no cluster meets the minimum size, return the average of all readings
    uint32_t total = 0;
    for (uint8_t i = 0; i < sample; i++) {
      total += readings[i];
    }
    return total / sample;
  }
}


void getDistance() {
  if(millis() - tkeepUS > usTime) {
    // distance = avgDist(60, 10);
    distance = filteredAvgDist(50, 10, 5, 3); // Adjust tolerance and minClusterSize as needed
    if(serial) Serial.print("\nAverage Distance = ");        //Output distance on arduino serial monitor 
    if(serial) Serial.println(distance);
    if(serial) Serial.println("\n");
    if(parkSpaceOccupied && distance > parkSpaceVehicleDistance) {
      parkSpaceOccupied = LOW;
      digitalWrite(parkFree, HIGH);
      digitalWrite(parkOcc, LOW);
     }
    else if (!parkSpaceOccupied && distance < parkSpaceVehicleDistance) {
      parkSpaceOccupied = HIGH;
      digitalWrite(parkFree, LOW);
      digitalWrite(parkOcc, HIGH);
     }
    tkeepUS = millis();
  }
}
