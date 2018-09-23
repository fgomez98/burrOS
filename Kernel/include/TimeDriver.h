//
//  GetTime.h
//
//
//  Created by Fermin Gomez on 5/22/18.
//
//

#ifndef TimeDriver_h
#define TimeDriver_h

/*Used to retrieve time using interruptions using assemly, using interruption to 70h, called from getTime()*/

unsigned int getHour();
unsigned int getMin();
unsigned int getSec();

#endif
