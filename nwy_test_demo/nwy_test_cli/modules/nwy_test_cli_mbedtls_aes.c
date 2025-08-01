#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"

#ifdef NWY_TEST_MBEDTLS_AES_SUPPORT
#include "mbedtls/aes.h"
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/ctr_drbg.h"

/*
 * AES test vectors from:
 *
 * http://csrc.nist.gov/archive/aes/rijndael/rijndael-vals.zip
 */
static const unsigned char aes_test_ecb_dec[3][16] =
{
    { 0x44, 0x41, 0x6A, 0xC2, 0xD1, 0xF5, 0x3C, 0x58,
      0x33, 0x03, 0x91, 0x7E, 0x6B, 0xE9, 0xEB, 0xE0 },
    { 0x48, 0xE3, 0x1E, 0x9E, 0x25, 0x67, 0x18, 0xF2,
      0x92, 0x29, 0x31, 0x9C, 0x19, 0xF1, 0x5B, 0xA4 },
    { 0x05, 0x8C, 0xCF, 0xFD, 0xBB, 0xCB, 0x38, 0x2D,
      0x1F, 0x6F, 0x56, 0x58, 0x5D, 0x8A, 0x4A, 0xDE }
};

static const unsigned char aes_test_ecb_enc[3][16] =
{
    { 0xC3, 0x4C, 0x05, 0x2C, 0xC0, 0xDA, 0x8D, 0x73,
      0x45, 0x1A, 0xFE, 0x5F, 0x03, 0xBE, 0x29, 0x7F },
    { 0xF3, 0xF6, 0x75, 0x2A, 0xE8, 0xD7, 0x83, 0x11,
      0x38, 0xF0, 0x41, 0x56, 0x06, 0x31, 0xB1, 0x14 },
    { 0x8B, 0x79, 0xEE, 0xCC, 0x93, 0xA0, 0xEE, 0x5D,
      0xFF, 0x30, 0xB4, 0xEA, 0x21, 0x63, 0x6D, 0xA4 }
};

#if defined(MBEDTLS_CIPHER_MODE_CBC)
static const unsigned char aes_test_cbc_dec[3][16] =
{
    { 0xFA, 0xCA, 0x37, 0xE0, 0xB0, 0xC8, 0x53, 0x73,
      0xDF, 0x70, 0x6E, 0x73, 0xF7, 0xC9, 0xAF, 0x86 },
    { 0x5D, 0xF6, 0x78, 0xDD, 0x17, 0xBA, 0x4E, 0x75,
      0xB6, 0x17, 0x68, 0xC6, 0xAD, 0xEF, 0x7C, 0x7B },
    { 0x48, 0x04, 0xE1, 0x81, 0x8F, 0xE6, 0x29, 0x75,
      0x19, 0xA3, 0xE8, 0x8C, 0x57, 0x31, 0x04, 0x13 }
};

static const unsigned char aes_test_cbc_enc[3][16] =
{
    { 0x8A, 0x05, 0xFC, 0x5E, 0x09, 0x5A, 0xF4, 0x84,
      0x8A, 0x08, 0xD3, 0x28, 0xD3, 0x68, 0x8E, 0x3D },
    { 0x7B, 0xD9, 0x66, 0xD5, 0x3A, 0xD8, 0xC1, 0xBB,
      0x85, 0xD2, 0xAD, 0xFA, 0xE8, 0x7B, 0xB1, 0x04 },
    { 0xFE, 0x3C, 0x53, 0x65, 0x3E, 0x2F, 0x45, 0xB5,
      0x6F, 0xCD, 0x88, 0xB2, 0xCC, 0x89, 0x8F, 0xF0 }
};
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 test vectors from:
 *
 * http://csrc.nist.gov/publications/nistpubs/800-38a/sp800-38a.pdf
 */
static const unsigned char aes_test_cfb128_key[3][32] =
{
    { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
      0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C },
    { 0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52,
      0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5,
      0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B },
    { 0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE,
      0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
      0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7,
      0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4 }
};

static const unsigned char aes_test_cfb128_iv[16] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

static const unsigned char aes_test_cfb128_pt[64] =
{
    0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
    0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
    0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C,
    0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
    0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11,
    0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
    0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17,
    0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10
};

