/*
 * Wolfenstein: Enemy Territory GPL Source Code
 * Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.
 *
 * ET: Legacy
 * Copyright (C) 2012-2023 ET:Legacy team <mail@etlegacy.com>
 *
 * This file is part of ET: Legacy - http://www.etlegacy.com
 *
 * ET: Legacy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ET: Legacy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ET: Legacy. If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, Wolfenstein: Enemy Territory GPL Source Code is also
 * subject to certain additional terms. You should have received a copy
 * of these additional terms immediately following the terms and conditions
 * of the GNU General Public License which accompanied the source code.
 * If not, please request a copy in writing from id Software at the address below.
 *
 * id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
 */
/**
 * @file l_precomp.h
 * @brief Pre-compiler
 */

#ifndef INCLUDE_L_PRECOMP_H
#define INCLUDE_L_PRECOMP_H

#ifndef _MAX_PATH
#define MAX_PATH            MAX_QPATH
#endif

#define DEFINE_FIXED            0x0001

#define BUILTIN_LINE            1
#define BUILTIN_FILE            2
#define BUILTIN_DATE            3
#define BUILTIN_TIME            4
#define BUILTIN_STDC            5

#define INDENT_IF               0x0001
#define INDENT_ELSE             0x0002
#define INDENT_ELIF             0x0004
#define INDENT_IFDEF            0x0008
#define INDENT_IFNDEF           0x0010

/**
 * @struct define_s
 * @typedef define_t
 * @brief Macro definitions
 */
typedef struct define_s
{
	char *name;                         ///< define name
	int flags;                          ///< define flags
	int builtin;                        ///< > 0 if builtin define
	int numparms;                       ///< number of define parameters
	token_t *parms;                     ///< define parameters
	token_t *tokens;                    ///< macro tokens (possibly containing parm tokens)
	struct define_s *next;              ///< next defined macro in a list
	struct define_s *hashnext;          ///< next define in the hash chain
} define_t;

/**
 * @struct indent_s
 * @typedef indent_t
 * @brief indents
 * used for conditional compilation directives:
 * \#if, \#else, \#elif, \#ifdef, \#ifndef
 */
typedef struct indent_s
{
	int type;                           ///< indent type
	int skip;                           ///< true if skipping current indent
	script_t *script;                   ///< script the indent was in
	struct indent_s *next;              ///< next indent on the indent stack
} indent_t;

/**
 * @struct source_s
 * @typedef source_t
 * @brief Source File
 */
typedef struct source_s
{
	char filename[_MAX_PATH];           ///< file name of the script
	char includepath[_MAX_PATH];        ///< path to include files
	punctuation_t *punctuations;        ///< punctuations to use
	script_t *scriptstack;              ///< stack with scripts of the source
	token_t *tokens;                    ///< tokens to read first
	define_t *defines;                  ///< list with macro definitions
	define_t **definehash;              ///< hash chain with defines
	indent_t *indentstack;              ///< stack with indents
	int skip;                           ///< > 0 if skipping conditional code
	token_t token;                      ///< last read token
} source_t;

// read a token from the source
int PC_ReadToken(source_t *source, token_t *token);
//  expect a certain token
//int PC_ExpectTokenString(source_t *source, char *string);
// expect a certain token type
//int PC_ExpectTokenType(source_t *source, int type, int subtype, token_t *token);
// expect a token
//int PC_ExpectAnyToken(source_t *source, token_t *token);
// returns true when the token is available
int PC_CheckTokenString(source_t *source, const char *string);
// returns true an reads the token when a token with the given type is available
//int PC_CheckTokenType(source_t *source, int type, int subtype, token_t *token);
// skip tokens until the given token string is read
//int PC_SkipUntilString(source_t *source, const char *string);
// unread the last token read from the script
//void PC_UnreadLastToken(source_t *source);
// unread the given token
void PC_UnreadToken(source_t *source, token_t *token);
// read a token only if on the same line, lines are concatenated with a slash
int PC_ReadLine(source_t *source, token_t *token);
// returns true if there was a white space in front of the token
int PC_WhiteSpaceBeforeToken(token_t *token);
// add a define to the source
//int PC_AddDefine(source_t *source, const char *string);
// add a globals define that will be added to all opened sources
int PC_AddGlobalDefine(const char *string);
// remove the given global define
//int PC_RemoveGlobalDefine(const char *name);
// remove all globals defines
void PC_RemoveAllGlobalDefines(void);
// add builtin defines
//void PC_AddBuiltinDefines(source_t *source);
// set the source include path
//void PC_SetIncludePath(source_t *source, char *path);
// set the punction set
//void PC_SetPunctuations(source_t *source, punctuation_t *p);
// set the base folder to load files from
//void PC_SetBaseFolder(const char *path);
// load a source file
source_t *LoadSourceFile(const char *filename);
// load a source from memory
//source_t *LoadSourceMemory(char *ptr, int length, const char *name);
// free the given source
void FreeSource(source_t *source);
// print a source error
void QDECL SourceError(source_t *source, const char *str, ...) _attribute ((format(printf, 2, 3)));
// print a source warning
void QDECL SourceWarning(source_t *source, const char *str, ...)  _attribute ((format(printf, 2, 3)));

int PC_LoadSourceHandle(const char *filename);
int PC_FreeSourceHandle(int handle);
int PC_ReadTokenHandle(int handle, struct pc_token_s *pc_token);
int PC_SourceFileAndLine(int handle, char *filename, int *line);
void PC_CheckOpenSourceHandles(void);
void PC_UnreadLastTokenHandle(int handle);

#endif // #ifndef INCLUDE_L_PRECOMP_H
