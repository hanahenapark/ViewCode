#ifndef EVAL_env_C
#define EVAL_env_C

#include <stdio.h>
#include <stdint.h>

void loadEnv(clispPtrENV clsp);

clispPtrENV loadLisp(void);

#endif

#ifdef CLISP

#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#include <stddef.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>


#define checkCorrectArgs(min_, max_) do { \
  int args_length_ = cLISPlist_length(args); \
  if (args_length_ < min_) { *e = ERROR_TOO_FEW_ARGS; return cLISPnull(); } \
  if (args_length_ > max_) { *e = ERROR_TOO_MANY_ARGS; return cLISPnull(); } \
} while (0);

static Lisp Evalcons(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(2, 2);
    Lisp x = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp y = cLISPcar(args);
    return cLISPcons(x, y, clsp);
}

static Lisp Evalcar(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPcar(cLISPcar(args));
}

static Lisp Evalcdr(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPcdr(cLISPcar(args));
}

static Lisp Evalexact_eq(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp a = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp b = cLISPcar(args);
    return cLISPmake_bool(cLISPeq(a, b));
}

static Lisp Evalequal(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp a = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp b = cLISPcar(args);
    return cLISPmake_bool(cLISPequal(a, b));
}

static Lisp Evalequal_r(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp a = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp b = cLISPcar(args);
    return cLISPmake_bool(cLISPequal_r(a, b));
}

static Lisp Evalis_null(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_bool(isNULLType(cLISPcar(args)));
}

static Lisp Evalis_pair(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_bool(cLISPtype(cLISPcar(args)) == cLispPAIR);
}

static Lisp Evalwrite(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(2, 2);
    Lisp a = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp b = cLISPcar(args);
    //FILE (car of cdr, second argument) 
    cLISPprintf(cLISPport(b), a);
    return cLISPnull();
}


static Lisp Evalterpri(Lisp args, cLispError* e, clispPtrENV clsp){
    printf("\n\n");
    return cLISPnull();
}
static Lisp Evalerror(Lisp args, cLispError* e, clispPtrENV clsp)
{
   if (isPairType(args))
   {
       Lisp l = cLISPcar(args);
       fputs(cLISPstring(l), cLISPstderr(clsp));
       args = cLISPcdr(args);
   }
   while (isPairType(args))
   {
       fputs(" ", cLISPstderr(clsp));
       cLISPprintf(cLISPstderr(clsp), cLISPcar(args));
       args = cLISPcdr(args);
   }

   *e = ERROR_RUNTIME;
   return cLISPnull();
}

static Lisp Evalsyntax_error(Lisp args, cLispError* e, clispPtrENV clsp)
{
    fprintf(cLISPstderr(clsp), "expand error: %s ", cLISPstring(cLISPcar(args)));
    args = cLISPcdr(args);
    if (!isNULLType(args))
    {
        cLISPprintf(cLISPstderr(clsp), cLISPcar(args));
    }
    fprintf(cLISPstderr(clsp), "\n");

    *e = ERROR_FORM_SYNTAX;
    return cLISPnull();
}

static Lisp Evalequals(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp to_check  = cLISPcar(args);
    if (isNULLType(to_check)) return cLISPtrue();
    args = cLISPcdr(args);

    while (isPairType(args))
    {
        if (cLISPbool(cLISPcar(args)) != cLISPbool(to_check)) return cLISPfalse();
        args = cLISPcdr(args);
    }
    return cLISPtrue();
}

static Lisp Evallist(Lisp args, cLispError* e, clispPtrENV clsp) { return args; }

static Lisp Evalis_list(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_bool(cLISPis_list(cLISPcar(args)));
}


static Lisp Evalappend(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp l = cLISPnull();  

    while (isPairType(args))
    {
        l = cLISPlist_append(l, cLISPcar(args), clsp);
        args = cLISPcdr(args);
    }
    return l;
}

static Lisp Evalappend_reverse(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(2, 2);

    Lisp l = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp tail = cLISPcar(args);

    return cLISPlist_reverse2(l, tail);
}

static Lisp Evallist_ref(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(2, 2);
    Lisp list = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp index = cLISPcar(args);
    return cLISPlist_ref(list, cLISPint(index));
}

static Lisp Evallength(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_int(cLISPlist_length(cLISPcar(args)));
}

