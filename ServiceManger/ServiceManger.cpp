#include "ServiceManger.h"

int CServiceManger::GetAllServices(string &strOutput)
{
	int iRes = SUCESS;
	strOutput = "All the services: \n";
	{
		ServiceMap mapServiceTmp;
		{
			lock_guard<mutex> oLock(m_ServiceMapMutex);
			mapServiceTmp = m_mapAllServices;
		}
		ServiceMap ::iterator iterServices = mapServiceTmp.begin();
		while (iterServices != mapServiceTmp.end())
		{
			strOutput += "Servcice: ";
			strOutput += iterServices->first;
			strOutput += "||DisplayName: ";
			strOutput += iterServices->second->GetDispName();
			strOutput += "||State: ";
			strOutput += iterServices->second->GetState();
			strOutput += "\n";
			iterServices++;
		}
	}
	strOutput += "\n";
	return SUCESS;
}

int CServiceManger::GetOneService(string & strServiceName,
								  string &strOutput)
{
	int iRes = SUCESS;
	ServiceMap mapServiceTmp;
	{
		lock_guard<mutex> oLock(m_ServiceMapMutex);
		mapServiceTmp = m_mapAllServices;
	}
	ServiceMap ::iterator iterTarget =
		mapServiceTmp.find(strServiceName);
	if (iterTarget != mapServiceTmp.end())
	{
		iRes = iterTarget->second->GetCurrentService(strOutput);
	}
	else
	{
		iRes = INPUTDOESNOTEXIST;
	}
	return iRes;
}

int CServiceManger::StartOneService(string strServName)
{
	int iRes = SUCESS;
	ServiceMap mapServiceTmp;
	{
		lock_guard<mutex> oLock(m_ServiceMapMutex);
		mapServiceTmp = m_mapAllServices;
	}
	ServiceMap ::iterator iterTarget =
		mapServiceTmp.find(strServName);
	if (iterTarget != mapServiceTmp.end())
	{
		iRes = iterTarget->second->StartCurService(m_SCMHandle);
	}
	else
	{
		iRes = INPUTDOESNOTEXIST;
	}
	return iRes;
}

int CServiceManger::StopOneService(string strServName)
{
	int iRes = 0;
	ServiceMap mapServiceTmp;
	{
		lock_guard<mutex> oLock(m_ServiceMapMutex);
		mapServiceTmp = m_mapAllServices;
	}
	ServiceMap ::iterator iterTarget =
		mapServiceTmp.find(strServName);
	if (iterTarget != mapServiceTmp.end())
	{
		iRes = iterTarget->second->StopCurService(m_SCMHandle);
	}
	else
	{
		iRes = INPUTDOESNOTEXIST;
	}
	return iRes;
}

int CServiceManger::ChangeStartType(string strServName,
									int iNewStartType)
{
	int iRes = 0;
	ServiceMap mapServiceTmp;
	{
		lock_guard<mutex> oLock(m_ServiceMapMutex);
		mapServiceTmp = m_mapAllServices;
	}
	ServiceMap ::iterator iterTarget =
		mapServiceTmp.find(strServName);
	if (iterTarget != mapServiceTmp.end())
	{
		iRes = iterTarget->second->ChangeStartType(m_SCMHandle, iNewStartType);
	}
	else
	{
		iRes = INPUTDOESNOTEXIST;
	}
	return iRes;
}

int CServiceManger::ChangeDispName(string strServName,
								   string StrNewDispName)
{
	int iRes = 0;
	ServiceMap mapServiceTmp;
	{
		lock_guard<mutex> oLock(m_ServiceMapMutex);
		mapServiceTmp = m_mapAllServices;
	}
	ServiceMap ::iterator iterTarget =
		mapServiceTmp.find(strServName);
	if (iterTarget != mapServiceTmp.end())
	{
		iRes = iterTarget->second->ChangeDispName(m_SCMHandle, StrNewDispName);
	}
	else
	{
		iRes = INPUTDOESNOTEXIST;
	}
	return iRes;
}

