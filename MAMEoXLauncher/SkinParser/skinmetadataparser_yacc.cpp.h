typedef union {
    const char		*stringVal;
    int						intVal;
		void					*voidPtrVal;
} YYSTYPE;
#define	_TOK_IDENTIFIER_	258
#define	_TOK_STRING_	259
#define	_TOK_MACROSET_	260
#define	_TOK_FILENAME_	261
#define	_TOK_INT_	262
#define	SKIN_OPEN	263
#define	SKIN_CLOSE	264
#define	METADATA_OPEN	265
#define	METADATA_CLOSE	266
#define	AUTHOR_OPEN	267
#define	AUTHOR_CLOSE	268
#define	NAME_OPEN	269
#define	NAME_CLOSE	270
#define	DESCRIPTION_OPEN	271
#define	DESCRIPTION_CLOSE	272
#define	PREVIEWFILE_OPEN	273
#define	PREVIEWFILE_CLOSE	274


extern YYSTYPE smplval;
