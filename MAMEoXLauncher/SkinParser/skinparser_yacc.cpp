
/*  A Bison parser, made from skinparser.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse spparse
#define yylex splex
#define yyerror sperror
#define yylval splval
#define yychar spchar
#define yydebug spdebug
#define yynerrs spnerrs
#define	_TOK_IDENTIFIER_	258
#define	_TOK_STRING_	259
#define	_TOK_MACRO_	260
#define	_TOK_MACRO_OPERATOR_	261
#define	_TOK_FILENAME_	262
#define	_TOK_INT_	263
#define	SKIN_OPEN	264
#define	SKIN_CLOSE	265
#define	METADATA_OPEN	266
#define	METADATA_CLOSE	267
#define	RESOURCEDEFINITIONS_OPEN	268
#define	RESOURCEDEFINITIONS_CLOSE	269
#define	HELPSCREEN_OPEN	270
#define	HELPSCREEN_CLOSE	271
#define	OPTIONSSCREEN_OPEN	272
#define	OPTIONSSCREEN_CLOSE	273
#define	ROMLISTSCREEN_OPEN	274
#define	ROMLISTSCREEN_CLOSE	275
#define	SPLASHSCREEN_OPEN	276
#define	SPLASHSCREEN_CLOSE	277
#define	MESSAGESCREEN_OPEN	278
#define	MESSAGESCREEN_CLOSE	279
#define	STARTMENU_OPEN	280
#define	STARTMENU_CLOSE	281
#define	VIRTUALKEYBOARD_OPEN	282
#define	VIRTUALKEYBOARD_CLOSE	283
#define	SCREENSAVER_OPEN	284
#define	SCREENSAVER_CLOSE	285
#define	AUTHOR_OPEN	286
#define	AUTHOR_CLOSE	287
#define	NAME_OPEN	288
#define	NAME_CLOSE	289
#define	DESCRIPTION_OPEN	290
#define	DESCRIPTION_CLOSE	291
#define	PREVIEWFILE_OPEN	292
#define	PREVIEWFILE_CLOSE	293
#define	RESOURCEFILE_OPEN	294
#define	RESOURCEFILE_CLOSE	295
#define	COLORCHANNELFILENAME_OPEN	296
#define	COLORCHANNELFILENAME_CLOSE	297
#define	ALPHACHANNELFILENAME_OPEN	298
#define	ALPHACHANNELFILENAME_CLOSE	299
#define	RESOURCE_OPEN	300
#define	RESOURCE_CLOSE	301
#define	ID_OPEN	302
#define	ID_CLOSE	303
#define	AREA_OPEN	304
#define	AREA_CLOSE	305
#define	LEFT_OPEN	306
#define	LEFT_CLOSE	307
#define	TOP_OPEN	308
#define	TOP_CLOSE	309
#define	RIGHT_OPEN	310
#define	RIGHT_CLOSE	311
#define	BOTTOM_OPEN	312
#define	BOTTOM_CLOSE	313
#define	BACKDROP_OPEN	314
#define	BACKDROP_CLOSE	315
#define	DEFAULTS_OPEN	316
#define	DEFAULTS_CLOSE	317
#define	TEXT_OPEN	318
#define	TEXT_CLOSE	319
#define	FONT_OPEN	320
#define	FONT_CLOSE	321
#define	COLOR_OPEN	322
#define	COLOR_CLOSE	323
#define	STRING_OPEN	324
#define	STRING_CLOSE	325
#define	CENTERVERTICALLY	326
#define	CENTERHORIZONTALLY	327
#define	UNBOUNDED	328
#define	ALPHA_OPEN	329
#define	ALPHA_CLOSE	330
#define	RED_OPEN	331
#define	RED_CLOSE	332
#define	GREEN_OPEN	333
#define	GREEN_CLOSE	334
#define	BLUE_OPEN	335
#define	BLUE_CLOSE	336
#define	ARGB_OPEN	337
#define	ARGB_CLOSE	338
#define	ICON_OPEN	339
#define	ICON_CLOSE	340
#define	TEXTUREID_OPEN	341
#define	TEXTUREID_CLOSE	342
#define	POSITION_OPEN	343
#define	POSITION_CLOSE	344
#define	X_OPEN	345
#define	X_CLOSE	346
#define	Y_OPEN	347
#define	Y_CLOSE	348
#define	SIZE_OPEN	349
#define	SIZE_CLOSE	350
#define	WIDTH_OPEN	351
#define	WIDTH_CLOSE	352
#define	HEIGHT_OPEN	353
#define	HEIGHT_CLOSE	354
#define	CONDITION_OPEN	355
#define	CONDITION_CLOSE	356


	//= I N C L U D E S ====================================================
	#include "MAMEoX.h"
	#include "xbox_FileIO.h"
	#include "StdString.h"
	#include "FontSet.h"

	#include "Skin.h"
	#include "SkinPoint.h"
	#include "SkinDimension.h"
	#include "SkinColor.h"


	#include <Xtl.h>
	#include <malloc.h>
	#include <vector>
	#include <map>
	#include <list>
	#include <algorithm>


	//= D E F I N E S ======================================================
	#define CLEAN_AND_DIE()			SkinParserCleanup(); YYABORT;


	//= S T R U C T U R E S ================================================
	/**
	 * \class		CSkinParserResourceFile
	 * \brief   
	 */
	class CSkinParserResourceFile
	{
	public:
			//---------------------------------------------------
			//---------------------------------------------------
		CSkinParserResourceFile( void ) {
			m_colorChannelFilename = m_alphaChannelFilename = "";
		}

		std::vector<CSkinResource>		m_resources;
		CStdString										m_colorChannelFilename;
		CStdString										m_alphaChannelFilename;
	};


	//= P R O T O T Y P E S ================================================
	void	sperror( const char * );
	void	sperror( const CStdString & );
	int		splex( void );


	static void SkinParserCleanup( void );
	static BOOL	FontTypeFromName( const char *name, fonttype *ret );
	static BOOL SkinParserAddResourceFile( const CSkinParserResourceFile &element, CStdString *errorReport );
	static BOOL SkinParserAddElement( CSkinElement *element, CStdString *errorReport );


	const char *SkinParserNewString( const char * );
	void SkinParserDeleteString( const char *str );


	//= G L O B A L = V A R S ==============================================

	static CSkinParserResourceFile			*g_tempResourceFile = NULL;
	static CSkinResource								*g_tempResource = NULL;
	static CSkinRect										*g_tempRect = NULL;
	static CSkinPoint										*g_tempPoint = NULL;
	static CSkinDimension								*g_tempDimension = NULL;
	static CSkinColor										*g_tempColor = NULL;
	static CSkinText										*g_tempText = NULL;
	static CSkinScreen									*g_tempScreen = NULL;
	static CSkinIcon										*g_tempIcon = NULL;

  static std::map< CStdString, CSkinElement *>			g_namedElements;
  static std::vector<CSkinElement*>									g_unnamedElements;
	static std::vector<LPDIRECT3DTEXTURE8>						g_spriteTextureList;
	static std::vector<const char *>									g_stringVars;

	//= F L E X = G E N E R A T E D ========================================

