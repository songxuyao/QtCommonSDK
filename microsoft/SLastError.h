#pragma once

#include "global.h"

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

