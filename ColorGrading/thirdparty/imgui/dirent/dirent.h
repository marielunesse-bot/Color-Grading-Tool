/*
 * Dirent interface for Microsoft Visual Studio
 * Version 1.23.1
 *
 * Copyright (C) 2006-2012 Toni Ronkko
 * This file is part of dirent.  Dirent may be freely distributed
 * under the MIT license.  For all details and documentation, see
 * https://github.com/tronkko/dirent
 */
#ifndef DIRENT_H
#define DIRENT_H

#include <windows.h>
#include <string.h>
#include <assert.h>

typedef struct dirent {
    char d_name[MAX_PATH + 1];
    WIN32_FIND_DATAA data;
} dirent;

typedef struct DIR {
    dirent current;
    int cached;
    HANDLE search_handle;
    char patt[MAX_PATH + 3];
} DIR;

static DIR *opendir(const char *dirname) {
    DIR *dirp;
    assert(dirname != NULL);
    
    dirp = (DIR*)malloc(sizeof(struct DIR));
    if (dirp) {
        char *p;
        
        strncpy(dirp->patt, dirname, sizeof(dirp->patt));
        dirp->patt[MAX_PATH] = '\0';
        
        p = strchr(dirp->patt, '\0');
        if (dirp->patt < p  &&  *(p-1) != '\\'  &&  *(p-1) != ':') {
            *p++ = '\\';
        }
        *p++ = '*';
        *p = '\0';
        
        dirp->search_handle = FindFirstFileA(dirp->patt, &dirp->current.data);
        if (dirp->search_handle == INVALID_HANDLE_VALUE) {
            free(dirp);
            return NULL;
        }
        
        dirp->cached = 1;
    }
    
    return dirp;
}

static struct dirent *readdir(DIR *dirp) {
    assert(dirp != NULL);
    
    if (dirp->search_handle == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    
    if (dirp->cached) {
        dirp->cached = 0;
    } else {
        if (FindNextFileA(dirp->search_handle, &dirp->current.data) == FALSE) {
            return NULL;
        }
    }
    
    strncpy(dirp->current.d_name, dirp->current.data.cFileName, sizeof(dirp->current.d_name));
    dirp->current.d_name[MAX_PATH] = '\0';
    
    return &dirp->current;
}

static int closedir(DIR *dirp) {
    assert(dirp != NULL);
    
    if (dirp->search_handle != INVALID_HANDLE_VALUE) {
        FindClose(dirp->search_handle);
    }
    
    free(dirp);
    return 0;
}

#endif /*DIRENT_H*/