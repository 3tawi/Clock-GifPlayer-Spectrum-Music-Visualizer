/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from LedEffects Snake by Robert Atkins: https://bitbucket.org/ratkins/ledeffects/src/26ed3c51912af6fac5f1304629c7b4ab7ac8ca4b/Snake.cpp?at=default
 * Copyright (c) 2013 Robert Atkins
 *
 */

#ifndef PatternDigitalClock2_H
#define PatternDigitalClock2_H


class PatternDigitalClock2 : public Drawable {
private:

public:
    PatternDigitalClock2() {
        name = (char *)"DigitalClock2";
    }

void getmesg() {
  xmsg = 128;
  co0 = random(128, 255);
  switch(msg) {
    case 0 :
        sensor.read();
        textmsg = Message;
        msg++;
        break;
    case 1 :
        textmsg  = "T:";
        textmsg += NewTemp;
        textmsg += (char)127;
        msg++;
        break;
    case 2 :
        textmsg  = "H:";
        textmsg += NewHumi;
        textmsg += "%";
        msg++;
        break;
    case 3 :
        textmsg  = rtc.getAmPm();
        textmsg += " ";
        textmsg += rtc.getDate(true);
        msg = 0;
        break;
    }
}  
void drawHMSTime() {
  String text = Mode24h ? rtc.getTime("%H") : rtc.getTime("%I");
  effects.setFont(NuFont);
  effects.setCursor(2, 25);
  effects.print(text);
  text = rtc.getTime("%M");
  effects.setCursor(77, 25);
  effects.print(text);
  text = flasher ? "" : ":";
  effects.setCursor(52, 25);
  effects.print(text);
  text = rtc.getTime("%S");
  effects.setFont(GPFont16x16);
  effects.setCursor(48, 21);
  effects.print(text);
  int xx = rtc.getSecond() * 2;
  xx = flashSe ? xx+4 : 123-xx;
  effects.fillEllipse(xx, 20, 4, 2);
    }


void drawMesg() {
  int text_length = -(textmsg.length() * 16);
  effects.setFont(GPFont16x16); 
  effects.setCursor(xmsg, 1);
  effects.print(textmsg);
  xmsg -= sp0;
  if (xmsg < text_length) {
    getmesg();
  }
}
void sentbuff() {
  mySeriel->write(UpHeader);
  mySeriel->write((uint8_t *)buffc, NUM_LEDS);
  mySeriel->write(endHeader);
  memset(buffc, 0x0, NUM_LEDS);
}

    unsigned int drawFrame() {
      effects.FillNoise();
      drawMesg();
      drawHMSTime();
      sentbuff();
      EVERY_N_SECONDS(1){
        flasher = !flasher;
        if (NewRTCm != rtc.getMinute()) {
        effects.NoiseVariablesSetup();
        flashSe = !flashSe;
        NewRTCm = rtc.getMinute();
        }
      }
      noise_x += 800;
      noise_y += 800;
      noise_z += 800;
      return 0;
    }
};

#endif
