//
//  Keyboard.h
//
//
//  Created by Fermin Gomez on 5/22/18.
//
//
#ifndef Keyboard_h
#define Keyboard_h

/*SEE .c for more information*/
void Keyboard_Handler();
void addToBuffer(char input);
/* Returns keyboard input if available, or 0 if there wasn't any */
char getKeyInput();


#endif /* Keyboard_h */
