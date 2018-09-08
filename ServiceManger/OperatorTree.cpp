#include "OperatorTree.h"

Operators::Operators()
{
	m_vecOperator.push_back("1. Get All Services:\n");
	m_vecOperator.push_back("2. Get one Service:\n");
	m_vecOperator.push_back("3. Start one Service:\n");
	m_vecOperator.push_back("4. Stop one Service:\n");
	m_vecOperator.push_back("5. Change Service Start Type:\n");
	m_vecOperator.push_back("6. Change Service DispName:\n");
	m_vecOperator.push_back("7. Change Service Description:\n");
	m_vecOperator.push_back("8. Create one Service:\n");
	m_vecOperator.push_back("9. Delete one Service:\n");
	m_vecOperator.push_back("10. Close the refresh tool:\n");
	m_vecOperator.push_back("11. Start the refresh tool:\n");
	m_vecOperator.push_back("12. Get the refresh tool state:\n");
	//m_vecOperator.push_back("10. Change Service Dependency:\n");
}

void Operators::Run()
{
	int iCommand;
	PrintOperators();
	while (cin >> iCommand)
	{
		if (iCommand == -1)
		{
			break;
		}
		std::system("cls");
		string strOutputs;
		int iRes = -1;
		switch (iCommand)
		{
		case 1 :
			iRes = GetAllServices(strOutputs);
			break;
		case 2 :
			iRes = GetOneService(strOutputs);
			break;
		case 3 :
			iRes = StartOneService(strOutputs);
			break;
		case 4 :
			iRes = StopOneService(strOutputs);
			break;
		case 5 :
			iRes = ChangeStartType(strOutputs);
			break;
		case 6 :
			iRes = ChangeDispName(strOutputs);
			break;
		case 7 :
			iRes = ChangeDesc(strOutputs);
			break;
		case 8 :
			iRes = CreateOneService(strOutputs);
			break;
		case 9 :
			iRes = DeleteOneService(strOutputs);
			break;
		case 10 :
			iRes = StopRefreshTool(strOutputs);
			break;
		case 11 :
			iRes = StartRefreshTool(strOutputs);
			break;
		case 12 :
			iRes = CheckRefreshtoolState(strOutputs);
			break;
		default:
			break;
		}
		if (iRes == SUCESS)
		{
			cout << strOutputs << "	Sucess" <<endl;
		}
		else if (iRes == INPUTDOESNOTEXIST) 
		{
			cout << "Input does not exist" << endl;
		}
		else
		{
			cout << "Error code: " << iRes << endl;
		}
		PrintOperators();
	}
}

void Operators::PrintOperators()
{
	BOOST_FOREACH(string strTmp, m_vecOperator)
	{
		cout<< strTmp;
	}
	cout<< "Waiting for input...\n";
}

void Operators::PrintResults()
{

}

int Operators::GetAllServices(string &strOutput)
{
	int iRes;
	cout << "GetAllServices:\n";
	iRes = m_oServiceManger.GetAllServices(strOutput);
	return iRes;
}

int Operators::GetOneService(string &strOutput)
{

	int iRes;
	string strServiceName;
	cout << "GetOneService:\n";
	cout << "Input the service name:\n";
	cin >>strServiceName;
	strOutput = "\nGet Service ";
	strOutput += strServiceName;
	strOutput += ": ";

	iRes = m_oServiceManger.GetOneService(
		strServiceName, strOutput);
	return iRes;
}

int Operators::StartOneService(string &strOutput)
{
	int iRes;
	string strServiceName;
	cout << "StartOneService:\n";
	cout << "Input the service name:\n";
	cin >>strServiceName;
	strOutput = "\nStart Service ";
	strOutput += strServiceName;
	strOutput += ": ";
	iRes = m_oServiceManger.StartOneService(strServiceName);
	return iRes;
}

int Operators::StopOneService(string &strOutput)
{
	int iRes;
	string strServiceName;
	cout << "StopOneService:\n";
	cout << "Input the service name:\n";
	cin >>strServiceName;
	strOutput = "\nStop Service ";
	strOutput += strServiceName;
	strOutput += ": ";
	iRes = m_oServiceManger.StopOneService(strServiceName);
	return iRes;
}
int Operators::ChangeStartType(string &strOutput)
{
	int iRes;
	string strServiceName;
	cout << "ChangeStartType:\n";
	cout << "Input the service name:\n";
	cin >>strServiceName;
	strOutput = "\nChange Service ";
	strOutput += strServiceName;
	strOutput += " start type: ";

	int iStartType;
	cout << "Input the new start type\n";
	cout << "2-AUTO_START, 3-DEMAND_START, 4-DISABLED\n";
	cin >> iStartType;

	iRes = m_oServiceManger.ChangeStartType(strServiceName,
		iStartType);
	return iRes;
}
int Operators::ChangeDispName(string &strOutput)
{
	int iRes;
	string strServiceName;
	cout << "ChangeDispName:\n";
	cout << "Input the service name:\n";
	cin >>strServiceName;
	strOutput = "\nChange DispName ";
	strOutput += strServiceName;
	strOutput += ": ";

	string strNewDispName;
	cout << "Input the new DispName\n";
	cin >> strNewDispName;

	iRes = m_oServiceManger.ChangeDispName(strServiceName,
		strNewDispName);
	return iRes;
}

int Operators::ChangeDesc(string &strOutput)
{
	int iRes;
	string strServiceName;
	cout << "ChangeDescription:\n";
	cout << "Input the service name:\n";
	cin >>strServiceName;
	strOutput = "\nChange Description ";
	strOutput += strServiceName;
	strOutput += ": ";

	string strNewDescription;
	cout << "Input the new Description\n";
	cin >> strNewDescription;

	iRes = m_oServiceManger.ChangeDesc(strServiceName,
		strNewDescription);
	return iRes;
}

int Operators::DeleteOneService(string &strOutput)
{
	int iRes;
	string strServiceName;
	cout << "DeleteOneService:\n";
	cout << "Input the service name:\n";
	cin >>strServiceName;
	strOutput = "\nDelete the service ";
	strOutput += strServiceName;
	strOutput += ": ";

	iRes = m_oServiceManger.DeleteOneService(strServiceName);
	return iRes;
}

int Operators::CreateOneService(string &strOutput)
{
	int iRes;
	string strServiceName;
	string strDispName;
	string strBinPath;
	int iStartType;
	cout << "CreateOneService:\n";

	cout << "Input the service name:\n";
	cin >>strServiceName;

	cout << "Input the service display  name:\n";
	cin >>strDispName;

	cout << "Input the new start type\n";
	cout << "2-AUTO_START, 3-DEMAND_START, 4-DISABLED\n";
	cin >> iStartType;

	cin.ignore();
	cout << "Input the bin path:\n";
	char sBinPath[500];
	cin.getline(sBinPath, 500, '\n');
	strBinPath = sBinPath;

	strOutput = "\nCreate the service ";
	strOutput += strServiceName;
	strOutput += ": ";

	iRes = m_oServiceManger.CreateOneService(strServiceName,
		strDispName, iStartType, strBinPath);
	return iRes;
}

int Operators::StopRefreshTool(string &strOutput)
{
	int iRes;
	iRes = m_oServiceManger.StopRefreshTool(strOutput);
	return iRes;
}

int Operators::StartRefreshTool(string &strOutput)
{
	int iRes;
	iRes = m_oServiceManger.StartRefreshTool(strOutput);
	return iRes;
}

int Operators::CheckRefreshtoolState(string &strOutput)
{
	int iRes;
	iRes = m_oServiceManger.CheckRefreshtoolState(strOutput);
	return iRes;
}