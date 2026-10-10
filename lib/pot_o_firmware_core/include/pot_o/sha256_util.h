#ifndef SHA256_UTIL_H
#define SHA256_UTIL_H

#include <stdint.h>
#include <stddef.h>

// SHA-256 context. The storage backend is platform-specific: mbedtls on ESP32
// (hardware-accelerated when CONFIG_MBEDTLS_HARDWARE_SHA=y) and BearSSL on
// ESP8266. The complete type is exposed so callers can allocate it on the stack.
#if defined(ESP32S_DEVICE)
  #include <mbedtls/sha256.h>
  typedef struct SHA256Ctx { mbedtls_sha256_context ctx; } SHA256Ctx;
#else
  #include <bearssl/bearssl_hash.h>
  typedef struct SHA256Ctx { br_sha256_context ctx; } SHA256Ctx;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void sha256_raw(const uint8_t* data, size_t len, uint8_t out[32]);

void sha256_init(SHA256Ctx* c);
void sha256_update(SHA256Ctx* c, const uint8_t* data, size_t len);
void sha256_finish(SHA256Ctx* c, uint8_t out[32]);

void bytes_to_hex(const uint8_t* in, size_t len, char* out);
int hex_to_bytes(const char* hex, uint8_t* out, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif // SHA256_UTIL_H
