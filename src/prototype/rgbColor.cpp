#include "rgbColor.h";

rgbColor::rgbColor(int value) {
  this->red = (value >> 16) & 0xff;
  this->green = (value >> 8) & 0xff;
  this->blue = value & 0xff;
}

rgbColor::rgbColor(byte red, byte green, byte blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
}

rgbColor::rgbColor(int red, int green, int blue) {
  this->red = this->intColorToByte(red);
  this->green = this->intColorToByte(green);
  this->blue = this->intColorToByte(blue);
}

byte rgbColor::getRed() {
  return this->red;
}

void rgbColor::setRed(int red) {
  this->red = this->intColorToByte(red);
}

void rgbColor::setRed(byte red) {
  this->red = red;
}

byte rgbColor::getGreen() {
  return this->green;
}

void rgbColor::setGreen(int green) {
  this->green = this->intColorToByte(green);
}

void rgbColor::setGreen(byte green) {
  this->green = green;
}

byte rgbColor::getBlue() {
  return this->blue;
}

void rgbColor::setBlue(int blue) {
  this->blue = this->intColorToByte(blue);
}

void rgbColor::setBlue(byte blue) {
  this->blue = blue;
}

byte rgbColor::intColorToByte(int color) {
  return color % 256;
}

