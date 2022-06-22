# Moon-Rover

Code for the Imperial College London moon rover project.

## Project branches

main - The branch containing the same version of the project as for the report. This version of the rover uses an adjustable radio sensor (61kHz and 89kHz) and an IR sensor.

Hall - Created shortly before the live demonstration, this version uses a fixed 61kHz radio, an IR sensor, and a Hall effect (magnetic) sensor. This was done because there were issues with adjusting the frequency of the radio (low selectivity), so this set of sensors was used to differentiate between minerals 1/2 and 3/4 better.

## Libraries used:
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
- [WiFi101](https://github.com/arduino-libraries/WiFi101)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [Kivy](https://github.com/kivy/kivy)
- [ExpressJS](https://github.com/expressjs/express)
- [CORS](https://github.com/expressjs/cors)
- [Body-parser](https://github.com/expressjs/body-parser)
- [Socket.io](https://github.com/socketio/socket.io)

## Dev dependencies
- [Nodemon](https://github.com/remy/nodemon/)