static const unsigned char aes_test_cfb128_ct[3][64] =
{
    { 0x3B, 0x3F, 0xD9, 0x2E, 0xB7, 0x2D, 0xAD, 0x20,
      0x33, 0x34, 0x49, 0xF8, 0xE8, 0x3C, 0xFB, 0x4A,
      0xC8, 0xA6, 0x45, 0x37, 0xA0, 0xB3, 0xA9, 0x3F,
      0xCD, 0xE3, 0xCD, 0xAD, 0x9F, 0x1C, 0xE5, 0x8B,
      0x26, 0x75, 0x1F, 0x67, 0xA3, 0xCB, 0xB1, 0x40,
      0xB1, 0x80, 0x8C, 0xF1, 0x87, 0xA4, 0xF4, 0xDF,
      0xC0, 0x4B, 0x05, 0x35, 0x7C, 0x5D, 0x1C, 0x0E,
      0xEA, 0xC4, 0xC6, 0x6F, 0x9F, 0xF7, 0xF2, 0xE6 },
    { 0xCD, 0xC8, 0x0D, 0x6F, 0xDD, 0xF1, 0x8C, 0xAB,
      0x34, 0xC2, 0x59, 0x09, 0xC9, 0x9A, 0x41, 0x74,
      0x67, 0xCE, 0x7F, 0x7F, 0x81, 0x17, 0x36, 0x21,
      0x96, 0x1A, 0x2B, 0x70, 0x17, 0x1D, 0x3D, 0x7A,
      0x2E, 0x1E, 0x8A, 0x1D, 0xD5, 0x9B, 0x88, 0xB1,
      0xC8, 0xE6, 0x0F, 0xED, 0x1E, 0xFA, 0xC4, 0xC9,
      0xC0, 0x5F, 0x9F, 0x9C, 0xA9, 0x83, 0x4F, 0xA0,
      0x42, 0xAE, 0x8F, 0xBA, 0x58, 0x4B, 0x09, 0xFF },
    { 0xDC, 0x7E, 0x84, 0xBF, 0xDA, 0x79, 0x16, 0x4B,
      0x7E, 0xCD, 0x84, 0x86, 0x98, 0x5D, 0x38, 0x60,
      0x39, 0xFF, 0xED, 0x14, 0x3B, 0x28, 0xB1, 0xC8,
      0x32, 0x11, 0x3C, 0x63, 0x31, 0xE5, 0x40, 0x7B,
      0xDF, 0x10, 0x13, 0x24, 0x15, 0xE5, 0x4B, 0x92,
      0xA1, 0x3E, 0xD0, 0xA8, 0x26, 0x7A, 0xE2, 0xF9,
      0x75, 0xA3, 0x85, 0x74, 0x1A, 0xB9, 0xCE, 0xF8,
      0x20, 0x31, 0x62, 0x3D, 0x55, 0xB1, 0xE4, 0x71 }
};
#endif /* MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_OFB)
/*
 * AES-OFB test vectors from:
 *
 * https://csrc.nist.gov/publications/detail/sp/800-38a/final
 */
static const unsigned char aes_test_ofb_key[3][32] =
{
    { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
      0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C },
    { 0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52,
      0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5,
      0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B },
    { 0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE,
      0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
      0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7,
      0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4 }
};

static const unsigned char aes_test_ofb_iv[16] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

static const unsigned char aes_test_ofb_pt[64] =
{
    0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
    0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
    0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C,
    0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
    0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11,
    0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
    0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17,
    0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10
};

static const unsigned char aes_test_ofb_ct[3][64] =
{
    { 0x3B, 0x3F, 0xD9, 0x2E, 0xB7, 0x2D, 0xAD, 0x20,
      0x33, 0x34, 0x49, 0xF8, 0xE8, 0x3C, 0xFB, 0x4A,
      0x77, 0x89, 0x50, 0x8d, 0x16, 0x91, 0x8f, 0x03,
      0xf5, 0x3c, 0x52, 0xda, 0xc5, 0x4e, 0xd8, 0x25,
      0x97, 0x40, 0x05, 0x1e, 0x9c, 0x5f, 0xec, 0xf6,
      0x43, 0x44, 0xf7, 0xa8, 0x22, 0x60, 0xed, 0xcc,
      0x30, 0x4c, 0x65, 0x28, 0xf6, 0x59, 0xc7, 0x78,
      0x66, 0xa5, 0x10, 0xd9, 0xc1, 0xd6, 0xae, 0x5e },
    { 0xCD, 0xC8, 0x0D, 0x6F, 0xDD, 0xF1, 0x8C, 0xAB,
      0x34, 0xC2, 0x59, 0x09, 0xC9, 0x9A, 0x41, 0x74,
      0xfc, 0xc2, 0x8b, 0x8d, 0x4c, 0x63, 0x83, 0x7c,
      0x09, 0xe8, 0x17, 0x00, 0xc1, 0x10, 0x04, 0x01,
      0x8d, 0x9a, 0x9a, 0xea, 0xc0, 0xf6, 0x59, 0x6f,
      0x55, 0x9c, 0x6d, 0x4d, 0xaf, 0x59, 0xa5, 0xf2,
      0x6d, 0x9f, 0x20, 0x08, 0x57, 0xca, 0x6c, 0x3e,
      0x9c, 0xac, 0x52, 0x4b, 0xd9, 0xac, 0xc9, 0x2a },
    { 0xDC, 0x7E, 0x84, 0xBF, 0xDA, 0x79, 0x16, 0x4B,
      0x7E, 0xCD, 0x84, 0x86, 0x98, 0x5D, 0x38, 0x60,
      0x4f, 0xeb, 0xdc, 0x67, 0x40, 0xd2, 0x0b, 0x3a,
      0xc8, 0x8f, 0x6a, 0xd8, 0x2a, 0x4f, 0xb0, 0x8d,
      0x71, 0xab, 0x47, 0xa0, 0x86, 0xe8, 0x6e, 0xed,
      0xf3, 0x9d, 0x1c, 0x5b, 0xba, 0x97, 0xc4, 0x08,
      0x01, 0x26, 0x14, 0x1d, 0x67, 0xf3, 0x7b, 0xe8,
      0x53, 0x8f, 0x5a, 0x8b, 0xe7, 0x40, 0xe4, 0x84 }
};
#endif /* MBEDTLS_CIPHER_MODE_OFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR test vectors from:
 *
 * http://www.faqs.org/rfcs/rfc3686.html
 */

