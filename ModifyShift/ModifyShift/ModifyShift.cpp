// ModifyShift.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include "windows.h"
#include "stdio.h"

#include <fstream>
using namespace std;

#define UP_FILE_NAME _T("C:\\windows\\system32\\T6280A\\power\\UpConverter.txt")
//#define UP_FILE_NAME _T("C:\\UpConverter.txt")
int mainProc(int startFreq, int endFreq, double value)
{
	bool bStart = false;
	int nPos = 0;
	int freq = 0;
	string tmp;
	string sValue;
	char szCap[MAX_PATH];
	vector<string> m_vecFin;
	vector<string>::iterator pVecFin;
	string str;
	sprintf(szCap, "%s.bak", UP_FILE_NAME);
	CopyFile(UP_FILE_NAME, szCap, false);
	ifstream fin(UP_FILE_NAME, ifstream::in | ifstream::binary);
	if (fin == NULL)
	{
		cerr << "Error open file!" << endl;
		throw 1;
	}

	

	sprintf(szCap, "%.2f", value);
	sValue = szCap;

	
	m_vecFin.clear();

	while(getline(fin, str))
	{
		m_vecFin.push_back(str);
	}
	fin.close();


	if (m_vecFin.size() == 0)
	{
		cerr << "Error" << endl;
		throw 2;
	}
	
	bStart = false;
	for (pVecFin = m_vecFin.begin(); pVecFin < m_vecFin.end(); pVecFin++)
	{
		if (bStart == false)
		{
			if (pVecFin->find("Gshift") != pVecFin->npos)
			{
				bStart = true;
			}
		}
		else
		{
			nPos = pVecFin->find(',');
			if (nPos == pVecFin->npos)
				continue;
			tmp = pVecFin->substr(0, nPos);
			freq = atoi(tmp.c_str());
			if (freq < startFreq)
				continue;
			if (freq > endFreq)
				break;
			nPos = pVecFin->rfind(',');
			tmp = pVecFin->substr(0, nPos) + ','+ sValue;
			*pVecFin = tmp;
		}
	}

	::DeleteFile(UP_FILE_NAME);
	fstream  fin2(UP_FILE_NAME, ios::out);

	for (pVecFin = m_vecFin.begin(); pVecFin < m_vecFin.end(); pVecFin++)
	{
		fin2 << *pVecFin << endl;
	}

	fin2.close();
	


	

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int d;
	int startFreq, endFreq;
	double mValue = 0;
	if (argc < 2)
	{
		startFreq = 1920;
		endFreq = 1990;
		mValue = -0.5;
	}
	else if (argc == 4)
	{
		startFreq = atoi(argv[1]);
		endFreq = atoi(argv[2]);
		mValue = atof(argv[3]);
	}
	else
	{
		cout << "Usage: ModifyShift StartFreq EndFreq Value" << endl;
		cout << "printf anykey to exit!" << endl;
		exit(-1);
	}
	try
	{
		mainProc(startFreq, endFreq, mValue);
	}
	catch(...)
	{
		cout << "catch exception" << endl;
	}
	cout << "printf anykey to exit!" << endl;
	MessageBox(NULL, "Convert OK!", "OK", MB_OK);
	return 0;
}
