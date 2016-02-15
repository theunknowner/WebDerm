/*
 * shadeshaperelation.h
 *
 *  Created on: Jul 16, 2015
 *      Author: jason
 */

#ifndef SHADESHAPERELATION_H_
#define SHADESHAPERELATION_H_

#include "../../global.h"
#include "../ShapeMatch/shapematch.h"
#include "srm.h"

class ShadeShape;
class Islands;
class Labels;
class ShadeShapeRelation : public ShapeMatch, public Srm {

private:
	String ssr_name;
	Srm srm;
	bool isImported;

public:
	void generate_srm(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec);
	void spatial_relation(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec, int write, String newName="");
	Srm& get_srm();
	String name();
	int getRelOpIndex(String relOp);
	void importSrmFiles(String name);
	void dbg_import_srm(bool setImport=false);

};

#endif /* SHADESHAPERELATION_H_ */
