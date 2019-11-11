# f1uxu5

f1uxu5 is a live lightpainting app. It is intended for live performance, experimenting and playing as it was developed with light juggling in mind. 

f1uxu5 emulates the looks of long exposure photography for a live video feed. It is targeted at Raspberry Pi for a low cost setup, e.g. at exhibitions, conventions, festivals etc., but also runs on Window and Linux.

f1uxu5 features control over "exposure" as well as some effects. It also has a web interface (websockets) to control its parameters. The **f1uxu5 !nterface** connects to f1uxu5 by a web-browser, so you can control f1uxu5 using any smarthphone.






## Parameters

**alpha**: The rate of fading of previous image data. Range is from 0.9 to 1.0, where 1.0 means that previous image data does not fade away.


**offsetX**: The offset of previous image data in x-dimension. Range is from -1.0 to 1.0, where negative values means translation of previous image data to the left, positive values correspond to translation to the right and value 0.0 means no translation.


**offsetY**: The offset of previous image data in y-dimension. Range is from -1.0 to 1.0, where negative values means translation of previous image data to the bottom, positive values correspond to translation to the top and value 0.0 means no translation.


**repetitions**: The repetition of the effects of offsetX and offsetY in a radial manner. Range is 1 to 21, where 1 is default. 


**flipH**: Flip image horizontally (left-to-right)


**flipV**: Flip video vertically (upside-down)



## Dependencies

[openframeworks 0.10.0 or 0.10.1](https://openframeworks.cc/)

[ofxLibwebsockets addon](https://github.com/robotconscience/ofxLibwebsockets)

[ofxOMXCamera addon for Raspberry Pi](https://github.com/jvcleave/ofxOMXCamera)


