#include "Service.h"

class CServiceManger
{
private:
	SC_HANDLE m_SCMHandle;
	ServiceMap m_mapAllServices;
	PtrThread m_ptrRefreshThread;
	mutex m_ServiceMapMutex;
	ofstream m_fsOutputToFile;
	bool m_bNeedRefresh;
	bool bFirstTime;
public:
	CServiceManger()
	{
		m_bNeedRefresh = true;
		bFirstTime = true;
		setlocale(LC_ALL,"Chinese-simplified");
		char sCurPath[500];
		GetCurrentDirectory(500, sCurPath);
		string strCurPath = sCurPath;
		strCurPath += "\\ServiceChange.txt";
		m_fsOutputToFile.open(strCurPath.c_str());
		m_SCMHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		m_ptrRefreshThread = PtrThread(
			new thread(bind(&CServiceManger::RefreshServiceMap, this)));

		setlocale(LC_ALL,"C");
	}
	~CServiceManger()
	{
		CloseServiceHandle(m_SCMHandle);
		m_fsOutputToFile.close();
	}

	int GetAllServices(string &strOutput);
	int GetOneService(string & strServiceName, string &strOutput);
	int StartOneService(string strServName);
	int StopOneService(string strServName);
	int ChangeStartType(string strServName,
		int iTargetStartType);
	int ChangeDispName(string strServName,
		string StrNewDispName);
	int ChangeDesc(string strServName,
		string StrNewDescription);
	int DeleteOneService(string strServName);

	int CreateOneService(string strServName,
		string strDispName,
		int iStartType,
		string strBinPath);

	int StopRefreshTool(string &strOutput);
	int StartRefreshTool(string &strOutput);
	int CheckRefreshtoolState(string &strOutput);
	void RefreshServiceMap();

	//调用QueryServiceConfig
	bool QueryServiceInfo(string strServName,
		LPQUERY_SERVICE_CONFIGA &pServConfig);

	//调用QueryServiceConfig2
	bool CServiceManger::QueryServiceInfoEx(string strServName,
		unsigned long ulInfoLevel,
		string &strInfoEx);
	string ConvertServState2String(unsigned long ulServState);
	string ConvertStartState2String(unsigned long ulStartState);
};