typedef union {
    const char		*stringVal;
    int						intVal;
		void					*voidPtrVal;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		174
#define	YYFLAG		-32768
#define	YYNTBASE	102

#define YYTRANSLATE(x) ((unsigned)(x) <= 356 ? yytranslate[x] : 146)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
    86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,    99,   100,   101
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     4,     6,     8,    10,    12,    14,    16,    21,    26,
    29,    32,    35,    38,    41,    44,    47,    50,    51,    56,
    61,    66,    69,    70,    74,    75,    81,    82,    87,    92,
    95,    96,    97,   102,   105,   108,   112,   114,   115,   120,
   124,   128,   132,   136,   140,   144,   148,   149,   152,   155,
   158,   159,   163,   164,   169,   174,   179,   184,   189,   190,
   191,   196,   201,   206,   207,   208,   213,   218,   223,   224,
   225,   230,   235,   240,   245,   250,   251,   255,   256,   261,
   266,   271,   274,   277,   280,   283,   286,   287,   288,   293,
   298,   301,   304,   307,   310
};

static const short yyrhs[] = {     9,
   104,    10,     0,     3,     0,     4,     0,     7,     0,     5,
     0,     6,     0,     8,     0,   104,    11,   105,    12,     0,
   104,    13,   107,    14,     0,   104,   115,     0,   104,   117,
     0,   104,   118,     0,   104,   119,     0,   104,   120,     0,
   104,   121,     0,   104,   122,     0,   104,   123,     0,     0,
   105,    31,   103,    32,     0,   105,    33,   103,    34,     0,
   105,    35,   103,    36,     0,   105,   106,     0,     0,    37,
   103,    38,     0,     0,   107,    39,   108,   109,    40,     0,
     0,   109,    41,     7,    42,     0,   109,    43,     7,    44,
     0,   109,   110,     0,     0,     0,    45,   111,   112,    46,
     0,   113,   114,     0,   114,   113,     0,    47,     3,    48,
     0,   127,     0,     0,    15,   116,   125,    16,     0,    17,
   124,    18,     0,    19,   124,    20,     0,    21,   124,    22,
     0,    23,   124,    24,     0,    25,   124,    26,     0,    27,
   124,    28,     0,    29,   124,    30,     0,     0,   125,   126,
     0,   125,   143,     0,   125,   140,     0,     0,    61,   140,
    62,     0,     0,    49,   128,   129,    50,     0,   129,    51,
     8,    52,     0,   129,    53,     8,    54,     0,   129,    55,
     8,    56,     0,   129,    57,     8,    58,     0,     0,     0,
    88,   131,   132,    89,     0,   132,    90,     8,    91,     0,
   132,    92,     8,    93,     0,     0,     0,    94,   134,   135,
    95,     0,   135,    96,     8,    97,     0,   135,    98,     8,
    99,     0,     0,     0,    67,   137,   138,    68,     0,   138,
    74,     8,    75,     0,   138,    76,     8,    77,     0,   138,
    78,     8,    79,     0,   138,    80,     8,    81,     0,     0,
   100,   103,   101,     0,     0,    63,   141,   142,    64,     0,
   142,    65,     3,    66,     0,   142,    69,     4,    70,     0,
   142,   136,     0,   142,   139,     0,   142,    71,     0,   142,
    72,     0,   142,    73,     0,     0,     0,    84,   144,   145,
    85,     0,   145,    86,     3,    87,     0,   145,   130,     0,
   145,   133,     0,   145,   127,     0,   145,   139,     0,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   277,   282,   282,   282,   282,   282,   282,   288,   288,   289,
   290,   291,   292,   293,   294,   295,   296,   297,   305,   305,
   306,   307,   308,   312,   326,   326,   326,   342,   342,   343,
   344,   349,   349,   356,   356,   362,   368,   375,   375,   386,
   389,   392,   395,   398,   401,   404,   409,   412,   412,   413,
   414,   419,   425,   425,   431,   431,   432,   433,   434,   440,
   440,   446,   446,   447,   453,   453,   459,   459,   460,   467,
   467,   473,   473,   474,   475,   476,   481,   487,   487,   493,
   493,   494,   495,   496,   497,   498,   499,   505,   505,   511,
   511,   512,   513,   514,   515
};