static Lisp Evaladd(Lisp args, cLispError* e, clispPtrENV clsp)
{
    LispInt exact = 0;
    LispReal inexact = 0; 

    while (isPairType(args))
    {
        Lisp x = cLISPcar(args);
        args = cLISPcdr(args);
        switch (cLISPtype(x))
        {
            case cLispINT:
                exact += cLISPint(x);
                break;
            case cLispREAL:
                inexact += cLISPreal(x);
                break;
            default:
                *e = ERROR_ARG_TYPE;
                return cLISPnull();
        }
    }
    
    return inexact == 0
        ? cLISPmake_int(exact)
        : cLISPmake_real(inexact + (LispReal)exact);
}

static Lisp Evalmult(Lisp args, cLispError* e, clispPtrENV clsp)
{
    LispInt exact = 1;
    LispReal inexact = 1; 

    while (isPairType(args))
    {
        Lisp x = cLISPcar(args);
        args = cLISPcdr(args);
        switch (cLISPtype(x))
        {
            case cLispINT:
                exact *= cLISPint(x);
                break;
            case cLispREAL:
                inexact *= cLISPreal(x);
                break;
            default:
                *e = ERROR_ARG_TYPE;
                return cLISPnull();
        }
    }
    
    return inexact == 1
        ? cLISPmake_int(exact)
        : cLISPmake_real(inexact * (LispReal)exact);
}

static Lisp Evalsub(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 2);
    Lisp x = cLISPcar(args);
    args = cLISPcdr(args);

    Lisp y;
    if (isNULLType(args))
    {
        y = x;
        x = cLISPmake_int(0);
    }
    else
    {
        y = cLISPcar(args);
    }
    switch (cLISPtype(x))
    {
        case cLispREAL:
            return cLISPmake_real(cLISPreal(x) - cLISPnumber_to_real(y));
        case cLispINT:
            switch (cLISPtype(y))
            {
                case cLispREAL:
                    return cLISPmake_real(cLISPnumber_to_real(x) - cLISPreal(y));
                case cLispINT:
                    return cLISPmake_int(cLISPint(x) - cLISPint(y));
                default:
                    *e = ERROR_ARG_TYPE;
                    return cLISPnull();
            }
            break;
        default:
            *e = ERROR_ARG_TYPE;
            return cLISPnull();
    }
}

static Lisp Evaldivide(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(2, 2);
    Lisp x = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp y = cLISPcar(args);

    switch (cLISPtype(x))
    {
        case cLispREAL:
            return cLISPmake_real(cLISPreal(x) / cLISPnumber_to_real(y));
        case cLispINT:
            switch (cLISPtype(y))
            {
                case cLispREAL:
                    return cLISPmake_real(cLISPnumber_to_real(x) / cLISPreal(y));
                case cLispINT:

                    return cLISPmake_int(cLISPint(x) / cLISPint(y));
                default:
                    *e = ERROR_ARG_TYPE;
                    return cLISPnull();
            }
            break;
        default:
            *e = ERROR_ARG_TYPE;
            return cLISPnull();
    }
}

static Lisp Evalless(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(2, 2);
    Lisp x = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp y = cLISPcar(args);

    switch (cLISPtype(x))
    {
        case cLispREAL:
            return cLISPmake_bool(cLISPreal(x) < cLISPnumber_to_real(y));
        case cLispINT:
            switch (cLISPtype(y))
            {
                case cLispREAL:
                    return cLISPmake_bool(cLISPnumber_to_real(x) < cLISPreal(y));
                case cLispINT:
                    return cLISPmake_bool(cLISPint(x) < cLISPint(y));
                default:
                    *e = ERROR_ARG_TYPE;
                    return cLISPnull();
            }
            break;
        default:
            *e = ERROR_ARG_TYPE;
            return cLISPnull();
    }
}

static Lisp Evalto_exact(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_int(cLISPnumber_to_int(cLISPcar(args)));
}

static Lisp Evalto_inexact(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_real(cLISPnumber_to_real(cLISPcar(args)));
}

static Lisp Evalis_symbol(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_bool(cLISPtype(cLISPcar(args)) == cLispSYMBOL);
}

static Lisp Evalstring_to_symbol(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    Lisp val = cLISPcar(args);
    if (cLISPtype(val) != cLispSTRING)
    {
        *e = ERROR_ARG_TYPE;
        return cLISPnull();
    }
    else
    {
        return createSymbol(cLISPstring(val), clsp);
    }
}

