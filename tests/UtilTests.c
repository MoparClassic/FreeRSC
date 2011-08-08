#include "UtilTests.h"

#include <stdio.h>

#ifndef HAVE_STRDUP

static void
test_strdup(CuTest *tc)
{

}
#endif

#ifndef HAVE_STRLCAT

static void
test_strlcat(CuTest *tc)
{

}
#endif

#ifndef HAVE_STRLCPY

static void
test_strlcpy(CuTest *tc)
{

}
#endif

static void
test_trim(CuTest *tc)
{
    const char *a = "   Lol hi   ";
    char dst[20];

    memset(dst, '\0', sizeof(dst));

    trim(dst, a, sizeof(dst));
    printf("'%s'\n", dst);
}

CuSuite
*get_util_test_suite()
{
    CuSuite *suite = CuSuiteNew();
#ifndef HAVE_STRDUP
    SUITE_ADD_TEST(suite, &test_strdup);
#endif

#ifndef HAVE_STRLCAT
    SUITE_ADD_TEST(suite, &test_strlcat);
#endif

#ifndef HAVE_STRLCPY
    SUITE_ADD_TEST(suite, &test_strlcpy);
#endif

    SUITE_ADD_TEST(suite, &test_trim);
    return suite;
}
