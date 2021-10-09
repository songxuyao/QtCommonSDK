#include "SLastError.h"

SLastError::SLastError()
{
	ErrorCode = GetLastError();
}

SLastError::SLastError(NTSTATUS status)
{
	ErrorCode = RtlNtStatusToDosError(status);
}

SLastError::~SLastError()
{
}

SLastError::operator DWORD()
{
	return ErrorCode;
}

QString SLastError::ToQString()
{
	LPTSTR lpBuffer;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpBuffer,
		0, NULL);

#ifdef UNICODE
	ErrorDetail = QString::fromWCharArray(lpBuffer);
#else
	ErrorDetail = QString::fromLocal8Bit(lpBuffer);
#endif

	return ErrorDetail;
}

char* SLastError::ToString()
{
	ToQString();
	return ErrorDetail.toUtf8().data();
}