static Lisp Evalgensym(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(0, 0);
    return cLISPgen_symbol(clsp);
}

static Lisp Evalstring_append(Lisp args, cLispError* e, clispPtrENV clsp)
{
    int count = 0;
    Lisp it = args;
    while (isPairType(it))
    {
        Lisp x = cLISPcar(it);
        count += strlen(cLISPstring(x));
        it = cLISPcdr(it);
    }

    Lisp result = cLISPmake_string(count, clsp);
    char* c = cLISPbuffer(result);

    it = args;
    while (isPairType(it))
    {
        Lisp x = cLISPcar(it);
        int n = (LispInt)strlen(cLISPstring(x));
        memcpy(c, cLISPstring(x), n);
        c += n;
        it = cLISPcdr(it);
    }
    return result;
}

static Lisp Evalstring_to_list(Lisp args, cLispError* e, clispPtrENV clsp)
{
    const char* c = cLISPstring(cLISPcar(args));
    Lisp tail = cLISPnull();
    while (*c)
    {
        tail = cLISPcons(cLISPmake_char(*c), tail, clsp);
        ++c;
    }
    return cLISPlist_reverse(tail);
}

static Lisp Evallist_to_string(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp l = cLISPcar(args);
    Lisp result = cLISPmake_string(cLISPlist_length(l), clsp);
    char* s = cLISPbuffer(result);
    
    while (isPairType(l))
    {
        *s = (char)cLISPchar(cLISPcar(l));
        ++s;
        l = cLISPcdr(l);
    }
    return result;
}

static Lisp Evalstring_to_number(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    const char* string = cLISPstring(cLISPcar(args));
    if (strchr(string, '.'))
    {
        return cLISPparse_real(string);
    }
    else
    {
        return cLISPparse_int(string);
    }
}

static Lisp Evalnumber_to_string(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    char tempArray[64];
    Lisp val = cLISPcar(args);
    switch (cLISPtype(val))
    {
        case cLispREAL:
            snprintf(tempArray, 64, "%f", cLISPreal(val));
            break;
        case cLispINT:
            snprintf(tempArray, 64, "%lli", cLISPint(val));
            break;
        default:
        {
            *e = ERROR_ARG_TYPE;
            return cLISPnull();
        }
    }
    return cLISPmake_string2(tempArray, clsp);
}

static Lisp Evalis_char(Lisp args, cLispError* e, clispPtrENV clsp)
{
    return cLISPmake_bool(cLISPtype(cLISPcar(args)) == cLispCHAR);
}

static Lisp Evalis_int(Lisp args, cLispError* e, clispPtrENV clsp)
{
    return cLISPmake_bool(cLISPtype(cLISPcar(args)) == cLispINT);
}

static Lisp Evalis_real(Lisp args, cLispError* e, clispPtrENV clsp)
{
    clispType t = cLISPtype(cLISPcar(args));
    return cLISPmake_bool(t == cLispINT || t == cLispREAL);
}


static Lisp Evalnot(Lisp args, cLispError* e, clispPtrENV clsp)
{
    return cLISPmake_bool(!isboolTrue(cLISPcar(args)));
}

static Lisp Evalis_even(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_bool( (cLISPint(cLISPcar(args)) & 1) == 0);
}

static Lisp Evalexp(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    return cLISPmake_real( exp(cLISPnumber_to_real(cLISPcar(args))) );
}

static Lisp Evalabs(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    Lisp x = cLISPcar(args);
    switch (cLISPtype(x))
    {
        case cLispINT:
            return cLISPmake_int(llabs(cLISPint(x)));
        case cLispREAL:
            return cLISPmake_real(fabs(cLISPreal(x)));
        default:
            *e = ERROR_ARG_TYPE;
            return cLISPnull();
    }
}

static Lisp Evalis_vector(Lisp args, cLispError* e, clispPtrENV clsp)
{
    return cLISPmake_bool( cLISPtype(cLISPcar(args)) == cLispVECTOR );
}

static Lisp Evalmake_vector(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp length = cLISPcar(args);

    if (cLISPtype(length) != cLispINT)
    {
        *e = ERROR_ARG_TYPE;
        return cLISPnull();
    }

    Lisp v = cLISPmake_vector(cLISPint(length), clsp);
    args = cLISPcdr(args);

    if (!isNULLType(args))
        cLISPvector_fill(v, cLISPcar(args));
    return v;
}

