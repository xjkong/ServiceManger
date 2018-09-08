#include"Service.h"

string CService::GetServName()
{
	return m_strServName;
}
string CService::GetDispName()
{
	return m_strDispName;
}
string CService::GetDescription()
{
	return m_strDescription;
}
string CService::GetBinPath()
{
	return m_strBinPath;
}
string CService::GetStartType()
{
	return m_strStartType;
}
string CService::GetState()
{
	return m_strState;
}
string CService::GetDependency()
{
	return m_strDependency;
}

void CService::SetServName(string strServName)
{
	m_strServName = strServName;
}
void CService::SetDispName(string strDispName)
{
	m_strDispName = strDispName;
}
void CService::SetDescription(string strDescription)
{
	m_strDescription = strDescription;
}
void CService::SetBinPath(string strBinPath)
{
	m_strBinPath = strBinPath;
}
void CService::SetStartType(string strStartType)
{
	m_strStartType = strStartType;
}
void CService::SetState(string strState)
{
	m_strState = strState;
}
void CService::SetDependency(string strDependency)
{
	m_strDependency = strDependency;
}

bool CService::GetChangedInfo(CService &oNewService,
							  string& strChangedInfo)
{
	if (ServiceEqual(oNewService))
	{
		return false;
	}
	int i = oNewService.GetDispName().length();
	strChangedInfo = "Service: [";
	strChangedInfo += m_strServName;
	strChangedInfo += "] ";

	strChangedInfo += "Display name: [";
	strChangedInfo += oNewService.GetDispName();
	strChangedInfo += "] ";

	if (m_strDispName != oNewService.GetDispName())
	{
		strChangedInfo += "|DisplayName changes to ";
		strChangedInfo += oNewService.GetDispName() ;
		strChangedInfo += "; ";
	}
	if (m_strBinPath != oNewService.GetBinPath())
	{
		strChangedInfo += "|BinPath changes to " ;
		strChangedInfo += oNewService.GetBinPath();
		strChangedInfo += "; ";
	}
	if (m_strDescription != oNewService.GetDescription())
	{
		strChangedInfo += "|Description changes to ";
		strChangedInfo += oNewService.GetDescription();
		strChangedInfo += "; ";
	}
	if (m_strState != oNewService.GetState())
	{
		strChangedInfo += "|State changes to ";
		strChangedInfo += oNewService.GetState();
		strChangedInfo += "; ";
	}
	if (m_strStartType != oNewService.GetStartType())
	{
		strChangedInfo += "|StartType changes to ";
		strChangedInfo += oNewService.GetStartType();
		strChangedInfo += "; ";
	}
	if (m_strDependency != oNewService.GetDependency())
	{
		strChangedInfo += "|Dependency changes to ";
		strChangedInfo += oNewService.GetDependency();
		strChangedInfo += "; ";
	}
	strChangedInfo += "\n";
	return true;
}

bool CService::ServiceEqual(CService &oNewService)
{
	bool Changed = true;
	Changed &= (m_strServName == oNewService.GetServName());
	Changed &= (m_strDispName == oNewService.GetDispName());
	Changed &= (m_strBinPath == oNewService.GetBinPath());
	Changed &= (m_strDependency == oNewService.GetDependency());
	Changed &= (m_strDescription == oNewService.GetDescription());
	Changed &= (m_strStartType == oNewService.GetStartType());
	Changed &= (m_strState == oNewService.GetState());
	return Changed;
}

int CService::GetCurrentService(string &strOutput)
{
	int iRes = SUCESS;
	strOutput = "\n+++++++++++++++++++++++++++++\n";
	strOutput += "Service: ";
	strOutput += m_strServName;
	strOutput += " ";
	strOutput += "\nDisplay Name is: ";
	strOutput += GetDispName();
	strOutput += "\nState is:        ";
	strOutput += GetState();
	strOutput += "\nStartType is:    ";
	strOutput += GetStartType();
	strOutput += "\nDescription is:  ";
	strOutput += GetDescription();
	strOutput += "\nBinPath is:      ";
	strOutput += GetBinPath();
	strOutput += "\nDependency is:   ";
	strOutput += GetDependency();
	strOutput += "\n+++++++++++++++++++++++++++++\n";
	return iRes;
}

