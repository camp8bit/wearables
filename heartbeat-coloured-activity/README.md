
# Heartbeat Simulator with Colour Activity

Shows a different hue and brightness based on accelerometer activity,
with the idea that low movement means colder and less bright colours,
with more movement turning it into warmer and brighter colours.

Works with a standard 3-axis analog accelerometer and NeoPixel lights.

Credit to [Ben Nolan](http://github.com/bnolan) for the heart beat LED logic.
And for lending me some Neopixels to play around with!

## Configuration

Download the [FastLED](http://fastled.io) library and follow the installation instructions.
Configure the data pins (`DATA_PIN`, `ACCEL*_PIN`) as well as `NUM_LEDS` to your own setup.

Since accelerometer readings can depend on the specific device setup,
you should adjust `minMagnitude` and `maxMagnitude` to your own preferences.

## License

The MIT License (MIT)
Copyright 2015 Ingo Schommer

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.