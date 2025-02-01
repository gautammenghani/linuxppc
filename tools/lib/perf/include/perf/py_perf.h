/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBPERF_PY_PERF_H
#define __LIBPERF_PY_PERF_H

#define PY_SSIZE_T_CLEAN
#include <stdlib.h>
#include <perf/threadmap.h>
#include <Python.h>

typedef struct {
	PyObject_HEAD
	struct perf_thread_map *ptr;
} py_perf_thread_map;

static void py_perf_thread_map_dealloc(py_perf_thread_map *thread_map)
{
	free(thread_map->ptr);
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

// perf_evlist declarations

typedef struct {
	PyObject_HEAD
	struct perf_evlist *ptr;
} py_perf_evlist;

static void py_perf_evlist_dealloc(py_perf_evlist *evlist)
{
	free(evlist->ptr);
	Py_DECREF(evlist);
	PyObject_Del((PyObject *)evlist);
}

static PyTypeObject py_perf_evlist_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_evlist",
	.tp_doc = "Perf evlist object",
	.tp_basicsize = sizeof(py_perf_evlist),
	.tp_dealloc = (destructor)py_perf_evlist_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
};

// perf_evsel declarations
typedef struct {
	PyObject_HEAD
	struct perf_evsel *ptr;
} py_perf_evsel;

static void py_perf_evsel_dealloc(py_perf_evsel *evsel)
{
	free(evsel->ptr);
	Py_DECREF(evsel);
	PyObject_Del((PyObject *)evsel);
}

static PyTypeObject py_perf_evsel_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_evsel",
	.tp_doc = "Perf evsel object",
	.tp_basicsize = sizeof(py_perf_evsel),
	.tp_dealloc = (destructor)py_perf_evsel_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
};

// perf_cpu_map declarations
typedef struct {
	PyObject_HEAD
	struct perf_cpu_map *ptr;
} py_perf_cpu_map;

static void py_perf_cpu_map_dealloc(py_perf_cpu_map *cpu_map)
{
	free(cpu_map->ptr);
	Py_DECREF(cpu_map);
	PyObject_Del((PyObject *)cpu_map);
}

static PyTypeObject py_perf_cpu_map_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_cpu_map",
	.tp_doc = "Perf cpu_map object",
	.tp_basicsize = sizeof(py_perf_cpu_map),
	.tp_dealloc = (destructor)py_perf_cpu_map_dealloc,
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
