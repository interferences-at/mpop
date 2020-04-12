/**
 * send osc to dataviz
 *
 * click in the window to send random barchart values to the dataviz application.
 *
 * Dependencies: The oscP5 library.
 */
 
import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;

void setup() {
  size(400, 400);
  frameRate(25);
  // Receiver:
  oscP5 = new OscP5(this, 12000);
  // Send address:
  myRemoteLocation = new NetAddress("127.0.0.1", 31337);
}

void draw() {
  background(0);
  fill(125, 125, 125);
  textSize(32);
  text("click to send", 50, 50);
}

void mousePressed() {
  OscMessage myMessage = new OscMessage("/dataviz/0/barchart"); 
  myMessage.add(int(random(100)));
  myMessage.add(int(random(100)));
  myMessage.add(int(random(100)));
  oscP5.send(myMessage, myRemoteLocation); 
}

/* 
 * incoming osc message are forwarded to the oscEvent method.
 */
void oscEvent(OscMessage theOscMessage) {
  print("### received an osc message.");
  print(" addrpattern: " + theOscMessage.addrPattern());
  println(" typetag: " + theOscMessage.typetag());
}
