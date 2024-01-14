Based on a really cool project - iffybooks/pocket-wifi-portal

This is a wifi portal that can be hosted on an Wesmos D1 mini, which is cheap development board with wifi that can be programmed using the Arduino IDE.

The idea is to offer some kind of experience over wifi for users in immediate vicinity of the board. The original project from iffybooks explains how to use this board
to host a static website with interesting articles for others to read as a 'pop up free library'. In this version I kept that functionality and also added a live chat feature powered by websockets.

To copy to data folder:  
Create `destination.txt` with path to data folder  
Run `./copyToDataFolder.sh`

To update chip:  
Find Free_Reading_Wifi.ino and open it  
Tools > ESP8266 Sketch Data upload  
Everything in `/data` will be copied

Demo - 

![library-with-chat-demo](library-with-chat-demo.gif)
