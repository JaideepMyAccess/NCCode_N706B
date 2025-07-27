/**
 * \file asros_mbedtls_config.h
 *
 * \brief Minimal configuration of features that do not require an entropy source
 */
/*
 *  Copyright (C) 2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 * Minimal configuration of features that do not require an entropy source
 * Distinguishing reatures:
 * - no entropy module
 * - no TLS protocol implementation available due to absence of an entropy
 *   source
 *
 * See README.txt for usage instructions.
 */

#ifndef MBEDTLS_CONFIG_DM_H
#define MBEDTLS_CONFIG_DM_H

//this is Minimal configuration of features for data module 
/*mbedtls macro sync define begin:*/
/*mbedtls macro sync define begin:*/
/*mbedtls macro sync define begin:*/

/* System support */
#define MBEDTLS_HAVE_ASM
#define MBEDTLS_HAVE_TIME
#define MBEDTLS_TIMING_ALT

#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
//#define MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED

/* mbed TLS feature support */
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CIPHER_PADDING_PKCS7
#define MBEDTLS_REMOVE_ARC4_CIPHERSUITES
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
//#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
//#define MBEDTLS_ECP_DP_CURVE25519_ENABLED
//#define MBEDTLS_ECP_NIST_OPTIM
//#define MBEDTLS_ECDSA_DETERMINISTIC // closed for reduce code size
#define MBEDTLS_PK_RSA_ALT_SUPPORT
#define MBEDTLS_PKCS1_V15
#define MBEDTLS_PKCS1_V21
//#define MBEDTLS_SELF_TEST // closed for reduce code size
//#define MBEDTLS_VERSION_FEATURES //closed for reduce code size
//#define MBEDTLS_X509_CHECK_KEY_USAGE
//#define MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE

/* mbed TLS modules */
#define MBEDTLS_AES_C
#define MBEDTLS_ASN1_PARSE_C
//#define MBEDTLS_ASN1_WRITE_C // closed for reduce code size
#define MBEDTLS_BASE64_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_CCM_C
#define MBEDTLS_CIPHER_C

#define MBEDTLS_ECDH_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C // closed for reduce code size
#define MBEDTLS_ECP_C // closed for reduce code size

//#define MBEDTLS_ERROR_C //closed for reduce code size
#define MBEDTLS_GCM_C
//#define MBEDTLS_HMAC_DRBG_C // closed for reduce code size
#define MBEDTLS_MD_C
#define MBEDTLS_OID_C
#define MBEDTLS_PEM_PARSE_C
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_PK_WRITE_C
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_RSA_C
#define MBEDTLS_SHA224_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SHA512_C
//#define MBEDTLS_VERSION_C // closed for reduce code size
#define MBEDTLS_X509_USE_C
#define MBEDTLS_X509_CRT_PARSE_C
#define MBEDTLS_X509_CRL_PARSE_C
#define MBEDTLS_CMAC_C
#define MBEDTLS_MD5_C
#define MBEDTLS_NET_C
#define MBEDTLS_SHA1_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_SSL_CLI_C
//#define MBEDTLS_SSL_SRV_C
//#define MBEDTLS_CERTS_C // need open?
//#define MBEDTLS_SSL_SESSION_TICKETS
#define MBEDTLS_SSL_RENEGOTIATION

#if defined(MBEDTLS_CONFIG_TLS_DEBUG)
#define MBEDTLS_DEBUG_C // closed for reduce code size
#endif

#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_KEEP_PEER_CERTIFICATE

#define MBEDTLS_DHM_C
//#define MBEDTLS_CAMELLIA_C
#define MBEDTLS_DES_C
//#define MBEDTLS_ARC4_C

/**
 * \def MBEDTLS_X509_ALLOW_UNSUPPORTED_CRITICAL_EXTENSION
 *
 * If set, the X509 parser will not break-off when parsing an X509 certificate
 * and encountering an unknown critical extension.
 *
 * \warning Depending on your PKI use, enabling this can be a security risk!
 *
 * Uncomment to prevent an error.
 */
#ifdef LPA_SUPPORT
#define MBEDTLS_X509_ALLOW_UNSUPPORTED_CRITICAL_EXTENSION
#endif
#ifdef FEATURE_NWY
#define MBEDTLS_SSL_CIPHERSUITES \
						MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384, \
						MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA, \
						MBEDTLS_TLS_DHE_PSK_WITH_AES_128_CCM_8, \
						MBEDTLS_TLS_PSK_WITH_AES_128_CCM_8, \
						MBEDTLS_TLS_PSK_WITH_AES_128_CBC_SHA, \
						MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256, \
						MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256, \
						MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA, \
						MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA,\
						MBEDTLS_TLS1_3_AES_128_GCM_SHA256,\
						MBEDTLS_TLS1_3_AES_256_GCM_SHA384,\
						MBEDTLS_TLS1_3_CHACHA20_POLY1305_SHA256,\
						MBEDTLS_TLS1_3_AES_128_CCM_SHA256,\
                        MBEDTLS_TLS1_3_AES_128_CCM_8_SHA256, \
                        MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256

#else
#define MBEDTLS_SSL_CIPHERSUITES \
			MBEDTLS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384, \
			MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA, \
			MBEDTLS_TLS_DHE_PSK_WITH_AES_128_CCM_8, \
			MBEDTLS_TLS_PSK_WITH_AES_128_CCM_8, \
			MBEDTLS_TLS_PSK_WITH_AES_128_CBC_SHA, \
			MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256, \
			MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256, \
			MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA, \
			MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA
#endif

/*mbedtls macro sync define end:*/
/*mbedtls macro sync define end:*/
/*mbedtls macro sync define end:*/

#endif /* MBEDTLS_CONFIG_DM_H */
