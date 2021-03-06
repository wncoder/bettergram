/*
This file is part of Bettergram.

For license and copyright information please follow this link:
https://github.com/bettergram/bettergram/blob/master/LEGAL
*/
#pragma once

#include "mtproto/auth_key.h"
#include "mtproto/connection_abstract.h"
#include "base/timer.h"

namespace MTP {
namespace internal {

class ResolvingConnection : public AbstractConnection {
public:
	ResolvingConnection(
		not_null<Instance*> instance,
		QThread *thread,
		const ProxyData &proxy,
		ConnectionPointer &&child);

	ConnectionPointer clone(const ProxyData &proxy) override;

	crl::time pingTime() const override;
	crl::time fullConnectTimeout() const override;
	void sendData(mtpBuffer &&buffer) override;
	void disconnectFromServer() override;
	void connectToServer(
		const QString &address,
		int port,
		const bytes::vector &protocolSecret,
		int16 protocolDcId) override;
	bool isConnected() const override;
	bool requiresExtendedPadding() const override;

	int32 debugState() const override;

	QString transport() const override;
	QString tag() const override;

private:
	void setChild(ConnectionPointer &&child);
	bool refreshChild();
	void emitError(int errorCode);

	void domainResolved(
		const QString &host,
		const QStringList &ips,
		qint64 expireAt);
	void handleError(int errorCode);
	void handleConnected();
	void handleDisconnected();
	void handleReceivedData();

	not_null<Instance*> _instance;
	ConnectionPointer _child;
	bool _connected = false;
	int _ipIndex = -1;
	QString _address;
	int _port = 0;
	bytes::vector _protocolSecret;
	int16 _protocolDcId = 0;
	base::Timer _timeoutTimer;

};

} // namespace internal
} // namespace MTP
