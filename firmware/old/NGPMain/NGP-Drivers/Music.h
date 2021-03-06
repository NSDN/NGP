/*
* Copy from NSDN(TM)-Beeper
**/

/*
* Music Player with STC15W408AS
* Created by D.zzm in 2016.7.29
*
* Some music from Touhou Project
* 
* The music data file
**/

#ifndef __MUISC_H_
#define __MUSIC_H_

#define MUSIC_LENGTH 64

const char MUSIC[] = { 
	21, 24, 26, 29, 31, 255, 29, 31, 33, 255, 29, 26, 24, 21, 29, 31,
	26, 255, 26, 29, 31, 255, 29, 31, 33, 255, 36, 38, 41, 40, 38, 36,
	38, 255, 36, 33, 31, 255, 33, 29, 31, 255, 29, 31, 33, 26, 31, 29,
	26, 255, 26, 24, 26, 255, 29, 31, 33, 31, 24, 255, 26, 255, 255, 255
};

#define MID_hina_LENGTH 192
const char MID_hina[] = {
	0x0B, 0x0B, 0x0B, 0x0A, 0x0B, 0x0B, 0x07, 0x0B, 0x07, 0x07, 0x0B, 0x0B, 0x0A, 0x07, 0x0A, 0x07, 
	0x0B, 0x0B, 0x0B, 0x0A, 0x0B, 0x0B, 0x07, 0x0B, 0x07, 0x07, 0x0B, 0x0B, 0x0A, 0x0A, 0x0A, 0x0A, 
	0x0E, 0x0E, 0x0E, 0x0D, 0x0E, 0x0E, 0x0A, 0x0E, 0x0A, 0x0A, 0x0E, 0x0E, 0x0D, 0x0A, 0x0D, 0x0A, 
	0x11, 0x11, 0x11, 0x10, 0x11, 0x11, 0x0E, 0x11, 0x0E, 0x0E, 0x11, 0x11, 0x10, 0x10, 0x10, 0x10, 
	0x0E, 0x0E, 0x0E, 0x13, 0x15, 0x15, 0x13, 0x15, 0x16, 0x15, 0x13, 0x11, 0x13, 0x13, 0x13, 0x13, 
	0x0E, 0x0E, 0x13, 0x13, 0x15, 0x15, 0x13, 0x15, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x0E, 0x11, 
	0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0A, 0x09, 0x0A, 0x09, 0x07, 0x05, 0x02, 0x02, 0x02, 0x05, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 
	0x0E, 0x0E, 0x0E, 0x13, 0x15, 0x15, 0x13, 0x15, 0x16, 0x15, 0x13, 0x11, 0x13, 0x13, 0x13, 0x13, 
	0x0E, 0x0E, 0x13, 0x13, 0x15, 0x15, 0x13, 0x15, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x0E, 0x11, 
	0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0A, 0x09, 0x0A, 0x09, 0x07, 0x05, 0x02, 0x02, 0x02, 0x05, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 
};

