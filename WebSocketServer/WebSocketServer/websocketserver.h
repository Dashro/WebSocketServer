#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QtWidgets/QMainWindow>
#include "ui_websocketserver.h"
#include "Data.h"

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QMainWindow
{
	Q_OBJECT

public:
    WebSocketServer(QWidget *parent = 0);
    ~WebSocketServer();

Q_SIGNALS:
	void closed();

private Q_SLOTS:
	void onNewConnection();
	void processTextMessage(QString message);
	void processBinaryMessage(QByteArray message);
	void socketDisconnected();

	void sendData();

private:
    Ui::WebSocketServerTestClass ui;

	QWebSocketServer		*m_pWebSocketServer;
	QList<QWebSocket *>		m_clients;
	CData					*m_Data;


};

#endif // WEBSOCKETSERVERTEST_H
