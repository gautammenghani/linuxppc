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

static void py_perf_thread_map_dealloc(py_perf_thread_map *thread_map)
{
	Py_DECREF(thread_map);
	PyObject_Del((PyObject *)thread_map);
}

static PyTypeObject py_perf_thread_map_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_thread_map",
	.tp_doc = "Perf thread map object",
	.tp_basicsize = sizeof(py_perf_thread_map),
	.tp_dealloc = (destructor)py_perf_thread_map_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
};

static void
python_push_type(const char *name, PyObject *module, PyTypeObject *type)
{
	if (PyType_Ready(type) == -1)
		printf("python_push_type: failed to ready %s", name);

	Py_INCREF(type);
}

LIBPERF_API PyMODINIT_FUNC PyInit_libperf(void);

#endif /* __LIBPERF_PY_PERF_H */
