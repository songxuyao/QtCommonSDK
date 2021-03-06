#pragma once

#include <QThread>
#include <QQueue>
#include <QSemaphore>
#include <QtDebug>

#include "SLogAction.h"

class CustomLogMessageHandler : public QThread
{
public:
	static CustomLogMessageHandler& Initialize();

	static void handle(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
	
	void Stop();

protected:
	void run() override;

private:
	CustomLogMessageHandler();

	QSemaphore _ExitSemaphore;
};
