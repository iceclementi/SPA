#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include "Common.h"
#include "SourceParser.h"
#include "DesignExtractor.h"
#include "QueryPreprocessor.h"
#include "QueryOptimiser.h"
#include "QueryEvaluator.h"
#include "AbstractWrapper.h"

using namespace std;

class TestWrapper : public AbstractWrapper {
 public:
	bool hasError = false;

	// default constructor
	TestWrapper();
  
	// destructor
	~TestWrapper();
  
	// method for parsing the SIMPLE source
	virtual void parse(string filename);
  
	// method for evaluating a query
	virtual void evaluate(string query, list<string>& results);

	virtual void cleanUp();
};

#endif