static Lisp Evalvector_grow(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp v = cLISPcar(args);
    Lisp length = cLISPcar(cLISPcdr(args));

    if (cLISPtype(length) != cLispINT || cLISPtype(v) != cLispVECTOR)
    {
        *e = ERROR_ARG_TYPE;
        return cLISPnull();
    }

    if (cLISPint(length) < cLISPvector_length(v))
    {
        *e = ERROR_OUT_OF_BOUNDS;
        return cLISPnull();
    }

    return cLISPvector_grow(v, cLISPint(length), clsp);
}

static Lisp Evalvector_length(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp v = cLISPcar(args);
    if (cLISPtype(v) != cLispVECTOR)
    {
        *e = ERROR_ARG_TYPE;
        return cLISPnull();
    }

    return cLISPmake_int(cLISPvector_length(v));
}


static Lisp Evalvector_set(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp v = cLISPlist_ref(args, 0);
    Lisp i = cLISPlist_ref(args, 1);
    Lisp x = cLISPlist_ref(args, 2);

    if (cLISPtype(v) != cLispVECTOR || cLISPtype(i) != cLispINT)
    {
        *e = ERROR_ARG_TYPE;
        return cLISPnull();
    }

    if (cLISPint(i) >= cLISPvector_length(v))
    {
        *e = ERROR_OUT_OF_BOUNDS;
        return cLISPnull();
    }

    cLISPvector_set(v, cLISPint(i), x);
    return cLISPnull();
}

static Lisp Evalapply(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp operator = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp op_args = cLISPcar(args);
    return cLISPapply(operator, op_args, e, clsp);
}


static Lisp Evaleval(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 2);
    Lisp expr = cLISPcar(args);
    args = cLISPcdr(args);
    Lisp env = isNULLType(args) ? cLISPenv(clsp) : cLISPcar(args);
    return cLISPevalPtr(expr, env, e, clsp);
}

static Lisp Evalsystem_env(Lisp args, cLispError* e, clispPtrENV clsp)
{
    checkCorrectArgs(1, 1);
    if (cLISPint(cLISPcar(args)) == 5)
    {
        return cLISPcdr(cLISPenv(clsp));
    }
    else
    {
        *e = ERROR_RUNTIME;
        return cLISPnull();
    }
}

static Lisp Evaluser_env(Lisp args, cLispError* e, clispPtrENV clsp)
{
    return cLISPenv(clsp);
}


#undef ARITY_CHECK

static const LispFuncDef cLispfuncDefinitions[] = {
    {"TERPRI", Evalterpri},
    { "CONS", Evalcons },
    { "CAR", Evalcar },
    { "CDR", Evalcdr },
    { "NULL?", Evalis_null },
    { "PAIR?", Evalis_pair },
    { "EVAL", Evaleval },
    { "ERROR", Evalerror },
    { "SYNTAX-ERROR", Evalsyntax_error },
    { "_WRITE", Evalwrite },
    { "EQ?", Evalexact_eq },    //values and types the same
    { "EQV?", Evalequal },
    { "EQUAL?", Evalequal_r }, //are values the same
    { "SYMBOL?", Evalis_symbol },
    { "NOT", Evalnot },
    { "LIST", Evallist },
    { "LIST?", Evalis_list },
    { "LENGTH", Evallength },
    { "APPEND", Evalappend },
    { "APPEND-REVERSE!", Evalappend_reverse },
    { "LIST-REF", Evallist_ref },
    { "VECTOR?", Evalis_vector },
    { "MAKE-VECTOR", Evalmake_vector },
    { "VECTOR-GROW", Evalvector_grow },
    { "VECTOR-LENGTH", Evalvector_length },
    { "VECTOR-SET!", Evalvector_set },
    { "STRING-APPEND", Evalstring_append },
    { "STRING->LIST", Evalstring_to_list },
    { "LIST->STRING", Evallist_to_string },
    { "STRING->NUMBER", Evalstring_to_number },
    { "NUMBER->STRING", Evalnumber_to_string },
    { "CHAR?", Evalis_char },
    { "CHAR=?", Evalequals },
    { "=", Evalequals },
    { "+", Evaladd },
    { "-", Evalsub },
    { "*", Evalmult },
    { "/", Evaldivide },
    { "<", Evalless },
    { "INTEGER?", Evalis_int },
    { "EVEN?", Evalis_even },
    { "REAL?", Evalis_real },
    { "EXP", Evalexp },
    { "MAGNITUDE", Evalabs },
    { "EXACT?", Evalis_int },
    { "EXACT->INEXACT", Evalto_inexact },
    { "INEXACT->EXACT", Evalto_exact },
    { "STRING->SYMBOL", Evalstring_to_symbol },
    { "GENSYM", Evalgensym },
    { "SCHEME-REPORT-ENVIRONMENT", Evalsystem_env },
    { "INTERACTION-ENVIRONMENT", Evaluser_env },

    { "APPLY", Evalapply },

    { NULL, NULL }
    
};

