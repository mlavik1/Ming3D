#ifndef MING3D_ASSERT_H
#define MING3D_Assert_H

#include "debug.h"

#ifdef _WIN32
#include <intrin.h>
#include <assert.h>
#else
#include <csignal>
#endif

#ifdef _WIN32
#define __Assert(expr) \
	if(!(expr)) { \
		LOG_ERROR() << "Assertion error on " << #expr; \
		__debugbreak(); \
	}

#define __AssertComment(expr, comment) \
	if(!(expr)) { \
		LOG_ERROR() << "Assertion error on " << #expr << ", \"" << comment << "\""; \
		__debugbreak(); \
	}
#else
#define __Assert(expr) \
	if(!(expr)) { \
		LOG_ERROR() << "Assertion error on " << #expr; \
		raise(SIGTRAP); \
	}

#define __AssertComment(expr, comment) \
	if(!(expr)) { \
		LOG_ERROR() << "Assertion error on " << #expr << ", \"" << comment << "\""; \
		raise(SIGTRAP); \
	}
#endif
#endif

