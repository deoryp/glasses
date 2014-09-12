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

typedef struct Player {
  int x;
  int y;
  int l; // length
};

Ball ball;
Player player1;
Player player2;

int frame = 0;

void moveBall() {
  ball.x = ball.x + ball.vX;
  ball.y = ball.y + ball.vY;
  if ((ball.x <= 0.0) || (ball.x >= 23.0)) {
    ball.vX = ball.vX * -1.0;
  }
  if ((ball.y <= 0.0) || (ball.y >= 7.0)) {
    ball.vY = ball.vY * -1.0;
  }
}

void drawBall(boolean clr) {
  int x = (int) ball.x;
  int y = (int) ball.y;
  if (clr) {
    GlassesBits[x][0] = 0;
  } else {
    GlassesBits[x][0] = 1 << y;
  }
}

void movePaddle(Player* p) {
  int y = (int)ball.y; 
  if (p->y - 1 < y) {
    if ((p->y + 1 + p->l) < 8) {
      p->y += 1;
    }
  } else if (p->y + 1 > y) {
    if ((p->y - 1) > 0) {
      p->y -= 1;
    }
  }
}

int paddle[] = {0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f};

void drawPaddles(boolean clr) {
  for (int i = 0; i < 2; i++) {
    int x = (i == 0) ? player1.x : player2.x;
    int y = (i == 0) ? player1.y : player2.y;
    int l =  (i == 0) ? player1.l : player2.l;
    if (clr) {
      GlassesBits[x][0] = 0;
    } else {
      GlassesBits[x][0] = paddle[l] << y;
    }
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
 
  drawBall(false);
  writeBitFrame(0,0);
  drawBall(true);
  
}

void pong() {
  if (!patternInit) {
    ball.x = 0;
    ball.y = 0;
    ball.vX = 1;
    ball.vY = 1;
    player1.x = 0;
    player1.y = 0;
    player1.l = 3;
    player2.x = 23;
    player2.y = 0;
    player2.l = 3;
    switchDrawType(0,0);
    patternInit = true;
  }
  
  frame++;
  if (frame > 40) {
    frame = 0;
  } else {
    return;
  }
  
  moveBall();
  if (ball.x < 4) {
    movePaddle(&player1);
  } else if (ball.x > 20) {
    movePaddle(&player2);
  }
  
  drawBall(false);
  drawPaddles(false);
  writeBitFrame(0,0);
  drawBall(true);
  drawPaddles(true);

}

int breaks[24];

void moveBreakBall() {
  ball.x = ball.x + ball.vX;
  ball.y = ball.y + ball.vY;
  
  int x = (int) ball.x;
  int y = (int) ball.y;
  
  boolean hitBreak = false;
  if (y <= 1 && breaks[x]) {
    breaks[x] = 0;
    if (x + 1 < 24) {
      breaks[x+1] = 0;
    }
    if (x - 1 > 0) {
      breaks[x-1] = 0;
    }
    hitBreak = true;
  } 
  if ((ball.x <= 0.0) || (ball.x >= 23.0) || hitBreak) {
    ball.vX = ball.vX * -1.0;
  }
  if ((ball.y <= 0.0) || (ball.y >= 7.0) || hitBreak) {
    ball.vY = ball.vY * -1.0;
  }
}

void drawBreaks(boolean clr) {
  for (int i = 0; i < 24; i++) {
    if (clr) {
      GlassesBits[i][0] = 0;
    } else {
      GlassesBits[i][0] |= breaks[i];
    }
  }
}

void boxBreak() {
  if (!patternInit) {
    ball.x = 1;
    ball.y = 6;
    ball.vX = 1;
    ball.vY = 1;
    for (int i = 0; i < 24; i++) {
      breaks[i] = 0x03;
    }
    switchDrawType(0,0);
    patternInit = true;
  }
  
  frame++;
  if (frame > 50) {
    frame = 0;
  } else {
    return;
  }
  
  moveBreakBall();
  
  drawBall(false);
  drawBreaks(false);
  writeBitFrame(0,0);
  drawBall(true);
  drawBreaks(true);
}

