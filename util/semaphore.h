// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Based on
// https://cs.chromium.org/chromium/src/v8/src/base/platform/semaphore.h

#ifndef UTIL_SEMAPHORE_H_
#define UTIL_SEMAPHORE_H_

#include "base/macros.h"
#include "util/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_MACOSX)
#include <mach/mach.h>
#elif defined(OS_LINUX)
#include <semaphore.h>
#else
#include <mutex>
#include <condition_variable>
class semaphore_imp
{
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0;

public:
    semaphore_imp(int count);
    void notify();
    void wait();
    bool try_wait();
};
#endif

class Semaphore {
 public:
  explicit Semaphore(int count);
  ~Semaphore();

  // Increments the semaphore counter.
  void Signal();

  // Decrements the semaphore counter if it is positive, or blocks until it
  // becomes positive and then decrements the counter.
  void Wait();

#if defined(OS_MACOSX)
  typedef semaphore_t NativeHandle;
#elif defined(OS_LINUX)
  typedef sem_t NativeHandle;
#elif defined(OS_WIN)
  typedef HANDLE NativeHandle;
#else
  typedef semaphore_imp* NativeHandle;
#endif

  NativeHandle& native_handle() { return native_handle_; }
  const NativeHandle& native_handle() const { return native_handle_; }

 private:
  NativeHandle native_handle_;

  DISALLOW_COPY_AND_ASSIGN(Semaphore);
};

#endif  // UTIL_SEMAPHORE_H_
