#ifndef __STDATOMIC_H__
#define __STDATOMIC_H__

#ifdef __cplusplus

#include <atomic>

using std::atomic_flag;
using std::atomic_bool;
//using std::atomic_address;
using std::atomic_char;
using std::atomic_schar;
using std::atomic_uchar;
using std::atomic_short;
using std::atomic_ushort;
using std::atomic_int;
using std::atomic_uint;
using std::atomic_long;
using std::atomic_ulong;
using std::atomic_llong;
using std::atomic_ullong;
using std::atomic_wchar_t;

using std::atomic_size_t;
using std::atomic_uintptr_t;

using std::atomic;
using std::memory_order;
using std::memory_order_relaxed;
using std::memory_order_acquire;
using std::memory_order_release;
using std::memory_order_acq_rel;
using std::memory_order_seq_cst;

using std::atomic_thread_fence;
using std::atomic_signal_fence;

using std::atomic_init;

#define atomic_init(A, V) *A=V

#define atomic_load_explicit(A, MO) (*A).load(MO)
#define atomic_store_explicit(A, V, MO) (*A).store(V, MO)

#define atomic_fetch_add_explicit(A, V, MO) (*A).fetch_add(V, MO);
#define atomic_fetch_sub_explicit(A, V, MO) (*A).fetch_sub(V, MO);
#define atomic_compare_exchange_strong_explicit(A, E, V, MO, FMO) \
    (*A).compare_exchange_strong(*E, V, MO, FMO)

#endif

#endif  // __STDATOMIC_H__