static const char * const yytname[] = {   "$","error","$undefined.","_TOK_IDENTIFIER_",
"_TOK_STRING_","_TOK_MACRO_","_TOK_MACRO_OPERATOR_","_TOK_FILENAME_","_TOK_INT_",
"SKIN_OPEN","SKIN_CLOSE","METADATA_OPEN","METADATA_CLOSE","RESOURCEDEFINITIONS_OPEN",
"RESOURCEDEFINITIONS_CLOSE","HELPSCREEN_OPEN","HELPSCREEN_CLOSE","OPTIONSSCREEN_OPEN",
"OPTIONSSCREEN_CLOSE","ROMLISTSCREEN_OPEN","ROMLISTSCREEN_CLOSE","SPLASHSCREEN_OPEN",
"SPLASHSCREEN_CLOSE","MESSAGESCREEN_OPEN","MESSAGESCREEN_CLOSE","STARTMENU_OPEN",
"STARTMENU_CLOSE","VIRTUALKEYBOARD_OPEN","VIRTUALKEYBOARD_CLOSE","SCREENSAVER_OPEN",
"SCREENSAVER_CLOSE","AUTHOR_OPEN","AUTHOR_CLOSE","NAME_OPEN","NAME_CLOSE","DESCRIPTION_OPEN",
"DESCRIPTION_CLOSE","PREVIEWFILE_OPEN","PREVIEWFILE_CLOSE","RESOURCEFILE_OPEN",
"RESOURCEFILE_CLOSE","COLORCHANNELFILENAME_OPEN","COLORCHANNELFILENAME_CLOSE",
"ALPHACHANNELFILENAME_OPEN","ALPHACHANNELFILENAME_CLOSE","RESOURCE_OPEN","RESOURCE_CLOSE",
"ID_OPEN","ID_CLOSE","AREA_OPEN","AREA_CLOSE","LEFT_OPEN","LEFT_CLOSE","TOP_OPEN",
"TOP_CLOSE","RIGHT_OPEN","RIGHT_CLOSE","BOTTOM_OPEN","BOTTOM_CLOSE","BACKDROP_OPEN",
"BACKDROP_CLOSE","DEFAULTS_OPEN","DEFAULTS_CLOSE","TEXT_OPEN","TEXT_CLOSE","FONT_OPEN",
"FONT_CLOSE","COLOR_OPEN","COLOR_CLOSE","STRING_OPEN","STRING_CLOSE","CENTERVERTICALLY",
"CENTERHORIZONTALLY","UNBOUNDED","ALPHA_OPEN","ALPHA_CLOSE","RED_OPEN","RED_CLOSE",
"GREEN_OPEN","GREEN_CLOSE","BLUE_OPEN","BLUE_CLOSE","ARGB_OPEN","ARGB_CLOSE",
"ICON_OPEN","ICON_CLOSE","TEXTUREID_OPEN","TEXTUREID_CLOSE","POSITION_OPEN",
"POSITION_CLOSE","X_OPEN","X_CLOSE","Y_OPEN","Y_CLOSE","SIZE_OPEN","SIZE_CLOSE",
"WIDTH_OPEN","WIDTH_CLOSE","HEIGHT_OPEN","HEIGHT_CLOSE","CONDITION_OPEN","CONDITION_CLOSE",
"start_symbol","TEXTSTRING","skin","metadata","metadata_previewfile","resourcefiles",
"@1","resourcefile","resource","@2","resourcecontents","resourceID","resourceArea",
"helpscreen","@3","optionsscreen","romlistscreen","splashscreen","messagescreen",
"startmenu","virtualkeyboard","screensaver","garbage","screen","screendefaults",
"helper_area","@4","areacontents","helper_position","@5","positioncontents",
"helper_size","@6","sizecontents","helper_color","@7","colorcontents","helper_condition",
"helper_text","@8","textcontents","helper_icon","@9","iconcontents",""
};
#endif