#define MID_seija_LENGTH 192
const char MID_seija[] = {
	0x1A, 0xFF, 0x18, 0x15, 0x18, 0x15, 0x18, 0x1A, 0xFF, 0x1A, 0x1A, 0x15, 0x1D, 0x1C, 0x18, 0x1A, 
	0x1A, 0xFF, 0x18, 0x15, 0x18, 0x15, 0x18, 0x1A, 0xFF, 0x1A, 0x1A, 0x15, 0x1D, 0x1C, 0x18, 0xFF, 
	0x1A, 0xFF, 0x18, 0x15, 0x18, 0x15, 0x18, 0x1A, 0xFF, 0x1A, 0x1A, 0x15, 0x1D, 0x1C, 0x18, 0x1A, 
	0x1A, 0xFF, 0x18, 0x15, 0x18, 0x15, 0x18, 0x1A, 0xFF, 0x1A, 0x1A, 0x15, 0x1D, 0x1C, 0x1D, 0xFF, 
	0x1A, 0xFF, 0x18, 0x15, 0x18, 0x15, 0x18, 0x1A, 0xFF, 0x1A, 0x1A, 0x15, 0x1D, 0x1C, 0x18, 0x1A, 
	0x1A, 0xFF, 0x18, 0x15, 0x18, 0x15, 0x18, 0x1A, 0xFF, 0x1A, 0x1A, 0x15, 0x1D, 0x1C, 0x18, 0xFF, 
	0x1A, 0xFF, 0x18, 0x15, 0x18, 0x15, 0x18, 0x1A, 0xFF, 0x1A, 0x1A, 0x15, 0x1D, 0x1C, 0x18, 0x1A, 
	0x1A, 0xFF, 0x18, 0x15, 0x18, 0x15, 0x18, 0x1A, 0xFF, 0x1A, 0x1A, 0x15, 0x1D, 0x1C, 0x1D, 0xFF, 
	0x1A, 0xFF, 0xFF, 0x1D, 0x1A, 0x1D, 0x1C, 0x18, 0x1D, 0xFF, 0x1C, 0xFF, 0x1A, 0xFF, 0xFF, 0xFF, 
	0x1A, 0xFF, 0xFF, 0x1D, 0x1A, 0x1D, 0x1C, 0x18, 0x1F, 0xFF, 0x24, 0xFF, 0x21, 0xFF, 0xFF, 0xFF, 
	0x1A, 0xFF, 0xFF, 0x1D, 0x1A, 0x1D, 0x1C, 0x18, 0x1D, 0xFF, 0x1C, 0xFF, 0x1A, 0xFF, 0xFF, 0xFF, 
	0x1A, 0xFF, 0xFF, 0x1D, 0x1A, 0x1D, 0x1C, 0x18, 0x1F, 0xFF, 0x24, 0xFF, 0x21, 0xFF, 0xFF, 0xFF, 
};

#define MID_remilia_LENGTH 328
const char MID_remilia[] = {
	0x16, 0x1B, 0x1D, 0x1E, 0x1E, 0x1E, 0x20, 0x1D, 0x1D, 0x1D, 0x1E, 0x1B, 0x1B, 0x1B, 0x1B, 0xFF, 
	0x16, 0x1B, 0x1D, 0x1E, 0x1E, 0x1E, 0x20, 0x1D, 0x1D, 0x1D, 0x25, 0x22, 0x22, 0x22, 0x22, 0xFF, 
	0x22, 0x25, 0x27, 0x20, 0x20, 0x20, 0x20, 0xFF, 0x20, 0x22, 0x25, 0x1E, 0x1E, 0x1E, 0x1E, 0xFF, 
	0x1B, 0x1B, 0x1D, 0x1E, 0x1E, 0x1E, 0x20, 0x1D, 0x1D, 0x1D, 0x1A, 0x1B, 0x1B, 0x1B, 0xFF, 0x1D, 
	0x16, 0x1B, 0x1D, 0x1E, 0x1E, 0x1E, 0x20, 0x1D, 0x1D, 0x1D, 0x1E, 0x1B, 0x1B, 0x1B, 0x1B, 0xFF, 
	0x16, 0x1B, 0x1D, 0x1E, 0x1E, 0x1E, 0x20, 0x1D, 0x1D, 0x1D, 0x25, 0x22, 0x22, 0x22, 0x22, 0xFF, 
	0x22, 0x25, 0x27, 0x20, 0x20, 0x20, 0x20, 0xFF, 0x20, 0x22, 0x25, 0x1E, 0x1E, 0x1E, 0x1E, 0xFF, 
	0x1B, 0x1B, 0x1D, 0x1E, 0x1E, 0x1E, 0x20, 0x1D, 0x1D, 0x1D, 0x1A, 0x1B, 0x1B, 0x1B, 0x1B, 0x1D, 
	0xFF, 0x19, 0x1C, 0x1E, 0x1C, 0x1E, 0x1C, 0x1E, 0x21, 0x21, 0x20, 0x1E, 0x1C, 0x1E, 0x1C, 0x1E, 
	0x25, 0x25, 0x23, 0x21, 0x20, 0x20, 0x1E, 0x1D, 0x19, 0x19, 0x17, 0x19, 0x19, 0x19, 0x19, 0x19, 
	0xFF, 0x19, 0x1C, 0x1E, 0x1C, 0x1E, 0x1C, 0x1E, 0x21, 0x21, 0x20, 0x1E, 0x1C, 0x1E, 0x1C, 0x1E, 
	0x25, 0x25, 0x23, 0x21, 0x21, 0x20, 0x1E, 0x20, 0x21, 0x21, 0x23, 0x1D, 0x25, 0xFF, 0x1D, 0x25, 
	0xFF, 0x1D, 0x25, 0xFF, 0xFF, 0x22, 0x29, 0x27, 0x27, 0x27, 0x25, 0x24, 0x20, 0x20, 0x22, 0x22, 
	0x22, 0x22, 0x22, 0x22, 0xFF, 0x22, 0x29, 0x27, 0x27, 0x27, 0x25, 0x24, 0x2C, 0x2C, 0x29, 0x29, 
	0x29, 0x29, 0x29, 0x29, 0x29, 0xFF, 0x29, 0x27, 0x27, 0xFF, 0x25, 0x24, 0xFF, 0x20, 0x22, 0x22, 
	0x22, 0x22, 0x22, 0x22, 0xFF, 0x22, 0x24, 0x25, 0x24, 0x20, 0x22, 0x22, 0x22, 0x22, 0x22, 0x29, 
	0x29, 0x29, 0x29, 0x29, 0xFF, 0x22, 0x29, 0x27, 0x27, 0x27, 0x25, 0x24, 0x24, 0x20, 0x22, 0x22, 
	0x22, 0x22, 0x22, 0x22, 0xFF, 0x22, 0x29, 0x27, 0x27, 0x27, 0x25, 0x24, 0x2C, 0x2C, 0x29, 0x29, 
	0x29, 0x29, 0x29, 0xFF, 0xFF, 0x22, 0x29, 0x27, 0x27, 0x27, 0x25, 0x24, 0x24, 0x20, 0x22, 0x22, 
	0x22, 0x22, 0x22, 0x22, 0xFF, 0x22, 0x24, 0x25, 0x24, 0x20, 0x22, 0x22, 0x22, 0x22, 0x22, 0x25, 
	0x25, 0x25, 0x25, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};

