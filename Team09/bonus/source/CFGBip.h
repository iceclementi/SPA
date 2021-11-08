#include "Common.h"

class CFGBip {
public:
	enum class NODE_TYPE {
		BASIC, WHILE, IF, IF_END, CALL
	};

	CFGBip();
	CFGBip(PROG_LINE line);
	CFGBip(PROG_LINE progLine, NODE_TYPE nodeType);

	bool addChildren(CFGBip &node);
	bool addDummyParents(CFGBip* node);

	bool isDummy();
	bool isBasic();
	bool isWhile();
	bool isIf();
	bool isIfEnd();
	bool isCall();
	bool areEqual(CFGBip node);
	void setType(NODE_TYPE nodeType);

	vector<CFGBip>& getChildren();
	vector<CFGBip*> getDummyParents();
	PROG_LINE getProgLine();
	CFGBip& getSmallNext();
	CFGBip& getLargeNext();

private:
	PROG_LINE progLine;

	NODE_TYPE nodeType;
	vector<CFGBip> children;
	vector<CFGBip*> dummyParents;
};