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

#ifndef FLUENTD_H
#define FLUENTD_H

#include <QtCore/QObject>

class Fluentd : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ConnectionType connectionType READ connectionType WRITE connectionType NOTIFY connectionTypeChanged)
    Q_PROPERTY(QString host READ host WRITE host NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE port NOTIFY portChanged)
    Q_PROPERTY(QString socket READ socket WRITE socket NOTIFY socketChanged)
    Q_ENUMS(ConnectionType)

public:
    enum ConnectionType {
        TcpSocket,
        UnixDomainSocket
    };

    explicit Fluentd(QObject *parent = 0);

    Q_INVOKABLE void send(const QString &tag, const QJsonObject &value);

signals:
    void hostChanged(const QString &host);
    void portChanged(int port);
    void connectionTypeChanged(bool connectionType);
    void socketChanged(const QString &socket);

private:
    class Private;
    Private *d;

#define ADD_PROPERTY(type, name, type2) \
public: \
    type name() const { return m_##name; } \
    void name(type name) { \
        if (m_##name == name) return; \
        m_##name = name; \
        emit name##Changed(name); \
    } \
private: \
    type2 m_##name;

    ADD_PROPERTY(ConnectionType, connectionType, ConnectionType)
    ADD_PROPERTY(const QString &, host, QString)
    ADD_PROPERTY(int, port, int)
    ADD_PROPERTY(const QString &, socket, QString)
};

#endif // FLUENTD_H