static const unsigned char aes_test_ctr_key[3][16] =
{
    { 0xAE, 0x68, 0x52, 0xF8, 0x12, 0x10, 0x67, 0xCC,
      0x4B, 0xF7, 0xA5, 0x76, 0x55, 0x77, 0xF3, 0x9E },
    { 0x7E, 0x24, 0x06, 0x78, 0x17, 0xFA, 0xE0, 0xD7,
      0x43, 0xD6, 0xCE, 0x1F, 0x32, 0x53, 0x91, 0x63 },
    { 0x76, 0x91, 0xBE, 0x03, 0x5E, 0x50, 0x20, 0xA8,
      0xAC, 0x6E, 0x61, 0x85, 0x29, 0xF9, 0xA0, 0xDC }
};

static const unsigned char aes_test_ctr_nonce_counter[3][16] =
{
    { 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
    { 0x00, 0x6C, 0xB6, 0xDB, 0xC0, 0x54, 0x3B, 0x59,
      0xDA, 0x48, 0xD9, 0x0B, 0x00, 0x00, 0x00, 0x01 },
    { 0x00, 0xE0, 0x01, 0x7B, 0x27, 0x77, 0x7F, 0x3F,
      0x4A, 0x17, 0x86, 0xF0, 0x00, 0x00, 0x00, 0x01 }
};

static const unsigned char aes_test_ctr_pt[3][48] =
{
    { 0x53, 0x69, 0x6E, 0x67, 0x6C, 0x65, 0x20, 0x62,
      0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x6D, 0x73, 0x67 },

    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
      0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F },

    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
      0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
      0x20, 0x21, 0x22, 0x23 }
};

static const unsigned char aes_test_ctr_ct[3][48] =
{
    { 0xE4, 0x09, 0x5D, 0x4F, 0xB7, 0xA7, 0xB3, 0x79,
      0x2D, 0x61, 0x75, 0xA3, 0x26, 0x13, 0x11, 0xB8 },
    { 0x51, 0x04, 0xA1, 0x06, 0x16, 0x8A, 0x72, 0xD9,
      0x79, 0x0D, 0x41, 0xEE, 0x8E, 0xDA, 0xD3, 0x88,
      0xEB, 0x2E, 0x1E, 0xFC, 0x46, 0xDA, 0x57, 0xC8,
      0xFC, 0xE6, 0x30, 0xDF, 0x91, 0x41, 0xBE, 0x28 },
    { 0xC1, 0xCF, 0x48, 0xA8, 0x9F, 0x2F, 0xFD, 0xD9,
      0xCF, 0x46, 0x52, 0xE9, 0xEF, 0xDB, 0x72, 0xD7,
      0x45, 0x40, 0xA4, 0x2B, 0xDE, 0x6D, 0x78, 0x36,
      0xD5, 0x9A, 0x5C, 0xEA, 0xAE, 0xF3, 0x10, 0x53,
      0x25, 0xB2, 0x07, 0x2F }
};

static const int aes_test_ctr_len[3] =
    { 16, 32, 36 };
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#if defined(MBEDTLS_CIPHER_MODE_XTS)
/*
 * AES-XTS test vectors from:
 *
 * IEEE P1619/D16 Annex B
 * https://web.archive.org/web/20150629024421/http://grouper.ieee.org/groups/1619/email/pdf00086.pdf
 * (Archived from original at http://grouper.ieee.org/groups/1619/email/pdf00086.pdf)
 */
static const unsigned char aes_test_xts_key[][32] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
      0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
      0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
      0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 },
    { 0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8,
      0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0,
      0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
      0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 },
};

