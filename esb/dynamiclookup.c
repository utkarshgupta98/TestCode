/**
 * When the function to be invoked is determined via a table lookup,
 * following is one of the ways to invoke a dyamically loaded function in C.
 * Help taken from: https://stackoverflow.com/a/1120834
 */
#include <stdio.h>
#include <string.h>

/**
 * These should come from the header file of the respective service
 * provider's implementation of the transformation and transport.
 */
void tranform_bmd_PO_svc(void* ptr) { printf("Transformation for PO service: %d\n", *((int*)ptr)); }
void transport_bmd_PO_svc(void* ptr) { printf("Transport for PO service\n"); }

void tranform_bmd_Credit_svc(void* ptr) { printf("Transformation for Credit service: %d\n", *((int*)ptr)); }
void transport_bmd_Credit_svc(void* ptr) { printf("Transport for Credit service\n"); }
void transform_bmd_IFSC_svc(void *ptr)
{
   printf("IFSC Service Transform");
}
void transport_bmd_IFSC_svc(void *ptr)
{
    printf("IFSC Service Transport");
}
/* The ESB would have this table */
const static struct
{
    const char *name;
    void (*func)(void*);
} function_map[] = {
    {"PO svc tranform", tranform_bmd_PO_svc},
    {"PO svc transport", transport_bmd_PO_svc},
    {"Credit service tranform", tranform_bmd_Credit_svc},
    {"Credit service transport", transport_bmd_Credit_svc},
    {"IFSC service transform",transform_bmd_IFSC_svc},
    {"IFSC service transport",transport_bmd_IFSC_svc},
};

/* This is how the ESB may dynamically invoke the service adapter functions */
int call_function(const char *name, void* data)
{
    for (int i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++)
    {
        if (!strcmp(function_map[i].name, name) && function_map[i].func)
        {
            function_map[i].func(data);
            return 0;
        }
    }
    return -1;
}
