/*
 * myscripts.h
 *
 *  Created on: Apr 24, 2015
 *      Author: jason
 */

#ifndef MYSCRIPTS_H_
#define MYSCRIPTS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/src/ShadeShape/shadeshape.h"

class ShadeShape;
namespace Scripts {
int getPeakClusters(vector<double> &data_vec);

void script1();
ShadeShape script2(String name);
void script3();
void script4();
void script5();
void script6();
void script7();
void script8();
void script9();
void script10();
void script11();
void script12();
void script13();
void script14();
void script15();
void script16();
void script17();
void script18();
void script19();
void script20();
void script21();
void script22();
void script23();
void script24();
void script25();
void script26();
void script27(String name);
void script28a();
void script28b();
void script29();
void script30(String name);
ShadeShape script31(String filename);

void script_createTrainingLabels();
void script_createAllTrainingLabels();
void script_createAllTrainingLabels2();
void checkAllTestData();
void checkAllTestData2();
void script_createTestDataList();
void script_checkHitRatioTestData();

}
#endif /* MYSCRIPTS_H_ */
