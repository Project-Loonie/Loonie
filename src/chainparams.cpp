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
    boost::assign::map_list_of(0, uint256("0x00000a980907dfbdeb25abeed64d59cca87d7687876e74ed208c6b1e7e059d8a"));
	

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
	1543197839, // * UNIX timestamp of last checkpoint block
	0,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    400        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1543197900,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1543198000,
    0,
    0};

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
        pchMessageStart[0] = 0xd9;
        pchMessageStart[1] = 0x32;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0x6b;
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
        nLastPOWBlock = 1000;
        nZerocoinStartHeight = 1000; // Chain moving at normal pace at this block.

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
        const char* pszTimestamp = "If at first you don't succeed, try, try, try again - November, 2018";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1543197839;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 7091600;

        // Uncomment the following block of code if you want to create a genesis block by running ./loonied.
        // Make sure to create a new psztimestamp and set your nonce to 0 before compiling, also comment out the assert lines below this block!
        /*printf("calc new genesis block\n");
        printf("hashMerkleRoot %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("bnProofOfWorkLimit 0x%x\n", bnProofOfWorkLimit.GetCompact());
        printf("genesis.nBits 0x%x\n", genesis.nBits);

        for (genesis.nNonce = 0; ; genesis.nNonce++) {
          hashGenesisBlock = genesis.GetHash();
          if (hashGenesisBlock <= bnProofOfWorkLimit) break;
        }

        printf("hashGenesisBlock %s\n", hashGenesisBlock.ToString().c_str());
        printf("genesis.nNonce %d\n", genesis.nNonce);
        printf("genesis.nTime %d\n", genesis.nTime);*/

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000a980907dfbdeb25abeed64d59cca87d7687876e74ed208c6b1e7e059d8a"));
        assert(genesis.hashMerkleRoot == uint256("0xb85a26eec5f714f58308a16b234a76add4414e46e737b7a3f60c7b5eaba9f31c"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // vSeeds.push_back(CDNSSeedData("195.181.244.223", "195.181.244.223"));
        // vSeeds.push_back(CDNSSeedData("40.122.72.200", "40.122.72.200"));
        // vSeeds.push_back(CDNSSeedData("40.68.1.137", "40.68.1.137"));
        //vSeeds.push_back(CDNSSeedData("3", "seed4.loonie.io"));
        //vSeeds.push_back(CDNSSeedData("4", "seed5.loonie.io"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 25);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 2);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x67)(0x62)(0xB2)(0xF4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x44)(0xb0)(0xAD)(0xF8).convert_to_container<std::vector<unsigned char> >();
        //  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md 109   0x8000006d
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x41).convert_to_container<std::vector<unsigned char> >();

        //bech32_hrp = "xln";

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
        strSporkKey = "04d569777d5472fe729fdea1b6fc8f9d679cb46b1eb313da3aa03764a883a5530b90bcb3b1b6ef543f16e49142fc910de8aa45fbd74ab5cc518612da8a48f9f0a7";
        strObfuscationPoolDummyAddress = "Lr5iHgGJJ6xGHqivmdZrYjRnhaYf7AJ2Mj";

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494029183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892984591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733046974880842540179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774467920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901166575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951398636564391212010397122822120720357";

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
        pchMessageStart[0] = 0x99;
        pchMessageStart[1] = 0x73;
        pchMessageStart[2] = 0xbb;
        pchMessageStart[3] = 0xa6;
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
        nLastPOWBlock = 200;
        nZerocoinStartHeight = 200;

        nZerocoinLastOldParams = 100000000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1543197900;
        genesis.nNonce = 7882620;

        // Uncomment for genesis block creation
        /*printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
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
        }*/

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000164337bd26f58fdfe4430d885cb2f272d5ca3079743766f6c2d60b78b29"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet loonie addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet loonie script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet loonie BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet loonie BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet loonie BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        bech32_hrp = "txln";

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "045e87a1216ed3874515f5044e46d74c1bac67f2d72e042101ab4024a4851c038d306431cd5c137af0704a9ff6da716e8f96d24e71a3d3527c0e8c0fab5af2263f";
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
        pchMessageStart[0] = 0x6b;
        pchMessageStart[1] = 0x62;
        pchMessageStart[2] = 0xc3;
        pchMessageStart[3] = 0xed;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 1 * 60; // Loonie: 1 day
        nTargetSpacing = 1 * 60;        // Loonie: 1 minutes
        bnProofOfWorkLimit = 0x207fffff; //~uint256(0) >> 1;
        genesis.nTime = 1543198000;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 7590263;
        nMaturity = 0;
        nLastPOWBlock = 999999999; // PoS complicates Regtest because of timing issues
        nZerocoinLastOldParams = 499;
        nZerocoinStartHeight = 100;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 59923;

        // Uncomment for genesis block creation
        /*printf("hashGenesisBlock %s\n", hashGenesisBlock.ToString().c_str());
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
        }*/

        assert(hashGenesisBlock == uint256("0x00000a89fd933ceaf7d2734b57ef33e959e25b3fa3f192930764ad4ea9151b2f"));

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

        strSporkKey = "048ea0e5c422637834bffd0588817bf90d5b19423a9d6b6959bc480d0d8bfc837ef6d4d6f90213bcde3e8a160dc7fa98840555e0bf0d7402001eafdcb8e6a5a18d";
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
