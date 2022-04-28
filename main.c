#include <glib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wordstat.h"


#define NULLSTR(x) (x) ? (x) : "<null>"

static bool
add_and_check(WordStat *ws,
              size_t counter,
              const char *text,
              const char *word,
              size_t *number)
{
    bool ret = true;
    size_t actual_number = SIZE_MAX;

    printf("Test %zu: ", counter);

    if (text) {
        WordStatAddText(ws, text);
    }

    g_autofree char *actual_word = WordStatGetMostFrequent(ws,
                                                           number ? &actual_number : NULL);

    if (g_strcmp0(word, actual_word) != 0) {
        printf("FAIL\nExpected '%s', but got '%s'\n",
               NULLSTR(word), NULLSTR(actual_word));
        ret = false;
    }

    if (number && *number != actual_number) {
        if (ret) {
            printf("FAIL\n");
            printf("Expected '%zu', but got '%zu'\n", *number, actual_number);
            ret = false;
        }
    }

    if (ret)
        printf("OK\n");

    return ret;
}

#ifdef RUN_ALL_TESTS

# define DO_TEST(text, word, number) \
    do { \
        size_t tmp = number; \
        if (!add_and_check(ws, __COUNTER__, text, word, &tmp)) \
            fail = true; \
    } while (0)

# define DO_TEST_SIMPLE(text, word) \
    do { \
        if (!add_and_check(ws, __COUNTER__, text, word, NULL)) \
            fail = true; \
    } while (0)

# define RUN_COND

#else

# define DO_TEST(text, word, number) \
    do { \
        size_t tmp = number; \
        fail = fail || !add_and_check(ws, __COUNTER__, text, word, &tmp); \
    } while (0)

# define DO_TEST_SIMPLE(text, word) \
    fail = fail || !add_and_check(ws, __COUNTER__, text, word, NULL)

# define RUN_COND if (!fail)

#endif

int
main(int argc G_GNUC_UNUSED,
     char *argv[] G_GNUC_UNUSED)
{
    bool fail = false;
    g_autofree char *tmp = NULL;
    g_autofree char *dyn = g_strdup("This text is text, text and more text.");
    g_autoptr(WordStat) ws_dummy = WordStatNew("", false);
    g_autoptr(WordStat) ws = WordStatNew(NULL, false);

    if (ws_dummy) {
        printf("WordStatNew() for @ws_dummy should have failed, but did not\n");
        fail = true;
        goto cleanup;
    }

    if (ws) {
        printf("WordStatNew() for @ws should have failed, but did not\n");
        fail = true;
        goto cleanup;
    }

    WordStatFree(ws_dummy);
    WordStatFree(ws);

    ws_dummy = WordStatNew(" ", false);
    if (!ws_dummy) {
        printf("WordStatNew() for @ws_dummy has failed\n");
        fail = true;
        goto cleanup;
    }

    ws = WordStatNew(" .,", false);
    if (!ws) {
        printf("WordStatNew() for @ws has failed\n");
        fail = true;
        goto cleanup;
    }

    DO_TEST_SIMPLE(NULL, NULL);
    DO_TEST(NULL, NULL, 0);
    DO_TEST("", NULL, 0);
    DO_TEST(" ", NULL, 0);

    DO_TEST_SIMPLE("asdf", "asdf");
    DO_TEST(NULL, "asdf", 1);

    DO_TEST_SIMPLE("Cupcake ipsum dolor sit amet gummi bears. More bears.", "bears");
    DO_TEST(NULL, "bears", 2);
    DO_TEST("Bears", "bears", 3);
    DO_TEST(dyn, "text", 4);

    g_clear_pointer(&dyn, g_free);
    DO_TEST(NULL, "text", 4);

    /* Bit overcomplicated test */
    RUN_COND {
        printf("Test %d: ", __COUNTER__);
        tmp = WordStatGetMostFrequent(ws, NULL);
        if (g_strcmp0("text", tmp) != 0) {
            printf("FAIL\nExpected 'text', but got '%s'\n", NULLSTR(tmp));
            fail = true;
        } else {
            printf("OK\n");
        }
    }

    WordStatFree(ws);
    ws = WordStatNew("[^A-Za-z]+", true);
    if (!ws) {
        printf("WordStatNew() for @ws has failed\n");
        fail = true;
        goto cleanup;
    }

    RUN_COND {
        printf("Test %d: ", __COUNTER__);
        if (g_strcmp0("text", tmp) != 0) {
            printf("FAIL\nReturned string is not kept after WordStatFree()\n");
            fail = true;
        } else {
            printf("OK\n");
        }
    }

    DO_TEST("\t\n\n\n\a\r\r!@&#*)(&#$]@{}|", NULL, 0);
    DO_TEST(" ,.,#$()#asdf$\t\n /.,/.182734!@#$!!^(*&!@)&,", "asdf", 1);

    DO_TEST("Marzipan  dessert  cottons  candy  fruitcake  sesame  snaps.\n"
            "Oat  cake  tart  pastry  cotton  candy  macaroon  pie  cupcake.\n"
            "Wafer  lollipop  cotton  candy  cookie  powder  chocolate  cake.  "
            "Gingerbread  cake  marzipan  sesame  snaps  pie  jelly  beans  "
            "dessert  tiramisu.  Candy  canes  ice  cream  donut  halvah  "
            "chocolate  bonbon.",
            "candy", 4);

    WordStatFree(ws_dummy);
    ws_dummy = WordStatNew(".*", true);
    if (!ws) {
        printf("WordStatNew() for @ws_dummy has failed\n");
        fail = true;
        goto cleanup;
    }

    DO_TEST("Halvah lemon drops sweet cake lemon drops. Croissant sugar plum "
            "fruitcake sweet roll carrot cake. Jelly-o jelly beans chupa chups "
            "cookie soufflé caramels. Croissant topping shortbread shortbread "
            "chocolate bar sesame snaps pie gummies.",
            "cake", 5);

    DO_TEST("Gathered By GRAVITY from Which We SPRING EMERGED INTO "
            "Consciousness cosmic Ocean HOW Far Away ACROSS The CENTURIES. "
            "BRAIN Is the Seed OF Intelligence THE Carbon IN our APPLE pies "
            "A STILL more GLORIOUS DAWN awaits NETWORK OF wormholes MADE in "
            "THE interiors OF COLLAPSING stars THE ASH of STELLAR ALCHEMY? "
            "vastness IS bearable ONLY Through Love DISPASSIONATE "
            "Extraterrestrial OBSERVER Vanquish THE Impossible Bits OF Moving "
            "FLUFF VENTURE Intelligent BEINGS And BILLIons Upon Billions UPON "
            "BiLlIoNs Upon BILLIONS Upon BILLIONS upon biLlIONS upon biLLIons.",
            "billions", 7);

 cleanup:
    if (fail) {
        fprintf(stderr, "Some tests failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
