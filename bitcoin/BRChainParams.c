//
//  BRChainParams.c
//  BRCore
//
//  Created by Aaron Voisine on 3/11/19.
//  Copyright (c) 2019 breadwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#include "BRChainParams.h"

static const char *BRMainNetDNSSeeds[] = {
    "dnsseed.sumnode.io.", "dnsseed.sumcoinpool.org.", "dnsseed.sumcoinmining.org.", "dnsseed.sumfaucet.co.",
    "sumdnsseed.moonypool.com.", "dnsseed.minesum.com.", NULL
};

static const char *BRTestNetDNSSeeds[] = {
    "dnsseed.sumnode.io.", "dnsseed.sumcoinpool.org.", "dnsseed.sumcoinmining.org.", "dnsseed.sumfaucet.co.",
    "sumdnsseed.moonypool.com.", NULL
};

// blockchain checkpoints - these are also used as starting points for partial chain downloads, so they must be at
// difficulty transition boundaries in order to verify the block difficulty at the immediately following transition
static const BRCheckPoint BRMainNetCheckpoints[] = {
    {      0, uint256("37d4696c5072cd012f3b7c651e5ce56a1383577e4edacc2d289ec9b25eebfd5e"), 1554579000, 0x1e0ffff0 },
    {   2880, uint256("31dfe91b64cbbb167b4e4c644ad7b008bb4bb8ca4e42aea02f938f445dc37cff"), 1554622048, 0x1e07f80d },
    {   5760, uint256("87de6e561bb0ab66095b272450cabbb0b11fb272e5ae37123ba1464a5de74f5c"), 1554647659, 0x1e025cc4 },
    {   8640, uint256("19c100388b8a8bc0230f43c4355733e2b3375e08f1667449678ec14f66174aae"), 1554685252, 0x1e01071f },
    {  11520, uint256("0fcc3eacab1e532325f1c5bde0372b78e999a504a8eaaf3d4628038de6735d30"), 1554722824, 0x1d727780 },
    {  14400, uint256("6b55672438b707c59c95e7681a9914c4d201d2cdf5f42d902d53ed23042e028a"), 1554754827, 0x1d2a6421 },
    {  17280, uint256("23886b646dc4601c788cf0014924ea3d9403534a724285327be63db5ea390123"), 1554791948, 0x1d123584 },
    {  57600, uint256("edbe7a9e2e20c8c12fea25dc9be9268bfa308145fa68d2d2ce54dc405cf12e76"), 1555612735, 0x1c01a52a },
    {  86400, uint256("8989bab14675dc54c64f287957d624ad8c057a2fb4f200d038f7862d8f5fa76d"), 1556457190, 0x1c0145da },
    { 115200, uint256("e386f62c30a46f6b565ffc8a3fdf73e5e76e9f2f400d5cba1911e7732b52bd8d"), 1557312544, 0x1c012412 },
    { 144000, uint256("210da711b7a617fbf3ae0893935973ff60a17e21e0a396224eb553c0439311d5"), 1558174688, 0x1c011ae1 },
    { 172800, uint256("341f603628dbe328512d9507066241d9fcc4c81ac91dd9bc87e2184ed5787b80"), 1559034517, 0x1c0104c5 },
    { 201600, uint256("6bbde0a86b6e1efa71333bb3636f02931ebb872157b48c674efde25054a571b3"), 1559895468, 0x1c00dd37 },
    { 216000, uint256("7f1d6d0c386cc6a6d5eb85877adb039d7747c6047d84e311b7213bf237c78b7c"), 1560306060, 0x1c00a311 },
    { 230400, uint256("8857a4bd70e6aee0eec8902cd88ed378c3d73d625a3920c67d393c83176d12af"), 1560771501, 0x1c00e9fc },
    { 244800, uint256("b0b0f2ed9f2caa940afa15a178f2bf106d3b59eed2e19379bc3d54219ed674a4"), 1561203773, 0x1c00d8ac },
    { 259200, uint256("886fbe4eebd29e5aba9c3cb1d2319aaba6053d6e6956d7af79f99da19641895e"), 1561609882, 0x1c009c64 },
    { 273600, uint256("2aff5d7931a37d16ed1cbb1e81ad1b5f4cb357be5d844d5f9a7f3ebea956aa1a"), 1562044711, 0x1c009d96 },
    { 280800, uint256("a0e895c7972d1aa031e4154b59472267d14c34ef5d7067b80a89f5f102e688e4"), 1562261715, 0x1c00a4ba },
    { 283680, uint256("8858031b1cee059a036f3075566a93c6e2ff425854f551dad377a07c21698cc4"), 1562332721, 0x1c0095c6 },
    { 286560, uint256("cb9fac6220ec62ba195dad103ec030df11735bc741d8697a331b9a05bbcab156"), 1562394830, 0x1b68c052 },
    { 315360, uint256("f963b0f0c2f1a217c1f5f3da0ad0efc0d0715397ceb1259023aa809317a97827"), 1563318882, 0x1c00948c },
    { 344160, uint256("d6d5348e4777bff49e6fa98a10b63f3191afec39103609e85668508b806a0ff1"), 1564192191, 0x1c00a930 },
    { 372960, uint256("de350113ba6a8b8fdc2496f7527dd64cc79da560dd54d72a4cdffad2112f0da6"), 1565057466, 0x1c009df1 },
    { 401760, uint256("3062855d8f1fa0043c39aa56f2d0aa160f910491dee4b64503999e9935c87153"), 1565939245, 0x1c00bb2b },
    { 427680, uint256("72d7ee79e1aa2c4f46d61f9d569a0622ce49b13faf4dcc42d0e83786c4a60fb9"), 1566694938, 0x1c0099eb },
    { 457920, uint256("717050f4a4a96f2d7b4c13fe2db70dc2998c31fea01607d9529b8dfeb434bc56"), 1567617376, 0x1c008ee4 },
    { 472320, uint256("3dccd8aaef590f102c7e3cd8200eb666a68d0356a24c87b51617314adb73e24c"), 1567617376, 0x1c008ee4 },
    { 501120, uint256("b0f6796740602fa097685f4d572090e956b33154914bd20ef2bedbf01a706d31"), 1568885501, 0x1b5e4e46 },
    { 529920, uint256("be82c887af163c8c3a117497cfa909113067e25a8258b6644684c28d28ff1fdd"), 1569797529, 0x1c0097d6 },
    { 544320, uint256("3687c221709bd7c1c0848097df91cf6b1ff413e352e4f65e1ced6f4c94e07ec7"), 1570246872, 0x1c00b7a9 },
    { 558720, uint256("25ed1e5a99938e5fd2503c4be4a435d7d582387f0d07587f5fccf03f7244282a"), 1570674328, 0x1c00ad20 },
    { 573120, uint256("7eefb09f6c5ad6874352b4c089d0dd8e9cf34b4d2537bcbef6bd0ea70a065998"), 1571109642, 0x1c00b255 },
    { 587520, uint256("5952411159da3f15c29795673730166b1adb6d66e4abe681c1b6ea994589cb6b"), 1571553616, 0x1c00cc2d },
    { 601920, uint256("794df734a9ddcf3b7d460de7fde59e2ef83e9311e19800662344c46b2cec66e5"), 1571982260, 0x1c00bee9 },
    { 616320, uint256("2df1470ec7f86b972ca0d2d64de1306813e1e882e2e9f5cb4906a175311ef168"), 1572418239, 0x1c00c64d },
    { 645120, uint256("4ce5317488df663f0430caee55c602d2a29170812690b818f91a9c64fb4bfaac"), 1573275493, 0x1c00b5a6 },
    { 817920, uint256("ef7aa22745d200dd6966995139e3ef35ebd97561f32555bfdeada1c2cdbce8be"), 1578489707, 0x1c00d1d8 },
    { 846720, uint256("16590b7ad48ea99a484e1c2fa686895623c4bb9ed14d832717351a588cad742b"), 1579364469, 0x1c00a5a9 }

      //875520{
      //904320{
};