static const unsigned char aes_test_xts_pt32[][32] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44 },
    { 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44 },
};

static const unsigned char aes_test_xts_ct32[][32] =
{
    { 0x91, 0x7c, 0xf6, 0x9e, 0xbd, 0x68, 0xb2, 0xec,
      0x9b, 0x9f, 0xe9, 0xa3, 0xea, 0xdd, 0xa6, 0x92,
      0xcd, 0x43, 0xd2, 0xf5, 0x95, 0x98, 0xed, 0x85,
      0x8c, 0x02, 0xc2, 0x65, 0x2f, 0xbf, 0x92, 0x2e },
    { 0xc4, 0x54, 0x18, 0x5e, 0x6a, 0x16, 0x93, 0x6e,
      0x39, 0x33, 0x40, 0x38, 0xac, 0xef, 0x83, 0x8b,
      0xfb, 0x18, 0x6f, 0xff, 0x74, 0x80, 0xad, 0xc4,
      0x28, 0x93, 0x82, 0xec, 0xd6, 0xd3, 0x94, 0xf0 },
    { 0xaf, 0x85, 0x33, 0x6b, 0x59, 0x7a, 0xfc, 0x1a,
      0x90, 0x0b, 0x2e, 0xb2, 0x1e, 0xc9, 0x49, 0xd2,
      0x92, 0xdf, 0x4c, 0x04, 0x7e, 0x0b, 0x21, 0x53,
      0x21, 0x86, 0xa5, 0x97, 0x1a, 0x22, 0x7a, 0x89 },
};

