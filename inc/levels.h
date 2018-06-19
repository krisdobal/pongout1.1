
#ifndef _LEVELS_H_
#define _LEVELS_H_

// Levels are defined as a 8x32-bit array, each bit representing one block.
// A separate 8x23-bit array defines the special blocks.

const uint32_t defaultLevel[8] = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};
const uint32_t defaultSpecial[8] = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};

const uint32_t  level1[8] = {0x00000000,0x030FF0C0,0x030FF0C0,0x00000000,0x00000000,0x030FF0C0,0x030FF0C0,0x00000000};
const uint32_t  special1[8] = {0x00000000,0x000C3000,0x000C3000,0x000C3000,0x000C3000,0x000C3000,0x000C3000,0x000C3000};

//Level 2 Skal nok lige have en smule gennemgang af special funktionen
const uint32_t  level2[8] = {0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
const uint32_t  special2[8] = {0x00000000,0x00000000,0x03333330,0x000C3000,0x0CCC3330,0x00000000,0x00000000,0xFFFFFFFF};



#endif /* _LEVELS_H_ */
