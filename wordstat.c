#include "wordstat.h"

/*
 * This should hold everything that is needed to keep the working state.  Feel
 * free to add anything that is needed, the initial integer is there just so the
 * code compiles.
 */
struct _WordStat {
    GSList *words;
    char   *delim;
    bool    regex;
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

    while (ws->words) {
        GSList *tmp = ws->words;
        ws->words = ws->words->next;
        g_free(tmp->data);
        g_slist_free_1(tmp);
    }
    g_slist_free(ws->words);
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
    char **result = NULL;
    
    if (ws->regex)
        result = g_regex_split_simple(ws->delim, text, 0, 0);
    else
        result = g_strdupv(g_strsplit_set(text, ws->delim, 0));

    int i = 0;
    while (result[i] != NULL) {
        if(result[i][0])
            ws->words = g_slist_prepend(ws->words, g_strdup(result[i]));
        i++;
    }

    g_strfreev(result);

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
    size_t mx = 0;
    GSList *words = ws->words;

    if(!ws || !(ws->words))
        goto done;
    
    
    while (words) {
        size_t c = 1;
        char *cur = words->data;
        
        for (GSList *p = words->next; p; p = p->next) {
            if (!g_ascii_strcasecmp(words->data, p->data)) {
                c++;
            }
        }
        
        if (mx < c) {
            mx = c;
            g_free(word);
            word = g_ascii_strdown(cur, -1);
        }
        
        words = words->next;
        words = g_slist_remove_all(words, cur);
    }
    
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
    return g_slist_length(ws->words);
}
