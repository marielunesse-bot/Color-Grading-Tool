/*
 * tinyfiledialogs.h
 * cross-platform dialogs for windows, mac, linux
 * version 3.15.1
 * Copyright (c) 2014 - 2023 Guillaume Vareille http://ysengrin.com
 * http://tinyfiledialogs.sourceforge.net
 * http://tinyfiledialogs.gitlab.io
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef TINYFILEDIALOGS_H
#define TINYFILEDIALOGS_H

#ifdef __cplusplus
extern "C" {
#endif

extern char const * tinyfd_openFileDialog(
    char const * aTitle,
    char const * aDefaultPathAndFile,
    int aNumOfFilterPatterns,
    char const * const * aFilterPatterns,
    char const * aSingleFilterDescription,
    int aAllowMultipleSelects);

extern char const * tinyfd_saveFileDialog(
    char const * aTitle,
    char const * aDefaultPathAndFile,
    int aNumOfFilterPatterns,
    char const * const * aFilterPatterns,
    char const * aSingleFilterDescription);

extern char const * tinyfd_selectFolderDialog(
    char const * aTitle,
    char const * aDefaultPath);

extern int tinyfd_messageBox(
    char const * aTitle,
    char const * aMessage,
    char const * aDialogType,
    char const * aIconType,
    int aDefaultButton);

extern int tinyfd_notifyPopup(
    char const * aTitle,
    char const * aMessage,
    char const * aIconType);

extern char const * tinyfd_inputBox(
    char const * aTitle,
    char const * aMessage,
    char const * aDefaultInput);

extern char const * tinyfd_colorChooser(
    char const * aTitle,
    char const * aDefaultHexRGB,
    unsigned char const aDefaultRGB[3],
    unsigned char aoResultRGB[3]);

#ifdef __cplusplus
}
#endif

#endif /* TINYFILEDIALOGS_H */