int CServiceManger::ChangeDesc(string strServName,
							   string StrNewDesc)
{
	int iRes = 0;
	ServiceMap mapServiceTmp;
	{
		lock_guard<mutex> oLock(m_ServiceMapMutex);
		mapServiceTmp = m_mapAllServices;
	}
	ServiceMap ::iterator iterTarget =
		mapServiceTmp.find(strServName);
	if (iterTarget != mapServiceTmp.end())
	{
		iRes = iterTarget->second->ChangeDesc(m_SCMHandle, StrNewDesc);
	}
	else
	{
		iRes = INPUTDOESNOTEXIST;
	}
	return iRes;
}

int CServiceManger::DeleteOneService(string strServName)
{
	int iRes = 0;
	ServiceMap mapServiceTmp;
	{
		lock_guard<mutex> oLock(m_ServiceMapMutex);
		mapServiceTmp = m_mapAllServices;
	}
	ServiceMap ::iterator iterTarget =
		mapServiceTmp.find(strServName);
	if (iterTarget != mapServiceTmp.end())
	{
		iRes = iterTarget->second->DeleteCurService(m_SCMHandle);
	}
	else
	{
		iRes = INPUTDOESNOTEXIST;
	}
	return iRes;
}

int CServiceManger::CreateOneService(string strServName,
									 string strDispName,
									 int iStartType,
									 string strBinPath)
{
	int iRes = SUCESS;

	SC_HANDLE hServHandle = CreateService(m_SCMHandle,
		strServName.c_str(), strDispName.c_str(), SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS, iStartType, 
		SERVICE_ERROR_NORMAL, strBinPath.c_str(), 
		NULL, NULL, NULL, NULL, NULL);
	if (hServHandle == NULL) 
	{
		iRes = GetLastError();
	}
	CloseServiceHandle(hServHandle); 
	return iRes;
}

int CServiceManger::StopRefreshTool(string &strOutput)
{
	int iRes = SUCESS;
	m_bNeedRefresh = false;
	m_ptrRefreshThread->join();
	strOutput += "RefreshTool stopped sucessfuly\n";
	return iRes;
}

int CServiceManger::StartRefreshTool(string &strOutput)
{
	int iRes = SUCESS;
	if (m_ptrRefreshThread->joinable())
	{
		strOutput += "RefreshTool is already running\n";
	}
	else
	{
		m_bNeedRefresh = true;
		m_ptrRefreshThread->detach();
		try
		{
			m_ptrRefreshThread.reset();
			m_ptrRefreshThread = PtrThread(new 
				thread(bind(&CServiceManger::RefreshServiceMap, this)));
		}
		catch(...)
		{
			cout <<"ERROR!"<<endl;
		}
		if (m_ptrRefreshThread->joinable())
		{
			strOutput += "RefreshTool starts sucessfuly\n";
		}	
	}
	return iRes;
}

int CServiceManger::CheckRefreshtoolState(string &strOutput)
{
	int iRes = SUCESS;
	if (m_ptrRefreshThread->joinable())
	{
		strOutput += "RefreshTool is running \n";
	}
	else
	{
		strOutput += "RefreshTool stopped \n";
	}
	return iRes;
}

