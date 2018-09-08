#include "ServiceManger.h"

class Operators
{
private:
	vector<string> m_vecOperator;
	CServiceManger m_oServiceManger;
public:
	Operators();
	void Run();
	void PrintOperators();
	void PrintResults();
	int GetAllServices(string &strOutput);
	int GetOneService(string &strOutput);
	int StartOneService(string &strOutput);
	int StopOneService(string &strOutput);
	int ChangeStartType(string &strOutput);
	int ChangeDispName(string &strOutput);
	int ChangeDesc(string &strOutput);
	int DeleteOneService(string &strOutput);
	int CreateOneService(string &strOutput);
	int StopRefreshTool(string &strOutput);
	int StartRefreshTool(string &strOutput);
	int CheckRefreshtoolState(string &strOutput);
};

