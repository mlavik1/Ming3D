#ifndef MING3D_NETTARGET_H
#define MING3D_NETTARGET_H

namespace Ming3D
{
    /**
    * Target for a networked message (RPC, etc.)
    */
    enum class NetTarget
    {
        Everyone,
        Host,
        Clients,
        Others
    };
}

#endif
