byte scottBright = 255;
void scott() {
  
  if (!patternInit) {
    switchDrawType(0,0);
    patternInit = true;
  }

  GlassesBits[0][0] = 0xff;
  GlassesBits[12][0] = 0xff;  
  GlassesBits[0][1] = 0xff;
  GlassesPWM[0][0] = scottBright;
  GlassesPWM[0][1] = scottBright;
  GlassesPWM[0][2] = scottBright;
  GlassesPWM[0][3] = scottBright;
  scottBright -= 5;
  if (scottBright == 0) {
    scottBright = 256;
  }
  
  if (scottBright > 128) {
    writeBitFrame(0,0);
  } else {
    writeBitFrame(0,1);
  }
  writePWMFrame(0);
  
}

// limits = (0,0), (23, 7)

typedef struct Ball {
  float x;
  float y;
  float vX;
  float vY;
};

Ball ball;

int frame = 0;

boolean collisionX() {
  if ((ball.x <= 0.0) || (ball.x >= 23.0)) {
//    ball.vX = ball.vX * -1.0;
    return true;
  }
  return false;
}

boolean collisionY() {
  if ((ball.y <= 0.0) || (ball.y >= 7.0)) {
    return true;
  }
  return false;
}

void moveBall() {
  ball.x = ball.x + ball.vX;
  ball.y = ball.y + ball.vY;
  
  if (collisionX()) {
    ball.vX = ball.vX * -1.0;
  }
  if (collisionX()) {
    ball.vY = ball.vY * -1.0;
  }

}

void bounce() {
  if (!patternInit) {
    ball.x = 0;
    ball.y = 0;
    ball.vX = 1;
    ball.vY = 1;
    switchDrawType(0,0);
    patternInit = true;
  }
  
  frame++;
  if (frame > 50) {
    frame = 0;
  } else {
    return;
  }
  
  moveBall();

  int x = (int) ball.x;
  int y = (int) ball.y;
 
  GlassesBits[x][0] = 1 << y;
  writeBitFrame(0,0);
  GlassesBits[x][0] = 0;
  
}

