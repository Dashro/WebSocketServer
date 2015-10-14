#include "websocketserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include "qtimer.h"
#include "qjsondocument.h"


WebSocketServer::WebSocketServer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_Data = new CData;
	m_Data->openDatafile("C:/Users/Fabian/Documents/SolarDataTest/DataFile.csv");

	quint16 port = 80;

	m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Data Server"),
		QWebSocketServer::NonSecureMode, this);

	if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
		qDebug() << "Waiting for conncetion";
		ui.label_Adresse->setText(m_pWebSocketServer->serverUrl().toString());
		ui.label_Status->setText("Waiting for Connection");
		connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
			this, &WebSocketServer::onNewConnection);
		connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
	}
	

}

WebSocketServer::~WebSocketServer()
{
	qDebug() << "~";
}

void WebSocketServer::sendData()
{
	static int row = 0;
	//if (!row < m_Data->getTimeStamps().size())
	//	row = 0;

	for (int i = 0; i < m_clients.size(); i++)
	{
		QJsonObject dataScript;
		QStringList header = m_Data->getheaderList();

		dataScript["Name"] = "Data-Skript";
		QString test = m_Data->getTimeStamps().at(row);
		dataScript["TimeStamp"] = m_Data->getTimeStamps().at(row);
		dataScript["Einspeisung"] = m_Data->getCollum(header.indexOf("Einspeisung\n")).at(row);
		dataScript["Bezug"] = m_Data->getCollum(header.indexOf("Bezug")).at(row);
		dataScript["ETotal"] = m_Data->getCollum(header.indexOf("ETotal")).at(row);

		m_clients.at(i)->sendBinaryMessage(QJsonDocument(dataScript).toBinaryData());
	}
	row++;
}

void WebSocketServer::onNewConnection()
{
	qDebug() << "new connection";
	ui.label_Status->setText("Socked Connected");
	QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

	connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
	connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
	connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

	QJsonObject initialScript;

	initialScript["Name"] = "Initial-Skript";
	initialScript["TimeStamp"] = "dd/MM/yyyy HH:mm:ss";
	initialScript["Einspeisung"] = "Wh";
	initialScript["Bezug"] = "Wh";
	initialScript["ETotal"] = "kWh";

	pSocket->sendBinaryMessage(QJsonDocument(initialScript).toBinaryData());

	m_clients << pSocket;


	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(sendData()));
	timer->start(1000);
}

void WebSocketServer::processTextMessage(QString message)
{
	qDebug() << "process Text Message";
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
}

void WebSocketServer::processBinaryMessage(QByteArray message)
{
	qDebug() << "process Binary Message";
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	if (pClient) {
		pClient->sendBinaryMessage(message);
	}
}

void WebSocketServer::socketDisconnected()
{
	qDebug() << "socket Disconnected";
	ui.label_Status->setText("Socket Disconnected");
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	if (pClient) {
		m_clients.removeAll(pClient);
		pClient->deleteLater();
	}
}