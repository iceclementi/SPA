#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;


// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    try {
        SourceParser::SourceParser parser = SourceParser::SourceParser(filename);
        TNode astRoot = parser.parseProgram();
        PROC_MATRIX procMatrix = parser.getMatrix();
        DesignExtractor::DesignExtractor(astRoot, procMatrix).traverse();
    }
    catch (string& e) {
        cout << e << endl; 
        hasError = true;
    }
    catch (const char*& e) {
        cout << e << endl;
        hasError = true;
    }
    catch (...) {
        cout << "Error: An unexpected error has occurred" << endl;
        hasError = true;
    }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
    if (!hasError) {
	    QueryObject queryObject = QueryPreprocessor(query).parseQuery();
        queryObject = QueryOptimiser(queryObject).optimiseQuery();
        //queryObject = QueryOptimiser(queryObject).noOptimiseQuery();
	    results = QueryEvaluator(queryObject).evaluateQuery();
    }
    else {
        results = list<string>();
    }

    cleanUp();
}

void TestWrapper::cleanUp() {
    PKB::clearCache();
    SynonymStorageManager::clear();
}
