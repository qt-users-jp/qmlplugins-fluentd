/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "qfluentd.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QPointer>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QLocalSocket>

class QFluentd::Private : public QObject
{
    Q_OBJECT
public:
    Private(QFluentd *parent);

    bool send(const QString &tag, const QJsonObject &value);

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void stateChanged(QLocalSocket::LocalSocketState socketState);

private:
    QFluentd *q;
    QPointer<QIODevice> connection;

public:
    ConnectionState connectionState;
    ConnectionType connectionType;
    QString host;
    int port;
    QString socket;
};

QFluentd::Private::Private(QFluentd *parent)
    : QObject(parent)
    , q(parent)
    , connectionState(UnconnectedState)
    , connectionType(TcpSocket)
    , host(QLatin1String("127.0.0.1"))
    , port(24224)
    , socket(QLatin1String("/var/run/fluent/fluent.sock"))
{
}

bool QFluentd::Private::send(const QString &tag, const QJsonObject &value)
{
    if (connection.isNull()) {
        switch (connectionType) {
        case UnixDomainSocket: {
            QLocalSocket *s = new QLocalSocket(this);
            connect(s, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(stateChanged(QLocalSocket::LocalSocketState)));
            s->connectToServer(socket);
            if (s->waitForConnected()) {
                connect(s, SIGNAL(disconnected()), s, SLOT(deleteLater()));
                connection = s;
            } else {
                qWarning() << s->errorString() << s;
                s->deleteLater();
                return false;
            }
            break; }
        case TcpSocket: {
            QTcpSocket *s = new QTcpSocket(this);
            connect(s, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
            s->connectToHost(host, port);
            if (s->waitForConnected()) {
                connect(s, SIGNAL(disconnected()), s, SLOT(deleteLater()));
                connection = s;
            } else {
                qWarning() << s->errorString() << s;
                s->deleteLater();
                return false;
            }
            break; }
        }
    }

    if (!connection) {
        return false;
    }
#if 0
    // time sent like 1.35959e+09 which doesn't work
    QJsonArray array;
    array.push_back(tag);
    array.push_back(static_cast<int>(QDateTime::currentDateTime().toTime_t()));
    array.push_back(value);
    QJsonDocument json(array);
    QByteArray data = json.toJson();
#else
    QJsonDocument json(value);
    QByteArray data;
    data.append("[\"");
    data.append(tag.toUtf8());
    data.append("\", ");
    data.append(QByteArray::number(static_cast<double>(QDateTime::currentDateTime().toTime_t()), 'f'));
    data.append(", ");
    data.append(json.toJson());
    data.append(" ]");
#endif
    connection->write(data);
    return true;
}

void QFluentd::Private::stateChanged(QAbstractSocket::SocketState socketState)
{
    q->setConnectionState(static_cast<QFluentd::ConnectionState>(socketState));
}

void QFluentd::Private::stateChanged(QLocalSocket::LocalSocketState socketState)
{
    q->setConnectionState(static_cast<QFluentd::ConnectionState>(socketState));
}

QFluentd::QFluentd(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

bool QFluentd::send(const QString &tag, const QJsonObject &value)
{
    return d->send(tag, value);
}

#define PROPERTY_IMPL(type, name, Name) \
    type QFluentd::name() const \
{ \
    return d->name; \
    } \
    \
    void QFluentd::set##Name(type name) \
{ \
    if (d->name == name) return; \
    d->name = name; \
    emit name##Changed(name); \
    }

PROPERTY_IMPL(QFluentd::ConnectionState, connectionState, ConnectionState)
PROPERTY_IMPL(QFluentd::ConnectionType, connectionType, ConnectionType)
PROPERTY_IMPL(const QString &, host, Host)
PROPERTY_IMPL(int, port, Port)
PROPERTY_IMPL(const QString &, socket, Socket)
#undef PROPERTY_IMPL
#include "qfluentd.moc"
