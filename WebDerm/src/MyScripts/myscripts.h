/*
 * myscripts.h
 *
 *  Created on: Apr 24, 2015
 *      Author: jason
 */

#ifndef MYSCRIPTS_H_
#define MYSCRIPTS_H_

#include "../global.h"
#include "../ShadeShape/shadeshape.h"

class ShadeShape;
namespace Scripts {

Mat colorNormalizationScript(String filename);
Mat getSkinScript(String filename);
ShadeShape discreteShadeShapeScript(String filename);

void script1();
ShadeShape script31(String filename);
void script32(String filename);
void script33(String filename);

void script_createTrainingLabels();
void script_createAllTrainingLabels();
void script_createAllTrainingLabels2(String shape);
void script_createAllTrainingLabels3();
void script_createAllTrainingLabels3b(String shape);
void checkAllTestData();
void checkAllTestData2();
void checkAllTestData3();
void script_createTestDataList();
void script_checkHitRatioTestData();

void test_statsign_script();

}
#endif /* MYSCRIPTS_H_ */
