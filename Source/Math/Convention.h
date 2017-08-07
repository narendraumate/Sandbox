//
//  Convention.h
//
//
//  Created by Narendra Umate on 8/6/17.
//

#ifndef Convention_h
#define Convention_h

#include "Convention.h"

typedef enum {
	X = 1 << 0,
	Y = 1 << 1,
	Z = 1 << 2,
} Axis;

typedef enum {
	O1 = 1 << 0,
	O2 = 1 << 1,
	O3 = 1 << 2,
} Order;

typedef enum {
	X1 = 1 << 0,
	Y1 = 1 << 1,
	Z1 = 1 << 2,

	X2 = 1 << 3,
	Y2 = 1 << 4,
	Z2 = 1 << 5,

	X3 = 1 << 6,
	Y3 = 1 << 7,
	Z3 = 1 << 8,
} AxisOrder;

typedef enum {
	X3Y2Z1 = X3 | Y2 | Z1,
	Y3Z2X1 = Y3 | Z2 | X1,
	Z3X2Y1 = Z3 | X2 | Y1,
	X3Z2Y1 = X3 | Z2 | Y1,
	Y3X2Z1 = Y3 | X2 | Z1,
	Z3Y2X1 = Z3 | Y2 | X1,
} AxisOrderConvention;

typedef enum {
	R = 1 << 0,
	L = 1 << 1,
	U = 1 << 2,
	D = 1 << 3,
	B = 1 << 4,
	F = 1 << 5,
} Direction;

typedef enum {
	RX = 1 << 0,
	LX = 1 << 1,
	UX = 1 << 2,
	DX = 1 << 3,
	BX = 1 << 4,
	FX = 1 << 5,

	RY = 1 << 6,
	LY = 1 << 7,
	UY = 1 << 8,
	DY = 1 << 9,
	BY = 1 << 10,
	FY = 1 << 11,

	RZ = 1 << 12,
	LZ = 1 << 13,
	UZ = 1 << 14,
	DZ = 1 << 15,
	BZ = 1 << 16,
	FZ = 1 << 17,
} AxisDirection;

typedef enum {
	RUB = RX | UY | BZ,
	RUF = RX | UY | FZ,
	RBU = RX | BY | UZ,
	RBD = RX | BY | DZ,
	RDB = RX | DY | BZ,
	RDF = RX | DY | FZ,
	RFU = RX | FY | UZ,
	RFD = RX | FY | DZ,
	URB = UX | RY | BZ,
	URF = UX | RY | FZ,
	UBR = UX | BY | RZ,
	UBL = UX | BY | LZ,
	ULB = UX | LY | BZ,
	ULF = UX | LY | FZ,
	UFR = UX | FY | RZ,
	UFL = UX | FY | LZ,
	BRU = BX | RY | UZ,
	BRD = BX | RY | DZ,
	BUR = BX | UY | RZ,
	BUL = BX | UY | LZ,
	BLU = BX | LY | UZ,
	BLD = BX | LY | DZ,
	BDR = BX | DY | RZ,
	BDL = BX | DY | LZ,
	LUB = LX | UY | BZ,
	LUF = LX | UY | FZ,
	LBU = LX | BY | UZ,
	LBD = LX | BY | DZ,
	LDB = LX | DY | BZ,
	LDF = LX | DY | FZ,
	LFU = LX | FY | UZ,
	LFD = LX | FY | DZ,
	DRB = DX | RY | BZ,
	DRF = DX | RY | FZ,
	DBR = DX | BY | RZ,
	DBL = DX | BY | LZ,
	DLB = DX | LY | BZ,
	DLF = DX | LY | FZ,
	DFR = DX | FY | RZ,
	DFL = DX | FY | LZ,
	FRU = FX | RY | UZ,
	FRD = FX | RY | DZ,
	FUR = FX | UY | RZ,
	FUL = FX | UY | LZ,
	FLU = FX | LY | UZ,
	FLD = FX | LY | DZ,
	FDR = FX | DY | RZ,
	FDL = FX | DY | LZ,
} AxisDirectionConvention;

#if 0
void mainRunMatrix();
void mainGenMatrix();
#endif

#endif /* Convention_h */
