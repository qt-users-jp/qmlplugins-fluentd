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

#ifndef QFLUENTD_H
#define QFLUENTD_H

#include <QtCore/QObject>
#include <QtNetwork/QAbstractSocket>

#ifdef FLUENTD_LIBRARY
# define FLUENTD_EXPORT Q_DECL_EXPORT
#else
# define FLUENTD_EXPORT Q_DECL_IMPORT
#endif

class FLUENTD_EXPORT QFluentd : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ConnectionState connectionState READ connectionState WRITE setConnectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionType connectionType READ connectionType WRITE setConnectionType NOTIFY connectionTypeChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString socket READ socket WRITE setSocket NOTIFY socketChanged)
    Q_ENUMS(ConnectionState)
    Q_ENUMS(ConnectionType)

public:
    enum ConnectionState
    {
        UnconnectedState = QAbstractSocket::UnconnectedState,
        HostLookupState = QAbstractSocket::HostLookupState,
        ConnectingState = QAbstractSocket::ConnectingState,
        ConnectedState = QAbstractSocket::ConnectedState,
        ClosingState = QAbstractSocket::ClosingState
    };

    enum ConnectionType {
        TcpSocket,
        UnixDomainSocket
    };

    explicit QFluentd(QObject *parent = 0);

    ConnectionState connectionState() const;
    ConnectionType connectionType() const;
    const QString &host() const;
    int port() const;
    const QString &socket() const;

    Q_INVOKABLE bool send(const QString &tag, const QJsonObject &value);

public slots:
    void setConnectionType(ConnectionType connectionType);
    void setHost(const QString &host);
    void setPort(int port);
    void setSocket(const QString &socket);


signals:
    void connectionStateChanged(ConnectionState connectionState);
    void connectionTypeChanged(ConnectionType connectionType);
    void hostChanged(const QString &host);
    void portChanged(int port);
    void socketChanged(const QString &socket);

protected:
    void setConnectionState(ConnectionState connectionState);

private:
    class Private;
    Private *d;
};

#endif // QFLUENTD_H
