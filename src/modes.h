// modes.h
// Andrew S. Ng
// Created: 2021-04-09
// 
// For CS 321 DOS VGA Graphics
// Header for mode setting routines

#ifndef MODES_H
#define MODES_H


void setModeText();

void setMode13h();

void setModeX();

void showPage(uint16_t startOffset);


#endif  // MODES_H