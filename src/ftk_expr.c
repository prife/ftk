/*
 * File:    expr.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   express eval
 *
 * Copyright (c) 2009  Li XianJing <xianjimli@hotmail.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2009-07-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

typedef enum tagToken
{
	TOK_NONE,   
	TOK_ADD,      //'+'
	TOK_SUB,      //'-'
	TOK_MULTI,    //'*'
	TOK_DIV,      //'/'
	TOK_LPAREN,   //'('
	TOK_RPAREN,   //')'
	TOK_NUM,      //number
	TOK_NR
}Token;

typedef struct tagLex
{
	const char*  buffer;
	size_t read_pos;
	
	double val;
	Token  token_type;
	int    b_unget_token;
}Lex, *PLex;

static double EvalExpr(PLex pLex);

static Token LexGetNumToken(PLex thiz)
{
	enum
	{
		STAT_NONE,
		STAT_AFTER_0,
		STAT_AFTER_X,
		STAT_FINISH
	}state = STAT_NONE;

	thiz->val = 0;
	for(;thiz->buffer[thiz->read_pos] != '\0' && state != STAT_FINISH; thiz->read_pos++)
	{
		char c = thiz->buffer[thiz->read_pos];
		switch(state)
		{
			case STAT_NONE:
			{
				if(c == '0')
				{
					state = STAT_AFTER_0;
				}
				else
				{
					double val = 0;
					sscanf(thiz->buffer+thiz->read_pos, "%lf", &val);
					thiz->val = val;
					state = STAT_FINISH;
				}
				break;
			}
			case STAT_AFTER_0:
			{
				if(c == 'x' || c == 'X')
				{
					state = STAT_AFTER_X;
				}
				else
				{
					int val = 0;
					sscanf(thiz->buffer+thiz->read_pos, "%o", &val);
					thiz->val = val;
					state = STAT_FINISH;
				}
				break;
			}
			case STAT_AFTER_X:
			{
				int val = 0;
				sscanf(thiz->buffer+thiz->read_pos, "%x", &val);
				thiz->val = val;
				state = STAT_FINISH;
				break;
			}
			default:break;
		}
	}

	while(thiz->buffer[thiz->read_pos] != '\0' && 
		(isdigit(thiz->buffer[thiz->read_pos]) || thiz->buffer[thiz->read_pos] == '.'))
	{
		++thiz->read_pos;
	}

	return;
}

static Token LexGetToken(PLex thiz)
{
	assert(thiz != NULL && thiz->buffer != NULL);

	if(thiz == NULL || thiz->buffer == NULL || thiz->buffer[thiz->read_pos] == '\0')
	{
		thiz->token_type = TOK_NONE;
		
		return thiz->token_type;
	}

	if(thiz->b_unget_token)
	{
		thiz->b_unget_token = !thiz->b_unget_token;
		
		return thiz->token_type;
	}

	thiz->token_type = TOK_NONE;

	while(thiz->buffer[thiz->read_pos] != '\0')
	{
		switch(thiz->buffer[thiz->read_pos])
		{
			case '+':
			{
				thiz->token_type = TOK_ADD;
				break;
			}
			case '-':
			{
				thiz->token_type = TOK_SUB;
				break;
			}
			case '*':
			{
				thiz->token_type = TOK_MULTI;
				break;
			}
			case '/':
			{
				thiz->token_type = TOK_DIV;
				break;
			}
			case '(':
			{
				thiz->token_type = TOK_LPAREN;
				break;
			}
			case ')':
			{
				thiz->token_type = TOK_LPAREN;
				break;
			}
			default:
			{
				if(isdigit(thiz->buffer[thiz->read_pos]))
				{
					LexGetNumToken(thiz);
					thiz->token_type = TOK_NUM;
				}
				else
				{
					//skip invalid cahr.
				}
				break;
			}
		}

		if(thiz->token_type != TOK_NUM)
		{
			++thiz->read_pos;
		}

		if(thiz->token_type != TOK_NONE)
		{
			break;
		}
	}
	
	return thiz->token_type;
}

static void LexUngetToken(PLex thiz)
{
	assert(thiz != NULL && thiz->buffer != NULL);

	thiz->b_unget_token = !thiz->b_unget_token;

	return;
}

//<FACTOR> ::= ( <EXPR> ) | number
static double EvalFactor(PLex pLex)
{
	double val = 0;
	Token token_type = LexGetToken(pLex);
	
	switch(token_type)
	{
		case TOK_LPAREN:
		{
			val = EvalExpr(pLex);
			break;
		}
		case TOK_NUM:
		{
			val = pLex->val;
			break;
		}
		default:
		{
			//unexpected token.
			break;
		}
	}

	return val;
}

//<TERM>  ::= <FACTOR>  { <MULOP> <FACTOR> }
//<MULOP> ::= * | /
static double EvalTerm(PLex pLex)
{
	double val = EvalFactor(pLex);
	Token token_type = LexGetToken(pLex);
	
	while(token_type == TOK_MULTI || token_type == TOK_DIV)
	{
		if(token_type == TOK_MULTI)
		{
			val *= EvalFactor(pLex);
		}
		else
		{
			val /= EvalFactor(pLex);
		}
		token_type = LexGetToken(pLex);
	}

	LexUngetToken(pLex);
	
	return val;
}

//EXPR  ::= TERM { ADDOP TERM }
//ADDOP ::= + | -
static double EvalExpr(PLex pLex)
{
	double val = EvalTerm(pLex);
	Token token_type = LexGetToken(pLex);
	
	while(token_type == TOK_ADD || token_type == TOK_SUB)
	{
		if(token_type == TOK_ADD)
		{
			val += EvalTerm(pLex);
		}
		else
		{
			val -= EvalTerm(pLex);
		}

		token_type = LexGetToken(pLex);
	}

	//LexUngetToken(pLex);

	return val;
}
		
double ftk_expr_eval(const char* expr)
{
	Lex aLex = {0};
	aLex.buffer = expr;
	aLex.read_pos = 0;

	return EvalExpr(&aLex);
}

#ifdef EXPR_TEST
typedef struct tagTESTCASE_T
{
	double val;
	char* expr;
}TESTCASE_T, *PTESTCASE_T;

TESTCASE_T g_aTestcase[] =
{
	{8, "010"},
	{9, "011"},
	{1, "0x1"},
	{1, "0x001"},
	{15, "0x0F"},
	{15, "0XF"},
	{5, "05"},
	{15, "05+(5+0x5)"},
	{1, "1.00"},
	{3, "1.5+1.5"},
	{3, "1+1+1"},
	{3, "(1+1+1)*1.00"},
	{7.5, "(1.2+1.8)*2.5"},
	{1, "1"},
	{2, "1+1"},
	{3, "1+1+1"},
	{4, "(1+1)*2"},
	{5, "2*2+1"},
	{6, "2+(2*2)"},
	{7, "(1+2)+(2*2)"},
	{8, "64/8)"},
	{9, "(82-1)/9)"},
	{0, "(64/8-8)"},
	{1, "  1  "},
	{2, "  1  +  1  "},
	{3, "  1  +  1  +  1"},
	{4, "  (  1  +  1  )  *  2  "},
	{5, "  2  *  2  +  1  "},
	{6, "  2  +  (  2  *  2  )"},
	{7, "  (  1  +  2  )  +  (  2  *  2  )  "},
	{8, "  64 /  8  ) "},
	{9, "  (  82 -  1  )  /  9  )  "},
	{0, "  (  64 /  8  -  8  )  "},
	{0, NULL}
};

int main(int argc, char* argv[])
{
	int i = 0;
	double val = 0;

	while(g_aTestcase[i].expr != NULL)
	{
		val = expr_eval(g_aTestcase[i].expr);
		printf("%s=%lf\n", g_aTestcase[i].expr, val);
		assert(g_aTestcase[i].val == val);

		i++;
	}
	
	return 0;
}
#endif/*EXPR_TEST*/
