// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017 The Phore developers
// Copyright (c) 2018 The Loonie developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (     0, uint256("0x001"));
	

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
	1534993375, // * UNIX timestamp of last checkpoint block
	0,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    400        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1533640145,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus;
    bnTrustedModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

libzerocoin::ZerocoinParams* CChainParams::OldZerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus;
    bnTrustedModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x5d;
        pchMessageStart[1] = 0xe6;
        pchMessageStart[2] = 0xb0;
        pchMessageStart[3] = 0x4b;
        nDefaultPort = 58923;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Loonie: 1 minute
        nTargetSpacing = 1 * 60;  // Loonie: 1 minute
        nMaturity = 20;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 130000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 2000;
        nZerocoinStartHeight = 1500; // Chain moving at normal pace at this block.

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "A rude awakening causes people to lose their sleep! Nov, 2018";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1533640145;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 3226936;

        // Uncomment the following block of code if you want to create a genesis block by running ./loonied.
        // Make sure to create a new psztimestamp and set your nonce to 0 before compiling, also comment out the assert lines below this block!
        printf("calc new genesis block\n");
        printf("hashMerkleRoot %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("bnProofOfWorkLimit 0x%x\n", bnProofOfWorkLimit.GetCompact());
        printf("genesis.nBits 0x%x\n", genesis.nBits);

        for (genesis.nNonce = 0; ; genesis.nNonce++) {
          hashGenesisBlock = genesis.GetHash();
          if (hashGenesisBlock <= bnProofOfWorkLimit) break;
        }

        printf("hashGenesisBlock %s\n", hashGenesisBlock.ToString().c_str());
        printf("genesis.nNonce %d\n", genesis.nNonce);
        printf("genesis.nTime %d\n", genesis.nTime);

        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x180b567eddde3001577f12de7d94c0e9a2db6121c9604d12d6ffe10cd54853fe"));
        //assert(genesis.hashMerkleRoot == uint256("0x1c2771797cba9b90fb491e2fa45c2b140fe0575e0cca97aa8c0adca84e9e028e"));

        vSeeds.push_back(CDNSSeedData("0", "195.181.244.223"));
        vSeeds.push_back(CDNSSeedData("1", "40.122.72.200"));
        vSeeds.push_back(CDNSSeedData("2", "40.68.1.137"));
        //vSeeds.push_back(CDNSSeedData("3", "seed4.loonie.io"));
        //vSeeds.push_back(CDNSSeedData("4", "seed5.loonie.io"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48); //LOONIE Addresses start with L
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 2);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 25);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x16)(0x8b)(0x53)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x0a)(0xcc)(0xd2)(0x2B).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x2e).convert_to_container<std::vector<unsigned char> >();

        bech32_hrp = "xln";

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04e53dc6ca6e7d2571b8ef53da4fa125d6f71d2b11c8eb1a91a4c97790521630620e021709f57cf5b7b7ae0b6dd69c0417fa15a5354cc834173eccd9b2fc654a14";
        strObfuscationPoolDummyAddress = "Lr5iHgGJJ6xGHqivmdZrYjRnhaYf7AJ2Mj";

        /** Zerocoin */
        zerocoinModulus = "0xc95577b6ace0049b0a27c779af38779355abadde4a1d80c353f6cb697a7ae5a087bad39caa5798478551d0f9d91e6267716506f32412de1d19d17588765eb9502b85c6a18abdb05791cfd8b734e960281193705eeece210920cc922b3af3ceb178bf12c22eb565d5767fbf19545639be8953c2c38ffad41f3371e4aac750ac2d7bd614b3faabb453081d5d88fdbb803657a980bc93707e4b14233a2358c97763bf28f7c933206071477e8b371f229bc9ce7d6ef0ed7163aa5dfe13bc15f7816348b328fa2c1e69d5c88f7b94cee7829d56d1842d77d7bb8692e9fc7b7db059836500de8d57eb43c345feb58671503b932829112941367996b03871300f25efb5";

        nZerocoinLastOldParams = 99999999; // Updated to defer zerocoin v2 for further testing.
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudgetFeeConfirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xab;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0xc9;
        pchMessageStart[3] = 0x84;
        nDefaultPort = 59923;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Loonie: 1 day
        nTargetSpacing = 1 * 10;  // Loonie: 1 minute
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nMaxMoneyOut = 43199500 * COIN;
        nLastPOWBlock = 50000;
        nZerocoinStartHeight = 200;

        nZerocoinLastOldParams = 100000000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1533640145;
        genesis.nNonce = 1;

        // Uncomment for genesis block creation
        printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        {
          printf("(test)calc new genesis block\n");
          printf("hashMerkleRoot %s\n", genesis.hashMerkleRoot.ToString().c_str());
          printf("bnProofOfWorkLimit 0x%x\n", bnProofOfWorkLimit.GetCompact());
          printf("genesis.nBits 0x%x\n", genesis.nBits);

          for (genesis.nNonce = 0; ; genesis.nNonce++) {
            hashGenesisBlock = genesis.GetHash();
            if (hashGenesisBlock <= bnProofOfWorkLimit) break;
          }

          printf("testhashGenesisBlock %s\n", hashGenesisBlock.ToString().c_str());
          printf("testgenesis.nNonce %d\n", genesis.nNonce);
        }

        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x180b567eddde3001577f12de7d94c0e9a2db6121c9604d12d6ffe10cd54853fe"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 56); // Testnet loonie addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 2);  // Testnet loonie script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet loonie BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x4a)(0x80)(0x41)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet loonie BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x4a)(0x80)(0x48)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet loonie BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x01)(0x00)(0x00)(0x40).convert_to_container<std::vector<unsigned char> >();

        bech32_hrp = "txln";

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04ed6d0930754e51bfcbd7414736ea60eb91a76a7de4734b165056ae4b24671779d7ad622b87edb6d50db693163d67fa362046e26d9c06341d4459f524f7627ae7";
        strObfuscationPoolDummyAddress = "LnbiHgGJJ6xGHqivmdZrYjRnhaYf6AJ2Mp";
        nBudgetFeeConfirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                     // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xb2;
        pchMessageStart[1] = 0xae;
        pchMessageStart[2] = 0x5e;
        pchMessageStart[3] = 0xcc;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 1 * 60; // Loonie: 1 day
        nTargetSpacing = 1 * 60;        // Loonie: 1 minutes
        bnProofOfWorkLimit = 0x207fffff; //~uint256(0) >> 1;
        genesis.nTime = 1533640145;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 1;
        nMaturity = 0;
        nLastPOWBlock = 999999999; // PoS complicates Regtest because of timing issues
        nZerocoinLastOldParams = 499;
        nZerocoinStartHeight = 100;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 59923;

        // Uncomment for genesis block creation
        printf("hashGenesisBlock %s\n", hashGenesisBlock.ToString().c_str());
        printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());

        {
          printf("(regtest)calc new genesis block\n");
          printf("hashMerkleRoot %s\n", genesis.hashMerkleRoot.ToString().c_str());
          printf("bnProofOfWorkLimit 0x%x\n", bnProofOfWorkLimit.GetCompact());
          printf("genesis.nBits 0x%x\n", genesis.nBits);

          for (genesis.nNonce = 0; ; genesis.nNonce++) {
            hashGenesisBlock = genesis.GetHash();
            if (hashGenesisBlock <= bnProofOfWorkLimit) break;
          }

          printf("hashGenesisBlock %s\n", hashGenesisBlock.ToString().c_str());
          printf("genesis.nNonce %d\n", genesis.nNonce);
        }

        assert(hashGenesisBlock == uint256("0x180b567eddde3001577f12de7d94c0e9a2db6121c9604d12d6ffe10cd54853fe"));

        bech32_hrp = "xlnrt";

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        nRequiredAccumulation = 1;

        strSporkKey = "048aa0e5c422632834bffd0588817bf90d7b19423a9d6b6959bc480d0d8bfc837ef6d4d6f90213bcde3e8a160dc7fa98840555e0bf0d7402001eafdcb8e6a5a18d";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 59924;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