#define MID_9_LENGTH 322
const char MID_9[] = {
	0xFF, 0x1A, 0x1C, 0x1D, 0x1F, 0x21, 0x21, 0xFF, 0x21, 0x1D, 0x21, 0x1D, 0x1C, 0x1A, 0x1A, 0x1A, 
	0x1A, 0xFF, 0x1A, 0x1D, 0x21, 0x26, 0x26, 0xFF, 0x26, 0x24, 0x26, 0x24, 0x1F, 0x21, 0x21, 0x21, 
	0x21, 0x21, 0xFF, 0x28, 0x28, 0x29, 0x29, 0xFF, 0x29, 0x28, 0x29, 0x2D, 0x28, 0x2B, 0x26, 0x23, 
	0x2B, 0x26, 0x23, 0x28, 0x28, 0x29, 0x26, 0x21, 0x29, 0x28, 0x24, 0x1F, 0x28, 0x28, 0x28, 0xFF, 
	0x26, 0x26, 0x1A, 0x1C, 0x1D, 0x21, 0x21, 0xFF, 0x21, 0x1D, 0x21, 0x1D, 0x1C, 0x1A, 0x1A, 0x1A, 
	0x1A, 0xFF, 0x1A, 0x1D, 0x21, 0x26, 0x26, 0xFF, 0x26, 0x24, 0x26, 0x24, 0x1F, 0x21, 0x21, 0x21, 
	0x21, 0x21, 0xFF, 0x28, 0x28, 0x29, 0x29, 0xFF, 0x29, 0x28, 0x29, 0x2D, 0x28, 0x2B, 0x26, 0x23, 
	0x2B, 0x26, 0x23, 0x28, 0xFF, 0x29, 0x26, 0x21, 0x29, 0x28, 0x24, 0x1F, 0x28, 0x28, 0x28, 0xFF, 
	0x26, 0x26, 0x1A, 0x1C, 0x1D, 0x1D, 0x21, 0x21, 0xFF, 0x21, 0x1D, 0x21, 0x1D, 0x1C, 0x1A, 0x1A, 
	0x1A, 0x1A, 0xFF, 0x1A, 0x1D, 0x21, 0x26, 0x26, 0xFF, 0x26, 0x24, 0x26, 0x24, 0x1F, 0x21, 0x21, 
	0x21, 0x21, 0x21, 0xFF, 0x28, 0x28, 0x29, 0x29, 0xFF, 0x29, 0x28, 0x29, 0x2D, 0x28, 0x2B, 0x26, 
	0x23, 0x2B, 0x26, 0x23, 0x28, 0xFF, 0x29, 0x26, 0x21, 0x29, 0x28, 0x24, 0x1F, 0x28, 0x28, 0x28, 
	0xFF, 0x26, 0x26, 0x1A, 0x1C, 0x1D, 0x21, 0x21, 0xFF, 0x21, 0x1D, 0x21, 0x1D, 0x1C, 0x1A, 0x1A, 
	0x1A, 0x1A, 0xFF, 0x1A, 0x1D, 0x21, 0x26, 0x26, 0xFF, 0x26, 0x24, 0x26, 0x24, 0x1F, 0x21, 0x21, 
	0x21, 0x21, 0x21, 0xFF, 0x28, 0x28, 0x29, 0x29, 0xFF, 0x29, 0x28, 0x29, 0x2D, 0x28, 0x2B, 0x26, 
	0x23, 0x2B, 0x26, 0x23, 0x28, 0xFF, 0x29, 0x26, 0x21, 0x29, 0x28, 0x24, 0x1F, 0x28, 0x28, 0x28, 
	0xFF, 0x26, 0x26, 0xFF, 0x2D, 0xFF, 0x2D, 0x29, 0x26, 0x2D, 0x2B, 0x28, 0x24, 0x2B, 0x2B, 0x2B, 
	0xFF, 0x2D, 0x29, 0x29, 0xFF, 0x2D, 0x2D, 0x29, 0x26, 0x2D, 0x30, 0x2B, 0x28, 0x30, 0x2B, 0x2B, 
	0xFF, 0x2D, 0x2D, 0x2D, 0xFF, 0x26, 0x2D, 0x29, 0x26, 0x2D, 0x2B, 0x28, 0x24, 0x2B, 0x2B, 0x2B, 
	0xFF, 0x2D, 0x29, 0x29, 0xFF, 0x2D, 0x2D, 0x29, 0x26, 0x2D, 0x30, 0x2B, 0x28, 0x30, 0xFF, 0xFF, 
	0x30, 0xFF, 
};

