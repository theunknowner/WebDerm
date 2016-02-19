/*
 * ssr_match.h
 *
 *  Created on: Aug 17, 2015
 *      Author: jason
 */

#ifndef SSR_MATCH_H_
#define SSR_MATCH_H_

#include "../../global.h"
#include "shadeshaperelation.h"
#include "../ShapeMatch/shapematch.h"
#include "../../PrintStream/printstream.h"

class Islands;
class Labels;
class PrintStream;
class ShadeShapeRelationMatch : public ShadeShapeRelation, public ShapeMatch {
private:
	float matchScore;
	float mismatchScore;

	float entropy(float count);
	double contrastWeight(double esg, double relArea);
	float calculateArcScore(float score1, float score2);

	void match(ShadeShapeRelation &ssrUP, ShadeShapeRelation &ssrDB, String nStr="");
	float rVal[6] = {1.0,1.0,0.63,0.55,0.52,0.5};

	PrintStream esgPS;

public:
	void srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels, String nStr="");
	float getMatchScore();
	float getMismatchScore();
	float getShapeWeight(int shape, float prop);

	PrintStream& getEsgPrintStream();

	void importDownScaleSrms(String file, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &labels);
};



#endif /* SSR_MATCH_H_ */
