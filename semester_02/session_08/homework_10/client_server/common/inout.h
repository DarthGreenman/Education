/// inout.h

#ifndef INOUT_H_IN_MY_PROJECT
#define INOUT_H_IN_MY_PROJECT

#include "protocol.h"
#include <cstdint>
#include <qdatastream.h>

QDataStream& operator>>(QDataStream& ou, protocol::RequestTypeMessages& type);
QDataStream& operator<<(QDataStream& in, protocol::RequestTypeMessages type);

QDataStream& operator>>(QDataStream& ou, protocol::StatusMessages& status);
QDataStream& operator<<(QDataStream& in, protocol::StatusMessages status);

QDataStream& operator>>(QDataStream& ou, protocol::ServiceHeader& header);
QDataStream& operator<<(QDataStream& in, const protocol::ServiceHeader& header);

QDataStream& operator>>(QDataStream& ou, protocol::StatServer& statistics);
QDataStream& operator<<(QDataStream& in, const protocol::StatServer& statistics);


QDataStream& operator>>(QDataStream& ou, protocol::RequestTypeMessages& type)
{
	uint8_t typeInt{};
	ou >> typeInt;
	type = static_cast<protocol::RequestTypeMessages>(typeInt);
	return ou;
}

QDataStream& operator<<(QDataStream& in, protocol::RequestTypeMessages type)
{
	in << static_cast<uint8_t>(type);
	return in;
}

QDataStream& operator>>(QDataStream& ou, protocol::StatusMessages& status)
{
	uint8_t statusInt{};
	ou >> statusInt;
	status = static_cast<protocol::StatusMessages>(statusInt);
	return ou;
}

QDataStream& operator<<(QDataStream& in, protocol::StatusMessages status)
{
	in << static_cast<uint8_t>(status);
	return in;
}

QDataStream& operator>>(QDataStream& ou, protocol::ServiceHeader& header)
{
	ou >> header.idHead;
	ou >> header.typeMsg;
	ou >> header.statusMsg;
	ou >> header.sizeMsg;
	return ou;
}

QDataStream& operator<<(QDataStream& in, const protocol::ServiceHeader& header)
{
	in << header.idHead;
	in << header.typeMsg;
	in << header.statusMsg;
	in << header.sizeMsg;
	return in;
}

QDataStream& operator>>(QDataStream& ou, protocol::StatServer& statistics)
{
	ou >> statistics.bytesReceived;
	ou >> statistics.bytesTransferred;
	ou >> statistics.packetsReceived;
	ou >> statistics.packetsTransferred;
	ou >> statistics.serverUpTime;
	ou >> statistics.connectedClients;
	return ou;
}

QDataStream& operator<<(QDataStream& in, const protocol::StatServer& statistics)
{
	in << statistics.bytesReceived;
	in << statistics.bytesTransferred;
	in << statistics.packetsReceived;
	in << statistics.packetsTransferred;
	in << statistics.serverUpTime;
	in << statistics.connectedClients;
	return in;
}

#endif // !INOUT_H_IN_MY_PROJECT
