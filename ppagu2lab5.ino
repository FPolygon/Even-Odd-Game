/*
   1- Author: Francis Pagulayan - 665900279
   2- Lab: Lab 5 - Communication
   3- Description: In your own words, what is this code supposed to do?
    This code is for a game called the "even odd game". The game is played between a user and a computer.
    The game starts by prompting the user to enter a number between 1 and 5. If the user enters a number outside of that range, the game restarts. If the input is valid, the program prompts the user to choose "even" or "odd" by typing it into the serial monitor. The input is not case-sensitive, so the user can enter "even" or "EVEN" or "odD" or "ODD" and it will be treated the same.
    After the user has entered their choice, the program generates a random number between 1 and 5. It then adds the user's number and the random number together to get a sum. If the sum is even and the user chose "even" or if the sum is odd and the user chose "odd", then the user wins. If not, the user loses.
    The program will display whether the user won or lost, and will turn on either a green or red LED depending on the outcome. The program also moves a servo motor to point at the LED that is turned on, so that the user can see which color corresponds to winning and which corresponds to losing.
    Finally, the program waits for 5 seconds before restarting the game and prompting the user to enter a new number between 1 and 5 to start a new round of the game.
   4-Lab Questions:
What function did you use to change the servo position?
  write()
What are the angles of the servo motor for the three directions?
  90 for neutral, 115 for winning, 75 for losing
What functions/methods did you use to take input?
  Serial.readStringUntil(), Serial.read(), Serial.available()
   5- References: 
    https://www.arduino.cc/reference/en/language/functions/communication/serial/
    https://www.arduino.cc/reference/en/language/functions/communication/serial/available/
    https://docs.arduino.cc/learn/electronics/servo-motors
   6- Demo: Demonstration uploaded to voicethread
*/

#include <Servo.h>

Servo myservo;

// Define pins for the green and red LEDs
const int greenLed = 13;
const int redLed = 12;

void setup() {
  Serial.begin(9600);
  
  myservo.attach(9); // attach the servo to pin 9
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  
  digitalWrite(greenLed, LOW); // turn off the green LED
  digitalWrite(redLed, LOW); // turn off the red LED
  delay(1000);
  
  myservo.write(90);
  Serial.println("Welcome to Even Odd Game!!");
  Serial.println("Enter a number between 1 to 5.");
  delay(1000);
}

void loop() {
  // check if there are at least two bytes in the serial buffer
  if (Serial.available() >= 2) {
    int userNumber = Serial.readStringUntil('\n').toInt(); // read the first input as an integer
    
    // check if the user input is within the range of 1 to 5
    if (userNumber < 1 || userNumber > 5) {
      Serial.println("Wrong input");
      delay(500);
      Serial.read(); // clear the buffer
      Serial.read(); // clear the buffer
      setup();
      return;
    }
    
    Serial.print("You chose: ");
    Serial.println(userNumber);
    Serial.println("Now choose odd or even. Type 'even' or 'odd'.");
    
    // wait for the user to input 'even' or 'odd'
    while (Serial.available() == 0) { }
    String userChoice = Serial.readStringUntil('\n'); // read the second input as a string
    userChoice.toLowerCase(); // convert the user input to lowercase
    
    // check if the user input is valid (either 'even' or 'odd')
    if (userChoice != "even" && userChoice != "odd") {
      Serial.println("Wrong input");
      delay(500);
      Serial.read(); // clear the buffer
      Serial.read(); // clear the buffer
      setup();
      return;
    }
    
    Serial.print("You chose: ");
    Serial.println(userChoice);
    Serial.println("Generating...");
    
    // make the servo move back and forth to create some suspense
    for (int i = 0; i < 90; i++) {
      myservo.write(i);
      delay(10);
    }

    for (int i = 90; i > 0; i--) {
      myservo.write(i);
      delay(10);
    }

    int randomNumber = random(1, 6); // generate a random number between 1 and 5
    
    Serial.print("My number is: ");
    Serial.println(randomNumber);
    
    int sum = userNumber + randomNumber; // calculate the sum of the user input and the random number
    Serial.print("The sum is: ");
    Serial.println(sum);

    // check if the user wins or loses
    if ((sum % 2 == 0 && userChoice == "even") || (sum % 2 != 0 && userChoice == "odd")) {
      Serial.print("The sum is ");
      if (sum % 2 == 0) {
        Serial.print("even. ");
      } else {
        Serial.print("odd. ");
      }
      Serial.println("You win!");
      
      // turn on the green LED and point the servo towards it
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW); // turn off the red LED
      myservo.write(115); // point the servo towards the green LED
    } else { // the user loses
      Serial.println("You Lose.. Better Luck next time!");
      digitalWrite(greenLed, LOW); // turn off the green LED
      digitalWrite(redLed, HIGH); // turn on the red LED
      myservo.write(75); // point the servo towards the red LED
    }
    delay(5000);
    setup();
  }
}