void CServiceManger::RefreshServiceMap()
{
	while (true && m_bNeedRefresh)
	{
		ServiceMap oFormerServiceMap = m_mapAllServices;
		ServiceMap oNewServiceMap;
		ServiceList oServiceList;
		LPENUM_SERVICE_STATUSA lpServices = NULL;

		DWORD dwBytesCount = 0;
		DWORD dwServicesCount = 0;
		LPDWORD lpResumeHandle = NULL;
		bool bRet = EnumServicesStatus(m_SCMHandle, SERVICE_WIN32, 
			SERVICE_STATE_ALL, lpServices, 0, &dwBytesCount, &dwServicesCount, lpResumeHandle);
		if (!bRet)
		{
			if (ERROR_MORE_DATA == GetLastError())
			{
				lpServices = (LPENUM_SERVICE_STATUSA )(new char[dwBytesCount + 1]);

				bRet = EnumServicesStatus(m_SCMHandle,SERVICE_WIN32, 
					SERVICE_STATE_ALL, lpServices, dwBytesCount, &dwBytesCount, &dwServicesCount, lpResumeHandle);
				if (bRet)
				{
					for (int iCount = 0; iCount < dwServicesCount; 
						iCount++)
					{
						string strServiceName = lpServices[iCount].lpServiceName;
						ServicePtr ptrCurService(new CService);
						ptrCurService->SetServName(strServiceName);
						ptrCurService->SetDispName(lpServices[iCount].lpDisplayName);
						string strServState = ConvertServState2String(lpServices[iCount].ServiceStatus.dwCurrentState);
						ptrCurService->SetState(strServState);
						
						LPQUERY_SERVICE_CONFIGA pServConfig = NULL;
						bRet = QueryServiceInfo(lpServices[iCount].lpServiceName,
							pServConfig);
						if (bRet)
						{
							ptrCurService->SetBinPath(pServConfig->lpBinaryPathName);
							ptrCurService->SetDependency(pServConfig->lpDependencies);
							ptrCurService->SetStartType(ConvertStartState2String(pServConfig->dwStartType));
							delete pServConfig;
						}
						else
						{
							delete pServConfig;
							continue;
						}

						string strTargetInfo;
						bRet = QueryServiceInfoEx(lpServices[iCount].lpServiceName, SERVICE_CONFIG_DESCRIPTION,
								strTargetInfo);
						if (bRet)
						{
							ptrCurService->SetDescription(strTargetInfo);
						}
						else
						{
							continue;
						}

						oNewServiceMap.insert(make_pair(strServiceName, ptrCurService));
					}
				}
				delete lpServices;
			}
		}

		bool bServiceChanged = false;
		ServiceMap oNewServiceMapTmp;
		if (!bFirstTime)
		{
			oNewServiceMapTmp = oNewServiceMap;
		}

		ServiceMap::iterator iterServicerMap = oFormerServiceMap.begin();
		while(iterServicerMap != oFormerServiceMap.end()
			&& !bFirstTime)
		{
			string strChanged;
			ServiceMap::iterator iterNewServ = oNewServiceMapTmp.find(iterServicerMap->first);
			if (iterNewServ == oNewServiceMapTmp.end())
			{
				strChanged += "Service: [";
				strChanged += iterServicerMap->first;
				strChanged += "]";
				strChanged += " |Display name [";
				strChanged += iterServicerMap->second->GetDispName();
				strChanged += "] Deleted\n";
				bServiceChanged = true;
				m_fsOutputToFile << strChanged;
				m_fsOutputToFile.flush();
				iterServicerMap++;
				continue;
			}
			CService oNewService = *(iterNewServ->second);
			CService oOldService = *(iterServicerMap->second);

			if (oOldService.GetChangedInfo(oNewService, strChanged))
			{
				bServiceChanged = true;
				m_fsOutputToFile << strChanged;
				m_fsOutputToFile.flush();
			}
			oNewServiceMapTmp.erase(iterNewServ);
			iterServicerMap++;
		}

		BOOST_FOREACH(PairService oServicewPair, oNewServiceMapTmp)
		{
			string strChanged;
			strChanged += "Service: [";
			strChanged += oServicewPair.first;
			strChanged += "]";
			strChanged += " |Display name [";
			strChanged += oServicewPair.second->GetDispName();
			strChanged += "] Created! \n";
			bServiceChanged = true;
			m_fsOutputToFile << strChanged;
			m_fsOutputToFile.flush();
		}

		if (bServiceChanged || bFirstTime)
		{
			lock_guard<mutex> oLock(m_ServiceMapMutex);
			m_mapAllServices.clear();
			m_mapAllServices = oNewServiceMap;
		}
		bFirstTime = false;
		oFormerServiceMap.clear();
		oNewServiceMap.clear();
		Sleep (1*500);
	}
}

