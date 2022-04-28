#pragma once

#include <stdbool.h>
#include <glib.h>

typedef struct _WordStat WordStat;
typedef struct _WordStatInfo WordStatInfo;

WordStat *WordStatNew(const char *delim, bool regex);
void WordStatFree(WordStat *ws);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(WordStat, WordStatFree)

void
WordStatAddText(WordStat *ws,
                const char *text);

char *
WordStatGetMostFrequent(WordStat *ws,
                        size_t *num);
