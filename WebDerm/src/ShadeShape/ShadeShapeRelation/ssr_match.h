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
	Labels mergeLabels(Labels &labels);
	pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> downScaleSrm(ShadeShapeRelation &ssr, vector<vector<vector<int> > > &srm,Labels &labels, Labels &mergedLabels);
	float entropy(int count);
	float entropy(vector<vector<vector<vector<int> > > > srmCountUP,vector<vector<vector<vector<int> > > > srmAreaUP, Labels &upLabels, vector<vector<vector<vector<int> > > > srmCountDB, vector<vector<vector<vector<int> > > > srmAreaDB, Labels &dbLabels);
	float entropy(pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> &srmPairUP, Labels &upLabels, pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> &srmPairDB, Labels &dbLabels);
	float rVal[6] = {1.0,1.0,0.63,0.55,0.52,0.5};

public:
	float srm_match(pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> &srmPairUP, Labels &upMergedLabels, pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> &srmPairDB, Labels &dbMergedLabels);
	float srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels);
	void writeDownScaleSrm(String name, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &mergedLabels, int totalArea);
	void printDownScaleSrm(pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &mergedLabels);
	void importDownScaleSrms(String file, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &labels);
};



#endif /* SSR_MATCH_H_ */
