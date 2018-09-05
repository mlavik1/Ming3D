#ifndef MING3D_NETMESSAGETYPE_H
#define MING3D_NETMESSAGETYPE_H

namespace Ming3D
{
	enum NetMessageType
	{
		Ignored,
		RPC,
        ObjectCreation,
		ObjectReplication,
		ConnectionRequest,
        Log
	};
}

#endif // #ifndef MING3D_NETMESSAGETYPE_H