static const short yyr1[] = {     0,
   102,   103,   103,   103,   103,   103,   103,   104,   104,   104,
   104,   104,   104,   104,   104,   104,   104,   104,   105,   105,
   105,   105,   105,   106,   108,   107,   107,   109,   109,   109,
   109,   111,   110,   112,   112,   113,   114,   116,   115,   117,
   118,   119,   120,   121,   122,   123,   124,   125,   125,   125,
   125,   126,   128,   127,   129,   129,   129,   129,   129,   131,
   130,   132,   132,   132,   134,   133,   135,   135,   135,   137,
   136,   138,   138,   138,   138,   138,   139,   141,   140,   142,
   142,   142,   142,   142,   142,   142,   142,   144,   143,   145,
   145,   145,   145,   145,   145
};

static const short yyr2[] = {     0,
     3,     1,     1,     1,     1,     1,     1,     4,     4,     2,
     2,     2,     2,     2,     2,     2,     2,     0,     4,     4,
     4,     2,     0,     3,     0,     5,     0,     4,     4,     2,
     0,     0,     4,     2,     2,     3,     1,     0,     4,     3,
     3,     3,     3,     3,     3,     3,     0,     2,     2,     2,
     0,     3,     0,     4,     4,     4,     4,     4,     0,     0,
     4,     4,     4,     0,     0,     4,     4,     4,     0,     0,
     4,     4,     4,     4,     4,     0,     3,     0,     4,     4,
     4,     2,     2,     2,     2,     2,     0,     0,     4,     4,
     2,     2,     2,     2,     0
};

static const short yydefact[] = {     0,
    18,     0,     1,    23,    27,    38,    47,    47,    47,    47,
    47,    47,    47,    10,    11,    12,    13,    14,    15,    16,
    17,     0,     0,    51,     0,     0,     0,     0,     0,     0,
     0,     8,     0,     0,     0,     0,    22,     9,    25,     0,
    40,    41,    42,    43,    44,    45,    46,     2,     3,     5,
     6,     4,     7,     0,     0,     0,     0,    31,    39,     0,
    78,    88,    48,    50,    49,    19,    20,    21,    24,     0,
     0,    87,    95,    26,     0,     0,    32,    30,    52,     0,
     0,     0,     0,     0,    79,     0,    70,     0,    84,    85,
    86,     0,    82,    83,    53,    89,     0,    60,    65,    93,
    91,    92,    94,    28,    29,     0,     0,     0,     0,    37,
     0,    76,     0,     0,    59,     0,    64,    69,     0,    33,
    34,    35,    80,     0,    81,    77,     0,    90,     0,     0,
    36,    71,     0,     0,     0,     0,    54,     0,     0,     0,
     0,    61,     0,     0,    66,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    72,
    73,    74,    75,    55,    56,    57,    58,    62,    63,    67,
    68,     0,     0,     0
};

