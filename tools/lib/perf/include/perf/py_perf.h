/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBPERF_PY_PERF_H
#define __LIBPERF_PY_PERF_H

#define PY_SSIZE_T_CLEAN
#include <stdlib.h>
#include <perf/threadmap.h>
#include <structmember.h>
#include <Python.h>

// atomic_t
typedef struct {
	PyObject_HEAD
	int counter;
} py_atomic_t;

static PyMemberDef py_atomic_t_members[] = {
    {"counter", T_INT, offsetof(atomic_t, counter), 0, NULL},
};

static void py_atomic_t_dealloc(py_atomic_t *ctr)
{
	Py_DECREF(ctr);
	PyObject_Del((PyObject *)ctr);
}

static PyTypeObject py_atomic_t_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_atomic_t",
	.tp_doc = "Perf thread map object",
	.tp_basicsize = sizeof(py_atomic_t),
	.tp_dealloc = (destructor)py_atomic_t_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_members = py_atomic_t_members,
};

// refcount
typedef struct {
	PyObject_HEAD
	atomic_t refs;
} py_refcount;

static PyObject * py_refcount_get_refs(py_refcount *self, void *closure)
{
	return (PyObject *)&(self->refs);
}

static int py_refcount_set_refs(py_refcount *self, PyObject *value, void *closure)
{
    int refs;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The refs attribute must be an int");
        return -1;
    }
    refs = PyLong_AsLong(value);
    atomic_set(&self->refs, refs);
    return 0;
}

static PyGetSetDef py_refcount_getsetters[] = {
    {"refs",
     (getter)py_refcount_get_refs, (setter)py_refcount_set_refs,
     "atomic refs", NULL},
    {NULL}
};

static void py_refcount_dealloc(py_refcount *ctr)
{
	Py_DECREF(ctr);
	PyObject_Del((PyObject *)ctr);
}

static PyTypeObject py_refcount_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_refcount",
	.tp_doc = "Perf thread map object",
	.tp_basicsize = sizeof(py_refcount),
	.tp_dealloc = (destructor)py_refcount_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_getset = py_refcount_getsetters,
};

// thread_map_data
typedef struct {
	PyObject_HEAD
	struct thread_map_data tmap_data;
} py_thread_map_data;

static PyMemberDef py_thread_map_data_members[] = {
    {"pid", T_INT, offsetof(py_thread_map_data, tmap_data.pid), 0,  NULL},
    {"comm", T_STRING, offsetof(py_thread_map_data, tmap_data.comm), 0,  NULL},
};

static void py_thread_map_data_dealloc(py_thread_map_data *ctr)
{
	Py_DECREF(ctr);
	PyObject_Del((PyObject *)ctr);
}

static PyTypeObject py_thread_map_data_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_thread_map_data",
	.tp_doc = "Perf thread map object",
	.tp_basicsize = sizeof(py_thread_map_data),
	.tp_dealloc = (destructor)py_thread_map_data_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_members = py_thread_map_data_members,
};

// perf_thread_map
typedef struct {
	PyObject_HEAD
	struct perf_thread_map *perf_tmap;
} py_perf_thread_map;

static PyObject * py_perf_thread_map_get_refcnt(py_perf_thread_map *self, void *closure)
{
	return PyLong_FromLong(atomic_read(&(self->perf_tmap->refcnt.refs)));
}

static int py_perf_thread_map_set_refcnt(py_perf_thread_map *self, PyObject *value, void *closure)
{
    int new_nr;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The nr attribute must be an int");
        return -1;
    }
    new_nr = PyLong_AsLong(value);
    self->perf_tmap->nr = new_nr;
    return 0;
}

static PyObject * py_perf_thread_map_get_nr(py_perf_thread_map *self, void *closure)
{
	return PyLong_FromLong(self->perf_tmap->nr);
}

static int py_perf_thread_map_set_nr(py_perf_thread_map *self, PyObject *value, void *closure)
{
    int new_nr;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The nr attribute must be an int");
        return -1;
    }
    new_nr = PyLong_AsLong(value);
    self->perf_tmap->nr = new_nr;
    return 0;
}

static PyObject * py_perf_thread_map_get_err_thread(py_perf_thread_map *self, void *closure)
{
	return PyLong_FromLong(self->perf_tmap->err_thread);
}

static int py_perf_thread_map_set_err_thread(py_perf_thread_map *self, PyObject *value, void *closure)
{
    int new_err_thread;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The err_thread attribute must be an int");
        return -1;
    }
    new_err_thread = PyLong_AsLong(value);
    self->perf_tmap->err_thread = new_err_thread;
    return 0;
}

static PyObject * py_perf_thread_map_get_map(py_perf_thread_map *self, void *closure)
{
	PyObject *list = PyList_New(self->perf_tmap->nr);
	if (!list)
		return NULL;
	for (int i = 0; i < self->perf_tmap->nr; i++) {
		py_thread_map_data *tmap = PyObject_New(py_thread_map_data, &py_thread_map_data_type);
		tmap->tmap_data = (self->perf_tmap->map[i]);
		PyList_SetItem(list, i, (PyObject *)tmap);
	}
	return list;
}

static int py_perf_thread_map_set_map(py_perf_thread_map *self, PyObject *value, void *closure)
{
	Py_ssize_t size;
	PyObject *obj;
	if (!PyList_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "The map attribute must be a list");
		return -1;
	}
	size = PyList_Size(value);
	if (size != self->perf_tmap->nr) {
		PyErr_SetString(PyExc_TypeError, "Size of list is different");
		return -1;
	}
	for(int i = 0; i < size; i++) {
		obj = PyList_GetItem(value, i);
		self->perf_tmap->map[i].pid = ((py_thread_map_data *)obj)->tmap_data.pid;
		strncpy(self->perf_tmap->map[i].comm, ((py_thread_map_data *)obj)->tmap_data.comm, sizeof(self->perf_tmap->map[i].comm) - 1);
		self->perf_tmap->map[i].comm[sizeof(self->perf_tmap->map[i].comm) - 1] = '\0';
	}
	return 0;
}

static PyGetSetDef py_perf_thread_map_getsetters[] = {
    {"refcnt", (getter)py_perf_thread_map_get_refcnt, (setter)py_perf_thread_map_set_refcnt,"refcnt", NULL},
    {"nr", (getter)py_perf_thread_map_get_nr, (setter)py_perf_thread_map_set_nr,"number of threads", NULL},
    {"err_thread", (getter)py_perf_thread_map_get_err_thread, (setter)py_perf_thread_map_set_err_thread,"err thread", NULL},
    {"map", (getter)py_perf_thread_map_get_map, (setter)py_perf_thread_map_set_map,"map[] elements", NULL},
    {NULL}
};

static void py_perf_thread_map_dealloc(py_perf_thread_map *thread_map)
{
	free(thread_map->perf_tmap);
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
	.tp_getset = py_perf_thread_map_getsetters,
};

// perf_evlist
typedef struct {
	PyObject_HEAD
	struct perf_evlist *evlist;
} py_perf_evlist;

static void
python_push_type(const char *name, PyObject *module, PyTypeObject *type)
{
	if (PyType_Ready(type) == -1)
		printf("python_push_type: failed to ready %s", name);

	Py_INCREF(type);
}

LIBPERF_API PyMODINIT_FUNC PyInit_libperf(void);

#endif /* __LIBPERF_PY_PERF_H */
