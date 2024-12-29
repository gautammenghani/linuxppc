/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBPERF_PY_PERF_H
#define __LIBPERF_PY_PERF_H

#define PY_SSIZE_T_CLEAN
#include <perf/threadmap.h>
#include <Python.h>

typedef struct {
	PyObject_HEAD
	struct perf_thread_map *ptr;
} py_perf_thread_map;

LIBPERF_API PyMODINIT_FUNC PyInit_libperf(void);

#endif /* __LIBPERF_PY_PERF_H */
