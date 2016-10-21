#ifndef __LIBRACE_H__
#define __LIBRACE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void store_8(void *addr, uint8_t val) { *((uint8_t *)addr) = val; }
	void store_16(void *addr, uint16_t val) { *((uint16_t *)addr) = val; }
	void store_32(void *addr, uint32_t val) { *((uint32_t *)addr) = val; }
	void store_64(void *addr, uint64_t val) { *((uint64_t *)addr) = val; }

	void store_8_(void *addr);
	void store_16_(void *addr);
	void store_32_(void *addr);
	void store_64_(void *addr);

	uint8_t load_8(const void *addr) { return *((uint8_t *)addr); }
	uint16_t load_16(const void *addr) { return *((uint16_t *)addr); }
	uint32_t load_32(const void *addr) { return *((uint32_t *)addr); }
	uint64_t load_64(const void *addr) { return *((uint64_t *)addr); }

#ifdef __cplusplus
}
#endif

#endif  // __LIBRACE_H__