#define MID_U_N_Owen_was_her_LENGTH 454
const char MID_U_N_Owen_was_her[] = {
	0x25, 0xFF, 0x19, 0xFF, 0x24, 0xFF, 0x18, 0xFF, 0x28, 0xFF, 0x1C, 0xFF, 0x27, 0xFF, 0x1E, 0xFF, 
	0x1D, 0x29, 0x19, 0x29, 0x1C, 0x28, 0x18, 0x28, 0x1F, 0x2B, 0x1C, 0x2B, 0x1B, 0x2A, 0xFF, 0x2A, 
	0x1D, 0x29, 0x19, 0x29, 0x1C, 0x28, 0x18, 0x28, 0x1F, 0x2B, 0x1C, 0x2B, 0x1B, 0x2A, 0x1E, 0x2A, 
	0x1D, 0x29, 0x19, 0x29, 0x1C, 0x28, 0x18, 0x28, 0x1F, 0x2B, 0x1C, 0x2B, 0x1B, 0x2A, 0xFF, 0x2A, 
	0x16, 0xFF, 0x1D, 0xFF, 0x18, 0xFF, 0x1D, 0xFF, 0x19, 0xFF, 0x1B, 0x1D, 0x1B, 0xFF, 0x1F, 0xFF, 
	0x22, 0x1D, 0x24, 0x25, 0x24, 0x25, 0x22, 0x20, 0x1D, 0x20, 0x1B, 0x1D, 0x19, 0xFF, 0xFF, 0x19, 
	0x16, 0xFF, 0x1D, 0xFF, 0x18, 0xFF, 0x1D, 0xFF, 0x19, 0xFF, 0x1B, 0x1D, 0x1B, 0xFF, 0x1F, 0xFF, 
	0x22, 0x1D, 0x24, 0x25, 0x24, 0x25, 0x22, 0x20, 0x22, 0x22, 0x22, 0x22, 0x22, 0xFF, 0xFF, 0xFF, 
	0x16, 0xFF, 0x1D, 0xFF, 0x18, 0xFF, 0x1D, 0xFF, 0x19, 0xFF, 0x1B, 0x1D, 0x1B, 0xFF, 0x1F, 0xFF, 
	0x22, 0x1D, 0x24, 0x25, 0x24, 0x25, 0x22, 0x20, 0x1D, 0x20, 0x1B, 0x1D, 0x19, 0x19, 0x19, 0xFF, 
	0x16, 0xFF, 0x1D, 0xFF, 0x18, 0xFF, 0x1D, 0xFF, 0x19, 0xFF, 0x1B, 0x1D, 0x1B, 0xFF, 0x1F, 0xFF, 
	0x22, 0x1D, 0x24, 0x25, 0x24, 0x25, 0x22, 0x20, 0x22, 0x22, 0x22, 0x22, 0x22, 0xFF, 0xFF, 0xFF, 
	0x19, 0xFF, 0x19, 0xFF, 0x18, 0xFF, 0x18, 0xFF, 0x1C, 0xFF, 0x1C, 0xFF, 0x1B, 0xFF, 0x1B, 0xFF, 
	0x19, 0x29, 0x30, 0x29, 0x18, 0x28, 0x30, 0x28, 0x1C, 0x2B, 0x30, 0x2B, 0x1B, 0x2A, 0x30, 0x2A, 
	0x19, 0x29, 0x30, 0x29, 0x18, 0x28, 0x30, 0x28, 0x1C, 0x2B, 0x30, 0x2B, 0x1B, 0x2A, 0x30, 0x2A, 
	0x19, 0x29, 0x30, 0x29, 0x18, 0x28, 0x30, 0x28, 0x1C, 0x2B, 0x30, 0x2B, 0x1B, 0x2A, 0x30, 0x2A, 
	0x1D, 0x29, 0x19, 0x29, 0x1C, 0x28, 0x18, 0x28, 0x1F, 0x2B, 0x1C, 0x2B, 0x1B, 0x2A, 0x1E, 0x2A, 
	0x1D, 0x29, 0x19, 0x29, 0x1C, 0x28, 0x18, 0x28, 0x1F, 0x2B, 0x1C, 0x2B, 0x1B, 0x2A, 0x1E, 0x2A, 
	0x1D, 0x29, 0x19, 0x29, 0x1C, 0x28, 0x18, 0x28, 0x1F, 0x2B, 0x1C, 0x2B, 0x1B, 0x2A, 0x1E, 0x2A, 
	0x1D, 0x29, 0x19, 0x29, 0x1C, 0x28, 0x18, 0x28, 0x1F, 0x2B, 0x1C, 0x2B, 0x1B, 0x2A, 0x30, 0x2A, 
	0xFF, 0xFF, 0xFF, 0xFF, 0x16, 0xFF, 0x1D, 0xFF, 0x18, 0xFF, 0x1D, 0xFF, 0x19, 0xFF, 0x1B, 0x1D, 
	0x1B, 0xFF, 0x1F, 0xFF, 0x22, 0x1D, 0x24, 0x25, 0x24, 0x25, 0x22, 0x20, 0x1D, 0x20, 0x1B, 0x1D, 
	0x19, 0xFF, 0xFF, 0x19, 0x16, 0xFF, 0x1D, 0xFF, 0x18, 0xFF, 0x1D, 0xFF, 0x19, 0xFF, 0x1B, 0x1D, 
	0x1B, 0xFF, 0x1F, 0xFF, 0x22, 0x1D, 0x24, 0x25, 0x24, 0x25, 0x22, 0x20, 0x22, 0x22, 0x22, 0x22, 
	0x22, 0xFF, 0xFF, 0xFF, 0x16, 0xFF, 0x1D, 0xFF, 0x18, 0xFF, 0x1D, 0xFF, 0x19, 0xFF, 0x1B, 0x1D, 
	0x1B, 0xFF, 0x1F, 0xFF, 0x22, 0x1D, 0x24, 0x25, 0x24, 0x25, 0x22, 0x20, 0x1D, 0x20, 0x1B, 0x1D, 
	0x19, 0x19, 0x19, 0xFF, 0x16, 0xFF, 0x1D, 0xFF, 0x18, 0xFF, 0x1D, 0xFF, 0x19, 0xFF, 0x1B, 0x1D, 
	0x1B, 0xFF, 0x1F, 0xFF, 0x22, 0x1D, 0x24, 0x25, 0x24, 0x25, 0x22, 0x20, 0x22, 0x22, 0x22, 0x22, 
	0x22, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};

