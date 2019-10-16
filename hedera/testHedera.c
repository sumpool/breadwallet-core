//
//  testStellar.c
//  Core
//
//  Created by Carl Cherry on 5/21/2019.
//  Copyright © 2019 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.
//
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "support/BRArray.h"
#include "support/BRCrypto.h"
#include "support/BRBIP32Sequence.h"
#include "support/BRBIP39Mnemonic.h"
#include "support/BRBIP39WordsEn.h"
#include "support/BRKey.h"
#include "vendor/ed25519/ed25519.h"

#include "BRHederaTransaction.h"

static int debug_log = 0;

static uint8_t char2int(char input)
{
    if(input >= '0' && input <= '9')
        return input - '0';
    if(input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if(input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    return 0;
}

static void hex2bin(const char* src, uint8_t * target)
{
    while(*src && src[1])
    {
        *(target++) = (char2int(src[0]) << 4) | (char2int(src[1]) & 0x0f);
        src += 2;
    }
}

static void printBytes(const char* message, uint8_t * bytes, size_t byteSize)
{
    if (message) printf("%s\n", message);
    for(int i = 0; i < byteSize; i++) {
        if (i >= 0 && i % 8 == 0) printf("\n");
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

#define ED25519_SEED_KEY "ed25519 seed"

static void _deriveKeyImpl(BRKey *key, const void *seed, size_t seedLen, int depth, va_list vlist)
{
    UInt512 I;
    UInt256 secret, chainCode;
    assert(depth == 4);

    assert(key != NULL);
    assert(seed != NULL || seedLen == 0);

    if (key && (seed || seedLen == 0)) {
        // For the first hash the key is "ed25519 seed"
        BRHMAC(&I, BRSHA512, sizeof(UInt512), ED25519_SEED_KEY, strlen(ED25519_SEED_KEY), seed, seedLen);

        for (int i = 0; i < depth; i++) {
            secret = *(UInt256 *)&I;
            chainCode = *(UInt256 *)&I.u8[sizeof(UInt256)];

            // Take the result from the first hash and create some new data to hash
            // data = b'\x00' + secret + ser32(i)
            uint8_t data[37] = {0};
            memcpy(&data[1], secret.u8, 32);
            // Copy the bytes from i to the end of data
            uint32_t seq = va_arg(vlist, uint32_t);
            //assert(seq & 0xF0000000); // Needs to be hardened
            data[33] = (seq & 0xFF000000) >> 24;
            data[34] = (seq & 0x00FF0000) >> 16;
            data[35] = (seq & 0x0000FF00) >> 8;
            data[36] = (seq & 0x000000FF);

            // For the remaining hashing the key is the chaincode from the previous hash
            var_clean(&I); // clean before next hash
            BRHMAC(&I, BRSHA512, sizeof(UInt512), chainCode.u8, 32, data, 37);
        }

        // We are done - take that last hash result and it becomes our secret
        key->secret = *(UInt256 *)&I;

        // Erase from memory
        var_clean(&I);
        var_clean(&secret, &chainCode);
    }
}

static void _hederaDeriveKey(BRKey *key, const void *seed, size_t seedLen, int depth, ...)
{
    va_list ap;

    va_start(ap, depth);
    _deriveKeyImpl(key, seed, seedLen, depth, ap);
    va_end(ap);
}

static BRKey deriveHederaKeyFromSeed (UInt512 seed, uint32_t index)
{
    BRKey privateKey;

    // The BIP32 privateKey for m/44'/148'/%d' where %d is the key index
    // The stellar system only supports ed25519 key pairs. There are some restrictions when it comes
    // to creating key pairs for ed25519, described here:
    // https://github.com/stellar/stellar-protocol/blob/master/ecosystem/sep-0005.md
    _hederaDeriveKey(&privateKey, &seed, sizeof(UInt512), 4,
                      44 | BIP32_HARD,     // purpose  : BIP-44
                      3030 | BIP32_HARD,    // coin_type: HBAR
                      0 | BIP32_HARD,
                      0 | BIP32_HARD);

    return privateKey;
}

extern void
createHederaAccount(const char *paper_key)
{
    UInt512 seed = UINT512_ZERO;
    BRBIP39DeriveKey(seed.u8, paper_key, NULL); // no passphrase
    // The BIP32 privateKey for m/44'/60'/0'/0/index
    BRKey key = deriveHederaKeyFromSeed(seed, 0);

    unsigned char privateKey[64] = {0};
    unsigned char publicKey[32] = {0};
    ed25519_create_keypair(publicKey, privateKey, key.secret.u8);

    // Sign and verify
    unsigned char message[] = { 'h', 'e', 'r', 'e', ' ', 'i', 's', ' ', 't', 'h', 'e', ' ', 'm', 'e', 's', 's', 'a', 'g', 'e' };
    unsigned char signature[64];
    ed25519_sign(&signature[0], message, 19, publicKey, privateKey);
    int ret = ed25519_verify(signature, message, 19, publicKey);
    assert(ret == 1);
}


const char * paper_key_24 = "inmate flip alley wear offer often piece magnet surge toddler submit right radio absent pear floor belt raven price stove replace reduce plate home";
const char * paper_key_12 = "patient doctor olympic frog force glimpse endless antenna online dragon bargain someone";

static void hederaCryptoTests() {
    createHederaAccount(paper_key_12);
    createHederaAccount(paper_key_24);
}

static void createTransaction() {
    BRHederaTransaction transaction = hederaTransactionCreate();
    UInt512 seed = UINT512_ZERO;
    BRBIP39DeriveKey(seed.u8, paper_key_24, NULL); // no passphrase
    hederaTransactionSign(transaction, seed);
}

static void transaction_tests() {
    createTransaction();
}

extern void
runHederaTest (void /* ... */) {
    printf("Running hedera unit tests...\n");
    hederaCryptoTests();
    transaction_tests();
}