#include "wordstat.h"

/*
 * This should hold everything that is needed to keep the working state.  Feel
 * free to add anything that is needed, the initial integer is there just so the
 * code compiles.
 */
struct _WordStat {
    int unused_delete_me;
};

/*
 * Returns new structure that will be used when calling any APIs in this file.
 */
WordStat *
WordStatNew(const char *delim G_GNUC_UNUSED,
            bool regex G_GNUC_UNUSED)
{
    return NULL;
}

/*
 * Frees all the memory that was allocated for @ws and all its members during
 * the lifetime of the structure.
 */
void WordStatFree(WordStat *ws G_GNUC_UNUSED)
{
}

/*
 * Splits the @text into words and updates @ws accordingly.
 */
void
WordStatAddText(WordStat *ws G_GNUC_UNUSED,
                const char *text G_GNUC_UNUSED)
{
}

/*
 * Finds the most frequent word and returns it as a string.  Caller owns the
 * string and is responsible for freeing it.  If @num is supplied, the variable
 * it points to will be updated to hold the number of occurrences of the
 * returned word.
 */
char *
WordStatGetMostFrequent(WordStat *ws G_GNUC_UNUSED,
                        size_t *num G_GNUC_UNUSED)
{
    return NULL;
}
