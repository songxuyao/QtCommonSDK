#include "SLogAction.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <iostream>

QFile gLogFile;

SLogAction::SLogAction(WORD color,
	const QString& level,
	const QString& time,
	const QString& fileName,
	const QString& function,
	const QString& msg,
	qint32 fileLine,
	qint32 threadID)
	: _Level(level)
	, _Color(color)
	, _Time(time)
	, _FileName(fileName)
	, _FileLine(fileLine)
	, _Function(function)
	, _Message(msg)
	, _ThreadID(threadID)
	, _OriginColors(0)
{
	_hSTD = GetStdHandle(STD_OUTPUT_HANDLE);
}

SLogAction::~SLogAction()
{
}

void SLogAction::Execute()
{
	if (!gLogFile.isOpen())
	{
		auto now = QDateTime::currentDateTime();
		auto dir = QDir::current();
		dir.mkdir("logs");
		dir.cd("logs");

		QString qLogFileName = QString("%1.log")
			.arg(now.toString("yyyy-MM-dd-HH-mm-ss"));
		auto qLogFilePath = dir.absoluteFilePath(qLogFileName);
		gLogFile.setFileName(qLogFilePath);
		gLogFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	}

	QTextStream stm(&gLogFile);
	stm.setCodec("UTF-8");

	auto qThreadID = QString("%1").arg(_ThreadID, 5, 10, QLatin1Char('0'));
	auto qFileInfo = QString("%1:%2").arg(_FileName).arg(_FileLine);

	// 每条日志打印两行，第二行在第一行的消息内容位置开始
	auto nLabelLength = _Level.length() + qThreadID.length() + _Time.length() + 3;
	QString secondPrefix(nLabelLength, ' ');

#if ENABLE_CONSOLE
	SetConsoleColor(_Color);
	std::cout << _Level.toLocal8Bit().constData() << " "
		<< qThreadID.toLocal8Bit().constData() << " "
		<< _Time.toLocal8Bit().constData()
		<< "> ";

	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << _Message.toLocal8Bit().constData();
	std::cout << std::endl;
	std::cout << secondPrefix.toLocal8Bit().constData();

	SetConsoleColor(FOREGROUND_BLUE);

	std::cout << "(" << qFileInfo.toLocal8Bit().constData();
	if (!_Function.isEmpty())
	{
		std::cout << ":" << _Function.toLocal8Bit().constData();
	}

	std::cout << ")" << std::endl;
	RestoreConsoleAttribute();
#endif

#if ENABLE_FILE
	stm << _Level << " "
		<< qThreadID << " "
		<< _Time << "> "
		<< _Message << Qt::endl
		<< secondPrefix.toLocal8Bit().constData()
		<< "(" << qFileInfo;
	if (!_Function.isEmpty())
	{
		stm << ":" << _Function;
	}
	stm << ")" << Qt::endl;
	stm.flush();
#endif

#if ENABLE_DEBUGVIEW
#ifdef _UNICODE
	TCHAR tszLog[1024] = { 0 };
	_Message.toWCharArray(tszLog);
	OutputDebugString(tszLog);
#else
	OutputDebugString(_Message.toLocal8Bit().constData());
#endif
#endif
}

void SLogAction::SetConsoleColor(WORD nColor)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SecureZeroMemory(&csbi, sizeof(CONSOLE_SCREEN_BUFFER_INFO));

	/* Save the original console color */
	GetConsoleScreenBufferInfo(_hSTD, &csbi);
	_OriginColors = *(&csbi.wAttributes);

	SetConsoleTextAttribute(_hSTD, nColor);
}

void SLogAction::RestoreConsoleAttribute()
{
	SetConsoleTextAttribute(_hSTD, _OriginColors);
}

