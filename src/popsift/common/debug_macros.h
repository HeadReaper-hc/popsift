/*
 * Copyright 2016, Simula Research Laboratory
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include <cuda_runtime.h>

void pop_check_last_error( const char* file,
                           size_t      line );
#define POP_CHK pop_check_last_error( __FILE__, __LINE__ )

namespace popsift {
namespace cuda {
void malloc_dev( void** ptr, int sz,
                 const char* file, int line );

void malloc_hst( void** ptr, int sz,
                 const char* file, int line );

template<class T>
T* malloc_devT( int num, const char* file, int line ) {
    void* ptr;
    malloc_dev( &ptr, num*sizeof(T), file, line );
    return (T*)ptr;
}

template<class T>
T* malloc_hstT( int num, const char* file, int line ) {
    void* ptr;
    malloc_hst( &ptr, num*sizeof(T), file, line );
    return (T*)ptr;
}

void memcpy_sync( void* dst, const void* src, size_t sz,
                   cudaMemcpyKind type,
                   const char* file, size_t line );
#define popcuda_memcpy_sync( dst, src, sz, type ) \
    popsift::cuda::memcpy_sync( dst, src, sz, type, __FILE__, __LINE__ )

void memcpy_async( void* dst, const void* src, size_t sz,
                   cudaMemcpyKind type, cudaStream_t stream,
                   const char* file, size_t line );
#define popcuda_memcpy_async( dst, src, sz, type, stream ) \
    popsift::cuda::memcpy_async( dst, src, sz, type, stream, __FILE__, __LINE__ )

void memset_sync( void* ptr, int value, size_t bytes, const char* file, size_t line );
#define popcuda_memset_sync( ptr, val, sz ) \
    popsift::cuda::memset_sync( ptr, val, sz, __FILE__, __LINE__ )

void memset_async( void* ptr, int value, size_t bytes, cudaStream_t stream, const char* file, size_t line );
#define popcuda_memset_async( ptr, val, sz, stream ) \
    popsift::cuda::memset_async( ptr, val, sz, stream, __FILE__, __LINE__ )

cudaStream_t stream_create( const char* file, size_t line );
void         stream_destroy( cudaStream_t s, const char* file, size_t line );
cudaEvent_t  event_create( const char* file, size_t line );
void         event_destroy( cudaEvent_t ev, const char* file, size_t line );
};
};

#define POP_FATAL(s) { \
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl << "    " << s << std::endl; \
        exit( -__LINE__ ); \
    }

#define POP_FATAL_FL(s,file,line) { \
        std::cerr << file << ":" << line << std::endl << "    " << s << std::endl; \
        exit( -__LINE__ ); \
    }

#define POP_CHECK_NON_NULL(ptr,s) if( ptr == 0 ) { POP_FATAL_FL(s,__FILE__,__LINE__); }

#define POP_CHECK_NON_NULL_FL(ptr,s,file,line) if( ptr == 0 ) { POP_FATAL_FL(s,file,line); }

#define POP_INFO(s)
// #define POP_INFO(s) cerr << __FILE__ << ":" << __LINE__ << std::endl << "    " << s << endl

#define POP_INFO2(silent,s) \
    if (not silent) { \
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl << "    " << s << std::endl; \
    }

#define POP_CUDA_FATAL(err,s) { \
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl; \
        std::cerr << "    " << s << cudaGetErrorString(err) << std::endl; \
        exit( -__LINE__ ); \
    }
#define POP_CUDA_FATAL_TEST(err,s) if( err != cudaSuccess ) { POP_CUDA_FATAL(err,s); }

#define POP_CUDA_FREE( ptr ) { \
        cudaError_t err; \
        err = cudaFree( ptr ); \
        POP_CUDA_FATAL_TEST( err, "cudaFree failed: " ); \
    }

#define POP_CUDA_MALLOC_HOST( ptr, sz ) { \
        cudaError_t err; \
        err = cudaMallocHost( ptr, sz ); \
        POP_CUDA_FATAL_TEST( err, "cudaMallocHost failed: " ); \
    }

#define POP_CUDA_FREE_HOST( ptr ) { \
        cudaError_t err; \
        err = cudaFreeHost( ptr ); \
        POP_CUDA_FATAL_TEST( err, "cudaFreeHost failed: " ); \
    }

