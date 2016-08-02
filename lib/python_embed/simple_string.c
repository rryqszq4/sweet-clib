/*
 * gcc -o simple_string simple_string.c -I/usr/local/include/python2.7 -I/usr/local/include/python2.7 -fno-strict-aliasing -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/lib/python2.7/config -lpython2.7 -lpthread -ldl -lutil -lm -Xlinker -export-dynamic
 */

#include <Python.h>

int
main(int argc, char *argv[])
{
  Py_SetProgramName(argv[0]);  /* optional but recommended */
  Py_Initialize();
  PyRun_SimpleString("from time import time,ctime\n"
                     "print 'Today is',ctime(time())\n");
  Py_Finalize();
  return 0;
}
