#include "VideoModule.h"
#include "syscall.h"
#define STDOUT 1
#define WRITE 1
#define sgn(x) ((x<0)?-1:((x>0)?1:0))
#define abs(x) (((x)<0) ? -(x) : (x))

void drawPixel(int x, int y, Colour colour) {
    int rgb = colour.Red;
    rgb = (rgb << 8) + colour.Green;
    rgb = (rgb << 8) + colour.Blue;
    _syscall(_drawPixel, x, y, rgb);

}

void drawDigitalColon(Colour colour, int x, int y) {
    drawDigitalClockExp(colour, colon_map(), x, y);
}

void drawDigitalNumber(Colour colour, int number, int x, int y) {
    drawDigitalClockExp(colour, clock_map(number), x, y);
}
void drawDigitalClockExp(Colour colour, unsigned char * fontExp, int xPosition, int yPosition) {
    char font;
    int xpos = xPosition;
    for (int y = 0; y < 36; y++) {
        for (int j = 0; j < 8; j++) {
            font = fontExp[(y*8)+j];
            for (int x = 0; x < 8; x++) {
                if (((font >> (8-x)) %2) != 0){
                    drawPixel(x + xPosition, y + yPosition, colour);
                } else {
                    Colour black = {0,0,0};
                    drawPixel(x + xPosition, y + yPosition, black);
                }
            }
            xPosition += 8;
        }
        xPosition = xpos;
    }
}

void drawTime(char * time, Colour colour) {
    int xPosition = (XRESOLUTION/2) - 281;
    int yPosition = (YRESOLUTION/2) - 18;
    char c;
    int i = 0;
    while ((c=time[i++])) {
        if (c == ':') {
            drawDigitalColon(colour, xPosition, yPosition);
        } else {
            drawDigitalNumber(colour, (c - '0'), xPosition, yPosition);
        }
        xPosition += (64 + 10); // 10 es la distancia entre digitos
    }
}

void drawSushiStateAux(Colour colour, int sushiManAmount, int monsterAmount) {
    int xPosition = (XRESOLUTION/2) - (195+120);
    int yPosition = (YRESOLUTION/2) + 150;
    drawDigitalClockExp(colour, clock_map(5), xPosition, yPosition);
    xPosition += (64);
    drawDigitalColon(colour, xPosition, yPosition);
    xPosition += (64);
    drawDigitalClockExp(colour, clock_map(sushiManAmount), xPosition, yPosition);
    xPosition += (64 + 120);
    drawDigitalClockExp(colour, letterM_map(), xPosition, yPosition);
    xPosition += (64);
    drawDigitalColon(colour, xPosition, yPosition);
    xPosition += (64);
    drawDigitalClockExp(colour, clock_map(monsterAmount), xPosition, yPosition);
}

void drawSushiState(int sushiManAmount, int monsterAmount) {
    Colour white1 = {255, 255, 255};
    drawSushiStateAux(white1, sushiManAmount, monsterAmount);
}

void clearSushiState(int sushiManAmount, int monsterAmount) {
    Colour black = {0,0,0};
    drawSushiStateAux(black, sushiManAmount, monsterAmount);
    
}

void drawCircle(int x, int y, int radius, Colour colour) {
    int deltax = radius;
    int deltay = 0;
    int err = 0;
    
    while (deltax >= deltay)
    {
        drawPixel(x + deltax, y + deltay, colour);
        drawPixel(x + deltay, y + deltax, colour);
        drawPixel(x - deltay, y + deltax, colour);
        drawPixel(x - deltax, y + deltay, colour);
        drawPixel(x - deltax, y - deltay, colour);
        drawPixel(x - deltay, y - deltax, colour);
        drawPixel(x + deltay, y - deltax, colour);
        drawPixel(x + deltax, y - deltay, colour);
        
        deltay += 1;
        err += 1 + 2*deltay;
        if (2*(err-deltax) + 1 > 0)
        {
            deltax -= 1;
            err += 1 - 2*deltax;
        }
    }
}

void DrawFilledCircle(int x0, int y0, int radius, Colour colour) {
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;
    
    while (x >= y)
    {
        for (int i = x0 - x; i <= x0 + x; i++)
        {
            drawPixel(i, y0 + y, colour);
            drawPixel(i, y0 - y, colour);
        }
        for (int i = x0 - y; i <= x0 + y; i++)
        {
            drawPixel(i, y0 + x, colour);
            drawPixel(i, y0 - x, colour);
        }
        
        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

/**************************************************************************
 *  line_fast                                                             *
 *    draws a line using Bresenham's line-drawing algorithm, which uses   *
 *    no multiplication or division.                                      *
 **************************************************************************/

void line_fast(int x1, int y1, int x2, int y2, Colour colour) {
    int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;
    
    dx=x2-x1;      /* the horizontal distance of the line */
    dy=y2-y1;      /* the vertical distance of the line */
    dxabs=abs(dx);
    dyabs=abs(dy);
    sdx=sgn(dx);
    sdy=sgn(dy);
    x=dyabs>>1;
    y=dxabs>>1;
    px=x1;
    py=y1;
    
    //VGA[(py<<8)+(py<<6)+px]=color;
    
    if (dxabs>=dyabs) /* the line is more horizontal than vertical */
    {
        for(i=0;i<dxabs;i++)
        {
            y+=dyabs;
            if (y>=dxabs)
            {
                y-=dxabs;
                py+=sdy;
            }
            px+=sdx;
            drawPixel(px,py,colour);
        }
    }
    else /* the line is more vertical than horizontal */
    {
        for(i=0;i<dyabs;i++)
        {
            x+=dxabs;
            if (x>=dyabs)
            {
                x-=dyabs;
                px+=sdx;
            }
            py+=sdy;
            drawPixel(px,py,colour);
        }
    }
}

