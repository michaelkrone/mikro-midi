#pragma once

#include "IoNamespace.h"
#include "IoDefs.h"

BEGIN_IO_NAMESPACE

/* MAX72_ register addresses */
#define MAX72_NOOP 0x00
#define MAX72_DIGIT1 0x01
#define MAX72_DIGIT2 0x02
#define MAX72_DIGIT3 0x03
#define MAX72_DIGIT4 0x04
#define MAX72_DIGIT5 0x05
#define MAX72_DIGIT6 0x06
#define MAX72_DIGIT7 0x07
#define MAX72_DIGIT8 0x08
#define MAX72_DECODE 0x09
#define MAX72_INTENSITY 0x0A
#define MAX72_SCANLIMIT 0x0B
#define MAX72_SHUTDOWN 0x0C
#define MAX72_TEST 0x0F

/* MAX72_ register options */
#define MAX72_TESTMODEOFF 0
#define MAX72_TESTMODEON 1
#define MAX72_OFF 0
#define MAX72_ON 1

/* Other Options */
#define MAX72_INVERTOFF 0
#define MAX72_INVERTON 0xFF

#define MAX72_NO_DECIMAL -1

/* Character map for seven-segment display */
const byte MAX72_7SEG_CHAR_MAP[101] = {0x00, // SPACE
   0xA0, // !
   0x22, // "
   0x36, // #
   0x4B, // $
   0x5A, // %
   0x6F, // &
   0x02, // '
   0x4E, // (
   0x78, // )
   0x63, // *
   0x07, // +
   0x18, // ,
   0x01, // -
   0x80, // .
   0x25, // /
   0x7E, // 0
   0x30, // 1
   0x6D, // 2
   0x79, // 3
   0x33, // 4
   0x5B, // 5
   0x5F, // 6
   0x70, // 7
   0x7F, // 8
   0x7B, // 9
   0x09, // :
   0x19, // ;
   0x43, // <
   0x41, // =
   0x61, // >
   0x65, // ?
   0x7D, // @
   0x77, // A
   0x1F, // B
   0x4E, // C
   0x3D, // D
   0x4F, // E
   0x47, // F
   0x5E, // G
   0x37, // H
   0x30, // I
   0x38, // J
   0x57, // K
   0x0E, // L
   0x54, // M
   0x76, // N
   0x7E, // O
   0x67, // P
   0x73, // Q
   0x66, // R
   0x5B, // S
   0x0F, // T
   0x3E, // U
   0x3A, // V
   0x2A, // W
   0x37, // X
   0x3B, // Y
   0x69, // Z
   0x4E, // [
   0x13, // |
   0x78, // ]
   0x62, // ^
   0x08, // _
   0x02, // '
   0x77, // a
   0x1F, // b
   0x4E, // c
   0x3D, // d
   0x4F, // e
   0x47, // f
   0x5E, // g
   0x37, // h
   0x30, // i
   0x38, // j
   0x57, // k
   0x0E, // l
   0x54, // m
   0x76, // n
   0x7E, // o
   0x67, // p
   0x73, // q
   0x66, // r
   0x5B, // s
   0x0F, // t
   0x3E, // u
   0x3A, // v
   0x2A, // w
   0x37, // x
   0x3B, // y
   0x69 // z
 };

END_IO_NAMESPACE
