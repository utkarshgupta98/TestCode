#include "munit.h"
#include "esb.h"
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "../bmd/bmd_parser.h"


/*gcc test_esb.c munit.c bmd_parser.c database.c esb.c  `mysql_config --cflags --libs` `xml2-config --cflags --libs` -o test_esb
 */
static void *
test_process_xml_setup(const MunitParameter params[], void *user_data)
{
    char cwd[100];
    getcwd(cwd, sizeof(cwd));
    char path[100];
    sprintf(path, "%s/Test_files/%s", cwd,"HelloABC.xml");
    return strdup(path);
}

static void
test_parse_bmd_xml_tear_down(void *fixture)
{
 
}

static MunitResult
test_parse_bmd_xml(const MunitParameter params[], void *fixture)
{
    return MUNIT_OK;
}


