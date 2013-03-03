#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <bsd/sys/sys/eventhandler.h>

/* TEST1 */

#define INVOKE_NUM (100000)

typedef void (*void_func_fn)(void*);
EVENTHANDLER_DECLARE(test_handler1, void_func_fn);

void void_f1(void* arg)
{
    printf("f1\n");
}

void void_f2(void* arg)
{
    printf("f2\n");
}

void void_f3(void* arg)
{
    printf("f3\n");
}

/*
 * Should invoke f1 and then f2 (according to priority)
 * Should invoke f2 and then f3 (according to priority)
 *
 */
void test1(void)
{
    int i;
    eventhandler_tag t;

    printf("BSD EventHandler Test\n");

    t = EVENTHANDLER_REGISTER(test_handler1, void_f1, NULL, EVENTHANDLER_PRI_LAST);
    EVENTHANDLER_REGISTER(test_handler1, void_f2, NULL, EVENTHANDLER_PRI_FIRST);

    /* INVOKE_NUM Invocations */
    for (i=0; i<INVOKE_NUM; ++i)
        EVENTHANDLER_INVOKE(test_handler1);

    /* Deregister t */
    EVENTHANDLER_DEREGISTER(test_handler1, t);
    EVENTHANDLER_REGISTER(test_handler1, void_f3, NULL, EVENTHANDLER_PRI_FIRST);

    /* INVOKE_NUM Invocations */
    for (i=0; i<INVOKE_NUM; ++i)
        EVENTHANDLER_INVOKE(test_handler1);

    printf("BSD EventHandler Test Done\n");
}

int main(int argc, char **argv)
{
    test1();
    /* FIXME: test the eventhandler interface from multiple threads */

    return 0;
}