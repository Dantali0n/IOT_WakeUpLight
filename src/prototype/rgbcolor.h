/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

/*  * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Code by Dantali0n
 https://dantalion.nl
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _Rgb_Color_h
#ifdef __cplusplus
#define _Rgb_Color_h

typedef unsigned char byte;

class rgbColor {
  private:
    byte red, green, blue;   
  public:
    rgbColor();
    rgbColor(int value);
    rgbColor(byte red, byte green, byte blue);
    rgbColor(int red, int green, int blue);

    static byte intColorToByte(int color);

    byte getRed();
    void setRed(int red);
    void setRed(byte red);
    
    byte getGreen();
    void setGreen(int green);
    void setGreen(byte red);
    
    byte getBlue();
    void setBlue(int blue);
    void setBlue(byte blue);
};

#endif /* __cplusplus */
#endif /* _Rgb_Color_h */
