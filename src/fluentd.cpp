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

#include "fluentd.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QPointer>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QLocalSocket>

class Fluentd::Private : public QObject
{
    Q_OBJECT
public:
    Private(Fluentd *parent);

    void send(const QString &tag, const QJsonObject &value);
private:
    Fluentd *q;
    QPointer<QIODevice> socket;
};

Fluentd::Private::Private(Fluentd *parent)
    : QObject(parent)
    , q(parent)
{
}

void Fluentd::Private::send(const QString &tag, const QJsonObject &value)
{
    if (socket.isNull()) {
        switch (q->m_connectionType) {
        case UnixDomainSocket: {
            QLocalSocket *s = new QLocalSocket(this);
            s->connectToServer(q->m_socket);
            if (s->waitForConnected()) {
                connect(s, SIGNAL(disconnected()), s, SLOT(deleteLater()));
                socket = s;
            } else {
                qWarning() << s->errorString() << s;
                s->deleteLater();
            }
            break; }
        case TcpSocket: {
            QTcpSocket *s = new QTcpSocket(this);
            s->connectToHost(q->m_host, q->m_port);
            if (s->waitForConnected()) {
                connect(s, SIGNAL(disconnected()), s, SLOT(deleteLater()));
                socket = s;
            } else {
                qWarning() << s->errorString() << s;
                s->deleteLater();
            }
            break; }
        }
    }

    if (socket) {
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
        socket->write(data);
    }
}

Fluentd::Fluentd(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
    , m_connectionType(TcpSocket)
    , m_host("127.0.0.1")
    , m_port(24224)
    , m_socket("/var/run/fluent/fluent.sock")
{
}

void Fluentd::send(const QString &tag, const QJsonObject &value)
{
    d->send(tag, value);
}

#include "fluentd.moc"