static const unsigned char aes_test_xts_data_unit[][16] =
{
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
   { 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
   { 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

#endif /* MBEDTLS_CIPHER_MODE_XTS */

/*
 * Checkup routine
 */
void nwy_mbedtls_aes_self_test(void)
{
    int ret = 0, i, j, u, mode;
    int verbose = 1;
    unsigned int keybits;
    unsigned char key[32];
    unsigned char buf[64];
    const unsigned char *aes_tests;
#if defined(MBEDTLS_CIPHER_MODE_CBC) || defined(MBEDTLS_CIPHER_MODE_CFB)
    unsigned char iv[16];
#endif
#if defined(MBEDTLS_CIPHER_MODE_CBC)
    unsigned char prv[16];
#endif
#if defined(MBEDTLS_CIPHER_MODE_CTR) || defined(MBEDTLS_CIPHER_MODE_CFB) || \
    defined(MBEDTLS_CIPHER_MODE_OFB)
    size_t offset;
#endif
#if defined(MBEDTLS_CIPHER_MODE_CTR) || defined(MBEDTLS_CIPHER_MODE_XTS)
    int len;
#endif
#if defined(MBEDTLS_CIPHER_MODE_CTR)
    unsigned char nonce_counter[16];
    unsigned char stream_block[16];
#endif
    mbedtls_aes_context ctx;

    memset( key, 0, 32 );
    mbedtls_aes_init( &ctx );

    /*
     * ECB mode
     */
    for( i = 0; i < 6; i++ )
    {
        u = i >> 1;
        keybits = 128 + u * 64;
        mode = i & 1;

        if( verbose != 0 )
            nwy_test_cli_echo("\r\n  AES-ECB-%3d (%s): ", keybits,
                            ( mode == MBEDTLS_AES_DECRYPT ) ? "dec" : "enc" );

        memset( buf, 0, 16 );

        if( mode == MBEDTLS_AES_DECRYPT )
        {
            ret = mbedtls_aes_setkey_dec( &ctx, key, keybits );
            aes_tests = aes_test_ecb_dec[u];
        }
        else
        {
            ret = mbedtls_aes_setkey_enc( &ctx, key, keybits );
            aes_tests = aes_test_ecb_enc[u];
        }

        /*
         * AES-192 is an optional feature that may be unavailable when
         * there is an alternative underlying implementation i.e. when
         * MBEDTLS_AES_ALT is defined.
         */
        if( ret == MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED && keybits == 192 )
        {
            nwy_test_cli_echo("\r\nskipped\r\n" );
            continue;
        }
        else if( ret != 0 )
        {
            goto exit;
        }

        for( j = 0; j < 10000; j++ )
        {
            ret = mbedtls_aes_crypt_ecb( &ctx, mode, buf, buf );
            if( ret != 0 )
                goto exit;
        }

        if( memcmp( buf, aes_tests, 16 ) != 0 )
        {
            ret = 1;
            goto exit;
        }

        if( verbose != 0 )
            nwy_test_cli_echo("\r\npassed\r\n" );
    }

    if( verbose != 0 )
        nwy_test_cli_echo("\r\n\r\n" );

#if defined(MBEDTLS_CIPHER_MODE_CBC)
    /*
     * CBC mode
     */
    for( i = 0; i < 6; i++ )
    {
        u = i >> 1;
        keybits = 128 + u * 64;
        mode = i & 1;

        if( verbose != 0 )
            nwy_test_cli_echo("\r\n  AES-CBC-%3d (%s): ", keybits,
                            ( mode == MBEDTLS_AES_DECRYPT ) ? "dec" : "enc" );

        memset( iv , 0, 16 );
        memset( prv, 0, 16 );
        memset( buf, 0, 16 );

        if( mode == MBEDTLS_AES_DECRYPT )
        {
            ret = mbedtls_aes_setkey_dec( &ctx, key, keybits );
            aes_tests = aes_test_cbc_dec[u];
        }
        else
        {
            ret = mbedtls_aes_setkey_enc( &ctx, key, keybits );
            aes_tests = aes_test_cbc_enc[u];
        }

        /*
         * AES-192 is an optional feature that may be unavailable when
         * there is an alternative underlying implementation i.e. when
         * MBEDTLS_AES_ALT is defined.
         */
        if( ret == MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED && keybits == 192 )
        {
            nwy_test_cli_echo("\r\nskipped\r\n" );
            continue;
        }
        else if( ret != 0 )
        {
            goto exit;
        }

        for( j = 0; j < 10000; j++ )
        {
            if( mode == MBEDTLS_AES_ENCRYPT )
            {
                unsigned char tmp[16];

                memcpy( tmp, prv, 16 );
                memcpy( prv, buf, 16 );
                memcpy( buf, tmp, 16 );
            }

            ret = mbedtls_aes_crypt_cbc( &ctx, mode, 16, iv, buf, buf );
            if( ret != 0 )
                goto exit;

        }

        if( memcmp( buf, aes_tests, 16 ) != 0 )
        {
            ret = 1;
            goto exit;
        }

        if( verbose != 0 )
            nwy_test_cli_echo("\r\npassed\r\n" );
    }

    if( verbose != 0 )
        nwy_test_cli_echo("\r\n\r\n" );
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
    /*
     * CFB128 mode
     */
    for( i = 0; i < 6; i++ )
    {
        u = i >> 1;
        keybits = 128 + u * 64;
        mode = i & 1;

        if( verbose != 0 )
            nwy_test_cli_echo("\r\n  AES-CFB128-%3d (%s): ", keybits,
                            ( mode == MBEDTLS_AES_DECRYPT ) ? "dec" : "enc" );

        memcpy( iv,  aes_test_cfb128_iv, 16 );
        memcpy( key, aes_test_cfb128_key[u], keybits / 8 );

        offset = 0;
        ret = mbedtls_aes_setkey_enc( &ctx, key, keybits );
        /*
         * AES-192 is an optional feature that may be unavailable when
         * there is an alternative underlying implementation i.e. when
         * MBEDTLS_AES_ALT is defined.
         */
        if( ret == MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED && keybits == 192 )
        {
            nwy_test_cli_echo("\r\nskipped\r\n" );
            continue;
        }
        else if( ret != 0 )
        {
            goto exit;
        }

        if( mode == MBEDTLS_AES_DECRYPT )
        {
            memcpy( buf, aes_test_cfb128_ct[u], 64 );
            aes_tests = aes_test_cfb128_pt;
        }
        else
        {
            memcpy( buf, aes_test_cfb128_pt, 64 );
            aes_tests = aes_test_cfb128_ct[u];
        }

        ret = mbedtls_aes_crypt_cfb128( &ctx, mode, 64, &offset, iv, buf, buf );
        if( ret != 0 )
            goto exit;

        if( memcmp( buf, aes_tests, 64 ) != 0 )
        {
            ret = 1;
            goto exit;
        }

        if( verbose != 0 )
            nwy_test_cli_echo("\r\npassed\r\n" );
    }

    if( verbose != 0 )
        nwy_test_cli_echo("\r\n\r\n" );
#endif /* MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_OFB)
    /*
     * OFB mode
     */
    for( i = 0; i < 6; i++ )
    {
        u = i >> 1;
        keybits = 128 + u * 64;
        mode = i & 1;

        if( verbose != 0 )
            nwy_test_cli_echo("\r\n  AES-OFB-%3d (%s): ", keybits,
                            ( mode == MBEDTLS_AES_DECRYPT ) ? "dec" : "enc" );

        memcpy( iv,  aes_test_ofb_iv, 16 );
        memcpy( key, aes_test_ofb_key[u], keybits / 8 );

        offset = 0;
        ret = mbedtls_aes_setkey_enc( &ctx, key, keybits );
        /*
         * AES-192 is an optional feature that may be unavailable when
         * there is an alternative underlying implementation i.e. when
         * MBEDTLS_AES_ALT is defined.
         */
        if( ret == MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED && keybits == 192 )
        {
            nwy_test_cli_echo("\r\nskipped\r\n" );
            continue;
        }
        else if( ret != 0 )
        {
            goto exit;
        }

        if( mode == MBEDTLS_AES_DECRYPT )
        {
            memcpy( buf, aes_test_ofb_ct[u], 64 );
            aes_tests = aes_test_ofb_pt;
        }
        else
        {
            memcpy( buf, aes_test_ofb_pt, 64 );
            aes_tests = aes_test_ofb_ct[u];
        }

        ret = mbedtls_aes_crypt_ofb( &ctx, 64, &offset, iv, buf, buf );
        if( ret != 0 )
            goto exit;

        if( memcmp( buf, aes_tests, 64 ) != 0 )
        {
            ret = 1;
            goto exit;
        }

        if( verbose != 0 )
            nwy_test_cli_echo("\r\npassed\r\n" );
    }

    if( verbose != 0 )
        nwy_test_cli_echo("\r\n\r\n" );
#endif /* MBEDTLS_CIPHER_MODE_OFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
    /*
     * CTR mode
     */
    for( i = 0; i < 6; i++ )
    {
        u = i >> 1;
        mode = i & 1;

        if( verbose != 0 )
            nwy_test_cli_echo("\r\n  AES-CTR-128 (%s): ",
                            ( mode == MBEDTLS_AES_DECRYPT ) ? "dec" : "enc" );

        memcpy( nonce_counter, aes_test_ctr_nonce_counter[u], 16 );
        memcpy( key, aes_test_ctr_key[u], 16 );

        offset = 0;
        if( ( ret = mbedtls_aes_setkey_enc( &ctx, key, 128 ) ) != 0 )
            goto exit;

        len = aes_test_ctr_len[u];

        if( mode == MBEDTLS_AES_DECRYPT )
        {
            memcpy( buf, aes_test_ctr_ct[u], len );
            aes_tests = aes_test_ctr_pt[u];
        }
        else
        {
            memcpy( buf, aes_test_ctr_pt[u], len );
            aes_tests = aes_test_ctr_ct[u];
        }

        ret = mbedtls_aes_crypt_ctr( &ctx, len, &offset, nonce_counter,
                                     stream_block, buf, buf );
        if( ret != 0 )
            goto exit;

        if( memcmp( buf, aes_tests, len ) != 0 )
        {
            ret = 1;
            goto exit;
        }

        if( verbose != 0 )
            nwy_test_cli_echo("\r\npassed\r\n" );
    }

    if( verbose != 0 )
        nwy_test_cli_echo("\r\n\r\n" );
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#if defined(MBEDTLS_CIPHER_MODE_XTS)
    {
    static const int num_tests =
        sizeof(aes_test_xts_key) / sizeof(*aes_test_xts_key);
    mbedtls_aes_xts_context ctx_xts;

    /*
     * XTS mode
     */
    mbedtls_aes_xts_init( &ctx_xts );

    for( i = 0; i < num_tests << 1; i++ )
    {
        const unsigned char *data_unit;
        u = i >> 1;
        mode = i & 1;

        if( verbose != 0 )
            nwy_test_cli_echo("\r\n  AES-XTS-128 (%s): ",
                            ( mode == MBEDTLS_AES_DECRYPT ) ? "dec" : "enc" );

        memset( key, 0, sizeof( key ) );
        memcpy( key, aes_test_xts_key[u], 32 );
        data_unit = aes_test_xts_data_unit[u];

        len = sizeof( *aes_test_xts_ct32 );

        if( mode == MBEDTLS_AES_DECRYPT )
        {
            ret = mbedtls_aes_xts_setkey_dec( &ctx_xts, key, 256 );
            if( ret != 0)
                goto exit;
            memcpy( buf, aes_test_xts_ct32[u], len );
            aes_tests = aes_test_xts_pt32[u];
        }
        else
        {
            ret = mbedtls_aes_xts_setkey_enc( &ctx_xts, key, 256 );
            if( ret != 0)
                goto exit;
            memcpy( buf, aes_test_xts_pt32[u], len );
            aes_tests = aes_test_xts_ct32[u];
        }


        ret = mbedtls_aes_crypt_xts( &ctx_xts, mode, len, data_unit,
                                     buf, buf );
        if( ret != 0 )
            goto exit;

        if( memcmp( buf, aes_tests, len ) != 0 )
        {
            ret = 1;
            goto exit;
        }

        if( verbose != 0 )
            nwy_test_cli_echo("\r\npassed\r\n" );
    }

    if( verbose != 0 )
        nwy_test_cli_echo("\r\n\r\n" );

    mbedtls_aes_xts_free( &ctx_xts );
    }
#endif /* MBEDTLS_CIPHER_MODE_XTS */

    ret = 0;

exit:
    if( ret != 0 && verbose != 0 )
        nwy_test_cli_echo("\r\nfailed\r\n" );

    mbedtls_aes_free( &ctx );

    return;
}
int encrypt_data_by_aes_cbc(unsigned char *input, int input_length,unsigned char **encrypted_data) {
    mbedtls_aes_context aes_context;
    int result;
    int padding;
    int real_input_length;
    unsigned char *input_tmp;
    unsigned char error[64];
    const unsigned char key[16] = "1234567890ABCDEF";
    unsigned char iv[16] = "0099887766554433";

    // PKCS7 padding
    /*
    AES is a block cipher algorithm, where a block length must be 16 bytes and all data to be encrypted must be an integer multiple of 16 bytes.
    If the data to be encrypted is not an integer multiple of 16 bytes, it must be filled to an integer multiple of 16 bytes first. 
    Common filling methods include PKCS5 and PKCS7, with PKCS7 commonly used for filling.
    PKCS7 is padded to an integer multiple of 16 bytes. If the data to be encrypted happens to be an integer multiple of 16 bytes,
    16 bytes still need to be padded Calculate the difference between the current length distance and multiples of 16, 
    which is the value for filling in the recharge
     */
    padding = 16 - (input_length % 16);
    real_input_length = input_length + padding;
    input_tmp = malloc(real_input_length);
    if (input_tmp == NULL) {
        return -1; // Memory allocation failed
    }
    memcpy(input_tmp, input, input_length);
    memset(input_tmp + input_length, padding, padding);

    mbedtls_aes_init(&aes_context);
    //If AES-128 is used, the necessary length is 128 bits. If AES-256 is used, the key length is 256 bits
    result = mbedtls_aes_setkey_enc(&aes_context, key, 128);
    if (result != 0) {
        #ifdef MBEDTLS_ERROR_C
        mbedtls_strerror(result, error, sizeof(error));
        #endif
        goto error_exit;
    }

    *encrypted_data = malloc(real_input_length);
    if (*encrypted_data == NULL) {
        result = -1; // Memory allocation failed
        goto error_exit;
    }

    result = mbedtls_aes_crypt_cbc(&aes_context, MBEDTLS_AES_ENCRYPT, real_input_length, iv, input_tmp, *encrypted_data);
    if (result != 0) {
        #ifdef MBEDTLS_ERROR_C
        mbedtls_strerror(result, error, sizeof(error));
        #endif
        goto error_exit;
    }

    free(input_tmp);
    mbedtls_aes_free(&aes_context);
    return real_input_length;

error_exit:
    if (input_tmp != NULL) {
        free(input_tmp);
    }
    if (*encrypted_data != NULL) {
        free(*encrypted_data);
        *encrypted_data = NULL;
    }
    mbedtls_aes_free(&aes_context);
    return result;
}

int decrypt_data_by_aes_cbc(unsigned char *encrypted_data, int encrypted_length, unsigned char **decrypted_data) {
    mbedtls_aes_context aes_context;
    int result = 0;
    int padding = 0;
    unsigned char padding_code;
    int decrypted_data_length = 0;
    const unsigned char key[16] = "1234567890ABCDEF";
    unsigned char iv[16] = "0099887766554433";
    unsigned char error[64];
    unsigned char decrypted_with_padding[encrypted_length];

    mbedtls_aes_init(&aes_context);
    result = mbedtls_aes_setkey_dec(&aes_context, key, 128);
    if (result != 0) {
        #ifdef MBEDTLS_ERROR_C
        mbedtls_strerror(result, error, sizeof(error));
        #endif
        nwy_test_cli_echo("\r\nFailed to set key: %s\r\n", error);
        goto error_exit;
    }

    *decrypted_data = malloc(encrypted_length);
    if (*decrypted_data == NULL) {
        nwy_test_cli_echo("\r\nMemory allocation failed\r\n");
        goto error_exit;
    }
    memset(*decrypted_data, 0x00, encrypted_length);


    result = mbedtls_aes_crypt_cbc(&aes_context, MBEDTLS_AES_DECRYPT, encrypted_length, iv, encrypted_data, decrypted_with_padding);
    if (result != 0) {
        #ifdef MBEDTLS_ERROR_C
        mbedtls_strerror(result, error, sizeof(error));
        #endif
        nwy_test_cli_echo("\r\nFailed to decrypt data: %s\r\n", error);
        goto error_exit;
    }

    // Check and remove PKCS#7 padding
    padding_code = decrypted_with_padding[encrypted_length - 1];
    padding = (int)padding_code;
    if (padding < 1 || padding > 16 || encrypted_length < padding) {
        nwy_test_cli_echo("\r\nPadding code is illegal!\r\n");
        goto error_exit;
    }
    // Verify padding bytes
    for (int i = encrypted_length - padding; i < encrypted_length; i++) {
        if (decrypted_with_padding[i] != padding_code) {
            nwy_test_cli_echo("\r\nInvalid padding detected!\r\n");
            goto error_exit;
        }
    }
    decrypted_data_length = encrypted_length - padding;
    memcpy(*decrypted_data, decrypted_with_padding, decrypted_data_length);

    mbedtls_aes_free(&aes_context);
    return decrypted_data_length;

error_exit:
    mbedtls_aes_free(&aes_context);
    if (*decrypted_data != NULL) {
        free(*decrypted_data);
        *decrypted_data = NULL;
    }
    return -1;
}

int test_mbedtls_aes_cbc(unsigned char *input, int input_len) {
    unsigned char *encrypted_data = NULL;
    unsigned char *decrypted_data = NULL;
    int result = 0;

    nwy_test_cli_echo("\n plain data (%d):%s\r\n", input_len,input);
    nwy_test_cli_echo("\n plain data (%d):hex\r\n", input_len);
    for (int i = 0; i < input_len; i++) {
        nwy_test_cli_echo("%02x", input[i]);
    }
    nwy_test_cli_echo("\r\n");

    int encrypted_data_length = encrypt_data_by_aes_cbc(input, input_len, &encrypted_data);
    if (encrypted_data_length < 0) {
        nwy_test_cli_echo("Failed to encrypt data!\r\n");
        result = -1;
        goto cleanup;
    }
    if (encrypted_data_length % 16 != 0) {
        nwy_test_cli_echo("Encryption failed, data length is not a multiple of 16 (%d)!\r\n", encrypted_data_length);
        result = -1;
        goto cleanup;
    }

    nwy_test_cli_echo("\n encrypted data (%d):\r\n", encrypted_data_length);
    for (int i = 0; i < encrypted_data_length; i++) {
        nwy_test_cli_echo("%02x", encrypted_data[i]);
    }
    nwy_test_cli_echo("\r\n");

    int decrypted_data_length = decrypt_data_by_aes_cbc(encrypted_data, encrypted_data_length, &decrypted_data);
    if (decrypted_data_length < 0) {
        nwy_test_cli_echo("Failed to decrypt data!\r\n");
        result = -1;
        goto cleanup;
    }

    nwy_test_cli_echo("\ndecrypted data (%d):%s\r\n", decrypted_data_length,decrypted_data);
    nwy_test_cli_echo("\ndecrypted data (%d):hex\r\n", decrypted_data_length);
    for (int i = 0; i < decrypted_data_length; i++) {
        nwy_test_cli_echo("%02x", decrypted_data[i]);
    }
    nwy_test_cli_echo("\r\n");

cleanup:
    if (encrypted_data != NULL) {
        free(encrypted_data);
    }
    if (decrypted_data != NULL) {
        free(decrypted_data);
    }
    return result;
}



void nwy_mbedtls_aes_test(void)
{
    unsigned char input[256] = {0};
    int index = 0;
    nwy_test_cli_echo("nwy_mbedtls_aes_test begin\r\n");

    memset(input,0x00,sizeof(input));
    nwy_test_cli_echo("test_mbedtls_aes_cbc %d\r\n",index++);
    snprintf(input,sizeof(input),"%s","1231231231112223");
    test_mbedtls_aes_cbc(input,strlen(input));
    nwy_test_cli_echo("test_mbedtls_aes_cbc %d\r\n",index++);

    memset(input,0x00,sizeof(input));
    snprintf(input,sizeof(input),"%s","1231231231112223asdasdasd     asdasdasd   123");
    test_mbedtls_aes_cbc(input,strlen(input));

    nwy_test_cli_echo("test_mbedtls_aes_cbc %d\r\n",index++);

    memset(input,0x00,sizeof(input));
    snprintf(input,sizeof(input),"%s","1231231231112223zxczxczxc hello neoway");
    test_mbedtls_aes_cbc(input,strlen(input));

    nwy_test_cli_echo("test_mbedtls_aes_cbc %d\r\n",index++);

    memset(input,0x00,sizeof(input));
    snprintf(input,sizeof(input),"%s","1231231231112223sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss");
    test_mbedtls_aes_cbc(input,strlen(input));

    memset(input,0x00,sizeof(input));
    snprintf(input,sizeof(input),"%s","neoway hello");
    test_mbedtls_aes_cbc(input,strlen(input));


    nwy_mbedtls_aes_self_test();
    nwy_test_cli_echo("nwy_mbedtls_aes_test over\r\n");


}
#endif


