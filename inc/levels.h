
#ifndef _LEVELS_H_
#define _LEVELS_H_

// Levels are defined as a 8x32-bit array, each bit representing one block.
// A separate 8x23-bit array defines the special blocks.

const uint32_t defaultLevel[8] = {0x00000000,0x030FF0C0,0x030FF0C0,0x00000000,0x00000000,0x030FF0C0,0x030FF0C0,0x00000000};
const uint32_t defaultSpecial[8] = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};

const uint32_t  level1[8] = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};
const uint32_t  special1[8] = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};
//More levels...



#endif /* _LEVELS_H_ */
