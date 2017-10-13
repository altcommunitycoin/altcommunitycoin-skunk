// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2017 The altcommunitycoin Project www.altcommunitycoin.io
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (     0,     uint256("0x00000f14896ba98013ed07e0ecf6e29b360a20898aab5c23238fd08c17ac1b10") )
        (  1000,     uint256("0x43897b66f9fce27d400d6a90ce57d3f6bf8e05ac0f970eec3aeb7aa9db4ce71f") )
        (  2000,     uint256("0x825b0ab760d54f351c166feb83011379cc87cb9f7ea2f780e767fff0bb8fbf92") )
        (  5000,     uint256("0x72c75a83ece934dc22ba08a6b0e25f37a400d13c8856c21431b0be8bf7d22f31") )
        ( 10000,     uint256("0x771e841b1e86729193c14d9bd25eaf9c53ed22a8676d0594a05d90adaee2f52e") )
        ( 25000,     uint256("0xac4c189c34021582003ff38534bc1bd13590bc7b5d550f19078c217941e86261") )
        ( 35000,     uint256("0xad6e983fd49ead6303ae8052d1f1eb3bda5f486d7d3e5b30237db42f86677420") )
        ( 45000,     uint256("0xf301c4fe84c7a50665365816c9c609c6a31a571dc141edc5fc023fbd3174848b") )
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