//调用QueryServiceConfig2
bool CServiceManger::QueryServiceInfo(string strServName,
									  LPQUERY_SERVICE_CONFIGA &pServConfig)
{
	bool bSucess = false;
	SC_HANDLE hServHandle = OpenService(m_SCMHandle, 
			(LPCSTR)strServName.c_str(), SERVICE_ALL_ACCESS);
	if (NULL != hServHandle)
	{
		unsigned long ulByteCounts = 0;
		DWORD BytesNeeded;
		
		bSucess = QueryServiceConfig(hServHandle, pServConfig, 
			ulByteCounts, &BytesNeeded);

		if (!bSucess)
		{
			if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
			{
				ulByteCounts = BytesNeeded;
				pServConfig = (LPQUERY_SERVICE_CONFIGA)(new char[ulByteCounts + 1]);
				bSucess = QueryServiceConfig(hServHandle, pServConfig, 
					ulByteCounts, &BytesNeeded);
				bSucess  = true;
			}
		}
	}
	else
	{
		bSucess =false;
	}
	CloseServiceHandle(hServHandle);
	return bSucess;
}

//调用QueryServiceConfig2
bool CServiceManger::QueryServiceInfoEx(string strServName,
										unsigned long ulInfoLevel,
										string &strInfoEx)
{
	bool bSucess = false;
	SC_HANDLE hServHandle = OpenService(m_SCMHandle, 
		(LPCSTR)strServName.c_str(), SERVICE_ALL_ACCESS);
	if (NULL != hServHandle)
	{
		DWORD dwBytesCount = 0;
		DWORD dwBytesNeeded = NULL;
		LPSERVICE_DESCRIPTION pDescripTion = NULL;
		if (!QueryServiceConfig2(hServHandle, ulInfoLevel, 
			(LPBYTE)pDescripTion, dwBytesCount, &dwBytesNeeded))
		{
			if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
			{
				dwBytesCount = dwBytesNeeded;
				pDescripTion = (LPSERVICE_DESCRIPTION)new char[dwBytesNeeded + 1];
				bSucess = QueryServiceConfig2(hServHandle, ulInfoLevel, (LPBYTE)pDescripTion, 
					dwBytesCount, &dwBytesNeeded);
				if (bSucess && pDescripTion->lpDescription != NULL)
				{
					strInfoEx = pDescripTion->lpDescription;
				}	
				delete pDescripTion;
			}
		}
	}
	else
	{
		bSucess = false;
	}
	CloseServiceHandle(hServHandle);
	return bSucess;
}

string CServiceManger::ConvertStartState2String(unsigned long ulStartState)
{
	string strState = "Start_UnKnown";

	switch (ulStartState)
	{
		//正在恢复,由已暂停至已启动
	case SERVICE_AUTO_START:
		strState = "AUTO_START";
		break;
		//正在暂停
	case SERVICE_BOOT_START:
		strState = "BOOT_START";
		break;			
	case SERVICE_PAUSED:
		strState = "PAUSED";
		break;		
	case SERVICE_DEMAND_START:
		strState = "DEMAND_START";
		break;	
		//正在启动
	case SERVICE_DISABLED:
		strState = "DISABLED";
		break;
		//正在停止
	case SERVICE_SYSTEM_START:
		strState = "SYSTEM_START";
		break;
	default:
		strState = "Start_UnKnown";
	}
	return strState;
}

string CServiceManger::ConvertServState2String(unsigned long ulServState)
{
	string strState = "State_UnKnown";

	switch (ulServState)
	{
		//正在恢复,由已暂停至已启动
	case SERVICE_CONTINUE_PENDING:
		strState = "CONTINUE_PENDING";
		break;
		//正在暂停
	case SERVICE_PAUSE_PENDING:
		strState = "PAUSE_PENDING";
		break;			
	case SERVICE_PAUSED:
		strState = "PAUSED";
		break;		
	case SERVICE_RUNNING:
		strState = "RUNNING";
		break;	
		//正在启动
	case SERVICE_START_PENDING:
		strState = "START_PENDING";
		break;
		//正在停止
	case SERVICE_STOP_PENDING:
		strState = "STOP_PENDING";
		break;
	case SERVICE_STOPPED:
		strState = "STOPPED";
		break;
	default:
		strState = "State_UnKnown";
	}
	return strState;
}