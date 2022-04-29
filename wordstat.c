#include "wordstat.h"

/*
 * This should hold everything that is needed to keep the working state.  Feel
 * free to add anything that is needed, the initial integer is there just so the
 * code compiles.
 */
struct _WordStat {
    char **words;
    char *delim;
    bool regex;
};

/*
 * Returns new structure that will be used when calling any APIs in this file.
 */
WordStat *
WordStatNew(const char *delim,
            bool regex)
{
    if(!delim || delim[0]==0)
        return NULL;
    
    WordStat *ws = g_new(WordStat, 1);

    ws->words = NULL;
    ws->delim = g_strdup(delim);
    ws->regex = regex;
    
    return ws;
}

/*
 * Frees all the memory that was allocated for @ws and all its members during
 * the lifetime of the structure.
 */
void WordStatFree(WordStat *ws)
{
    if(!ws)
        return;

    g_strfreev(ws->words);
    g_free(ws->delim);

    g_free(ws);

    return;
}

/*
 * Splits the @text into words and updates @ws accordingly.
 */
void
WordStatAddText(WordStat *ws,
                const char *text)
{
   char *lower_text = g_ascii_strdown(text, -1);

   if (ws->regex)
        ws->words = g_regex_split_simple(ws->delim, lower_text, 0, 0);
   else
        ws->words = g_strsplit_set(lower_text, ws->delim, 0);

    g_free(lower_text);

    return;
  }
/*
 * Finds the most frequent word and returns it as a string.  Caller owns the
 * string and is responsible for freeing it.  If @num is supplied, the variable
 * it points to will be updated to hold the number of occurrences of the
 * returned word.
 */
char *
WordStatGetMostFrequent(WordStat *ws,
                        size_t *num)
{
    char *word = NULL;
    int mx = 0;
    char **words = NULL;

    if(!ws || !(ws->words))
        goto done;
    
    words = g_strdupv(ws->words);
    char **wordsptr = words;
    
    while (*wordsptr != NULL) {
        if ((*wordsptr)[0]) {
            int c = 1;
            
            for (char **cur = wordsptr + 1; *cur; cur++) {
                if (!g_strcmp0(*wordsptr, *cur))
                    c++, **cur = 0;
            }
            
            if (mx < c) {
                mx = c;
                g_free(word);
                word = g_strdup(*wordsptr);
            }
        }
        wordsptr++;
    }

    g_strfreev(words);
    
 done:
    if (num)
        *num = mx;
    
    return word;
}

/*
 * Returns the total amount of words added.
 */
size_t
WordStatGetTotal(WordStat *ws)
{
    if(!ws || !(ws->words))
        return 0;

    char **tmp = ws->words;

    int c = 0;
    
    while (*tmp) {
        if ((*tmp)[0])
            c++;
        tmp++;
    }
    
    return c;
}
