/*
 * Copyright (C) 2012-2014 Alexey Shcherbakov
 * Copyright (C) 2014-2015 Matt Broadstone
 * Contact: https://github.com/mbroadst/qamqp
 *
 * This file is part of the QAMQP Library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
#ifndef QAMQPCLIENT_H
#define QAMQPCLIENT_H

#include <QObject>
#include <QUrl>
#include <QHostAddress>
#include <QSslConfiguration>
#include <QSslError>

#include "qamqpglobal.h"

class QAmqpExchange;
class QAmqpQueue;
class QAmqpAuthenticator;
class QAmqpClientPrivate;
class QAMQP_EXPORT QAmqpClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 port READ port WRITE setPort)
    Q_PROPERTY(QString host READ host WRITE setHost)
    Q_PROPERTY(QString virtualHost READ virtualHost WRITE setVirtualHost)
    Q_PROPERTY(QString user READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(bool autoReconnect READ autoReconnect WRITE setAutoReconnect)
    Q_PROPERTY(qint16 channelMax READ channelMax WRITE setChannelMax)
    Q_PROPERTY(qint32 frameMax READ frameMax WRITE setFrameMax)
    Q_PROPERTY(qint16 heartbeatDelay READ heartbeatDelay() WRITE setHeartbeatDelay)

public:
    explicit QAmqpClient(QObject *parent = 0);
    ~QAmqpClient();

    // properties
    quint16 port() const;
    void setPort(quint16 port);

    QString host() const;
    void setHost(const QString &host);

    QString virtualHost() const;
    void setVirtualHost(const QString &virtualHost);

    QString username() const;
    void setUsername(const QString &username);

    QString password() const;
    void setPassword(const QString &password);

    void setAuth(QAmqpAuthenticator *auth);
    QAmqpAuthenticator *auth() const;

    bool autoReconnect() const;
    void setAutoReconnect(bool value);

    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE bool isConnected() const;

    qint16 channelMax() const;
    void setChannelMax(qint16 channelMax);

    qint32 frameMax() const;
    void setFrameMax(qint32 frameMax);

    qint16 heartbeatDelay() const;
    void setHeartbeatDelay(qint16 delay);

    void addCustomProperty(const QString &name, const QString &value);
    QString customProperty(const QString &name) const;

    QAbstractSocket::SocketError socketError() const;
    // ivan: add :
    Q_INVOKABLE QString socketErrorString() const;
    QAMQP::Error error() const;
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE QString errorString() const;

    QSslConfiguration sslConfiguration() const;
    void setSslConfiguration(const QSslConfiguration &config);

    // channels
    // ivan: Q_INVOKABLE added : updated QObject return
    Q_INVOKABLE QObject *createExchange(int channelNumber = -1);
    // ivan: Q_INVOKABLE added : updated QObject return
    Q_INVOKABLE QObject *createExchange(const QString &name, int channelNumber = -1);

    // ivan: Q_INVOKABLE added : updated QObject return
    Q_INVOKABLE QObject *createQueue(int channelNumber = -1);
    // ivan: Q_INVOKABLE added : updated QObject return
    Q_INVOKABLE QObject *createQueue(const QString &name, int channelNumber = -1);

    // methods
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void connectToHost(const QString &uri = QString());
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void connectToHost(const QHostAddress &address, quint16 port = AMQP_PORT);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void disconnectFromHost();

Q_SIGNALS:
    void connected();
    void disconnected();
    void error(QAMQP::Error error);
    void socketError(QAbstractSocket::SocketError error);
    void sslErrors(const QList<QSslError> &errors);

public Q_SLOTS:
    void ignoreSslErrors(const QList<QSslError> &errors);

protected:
    QAmqpClient(QAmqpClientPrivate *dd, QObject *parent = 0);

    Q_DISABLE_COPY(QAmqpClient)
    Q_DECLARE_PRIVATE(QAmqpClient)
    QScopedPointer<QAmqpClientPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_socketConnected())
    Q_PRIVATE_SLOT(d_func(), void _q_socketDisconnected())
    Q_PRIVATE_SLOT(d_func(), void _q_readyRead())
    Q_PRIVATE_SLOT(d_func(), void _q_socketError(QAbstractSocket::SocketError error))
    Q_PRIVATE_SLOT(d_func(), void _q_heartbeat())
    Q_PRIVATE_SLOT(d_func(), void _q_connect())
    Q_PRIVATE_SLOT(d_func(), void _q_disconnect())

    friend class QAmqpChannelPrivate;
    friend class QAmqpQueuePrivate;

};

#endif // QAMQPCLIENT_H
