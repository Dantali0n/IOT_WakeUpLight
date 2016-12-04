typedef unsigned char byte;

class rgbColor {
  private:
    byte red, green, blue;   
  public:
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