static const short yydefgoto[] = {   172,
    54,     2,    22,    37,    23,    58,    70,    78,    84,   107,
   108,   109,    14,    24,    15,    16,    17,    18,    19,    20,
    21,    25,    40,    63,   110,   115,   127,   101,   117,   129,
   102,   118,   130,    93,   112,   124,    94,    64,    72,    80,
    65,    73,    81
};

static const short yypact[] = {     7,
-32768,    50,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    -7,    -6,-32768,     2,    15,     0,    13,    18,    20,
     9,-32768,    77,    77,    77,    77,-32768,-32768,-32768,   -16,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    14,    16,    17,    19,-32768,-32768,    -5,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    11,
     4,-32768,-32768,-32768,    55,    57,-32768,-32768,-32768,   -58,
   -45,    28,    30,   -37,-32768,    69,-32768,    72,-32768,-32768,
-32768,    77,-32768,-32768,-32768,-32768,    75,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,    91,    51,    47,    52,-32768,
    32,-32768,    31,    -1,-32768,    21,-32768,-32768,    54,-32768,
-32768,-32768,-32768,   -51,-32768,-32768,   -19,-32768,   -71,    -3,
-32768,-32768,    95,    96,    97,    98,-32768,    99,   101,   102,
   103,-32768,   104,   105,-32768,   106,   107,    41,    40,    39,
    38,    68,    67,    66,    65,    33,    34,    29,    26,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   128,   129,-32768
};

static const short yypgoto[] = {-32768,
   -33,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    22,    24,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    78,-32768,-32768,    49,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,    53,    73,-32768,-32768,
-32768,-32768,-32768
};


#define	YYLAST		134


static const short yytable[] = {    59,
    55,    56,    57,    95,    32,    85,    86,    38,    87,   106,
    88,    95,    89,    90,    91,     1,   132,   142,   143,    41,
   144,    43,   133,    33,   134,    34,   135,    35,   136,    36,
   137,   138,    39,   139,    42,   140,    44,   141,    47,    96,
    97,    92,    98,    45,    60,    66,    61,    46,    99,    67,
    74,    75,    68,    76,    92,    77,    69,    61,   114,     3,
     4,    82,     5,    83,     6,    79,     7,    62,     8,   104,
     9,   111,    10,   105,    11,   113,    12,   116,    13,    48,
    49,    50,    51,    52,    53,    26,    27,    28,    29,    30,
    31,   145,   146,   119,   147,    95,   120,   123,   106,   126,
   125,   131,   148,   149,   150,   151,   152,   128,   153,   154,
   155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
   165,   166,   167,   168,   171,   170,   169,   173,   174,   100,
   122,   121,    71,   103
};

