/*
 * ssr_match.h
 *
 *  Created on: Aug 17, 2015
 *      Author: jason
 */

#ifndef SSR_MATCH_H_
#define SSR_MATCH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "shadeshaperelation.h"

class Islands;
class Labels;
class ShadeShapeRelationMatch : public ShadeShapeRelation {
private:
	float matchScore;
	float mismatchScore;

	float entropy(float count);
	float contrastWeight(float esg, float relArea);
	float calculateArcScore(float score1, float score2);

	void match(ShadeShapeRelation &ssrUP, ShadeShapeRelation &ssrDB);
	float rVal[6] = {1.0,1.0,0.63,0.55,0.52,0.5};
	float shapeWeight[8] = {1.0,1.0,1.0,1.0,0.7071,0.4082,0.7071,0.25};


public:
	void srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels);
	float getMatchScore();
	float getMismatchScore();
	float getShapeWeight(int shape, float prop);

	void importDownScaleSrms(String file, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &labels);
};



#endif /* SSR_MATCH_H_ */