void loadEnv(clispPtrENV clsp)
{
    static const char* libBasics1 = 
"(define-macro lambda (/\\_ args \n\
           (if (pair? args) \n\
               (if (pair? (cdr args)) \n\
                   (if (pair? (cdr (cdr args))) \n\
                       `(/\\_ ,(car args) ,(cons 'BEGIN (cdr args))) \n\
                       `(/\\_ ,(car args) ,(car (cdr args)))) \n\
                   (syntax-error \"lambda missing body expressions: (lambda (args) body)\")) \n\
               (syntax-error \"lambda missing argument: (lambda (args) body)\")))) \n\
 \n\
(define-macro set (lambda (var x) \n\
                     (begin \n\
                       (if (not (symbol? var)) (syntax-error \"set not a variable\")) \n\
                       `(SET! ,var ,x)))) \n\
 \n\
(define-macro define \n\
              (lambda (var . exprs) \n\
                (if (symbol? var) \n\
                    (if (pair? (cdr exprs)) \n\
                        (syntax-error \"define: (define var x)\") \n\
                        `(DEF ,var ,(car exprs))) \n\
                    (if (pair? var) \n\
                        `(DEF ,(car var) \n\
                               (LAMBDA ,(cdr var) \n\
                                       ,(if (null? (cdr exprs)) (car exprs) (cons 'BEGIN exprs)))) \n\
                        (syntax-error \"define: not a symbol\") )))) \n\
  \n\
(define (first x) (car x))  \n\
(define (second x) (car (cdr x)))  \n\
(define (third x) (car (cdr (cdr x))))  \n\
 \n\
(define (some? pred l)  \n\
  (if (null? l) #f  \n\
      (if (pred (car l)) #t  \n\
          (some? pred (cdr l)))))  \n\
 \n\
(define (_map1-helper proc l result)  \n\
  (if (null? l)  \n\
      (reverse! result)  \n\
      (_map1-helper proc  \n\
                    (cdr l)  \n\
                    (cons (proc (car l)) result))))  \n\
 \n\
(define (map1 proc l) (_map1-helper proc l '()))  \n\
 \n\
(define (for-each1 proc l)  \n\
  (if (null? l) '()  \n\
      (begin (proc (car l)) (for-each1 proc (cdr l )))))  \n\
 \n\
(define (reverse! l) (append-reverse! l '())) \n\
(define (reverse l) (reverse! (list-copy l)))  \n\
 \n\
(define (last-pair x)  \n\
 (if (pair? (cdr x))  \n\
  (last-pair (cdr x)) x))  \n\
 \n\
(define (list-tail x k)  \n\
 (if (zero? k) x  \n\
  (list-tail (cdr x) (- k 1))))  \n\
 \n\
(define (fold-left op acc lst) \n\
  (if (null? lst) acc  \n\
      (fold-left op (op acc (car lst)) (cdr lst)))) \n\
 \n\
(define (_expand-shorthand-body path)  \n\
  (if (null? path) (cons 'pair '())  \n\
      (list (if (char=? (car path) #\\A)  \n\
            (cons 'CAR (_expand-shorthand-body (cdr path)))))))  \n\
 \n\
(define (_expand-shorthand text)  \n\
  (cons 'DEFINE  (cons (list (string->symbol (string-append \"C\" text \"R\")) 'pair)  \n\
        (_expand-shorthand-body (string->list text)))))  \n\
 \n\
(define-macro _shorthand-accessors (lambda args (cons 'BEGIN (map1 _expand-shorthand args))))";

static const char* libBasics2 = 
"(define (_make-lambda args body)  \n\
  (list 'LAMBDA args (if (null? (cdr body)) (car body) (cons 'BEGIN body))))  \n\
  \n\
 \n\
; (LET <name> ((<var0> <expr0>) ... (<varN> <expr1>)) <body0> ... <bodyN>) \n\
;  => ((LAMBDA (<var0> ... <varN>) (BEGIN <body0> ... <bodyN>)) <expr0> ... <expr1>)             \n\
;  => named  \n\
;    ((lambda () \n\
;        (define <name> (LAMBDA (<var0> ... <varN>) (BEGIN <body0> ... <bodyN>))) \n\
;        (<name> <expr0> ... <exprN>))) \n\
 \n\
(define (_check-binding-list bindings)  \n\
  (for-each1 (lambda (entry)  \n\
               (if (not (pair? entry)) (syntax-error \"bad let binding\" entry))  \n\
               (if (not (symbol? (first entry))) (syntax-error \"let entry missing symbol\" entry))) bindings))  \n\
  \n\
(define (_let->combination var bindings body)  \n\
  (_check-binding-list bindings)  \n\
  (define body-func (_make-lambda (map1 (lambda (entry) (first entry)) bindings) body))  \n\
  (define initial-args (map1 (lambda (entry) (second entry)) bindings))  \n\
  (if (null? var)  \n\
      (cons body-func initial-args)  \n\
      (list (_make-lambda '() (list (list 'DEFINE var body-func) (cons var initial-args))))))  \n\
 \n\
(define-macro let (lambda args   \n\
                    (if (pair? (first args))  \n\
                        (_let->combination '() (car args) (cdr args))  \n\
                        (_let->combination (first args) (second args) (cdr (cdr args))))))  \n\
 \n\
(define (_let*-helper bindings body)  \n\
  (if (null? bindings) (if (null? (cdr body)) (car body) (cons 'BEGIN body))  \n\
      (list 'LET (list (car bindings)) (_let*-helper (cdr bindings) body))))  \n\
 \n\
(define-macro let* (lambda (bindings . body)  \n\
                     (_check-binding-list bindings)  \n\
                     (_let*-helper bindings body)))  \n\
 \n\
(define-macro letrec (lambda (bindings . body)  \n\
                       (_check-binding-list bindings)  \n\
                       (cons (_make-lambda (map1 (lambda (entry) (first entry)) bindings)  \n\
                                           (append (map1 (lambda (entry) (list 'SET! (first entry) (second entry)))  \n\
                                                         bindings) body))  \n\
                             (map1 (lambda (entry) '()) bindings))))  \n\
 \n\
 \n\
(define (_cond-check-clauses clauses)  \n\
  (for-each1 (lambda (clause)  \n\
               (if (not (pair? clause)) (syntax-error \"cond: invalid clause\"))  \n\
               (if (null? (cdr clause)) (syntax-error \"cond: clause missing expression\")))  \n\
             clauses))  \n\
 \n\
(define (_cond-helper clauses)  \n\
  (if (null? clauses) '()  \n\
      (if (eq? (car (car clauses)) 'ELSE)  \n\
          (cons 'BEGIN (cdr (car clauses)))  \n\
          (list 'IF  \n\
                (car (car clauses))  \n\
                (cons 'BEGIN (cdr (car clauses)))  \n\
                (_cond-helper (cdr clauses))))))  \n\
 \n\
(define-macro cond (lambda clauses  \n\
                     (begin  \n\
                       (_cond-check-clauses clauses)  \n\
                       (_cond-helper clauses))))";

static const char* libBasics3 = 
"(_shorthand-accessors \"AA\" \"DD\" \"AD\" \"DA\" \"AAA\" \"AAD\" \"ADA\" \"DAA\" \"ADD\" \"DAD\" \"DDA\" \"DDD\")  \n\
 \n\
(define (_and-helper preds)  \n\
  (cond ((null? preds) #t)  \n\
        ((null? (cdr preds)) (car preds))  \n\
        (else  \n\
          `(IF ,(car preds) ,(_and-helper (cdr preds)) #f))))  \n\
(define-macro and? (lambda preds (_and-helper preds)))  \n\
 \n\
(define (_or-helper preds var)  \n\
  (cond ((null? preds) #f)  \n\
        ((null? (cdr preds)) (car preds))  \n\
        (else  \n\
          `(BEGIN (SET! ,var ,(car preds))  \n\
                  (IF ,var ,var ,(_or-helper (cdr preds) var))))))  \n\
 \n\
(define-macro or? (lambda preds  \n\
                   (let ((var (gensym)))  \n\
                     `(LET ((,var '())) ,(_or-helper preds var)))))  \n\
 \n\
(define-macro case (lambda (key . clauses)  \n\
                     (let ((expr (gensym)))  \n\
                       `(LET ((,expr ,key))  \n\
                          ,(cons 'COND (map1 (lambda (entry)  \n\
                                               (cons (if (pair? (car entry))  \n\
                                                         `(MEMV ,expr (quote ,(car entry)))  \n\
                                                         (car entry))  \n\
                                                     (cdr entry))) clauses))))))  \n\
 \n\
(define-macro push  \n\
              (lambda (v l)  \n\
                `(begin (set ,l (cons ,v ,l)) ,l)))  \n\
 \n\
; (DO ((<var0> <init0> <step0>) ...)  (<test> <result>) <body>) \n\
(define-macro do  \n\
              (lambda (vars loop-check . loops)  \n\
                (let ( (names '()) (inits '()) (steps '()) (f (gensym)) )  \n\
                  (for-each1 (lambda (var)  \n\
                               (push (car var) names)  \n\
                               (set var (cdr var))  \n\
                               (push (car var) inits)  \n\
                               (set var (cdr var))  \n\
                               (push (car var) steps)) vars)  \n\
                  `((LAMBDA ()  \n\
                      (DEFINE ,f (LAMBDA ,names  \n\
                                   (IF ,(car loop-check)  \n\
                                       ,(if (pair? (cdr loop-check)) (car (cdr loop-check)) '())  \n\
                                       ,(cons 'BEGIN (append loops (list (cons f steps)))) )))  \n\
                      ,(cons f inits)  \n\
                      )) )))";

static const char* libBasics4 = 
"(define (number? x) (real? x))  \n\
 \n\
(define (>= a b) (not (< a b)))  \n\
(define (> a b) (< b a))  \n\
(define (<= a b) (not (< b a)))  \n\
 \n\
(define (max . ls)  \n\
  (fold-left (lambda (m x)  \n\
               (if (> x m)  \n\
                   x  \n\
                   m)) (car ls) (cdr ls)))  \n\
 \n\
(define (min . ls)  \n\
  (fold-left (lambda (m x)  \n\
               (if (< x m)  \n\
                   x  \n\
                   m)) (car ls) (cdr ls)))  \n\
 \n\
(define (_gcd-helper a b)  \n\
  (if (= b 0) a (_gcd-helper b (modulo a b))))  \n\
 \n\
(define (gcd . args)  \n\
  (if (null? args) 0  \n\
      (_gcd-helper (car args) (car (cdr args)))))  \n\
 \n\
(define (lcm . args)  \n\
  (if (null? args) 1  \n\
      (abs (* (/ (car args) (apply gcd args))  \n\
              (apply * (cdr args))))))";

static const char* libBasics5 = 
"(define (map proc . rest)  \n\
 (define (helper lists result)  \n\
  (if (some? null? lists)  \n\
   (reverse! result)  \n\
   (helper (map1 cdr lists)  \n\
    (cons (apply proc (map1 car lists)) result))))  \n\
 (helper rest '()))  \n\
 \n\
(define (for-each proc . rest)  \n\
 (define (helper lists)  \n\
  (if (some? null? lists)  \n\
   '()  \n\
   (begin  \n\
    (apply proc (map1 car lists))  \n\
    (helper (map1 cdr lists)))))  \n\
 (helper rest))  \n\
 \n\
(define (filter pred l)  \n\
 (define (helper l result)  \n\
  (cond ((null? l) result)  \n\
   ((pred (car l))  \n\
    (helper (cdr l) (cons (car l) result)))  \n\
   (else  \n\
    (helper (cdr l) result))))  \n\
 (reverse! (helper l '())))  \n\
 \n\
(define (reduce op default lst)  \n\
  (if (null? lst) \n\
      default \n\
      (fold-left op (car lst) (cdr lst))))  \n\
 \n\
(define (alist->hash-table alist)  \n\
  (define h (make-hash-table))  \n\
  (for-each1 (lambda (pair)  \n\
               (hash-table-set h (car pair) (cdr pair))) alist)  \n\
  h)  \n\
 \n\
(define (_assoc key list eq?)  \n\
 (if (null? list) #f  \n\
  (let ((pair (car list)))  \n\
    (if (and? (pair? pair) (eq? key (car pair)))  \n\
        pair  \n\
        (_assoc key (cdr list) eq?)))))  \n\
 \n\
(define (assoc key list) (_assoc key list equal?))  \n\
(define (assq key list) (_assoc key list eq?))  \n\
(define (assv key list) (_assoc key list eqv?))  \n\
 \n\
(define (_member x list eq?)  \n\
 (cond ((null? list) #f)  \n\
  ((eq? (car list) x) list)  \n\
  (else (_member x (cdr list) eq?))))  \n\
 \n\
(define (member x list) (_member x list equal?))  \n\
(define (memq x list) (_member x list eq?))  \n\
(define (memv x list) (_member x list eqv?))  \n\
  \n\
(define (make-initialized-vector l fn)  \n\
  (let ((v (make-vector l '())))  \n\
    (do ((i 0 (+ i 1)))  \n\
      ((>= i l) v)  \n\
      (vector-set v i (fn i)))))  \n\
 \n\
(define (vector-map fn v)  \n\
  (make-initialized-vector  \n\
    (vector-length v)  \n\
    (lambda (i) (fn (vector-ref v i)))))  \n\
 \n\
(define (vector-binary-search v key< unwrap-key key)  \n\
  (define (helper low high mid)  \n\
    (if (<= (- high low) 1)  \n\
        (if (key< (unwrap-key (vector-ref v low)) key) #f (vector-ref v low))  \n\
        (begin  \n\
          (set mid (+ low (quotient (- high low) 2)))  \n\
          (if (key< key (unwrap-key (vector-ref v mid)))  \n\
              (helper low mid 0)  \n\
              (helper mid high 0)))))  \n\
  (helper 0 (vector-length v) 0))";

static const char* libBasics6 = 
"(define (procedure? p) (or? (compiled-procedure? p) (compound-procedure? p)))  \n\
 \n\
(define (current-input-port) STD_IN) \n\
(define (current-output-port) STD_OUT) \n\
 \n\
(define (read . args)  \n\
  (_read (if (null? args) \n\
           (current-input-port) \n\
           (car args)))) \n\
 \n\
(define (write obj . args)  \n\
  (_write obj (if (null? args) \n\
                (current-output-port) \n\
                (car args)))) \n\
 \n\
(define (display obj . args)  \n\
  (_display obj (if (null? args) \n\
                  (current-output-port) \n\
                  (car args)))) \n\
 \n\
 \n\
(define (write-char obj . args)  \n\
  (_write-char obj (if (null? args) \n\
                     (current-output-port) \n\
                     (car args)))) \n\
 \n\
(define (flush-output-port . args)  \n\
  (_flush-output-port (if (null? args) \n\
                        (current-output-port) \n\
                        (car args)))) \n\
 \n\
(define-macro assert (lambda (body)  \n\
                       `(if ,body '()  \n\
                            (begin  \n\
                              (display (quote ,body))  \n\
                              (error \" assert failed\")))))  \n\
 \n\
(define-macro ==>  (lambda (test expected)  \n\
                `(assert (equal? ,test (quote ,expected))) ))";
    Lisp table = cLISPmake_table(clsp);
    cLISPtable_define_funcs(table, cLispfuncDefinitions, clsp);

    cLISPtable_set(table, createSymbol("STD_OUT", clsp), cLISPmake_port(stdout, 0),clsp);

    cLISPtable_set(table,createSymbol("STD_IN", clsp),cLISPmake_port(stdin, 1),
clsp
            );

    Lisp system_env = cLISPenv_extend(cLISPenv(clsp), table, clsp);
    Lisp user_env = cLISPenv_extend(system_env, cLISPmake_table(clsp), clsp);
    cLISPset_env(user_env, clsp);

    const char* macro_definers[] = {
        libBasics1, libBasics2,
        libBasics3, libBasics4,
        libBasics5, 
        libBasics6, 
    };

    int n = sizeof(macro_definers) / sizeof(const char*);

    for (int i = 0; i < n; ++i)
    {
        cLispError error;
        Lisp src = cLISPread(macro_definers[i], &error, clsp);

        if (error != ERROR_NONE)
        {
            return;
        }
 
        cLISPevalPtr(src, system_env, &error, clsp);

        if (error != ERROR_NONE)
        {
            return;
        }
    }

    cLISPcollect(cLISPnull(), clsp);


}

clispPtrENV loadLisp(void)
{
    clispPtrENV clsp = cLispInit();
    loadEnv(clsp);
    return clsp;
}


#endif