static const short yycheck[] = {    16,
    34,    35,    36,    49,    12,    64,    65,    14,    67,    47,
    69,    49,    71,    72,    73,     9,    68,    89,    90,    18,
    92,    22,    74,    31,    76,    33,    78,    35,    80,    37,
    50,    51,    39,    53,    20,    55,    24,    57,    30,    85,
    86,   100,    88,    26,    61,    32,    63,    28,    94,    34,
    40,    41,    36,    43,   100,    45,    38,    63,    92,    10,
    11,     7,    13,     7,    15,    62,    17,    84,    19,    42,
    21,     3,    23,    44,    25,     4,    27,     3,    29,     3,
     4,     5,     6,     7,     8,     8,     9,    10,    11,    12,
    13,    95,    96,     3,    98,    49,    46,    66,    47,   101,
    70,    48,     8,     8,     8,     8,     8,    87,     8,     8,
     8,     8,     8,     8,     8,    75,    77,    79,    81,    52,
    54,    56,    58,    91,    99,    97,    93,     0,     0,    81,
   109,   108,    60,    81
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
{;
    break;}
case 7:
{
													yyval.stringVal = yyvsp[0].stringVal;
												;
    break;}
case 18:
{;
    break;}
case 19:
{ g_parserSkinObject->m_author = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 20:
{ g_parserSkinObject->m_name = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 21:
{ g_parserSkinObject->m_description = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 23:
{;
    break;}
case 24:
{
													if( !g_parserSkinObject->LoadPreviewTexture( yyvsp[-1].stringVal ) )
													{														 
														sperror( CStdString("Failed to load preview file ") + yyvsp[-1].stringVal );
														SkinParserDeleteString( yyvsp[-1].stringVal );
														CLEAN_AND_DIE();
													}
												;
    break;}
case 25:
{ g_tempResourceFile = new CSkinParserResourceFile(); ;
    break;}
case 27:
{
											if( g_tempResourceFile )
											{
												CStdString errorReport;
												if( !SkinParserAddResourceFile( *g_tempResourceFile, &errorReport ) )
												{
													sperror( errorReport );
													SAFE_DELETE( g_tempResourceFile );
													CLEAN_AND_DIE();
												}
												SAFE_DELETE( g_tempResourceFile );
											}
										;
    break;}
case 28:
{ g_tempResourceFile->m_colorChannelFilename = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 29:
{ g_tempResourceFile->m_alphaChannelFilename = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 31:
{
										;
    break;}
case 32:
{ g_tempResource = new CSkinResource(); ;
    break;}
case 33:
{
											if( g_tempResourceFile )
												g_tempResourceFile->m_resources.push_back( *g_tempResource );
											SAFE_DELETE( g_tempResource );
										;
    break;}
case 35:
{
											/* Both tags are required, but order independent */
										;
    break;}
case 36:
{
											g_tempResource->m_ID = yyvsp[-1].stringVal;
											SkinParserDeleteString( yyvsp[-1].stringVal );
										;
    break;}
case 37:
{
											g_tempResource->m_area = *((CSkinRect*)(yyvsp[0].voidPtrVal));
											SAFE_DELETE( yyvsp[0].voidPtrVal );
										;
    break;}
case 38:
{ g_tempScreen = new CSkinScreen( HELPSCREEN_ID ); ;
    break;}
case 39:
{
											CStdString errorReport = "";
											if( !SkinParserAddElement( g_tempScreen, &errorReport ) )
											{
												sperror( errorReport );
												SAFE_DELETE( g_tempScreen );
												CLEAN_AND_DIE();
											}
										;
    break;}
case 40:
{;
    break;}
case 41:
{;
    break;}
case 42:
{;
    break;}
case 43:
{;
    break;}
case 44:
{;
    break;}
case 45:
{;
    break;}
case 46:
{;
    break;}
case 47:
{;
    break;}
case 49:
{ CStdString errorReport = ""; if( !g_tempScreen->AddElement( *((CSkinIcon*)yyvsp[0].voidPtrVal), &errorReport ) ) { sperror( errorReport ); SAFE_DELETE( yyvsp[0].voidPtrVal ); CLEAN_AND_DIE(); } SAFE_DELETE( yyvsp[0].voidPtrVal ); ;
    break;}
case 50:
{ CStdString errorReport = ""; if( !g_tempScreen->AddElement( *((CSkinText*)yyvsp[0].voidPtrVal), &errorReport ) ) { sperror( errorReport ); SAFE_DELETE( yyvsp[0].voidPtrVal ); CLEAN_AND_DIE(); } SAFE_DELETE( yyvsp[0].voidPtrVal ); ;
    break;}
case 51:
{;
    break;}
case 52:
{ g_tempScreen->m_defaultTextAttributes = *((CSkinText*)yyvsp[-1].voidPtrVal); SAFE_DELETE( yyvsp[-1].voidPtrVal ); ;
    break;}
case 53:
{ g_tempRect = new CSkinRect(); ;
    break;}
case 54:
{
											yyval.voidPtrVal = g_tempRect;
											g_tempRect = NULL;
										;
    break;}
case 55:
{ g_tempRect->m_left = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 56:
{ g_tempRect->m_top = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 57:
{ g_tempRect->m_right = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 58:
{ g_tempRect->m_bottom = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 59:
{;
    break;}
case 60:
{ g_tempPoint = new CSkinPoint(); ;
    break;}
case 61:
{
											yyval.voidPtrVal = g_tempPoint;
											g_tempPoint = NULL;
										;
    break;}
case 62:
{ g_tempPoint->m_x = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 63:
{ g_tempPoint->m_y = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 64:
{;
    break;}
case 65:
{ g_tempDimension = new CSkinDimension(); ;
    break;}
case 66:
{
											yyval.voidPtrVal = g_tempDimension;
											g_tempDimension = NULL;
										;
    break;}
case 67:
{ g_tempDimension->m_width = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 68:
{ g_tempDimension->m_height = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 69:
{;
    break;}
case 70:
{ g_tempColor = new CSkinColor(); ;
    break;}
case 71:
{
											yyval.voidPtrVal = g_tempColor;
											g_tempColor = NULL;
										;
    break;}
case 72:
{ g_tempColor->m_alpha = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 73:
{ g_tempColor->m_red = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 74:
{ g_tempColor->m_green = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 75:
{ g_tempColor->m_blue = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 76:
{;
    break;}
case 77:
{ 
											yyval.stringVal = yyvsp[-1].stringVal;
										;
    break;}
case 78:
{ g_tempText = new CSkinText(); ;
    break;}
case 79:
{
											yyval.voidPtrVal = g_tempText;
											g_tempText = NULL;
										;
    break;}
case 80:
{ if( !FontTypeFromName( yyvsp[-1].stringVal, &g_tempText->m_fontType ) ) { sperror( CStdString("Invalid font type ") + yyvsp[-1].stringVal ); SkinParserDeleteString( yyvsp[-1].stringVal ); CLEAN_AND_DIE(); } SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 81:
{ g_tempText->m_string = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 82:
{ g_tempText->m_color = *((CSkinColor*)yyvsp[0].voidPtrVal); SAFE_DELETE( yyvsp[0].voidPtrVal ); ;
    break;}
case 83:
{ g_tempText->m_condition = yyvsp[0].stringVal; SkinParserDeleteString( yyvsp[0].stringVal ); ;
    break;}
case 84:
{ g_tempText->m_fontFlags |= XBFONT_CENTER_Y; ;
    break;}
case 85:
{ g_tempText->m_fontFlags |= XBFONT_CENTER_X; ;
    break;}
case 86:
{ g_tempText->m_fontFlags &= ~XBFONT_TRUNCATED; ;
    break;}
case 87:
{;
    break;}
case 88:
{ g_tempIcon = new CSkinIcon(); ;
    break;}
case 89:
{
											yyval.voidPtrVal = g_tempIcon;
											g_tempIcon = NULL;
										;
    break;}
case 90:
{	g_tempIcon->m_textureID = yyvsp[-1].stringVal; SkinParserDeleteString( yyvsp[-1].stringVal ); ;
    break;}
case 91:
{ g_tempIcon->m_area.m_left = ((CSkinPoint*)yyvsp[0].voidPtrVal)->m_x; g_tempIcon->m_area.m_top = ((CSkinPoint*)yyvsp[0].voidPtrVal)->m_y; SAFE_DELETE( yyvsp[0].voidPtrVal ); ;
    break;}
case 92:
{ g_tempIcon->m_area.m_right = CStdString("x+") + (CStdString)((CSkinDimension*)yyvsp[0].voidPtrVal)->m_width; g_tempIcon->m_area.m_bottom = CStdString("y+") + (CStdString)((CSkinDimension*)yyvsp[0].voidPtrVal)->m_height; SAFE_DELETE( yyvsp[0].voidPtrVal ); ;
    break;}
case 93:
{ g_tempIcon->m_area = *((CSkinRect*)yyvsp[0].voidPtrVal); SAFE_DELETE( yyvsp[0].voidPtrVal ); ;
    break;}
case 94:
{ g_tempIcon->m_condition = yyvsp[0].stringVal; SkinParserDeleteString( yyvsp[0].stringVal ); ;
    break;}
case 95:
{;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}

/*= U S E R = C O D E =========================================*/

/*--------------------------------------------------------------------
	SkinParserCleanup
--------------------------------------------------------------------*/
static void SkinParserCleanup( void )
{
	for( std::vector<LPDIRECT3DTEXTURE8>::iterator i = g_spriteTextureList.begin(); i != g_spriteTextureList.end(); ++i )
		SAFE_RELEASE( (*i) );

	for( std::map< CStdString, CSkinElement *>::iterator i = g_namedElements.begin(); i != g_namedElements.end(); ++i )
		SAFE_DELETE( (*i).second );

	for( std::vector<CSkinElement*>::iterator i = g_unnamedElements.begin(); i != g_unnamedElements.end(); ++i )
		SAFE_DELETE( (*i) );

	for( std::vector<const char *>::iterator i = g_stringVars.begin(); i != g_stringVars.end(); ++i )
		free( (void*)(*i) );
	
	g_spriteTextureList.clear();
	g_namedElements.clear();
	g_unnamedElements.clear();
	g_stringVars.clear();

	SAFE_DELETE( g_tempResourceFile );
	SAFE_DELETE( g_tempResource );
	SAFE_DELETE( g_tempRect );
	SAFE_DELETE( g_tempPoint );
	SAFE_DELETE( g_tempDimension );
	SAFE_DELETE( g_tempColor );
	SAFE_DELETE( g_tempText );
	SAFE_DELETE( g_tempScreen );
	SAFE_DELETE( g_tempIcon );
}

/*--------------------------------------------------------------------
	SkinParserNewString
--------------------------------------------------------------------*/
const char *SkinParserNewString( const char *str )
{
	if( !str )
		return NULL;

	const char *ret = strdup( str );
	if( !ret )
	{
			// TODO: Handle OOM
		return NULL;
	}

	g_stringVars.push_back( ret );
	return ret;
}

/*--------------------------------------------------------------------
	SkinParserDeleteString
--------------------------------------------------------------------*/
void SkinParserDeleteString( const char *str )
{
	if( !str )
		return;

	std::vector<const char *>::iterator i = std::find( g_stringVars.begin(), g_stringVars.end(), str );
	if( i != g_stringVars.end() )
	{
		free( (void*)(*i) );
		g_stringVars.erase( i );
	}
}

/*--------------------------------------------------------------------
	SkinParserAddResourceFile
--------------------------------------------------------------------*/
static BOOL SkinParserAddResourceFile( const CSkinParserResourceFile &element, CStdString *errorReport )
{
	if( element.m_colorChannelFilename == "" )
	{
		*errorReport = "Missing color channel filename";
		return FALSE;
	}

	UINT32 textureID = g_spriteTextureList.size();
	LPDIRECT3DTEXTURE8 t = g_parserSkinObject->LoadSkinTexture( element.m_colorChannelFilename, element.m_alphaChannelFilename );
	if( !t )
	{
		*errorReport = "Failed to load texture. Filename: ";
		*errorReport += element.m_colorChannelFilename;
		return FALSE;
	}
	g_spriteTextureList.push_back( t );


	std::vector<CSkinResource>::const_iterator i = element.m_resources.begin();
	for( ; i != element.m_resources.end(); ++i )
	{
		if( (*i).m_ID == "" )
		{
			*errorReport = "Missing ID for resource!";
			return FALSE;
		}

	 if( g_namedElements.find( (*i).m_ID ) != g_namedElements.end() )
		{
			*errorReport = "Name conflict with ID ";
			*errorReport += (*i).m_ID;
			return FALSE;
		}

		CSkinResource *res = new CSkinResource( *i );
		if( !res )
		{
			*errorReport = "Out of memory copying Resource with ID ";
			*errorReport += (*i).m_ID;
			return FALSE;
		}

		res->m_textureID = textureID;
		g_namedElements[(*i).m_ID] = res;
	}

	return TRUE;
}


/*--------------------------------------------------------------------
	SkinParserAddElement
--------------------------------------------------------------------*/
static BOOL SkinParserAddElement( CSkinElement *element, CStdString *errorReport )
{
	if( element->m_ID == "" )
		g_unnamedElements.push_back( element );
	else
	{
		if( g_namedElements.find( element->m_ID ) != g_namedElements.end() )
		{
			*errorReport = "Name conflict with ID ";
			*errorReport += element->m_ID;
			return FALSE;
		}

		g_namedElements[element->m_ID] = element;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
	FontTypeFromName
--------------------------------------------------------------------*/
static BOOL	FontTypeFromName( const char *name, fonttype *ret )
{
	if( !stricmp( name, "default" ) || !stricmp( name, "standard" ) )
		*ret = FONTTYPE_DEFAULT;
	else if( !stricmp( name, "fixedwidth" ) )
		*ret = FONTTYPE_FIXEDWIDTH;
	else if( !stricmp( name, "largethin" ) )
		*ret = FONTTYPE_LARGETHIN;
	else if( !stricmp( name, "smallthin" ) )
		*ret = FONTTYPE_SMALLTHIN;

	return FALSE;
}
