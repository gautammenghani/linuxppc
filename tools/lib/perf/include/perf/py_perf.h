/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBPERF_PY_PERF_H
#define __LIBPERF_PY_PERF_H

#define PY_SSIZE_T_CLEAN
#include <stdlib.h>
#include <perf/threadmap.h>
#include <Python.h>


// atomic_t
typedef struct {
	PyObject_HEAD
	int counter;
} py_atomic_t;
static PyObject * py_atomic_t_get_counter(py_atomic_t *self, void *closure)
{
	return PyLong_FromLong(self->counter);
}
static int py_atomic_t_set_counter(py_atomic_t *self, PyObject *value, void *closure)
{
    int new_counter;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The counter attribute must be an int");
        return -1;
    }
    new_counter = PyLong_AsLong(value);
    self->counter = new_counter;
    return 0;
}
static PyGetSetDef py_atomic_t_getsetters[] = {
    {"atomic_t",
     (getter)py_atomic_t_get_counter, (setter)py_atomic_t_set_counter,
     "atomic counter", NULL},
    {NULL}
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
	.tp_getset = py_atomic_t_getsetters,
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
	struct thread_map_data *tmap_data;
} py_thread_map_data;
static PyObject * py_thread_map_data_get_pid(py_thread_map_data *self, void *closure)
{
	return PyLong_FromLong(self->tmap_data->pid);
}
static int py_thread_map_data_set_pid(py_thread_map_data *self, PyObject *value, void *closure)
{
    int pid;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "the pid attribute must be an int");
        return -1;
    }
    pid = PyLong_AsLong(value);
    self->tmap_data->pid = pid;
    return 0;
}
static PyObject *py_thread_map_data_get_comm(py_thread_map_data *self, void *closure)
{
	return PyUnicode_FromString(self->tmap_data->comm);
}
static int py_thread_map_data_set_comm(py_thread_map_data *self, PyObject *value, void *closure)
{
	const char *comm;
	if (!PyUnicode_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "the comm attribute must be a string");
		return -1;
	}
	comm = PyUnicode_AsUTF8(value);
	strncpy(self->tmap_data->comm, comm, sizeof(self->tmap_data->comm - 1));
	self->tmap_data->comm[sizeof(self->tmap_data->comm) - 1] = '\0';
	return 0;
}
static PyGetSetDef py_thread_map_data_getsetters[] = {
    {"pid", (getter)py_thread_map_data_get_pid, (setter)py_thread_map_data_set_pid, "thread map pid", NULL},
     {"comm", (getter)py_thread_map_data_get_comm, (setter)py_thread_map_data_set_comm,
     "thread map pid", NULL},
    {NULL}
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
	.tp_getset = py_thread_map_data_getsetters,
};
// perf_thread_map
typedef struct {
	PyObject_HEAD
	struct perf_thread_map *ptr;
} py_perf_thread_map;


static PyObject * py_perf_thread_map_get_refcnt(py_perf_thread_map *self, void *closure)
{
	return PyLong_FromLong(atomic_read(&(self->ptr->refcnt.refs)));
}
static int py_perf_thread_map_set_refcnt(py_perf_thread_map *self, PyObject *value, void *closure)
{
    int new_nr;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The nr attribute must be an int");
        return -1;
    }
    new_nr = PyLong_AsLong(value);
    self->ptr->nr = new_nr;
    return 0;
}
static PyObject * py_perf_thread_map_get_nr(py_perf_thread_map *self, void *closure)
{
	return PyLong_FromLong(self->ptr->nr);
}
static int py_perf_thread_map_set_nr(py_perf_thread_map *self, PyObject *value, void *closure)
{
    int new_nr;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The nr attribute must be an int");
        return -1;
    }
    new_nr = PyLong_AsLong(value);
    self->ptr->nr = new_nr;
    return 0;
}
static PyObject * py_perf_thread_map_get_err_thread(py_perf_thread_map *self, void *closure)
{
	return PyLong_FromLong(self->ptr->err_thread);
}
static int py_perf_thread_map_set_err_thread(py_perf_thread_map *self, PyObject *value, void *closure)
{
    int new_err_thread;
    if (!PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The err_thread attribute must be an int");
        return -1;
    }
    new_err_thread = PyLong_AsLong(value);
    self->ptr->err_thread = new_err_thread;
    return 0;
}
static PyObject * py_perf_thread_map_get_map(py_perf_thread_map *self, void *closure)
{
	PyObject *list = PyList_New(self->ptr->nr);
	if (!list)
		return NULL;
	for (int i = 0; i < self->ptr->nr; i++) {
		py_thread_map_data *tmap = PyObject_New(py_thread_map_data, &py_thread_map_data_type);
		tmap->tmap_data = &(self->ptr->map[i]);
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
	if (size != self->ptr->nr) {
		PyErr_SetString(PyExc_TypeError, "Size of list is different");
		return -1;
	}
	for(int i = 0; i < size; i++) {
		obj = PyList_GetItem(value, i);
		self->ptr->map[i].pid = ((py_thread_map_data *)obj)->tmap_data->pid;
		strncpy(self->ptr->map[i].comm, ((py_thread_map_data *)obj)->tmap_data->comm, sizeof(self->ptr->map[i].comm) - 1);
		self->ptr->map[i].comm[sizeof(self->ptr->map[i].comm) - 1] = '\0';
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
	.tp_getset = py_perf_thread_map_getsetters,
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
