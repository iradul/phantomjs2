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
#ifndef QAMQPQUEUE_H
#define QAMQPQUEUE_H

#include <QQueue>

#include "qamqpchannel.h"
#include "qamqpmessage.h"
#include "qamqpglobal.h"

class QAmqpClient;
class QAmqpClientPrivate;
class QAmqpExchange;
class QAmqpQueuePrivate;
class QAMQP_EXPORT QAmqpQueue : public QAmqpChannel, public QQueue<QAmqpMessage>
{
    Q_OBJECT
    Q_ENUMS(QueueOptions)
    Q_PROPERTY(int options READ options CONSTANT)
    Q_PROPERTY(QString consumerTag READ consumerTag WRITE setConsumerTag)
    // ivan: added : (inherited from QQueue)
    Q_PROPERTY(bool empty READ isEmpty)
    Q_ENUMS(QueueOption)
    Q_ENUMS(ConsumeOption)
    Q_ENUMS(RemoveOption)

public:
    enum QueueOption {
        NoOptions = 0x0,
        Passive = 0x01,
        Durable = 0x02,
        Exclusive = 0x04,
        AutoDelete = 0x08,
        NoWait = 0x10
    };
    Q_DECLARE_FLAGS(QueueOptions, QueueOption)
    int options() const;

    enum ConsumeOption {
        coNoLocal = 0x01,
        coNoAck = 0x02,
        coExclusive = 0x04,
        coNoWait = 0x08
    };
    Q_DECLARE_FLAGS(ConsumeOptions, ConsumeOption)

    enum RemoveOption {
        roForce = 0x0,
        roIfUnused = 0x01,
        roIfEmpty = 0x02,
        roNoWait = 0x04
    };
    Q_DECLARE_FLAGS(RemoveOptions, RemoveOption)

    ~QAmqpQueue();

    bool isConsuming() const;
    bool isDeclared() const;

    void setConsumerTag(const QString &consumerTag);
    QString consumerTag() const;

Q_SIGNALS:
    void declared();
    void bound();
    void unbound();
    void removed();
    void purged(int messageCount);

    void messageReceived();
    void empty();
    void consuming(const QString &consumerTag);
    void cancelled(const QString &consumerTag);

public Q_SLOTS:
    // AMQP Queue
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void declare(int options = Durable|AutoDelete);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void bind(const QString &exchangeName, const QString &key);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void bind(QAmqpExchange *exchange, const QString &key);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void unbind(const QString &exchangeName, const QString &key);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void unbind(QAmqpExchange *exchange, const QString &key);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void purge();
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void remove(int options = roIfUnused|roIfEmpty|roNoWait);

    // ivan: add :
    Q_INVOKABLE QVariantMap message();

    // AMQP Basic
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE bool consume(int options = NoOptions);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void get(bool noAck = true);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE bool cancel(bool noWait = false);
    void ack(const QAmqpMessage &message);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void ack(qlonglong deliveryTag, bool multiple);
    void reject(const QAmqpMessage &message, bool requeue);
    // ivan: Q_INVOKABLE added :
    Q_INVOKABLE void reject(qlonglong deliveryTag, bool requeue);

protected:
    // reimp Channel
    virtual void channelOpened();
    virtual void channelClosed();

private:
    explicit QAmqpQueue(int channelNumber = -1, QAmqpClient *parent = 0);

    Q_DISABLE_COPY(QAmqpQueue)
    Q_DECLARE_PRIVATE(QAmqpQueue)

    friend class QAmqpClient;
};

#endif  // QAMQPQUEUE_H