static const BRCheckPoint BRTestNetCheckpoints[] = {
    {       0, uint256("e1309964e3ac20bd3bf8f7cdd9ccfc9b5a6a779b9975abc1c89c132db618048c"), 1523718091, 0x1e0ffff0 }

};

static int BRMainNetVerifyDifficulty(const BRMerkleBlock *block, const BRSet *blockSet)
{
    const BRMerkleBlock *previous, *b = NULL;
    uint32_t i;

    assert(block != NULL);
    assert(blockSet != NULL);

    // check if we hit a difficulty transition, and find previous transition block
    if ((block->height % BLOCK_DIFFICULTY_INTERVAL) == 0) {
        for (i = 0, b = block; b && i < BLOCK_DIFFICULTY_INTERVAL; i++) {
            b = BRSetGet(blockSet, &b->prevBlock);
        }
    }

    previous = BRSetGet(blockSet, &block->prevBlock);
    return BRMerkleBlockVerifyDifficulty(block, previous, (b) ? b->timestamp : 0);
}

static int BRTestNetVerifyDifficulty(const BRMerkleBlock *block, const BRSet *blockSet)
{
    return 1; // XXX skip testnet difficulty check for now
}

extern const BRCheckPoint *BRChainParamsGetCheckpointBefore (const BRChainParams *params, uint32_t timestamp) {
    for (ssize_t index = params->checkpointsCount - 1; index >= 0; index--)
        if (params->checkpoints[index].timestamp < timestamp)
            return &params->checkpoints[index];
   return NULL;
}

extern const BRCheckPoint *BRChainParamsGetCheckpointBeforeBlockNumber (const BRChainParams *params, uint32_t blockNumber) {
    for (ssize_t index = params->checkpointsCount - 1; index >= 0; index--)
        if (params->checkpoints[index].height < blockNumber)
            return &params->checkpoints[index];
   return NULL;
}

static const BRChainParams BRMainNetParamsRecord = {
    BRMainNetDNSSeeds,
    3333,                  // standardPort SUM
    0xd0b5c0fa,            // magicNumber SUM
    SERVICES_NODE_WITNESS, // services
    BRMainNetVerifyDifficulty,
    BRMainNetCheckpoints,
    sizeof(BRMainNetCheckpoints)/sizeof(*BRMainNetCheckpoints),
    { BITCOIN_PUBKEY_PREFIX, BITCOIN_SCRIPT_PREFIX, BITCOIN_PRIVKEY_PREFIX, BITCOIN_BECH32_PREFIX },
    BITCOIN_FORKID
};
const BRChainParams *BRMainNetParams = &BRMainNetParamsRecord;

static const BRChainParams BRTestNetParamsRecord = {
    BRTestNetDNSSeeds,
    13333,                 // standardPort TSUM
    0xd3b7c2fd,            // magicNumber TSUM
    SERVICES_NODE_WITNESS, // services
    BRTestNetVerifyDifficulty,
    BRTestNetCheckpoints,
    sizeof(BRTestNetCheckpoints)/sizeof(*BRTestNetCheckpoints),
    { BITCOIN_PUBKEY_PREFIX_TEST, BITCOIN_SCRIPT_PREFIX_TEST, BITCOIN_PRIVKEY_PREFIX_TEST, BITCOIN_BECH32_PREFIX_TEST },
    BITCOIN_FORKID
};

const BRChainParams *BRTestNetParams = &BRTestNetParamsRecord;
