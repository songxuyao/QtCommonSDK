#pragma once

#include "global.h"
#include "ntdll/ntdll.h"

class SLastError
{
public:
	SLastError();
	SLastError(NTSTATUS status);
	~SLastError();

	operator DWORD();

	// 
	// ת�����ַ���
	//
	QString ToQString();
	char* ToString();

public:
	DWORD   ErrorCode;
	QString ErrorDetail;
};