int CService::StartCurService(SC_HANDLE hSCMHandle)
{
	int iRes = SUCESS;
	if ("RUNNING" == GetState()
		|| "START_PENDING" == GetState())
	{
		return iRes;
	}

	SC_HANDLE hServHandle = OpenService(hSCMHandle, 
		(LPCSTR)m_strServName.c_str(), SERVICE_ALL_ACCESS);
	if (hServHandle)
	{
		if (!StartService(hServHandle, NULL, NULL))
		{
			iRes = GetLastError();
		}
	}
	else
	{
		iRes = GetLastError();
	}
	CloseServiceHandle(hServHandle);
	return iRes;
}

int CService::StopCurService(SC_HANDLE hSCMHandle)
{
	int iRes = 0;

	if ("STOPPED" == GetState()
		|| "STOP_PENDING" == GetState())
	{
		return iRes;
	}

	SC_HANDLE hServHandle = OpenService(hSCMHandle, 
		(LPCSTR)m_strServName.c_str(), SERVICE_ALL_ACCESS);
	if (hServHandle)
	{
		SERVICE_STATUS oServiceState; 
		if (!ControlService(hServHandle, SERVICE_CONTROL_STOP,
			&oServiceState))
		{
			iRes= GetLastError();
		}
	}
	else
	{
		iRes = GetLastError();
	}
	CloseServiceHandle(hServHandle);
	return iRes;
}

int CService::ChangeStartType(SC_HANDLE hSCMHandle,
							  int iNewStartType)
{
	int iRes = 0;
	StopCurService(hSCMHandle);

	SC_HANDLE hServHandle = OpenService(hSCMHandle, 
		(LPCSTR)m_strServName.c_str(), SERVICE_ALL_ACCESS);
	if (hServHandle)
	{
		if (!ChangeServiceConfig(hServHandle, SERVICE_WIN32_OWN_PROCESS,
			iNewStartType, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL))
		{
			iRes = GetLastError();
		}
	}
	else
	{
		iRes = GetLastError();
	}
	CloseServiceHandle(hServHandle);
	return iRes;
}

int CService::ChangeDispName(SC_HANDLE hSCMHandle,
							 string StrNewDispName)
{
	int iRes = 0;
	StopCurService(hSCMHandle);

	SC_HANDLE hServHandle = OpenService(hSCMHandle, 
		(LPCSTR)m_strServName.c_str(), SERVICE_ALL_ACCESS);
	if (hServHandle)
	{
		if (!ChangeServiceConfig(hServHandle, SERVICE_NO_CHANGE ,
			SERVICE_NO_CHANGE , NULL, NULL, NULL, NULL, NULL, NULL, NULL, StrNewDispName.c_str()))
		{
			iRes = GetLastError();
		}
	}
	else
	{
		iRes = GetLastError();
	}
	CloseServiceHandle(hServHandle);
	return iRes;
}

int CService::ChangeDesc(SC_HANDLE hSCMHandle,
						 string StrNewDesc)
{
	int iRes = 0;
	StopCurService(hSCMHandle);

	SC_HANDLE hServHandle = OpenService(hSCMHandle, 
		(LPCSTR)m_strServName.c_str(), SERVICE_ALL_ACCESS);
	if (hServHandle)
	{
		SERVICE_DESCRIPTION  oDescription;
		oDescription.lpDescription = (LPSTR)StrNewDesc.c_str();
		if (!ChangeServiceConfig2(hServHandle, SERVICE_CONFIG_DESCRIPTION,
			&oDescription))
		{
			iRes = GetLastError();
		}
	}
	else
	{
		iRes = GetLastError();
	}
	CloseServiceHandle(hServHandle);
	return iRes;
}

int CService::DeleteCurService(SC_HANDLE hSCMHandle)
{
	int iRes = 0;
	StopCurService(hSCMHandle);

	SC_HANDLE hServHandle = OpenService(hSCMHandle, 
		(LPCSTR)m_strServName.c_str(), SERVICE_ALL_ACCESS);
	if (hServHandle)
	{
		DeleteService(hServHandle);
		{
			iRes = GetLastError();
		}
	}
	else
	{
		iRes = GetLastError();
	}
	CloseServiceHandle(hServHandle);
	return iRes;
}