#define MID_Staking_Your_Life_on_a_Prank_LENGTH 382
const char MID_Staking_Your_Life_on_a_Prank[] = {
	0x17, 0x17, 0xFF, 0x18, 0x18, 0x17, 0x17, 0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x17, 0x17, 0x16, 
	0x18, 0x18, 0xFF, 0x19, 0x19, 0x18, 0x18, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x18, 0x18, 0x17, 
	0x17, 0x17, 0xFF, 0x18, 0x18, 0x17, 0x17, 0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x17, 0x17, 0x16, 
	0x18, 0x18, 0xFF, 0x19, 0x19, 0x18, 0x18, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x18, 0x18, 0x17, 
	0x18, 0x18, 0xFF, 0x1B, 0x1B, 0x1D, 0x1D, 0x1F, 0x18, 0x18, 0xFF, 0x16, 0x16, 0x13, 0x16, 0x18, 
	0x18, 0x18, 0xFF, 0x1B, 0x1B, 0x1A, 0x18, 0x13, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x16, 0x18, 
	0x18, 0x18, 0xFF, 0x1B, 0x1B, 0x1D, 0x1D, 0x1F, 0x18, 0x18, 0xFF, 0x16, 0x16, 0x13, 0x16, 0x18, 
	0x18, 0x18, 0xFF, 0x1B, 0x1B, 0x1A, 0x18, 0x13, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x16, 0x18, 
	0x24, 0x24, 0xFF, 0x27, 0x27, 0x29, 0x29, 0x2B, 0x24, 0x24, 0xFF, 0x22, 0x22, 0x1F, 0x22, 0x24, 
	0x24, 0x24, 0xFF, 0x27, 0x27, 0x26, 0x24, 0x1F, 0x24, 0x24, 0x24, 0x24, 0x24, 0xFF, 0x22, 0x24, 
	0x24, 0x24, 0xFF, 0x27, 0x27, 0x29, 0x29, 0x2B, 0x24, 0x24, 0xFF, 0x22, 0x22, 0x1F, 0x22, 0x24, 
	0x24, 0x24, 0xFF, 0x27, 0x27, 0x26, 0x24, 0x1F, 0x24, 0x24, 0x24, 0x24, 0x24, 0xFF, 0x14, 0x13, 
	0x12, 0x18, 0x18, 0xFF, 0x19, 0x19, 0x18, 0x18, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x18, 0x18, 
	0x17, 0x19, 0x19, 0xFF, 0x1A, 0x1A, 0x19, 0x19, 0x18, 0x19, 0x19, 0x19, 0x1A, 0x1A, 0x19, 0x19, 
	0x18, 0x18, 0x18, 0xFF, 0x19, 0x19, 0x18, 0x18, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x18, 0x18, 
	0x17, 0x19, 0x19, 0xFF, 0x1A, 0x1A, 0x19, 0x19, 0x18, 0x19, 0x19, 0x19, 0x1A, 0x1A, 0x19, 0x19, 
	0x18, 0x19, 0x19, 0xFF, 0x1C, 0x1C, 0x1E, 0x1E, 0x20, 0x19, 0x19, 0xFF, 0x17, 0x17, 0x14, 0x17, 
	0x19, 0x19, 0x19, 0xFF, 0x1C, 0x1C, 0x1B, 0x19, 0x14, 0x19, 0x19, 0x19, 0x19, 0x19, 0xFF, 0x17, 
	0x19, 0x19, 0x19, 0xFF, 0x1C, 0x1C, 0x1E, 0x1E, 0x20, 0x19, 0x19, 0xFF, 0x17, 0x17, 0x14, 0x17, 
	0x19, 0x19, 0x19, 0xFF, 0x1C, 0x1C, 0x1B, 0x19, 0x14, 0x19, 0x19, 0x19, 0x19, 0x19, 0xFF, 0x17, 
	0x19, 0x25, 0x25, 0xFF, 0x28, 0x28, 0x2A, 0x2A, 0x2C, 0x25, 0x25, 0xFF, 0x23, 0x23, 0x20, 0x23, 
	0x25, 0x25, 0x25, 0xFF, 0x28, 0x28, 0x27, 0x25, 0x20, 0x25, 0x25, 0x25, 0x25, 0x25, 0xFF, 0x23, 
	0x25, 0x25, 0x25, 0xFF, 0x28, 0x28, 0x2A, 0x2A, 0x2C, 0x25, 0x25, 0xFF, 0x23, 0x23, 0x20, 0x23, 
	0x25, 0x25, 0x25, 0xFF, 0x28, 0x28, 0x27, 0x25, 0x20, 0x25, 0x25, 0x25, 0x25, 0x25, 
};

#endif
