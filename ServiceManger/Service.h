#include "Public.h"

class CService 
{
private:
	//服务名称
	string m_strServName;
	//服务显示名称
	string m_strDispName;
	//服务描述信息
	string m_strDescription;
	//服务执行文件路径
	string m_strBinPath;
	//服务启动类型
	string m_strStartType;
	//服务状态
	string m_strState;
	//服务依赖项
	string m_strDependency;
public:
	
	CService(
		string strServName = "", string strDispName = "",
		string strBinPath = "", string strDescription = "",
		string strStartType = "", string strDependency = "")
	{
		m_strServName = strServName;
		m_strDispName = strDispName;
		m_strDescription = strDescription;
		m_strBinPath = strBinPath;
		m_strStartType = strStartType;
		m_strDependency = strDependency;
	}
	~CService()
	{
	}
	string GetServName();
	string GetDispName();
	string GetDescription();
	string GetBinPath();
	string GetStartType();
	string GetState();
	string GetDependency();

	void SetServName(string strDispName);
	void SetDispName(string strDispName);
	void SetDescription(string strDescription);
	void SetBinPath(string strDispName);
	void SetStartType(string strStartType);
	void SetState(string strState);
	void SetDependency(string strDependency);
	bool GetChangedInfo(CService &oNewService, string& strChangedInfo);
	bool ServiceEqual(CService &oNewService);

	int GetCurrentService(string &strOutput);
	int StopCurService(SC_HANDLE hSCMHandle);
	int StartCurService(SC_HANDLE hSCMHandle);
	int ChangeStartType(SC_HANDLE hSCMHandle, int iNewStartType);
	int ChangeDispName(SC_HANDLE hSCMHandle, string StrNewDispName);
	int ChangeDesc(SC_HANDLE hSCMHandle, string StrNewDesc);
	int DeleteCurService(SC_HANDLE hSCMHandle);
	int CreateService();
};

typedef boost::shared_ptr<CService>  ServicePtr;
typedef vector<ServicePtr> ServiceList;
typedef map<string, ServicePtr> ServiceMap;
typedef pair<string, ServicePtr> PairService;