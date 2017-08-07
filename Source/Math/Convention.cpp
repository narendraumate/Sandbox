//
//  Convention.cpp
//
//
//  Created by Narendra Umate on 8/6/17.
//

#include <iostream>

#include "Convention.h"

#if 0
void mainRunMatrix() {
	const int count = 48;

	AxisDirectionConvention frame[count] = {
		RUB, RUF, RBU, RBD, RDB, RDF, RFU, RFD,
		URB, URF, UBR, UBL, ULB, ULF, UFR, UFL,
		BRU, BRD, BUR, BUL, BLU, BLD, BDR, BDL,
		LUB, LUF, LBU, LBD, LDB, LDF, LFU, LFD,
		DRB, DRF, DBR, DBL, DLB, DLF, DFR, DFL,
		FRU, FRD, FUR, FUL, FLU, FLD, FDR, FDL
	};

	char title[count][4] = {
		"RUB", "RUF", "RBU", "RBD", "RDB", "RDF", "RFU", "RFD",
		"URB", "URF", "UBR", "UBL", "ULB", "ULF", "UFR", "UFL",
		"BRU", "BRD", "BUR", "BUL", "BLU", "BLD", "BDR", "BDL",
		"LUB", "LUF", "LBU", "LBD", "LDB", "LDF", "LFU", "LFD",
		"DRB", "DRF", "DBR", "DBL", "DLB", "DLF", "DFR", "DFL",
		"FRU", "FRD", "FUR", "FUL", "FLU", "FLD", "FDR", "FDL"
	};
#if 0
	for (int i = 0; i < count; ++i) {
		std::cout << "----------------" << std::endl;
		std::cout << "frame[" << i << "]: " << title[i] << std::endl;
		std::cout << "xaxis[" << i << "]: " << AxisDirection(frame[i] & 0x0003F) << std::endl;
		std::cout << "yaxis[" << i << "]: " << AxisDirection(frame[i] & 0x00FC0) << std::endl;
		std::cout << "zaxis[" << i << "]: " << AxisDirection(frame[i] & 0x3F000) << std::endl;
		if (i == count - 1) std::cout << "----------------" << std::endl;
	}
#endif
	Mat4 straight[count][count];
	Mat4 inverted[count][count];

	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {
			straight[i][j] = convert(frame[i], frame[j]);
			inverted[i][j] = convert(frame[j], frame[i]);
		}
	}

	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {
			Mat4 transform = Mat4::identity;
			transform.d[3][0] = +1.0f;
			transform.d[3][1] = +2.0f;
			transform.d[3][2] = +3.0f;
			transform.d[3][3] = +1.0f;
			std::cout << title[i] << ": ";
			//std::cout << transform.Translation();
			std::cout << " " << title[j] << ": ";
			//std::cout << (straight[i][j] * (transform * inverted[i][j])).Translation();
			std::cout << std::endl;
		}
	}
}

void mainGenMatrix() {
	char array[6] = { 'R', 'U', 'B', 'L', 'D', 'F' };
	std::cout << "enum Frame {" << std::endl;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			for (int k = 0; k < 6; ++k) {
				if (i == j || j == k || k == i) {
					// std::cout << "\t" << array[i] << "" << array[j] << "" << array[k] << "" << " = " << array[i] << "0 | " << array[j] << "1 | " << array[k] << "2," << std::endl;
				} else if (i - j == 3 || j - k == 3 || k - i == 3 || j - i == 3 || k - j == 3 || i - k == 3) {
					// std::cout << "\t" << array[i] << "" << array[j] << "" << array[k] << "" << " = " << array[i] << "0 | " << array[j] << "1 | " << array[k] << "2," << std::endl;
				} else {
					std::cout << "\t" << array[i] << "" << array[j] << "" << array[k] << " = " << array[i] << "0 | " << array[j] << "1 | " << array[k] << "2," << std::endl;
				}
			}
		}
	}
	std::cout << "};" << std::endl;
}
#endif
