#ifndef CLISP_C
#define CLISP_C

#include <stdio.h>
#include <stdint.h> 
#include <stdbool.h> //bool in c
#include <fcntl.h> //O_RDONLY

#ifndef IDENT_MAX
#define IDENT_MAX 1024
#endif
#ifndef PAGE_SIZE
#define PAGE_SIZE 512 * 1024
#endif
#ifndef STACK_DEPTH
#define STACK_DEPTH 1024
#endif

//-------------------------------------DATA STRUCTURES----------------/
enum
{
    garbageClear = 0,
    garbageNonExist = 1, 
    garbageToVisit = 2, 
};

typedef struct Page
{
    struct Page* next;
    size_t size;
    size_t capacity;
    char buffer[];
} Page;
typedef struct
{
    Page* top;
    Page* bottom;
    size_t size;
    size_t page_count;
} Heap;

typedef double LispReal;
typedef long long LispInt;
typedef enum
{
    cLispNULL = 0,
    cLispTERPRI, 
    cLispSYMBOL,  
    cLispREAL,   
    cLispINT,   
    cLispCHAR,    
    cLispPAIR,    
    cLispSTRING,  
    cLispLAMBDA, 
    cLispFUNC,    
    cLispTABLE,  
    cLispBOOL,    
    cLispVECTOR,  
    cLispJUMP,    
    cLispPORT_IN,
    cLispPORT_OUT,
    cLispPTR,     
} clispType;

typedef union
{
    int charVal;
    LispReal realVal;
    LispInt intVal;   
    void* ptrVal;     
    void(*funcVal)(void); 
} cLispVal;

typedef struct //token
{
    cLispVal val;
    clispType type;
} Lisp; 

typedef enum
{
    ERROR_NONE = 0,
    ERROR_FILE_OPEN,
    ERROR_MMAP,
    ERROR_READ_SYNTAX,
    ERROR_FORM_SYNTAX,
    ERROR_UNDEFINED_VAR,
    ERROR_BAD_OP,
    ERROR_OUT_OF_BOUNDS,
    ERROR_ARG_TYPE,
    ERROR_TOO_MANY_ARGS,
    ERROR_TOO_FEW_ARGS,
    ERROR_RUNTIME,
} cLispError;

typedef struct
{
    struct LispStruct* p;
} clispPtrENV;
enum {
    SYM_IF = 0,
    SYM_TERPRI,
    SYM_DEFINE,
    SYM_DEFINE_MACRO,
    SYM_SET,
    SYM_LAMBDA,
    SYM_BEGIN,
    SYM_QUOTE,
    SYM_QUASI_QUOTE,
    SYM_UNQUOTE,
    SYM_UNQUOTE_SPLICE,
    SYM_CONS,
    SYM_COUNT
};
typedef Lisp(*LispCFunc)(Lisp, cLispError*, clispPtrENV);
typedef struct Chunk
{   
    union
    {
        struct Chunk* forward;
        size_t size;
    } link;
    
    union
    {
        struct
        {
            uint8_t car_type;
            uint8_t cdr_type;
        } pair;

        struct
        {
            int length;
        } vector;

        struct
        {
            int length;
        } symbol;

        

        struct
        {
            uint8_t body_type;
            uint8_t args_type;
        } lambda;

        struct
        {
            int capacity;
        } string;
    } d;

    uint8_t gc_state;
    uint8_t type;
} Chunk;

typedef struct
{
    Chunk chunk;
    cLispVal car;
    cLispVal cdr;
} Pair;

typedef struct
{
    Chunk chunk;
    char string[];
} String;

typedef struct
{
    Chunk chunk;
    cLispVal entries[];
} Vector;
typedef struct
{
    const char* name;
    LispCFunc func_ptr;
} LispFuncDef;

typedef struct
{
    Chunk chunk;
    int size;
    int capacity;
    cLispVal keys;
    cLispVal vals;
} Table;

typedef struct
{
    Chunk chunk;
    cLispVal next;
    char text[];
} Symbol;

typedef struct
{
    Chunk chunk;
    cLispVal body;
    cLispVal args;
    cLispVal env;
} Lambda;
typedef enum
{
    TOKEN_NONE = 0,
    TOKEN_TERPRI,
    TOKEN_L_PAREN,
    TOKEN_R_PAREN,
    TOKEN_PERIOD,
    TOKEN_QUOTE,
    TOKEN_BQUOTE,
    TOKEN_COMMA,
    TOKEN_AT,
    TOKEN_SYMBOL,
    TOKEN_STRING,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_BOOL,
    TOKEN_HASH_L_PAREN,
} TokenType;

typedef struct
{
    const char* start;
    const char* end;

    const char* token_start;
    const char* token_end;
    TokenType token;
} Scanner;

clispPtrENV cLispInit(void);
void cLISPshutdown(clispPtrENV clsp);

// Characters
Lisp cLISPmake_char(int c);
int cLISPchar(Lisp l);
Lisp cLISPeof(void);

Lisp cLISPcollect(Lisp root_to_save, clispPtrENV clsp);
void cLISPprint_collect_stats(clispPtrENV clsp);
const char *cLISPerror_string(cLispError error);

void cLISPset_env(Lisp env, clispPtrENV clsp);
Lisp cLISPenv(clispPtrENV clsp);

void cLISPset_stderr(FILE *file, clispPtrENV clsp);
FILE *cLISPstderr(clispPtrENV clsp);

// Macros 
Lisp cLISPmacro_table(clispPtrENV clsp);
void cLISPset_macro_table(Lisp table, clispPtrENV clsp);


Lisp (cLISPread)(const char *text, cLispError* out_error, clispPtrENV clsp);
Lisp cLISPread_path(const char* path, cLispError* out_error, clispPtrENV clsp);
Lisp cLISPread_file(FILE *file, cLispError* out_error, clispPtrENV clsp);
Lisp cLISPeval(Lisp expr, cLispError* out_error, clispPtrENV clsp);
Lisp cLISPevalPtr(Lisp expr, Lisp env, cLispError* out_error, clispPtrENV clsp);
Lisp cLISPapply(Lisp op, Lisp args, cLispError* out_error, clispPtrENV clsp);
Lisp cLISPmacroexpand(Lisp lisp, cLispError* out_error, clispPtrENV clsp);
void cLISPprintf(FILE *file, Lisp l);

Lisp cLISPcall_cc(Lisp proc, cLispError* out_error, clispPtrENV clsp);

#define cLISPtype(x) ((x).type)
#define cLISPeq(a, b) ((a).val.ptrVal == (b).val.ptrVal)
#define cLISPnull() ((Lisp) { .val = { .ptrVal = NULL }, .type = cLispNULL })
#define isNULLType(x) ((x).type == cLispNULL)
#define isPairType(p) ((p).type == cLispPAIR)
int cLISPequal(Lisp a, Lisp b);
int cLISPequal_r(Lisp a, Lisp b);
Lisp cLISPcar(Lisp p);
Lisp cLISPcdr(Lisp p);
void cLISPset_car(Lisp p, Lisp x);
void cLISPset_cdr(Lisp p, Lisp x);
Lisp cLISPcons(Lisp car, Lisp cdr, clispPtrENV clsp);

// Numbers
Lisp cLISPmake_int(LispInt n);
LispInt cLISPint(Lisp x);
Lisp cLISPparse_int(const char *string);

Lisp cLISPmake_real(LispReal x);
LispReal cLISPreal(Lisp x);
Lisp cLISPparse_real(const char *string);

LispReal cLISPnumber_to_real(Lisp x);
LispInt cLISPnumber_to_int(Lisp x);

// Booleans
Lisp cLISPmake_bool(int t);
int cLISPbool(Lisp x);
#define cLISPtrue() (cLISPmake_bool(1))
#define cLISPfalse() (cLISPmake_bool(0))
int isboolTrue(Lisp x);


// Null terminated byte strings (ASCII)
Lisp cLISPmake_string(int n, clispPtrENV clsp);
Lisp cLISPmake_string2(const char *c_string, clispPtrENV clsp);
Lisp cLISPsubstring(Lisp s, int start, int end, clispPtrENV clsp);

void cLISPstring_fill(Lisp s, Lisp character);
int cLISPstring_ref(Lisp s, int i);
void cLISPstring_set(Lisp s, int i, int c);
int cLISPstring_length(Lisp s);
const char *cLISPstring(Lisp s);

// Low level string storage
Lisp cLISPmake_buffer(int cap, clispPtrENV clsp);
Lisp cLISPbuffer_copy(Lisp s, clispPtrENV clsp);
void cLISPbuffer_fill(Lisp s, int start, int end, int x);
char *cLISPbuffer(Lisp s);
int cLISPbuffer_capacity(Lisp s);

// Symbols (interned strings)
Lisp createSymbol(const char *string, clispPtrENV clsp);
Lisp cLISPgen_symbol(clispPtrENV clsp); // uninterned
const char *cLISPsymbol_string(Lisp x);

// Arbitrary C objects.
Lisp cLISPmake_ptr(void *ptr);
void *cLISPptr(Lisp l);

// Ports
Lisp cLISPmake_port(FILE* file, int input);
FILE *cLISPport(Lisp l);


// DATA MANIPULATION

// Lists
Lisp cLISPlist_copy(Lisp x, clispPtrENV clsp);
Lisp cLISPmake_list(Lisp x, int n, clispPtrENV clsp);
Lisp cLISPmake_list2(Lisp *x, int n, clispPtrENV clsp);

Lisp cLISPlist_reverse(Lisp l); 
Lisp cLISPlist_reverse2(Lisp l, Lisp tail);  
Lisp cLISPlist_append(Lisp l, Lisp tail, clispPtrENV clsp);  
Lisp cLISPlist_advance(Lisp l, int i);  
Lisp cLISPlist_ref(Lisp l, int i);  
int cLISPlist_length(Lisp l);  
int cLISPis_list(Lisp l);  
Lisp cLISPalist_ref(Lisp l, Lisp key);  

// Vector
Lisp cLISPmake_vector(int n, clispPtrENV clsp); // Contents are uninitialized. Be careful.
Lisp cLISPmake_vectorFuncPtr(Lisp *x, int n, clispPtrENV clsp);

int cLISPvector_length(Lisp v);
Lisp cLISPvector_ref(Lisp v, int i);  
void cLISPvector_set(Lisp v, int i, Lisp x); 
Lisp cLISPvector_swap(Lisp v, int i, int j); 
void cLISPvector_fill(Lisp v, Lisp x);
Lisp cLISPvector_grow(Lisp v, int n, clispPtrENV clsp);
Lisp cLISPsubvector(Lisp old, int start, int end, clispPtrENV clsp);

// association vector like "alist"
Lisp cLISPavector_ref(Lisp l, Lisp key);  

// Hash tables
Lisp cLISPmake_table(clispPtrENV clsp);
void cLISPtable_set(Lisp t, Lisp key, Lisp x, clispPtrENV clsp);
Lisp cLISPtable_get(Lisp t, Lisp key, int* present);
int cLISPtable_size(Lisp t);
Lisp cLISPtable_to_alist(Lisp t, clispPtrENV clsp);

// -----------------------------------------
// LANGUAGE
// -----------------------------------------

// Lambdas (compound procedures)
Lisp cLISPmake_lambda(Lisp args, Lisp body, Lisp env, clispPtrENV clsp);
Lisp cLISPlambda_body(Lisp l);
Lisp cLISPlambda_env(Lisp l);

// C functions (compiled procedures)
Lisp cLISPmake_func(LispCFunc func_ptr);
LispCFunc cLISPfunc(Lisp l);

void cLISPtable_define_funcs(Lisp t, const LispFuncDef* defs, clispPtrENV clsp);

// Evaluation environments
Lisp cLISPenv_extend(Lisp l, Lisp table, clispPtrENV clsp);
Lisp cLISPenv_lookup(Lisp l, Lisp key, int *present);
void defineVM(Lisp l, Lisp key, Lisp x, clispPtrENV clsp);
int cLISPenv_set(Lisp l, Lisp key, Lisp x, clispPtrENV clsp);
int cLISPis_env(Lisp l);


#endif

#ifdef CLISP

#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

#ifdef _WIN32
#define LISP_NO_MMAP
#endif

#ifndef LISP_NO_MMAP 
#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#endif

#define POW2VERIFY(x) (((x) != 0) && ((x) & ((x)-1)) == 0)



static Page* page_create(size_t capacity)
{
    Page* page = malloc(sizeof(Page) + capacity);
    page->capacity = capacity;
    page->size = 0;
    page->next = NULL;
    return page;
}


void page_destroy(Page* page) { free(page); }

static void heap_init(Heap* heap)
{
    heap->bottom = page_create(PAGE_SIZE);
    heap->top = heap->bottom;
    
    heap->size = 0;
    heap->page_count = 1;
}

static void heapEnd(Heap* heap)
{
    Page* page = heap->bottom;
    while (page)
    {
        Page* next = page->next;
        page_destroy(page);
        page = next;
    }
    heap->bottom = NULL;
    heap->top = NULL;
}

static size_t align_to_bytes(size_t n, size_t k)
{
    return ((n + k - 1) / k) * k;
}

static void* heapAlloc(size_t alloc_size, clispType type, Heap* heap)
{
    assert(alloc_size > 0);
    alloc_size = align_to_bytes(alloc_size, sizeof(cLispVal));
    assert(alloc_size % sizeof(cLispVal) == 0);

    Page* to_use;
    if (alloc_size >= PAGE_SIZE)
    {
        to_use = page_create(alloc_size);
        to_use->next = heap->bottom;
        heap->bottom = to_use;
        ++heap->page_count;
    }
    else if (alloc_size + heap->top->size > heap->top->capacity)
    {
        to_use = page_create(PAGE_SIZE);
        heap->top->next = to_use;
        heap->top = to_use; 
        ++heap->page_count;
    }
    else
    {
        to_use = heap->top;
    }
    
    void* address = to_use->buffer + to_use->size;
    to_use->size += alloc_size;
    heap->size += alloc_size;

    Chunk* chunk = address;
    chunk->gc_state = garbageClear;
    chunk->link.size = alloc_size;
    chunk->type = type;
    return address;
}

struct LispStruct
{
    FILE* errFile;
    Heap heap;
    Lisp symbols_table;
    Lisp env;
    Lisp macros;
    Lisp* stack; 
    size_t stack_ptr;
    size_t stack_depth;
    int symbol_counter;

    Lisp symbol_cache[SYM_COUNT];

    size_t gc_stat_freed;
    size_t gc_stat_time;
};

static Lisp get_sym(int sym, clispPtrENV clsp) { return clsp.p->symbol_cache[sym]; }

static void* gc_alloc(size_t size, clispType type, clispPtrENV clsp)
{
    return heapAlloc(size, type, &clsp.p->heap);
}



static Lisp val_to_list_(cLispVal x)
{
    return (Lisp) { x, x.ptrVal == NULL ? cLispNULL : cLispPAIR };
}

int cLISPequal(Lisp a, Lisp b)
{
    switch (a.type)
    {
        case cLispNULL:
            return a.type == b.type;
        case cLispBOOL:
            return cLISPbool(a) == cLISPbool(b) && a.type == b.type;
        case cLispCHAR:
            return cLISPchar(a) == cLISPchar(b) && a.type == b.type;
        case cLispFUNC:
            return cLISPfunc(a) == cLISPfunc(b) && a.type == b.type;
        case cLispINT:
            if (b.type == cLispINT) return cLISPint(a) == cLISPint(b);
            else return cLISPnumber_to_real(a) == cLISPnumber_to_real(b);
        case cLispREAL:
            return cLISPreal(a) == cLISPnumber_to_real(b);
        default:
            return a.val.ptrVal == b.val.ptrVal && a.type == b.type;
    }
}

int cLISPequal_r(Lisp a, Lisp b)
{
    switch (a.type)
    {
        case cLispVECTOR:
        {
            if (a.type != b.type) return 0;
            int n = cLISPvector_length(a);
            int m = cLISPvector_length(b);
            if (n != m) return 0;
            
            for (int i = 0; i < n; ++i)
            {
                if (!cLISPequal_r(cLISPvector_ref(a, i), cLISPvector_ref(b, i)))
                    return 0;
            }
            
            return 1;
        }
        case cLispPAIR:
        {
            if (a.type != b.type) return 0;
            while (isPairType(a) && isPairType(b))
            {
                if (!cLISPequal_r(cLISPcar(a), cLISPcar(b))) return 0;
                a = cLISPcdr(a);
                b = cLISPcdr(b);
            }
            
            return cLISPequal_r(a, b);
        }
        case cLispSTRING:
        {
            return a.type == b.type && strcmp(cLISPstring(a), cLISPstring(b)) == 0;
        }
        default:
            return cLISPequal(a, b);
    }
}

Lisp cLISPmake_int(LispInt n)
{
    Lisp l;
    l.type = cLispINT;
    l.val.intVal = n;
    return l;
}

LispInt cLISPint(Lisp x) { return x.val.intVal; }

Lisp cLISPparse_int(const char* string)
{
    return cLISPmake_int((LispInt)strtol(string, NULL, 10));
}

Lisp cLISPmake_bool(int t)
{
    cLispVal val;
    val.charVal = t;
    return (Lisp) { val, cLispBOOL };
}

int cLISPbool(Lisp x) { return x.val.charVal; }

int isboolTrue(Lisp x)
{
     return (cLISPtype(x) == cLispBOOL && !cLISPbool(x)) ? 0 : 1;
}

Lisp cLISPmake_real(LispReal x)
{
    return (Lisp) { .val.realVal = x, .type = cLispREAL };
}

Lisp cLISPparse_real(const char* string)
{
    return cLISPmake_real(strtod(string, NULL));
}

LispReal cLISPreal(Lisp x) { return x.val.realVal; }

LispReal cLISPnumber_to_real(Lisp x)
{
    return cLISPtype(x) == cLispREAL ? x.val.realVal : (LispReal)cLISPint(x);
}

LispInt cLISPnumber_to_int(Lisp x)
{
    return cLISPtype(x) == cLispINT ? x.val.intVal : (LispInt)cLISPreal(x);
}

static Pair* pair_get_(Lisp p)
{
    assert(p.type == cLispPAIR);
    return p.val.ptrVal;
}

Lisp cLISPcar(Lisp p)
{
    const Pair* pair = pair_get_(p); 
    return (Lisp) { pair->car, (clispType)pair->chunk.d.pair.car_type };
}

Lisp cLISPcdr(Lisp p)
{
    const Pair* pair = pair_get_(p); 
    return (Lisp) { pair->cdr, (clispType)pair->chunk.d.pair.cdr_type };
}

void cLISPset_car(Lisp p, Lisp x)
{
    Pair* pair = pair_get_(p); 
    pair->car = x.val;
    pair->chunk.d.pair.car_type = x.type;
}

void cLISPset_cdr(Lisp p, Lisp x)
{
    Pair* pair = pair_get_(p); 
    pair->cdr = x.val;
    pair->chunk.d.pair.cdr_type = x.type;
}

Lisp cLISPcons(Lisp car, Lisp cdr, clispPtrENV clsp)
{
    Pair* pair = gc_alloc(sizeof(Pair), cLispPAIR, clsp);
    pair->car = car.val;
    pair->cdr = cdr.val;
    pair->chunk.d.pair.car_type = car.type;
    pair->chunk.d.pair.cdr_type = cdr.type;
    Lisp p;
    p.type = pair->chunk.type;
    p.val.ptrVal = pair;
    return p;
}

Lisp cLISPlist_copy(Lisp l, clispPtrENV clsp)
{
    Lisp tail = cLISPnull();
    while (isPairType(l))
    {
        tail = cLISPcons(cLISPcar(l), tail, clsp);
        l = cLISPcdr(l);
    }
    return cLISPlist_reverse(tail);
}

Lisp cLISPmake_list(Lisp x, int n, clispPtrENV clsp)
{
    Lisp tail = cLISPnull();
    for (int i = 0; i < n; ++i)
        tail = cLISPcons(x, tail, clsp);
    return tail;
}

Lisp cLISPmake_list2(Lisp *x, int n, clispPtrENV clsp)
{
    Lisp tail = cLISPnull();
    for (int i = n - 1; i >= 0; --i)
        tail = cLISPcons(x[i], tail, clsp);
    return tail;
}

Lisp cLISPlist_reverse2(Lisp l, Lisp tail)
{
    while (isPairType(l))
    {
        Lisp next = cLISPcdr(l);
        cLISPset_cdr(l, tail);
        tail = l;
        l = next;        
    }
    return tail;
}

Lisp cLISPlist_reverse(Lisp l) { return cLISPlist_reverse2(l, cLISPnull()); }

Lisp cLISPlist_append(Lisp l, Lisp tail, clispPtrENV clsp)
{
    // (a b) (c) -> (a b c)
    l = cLISPlist_reverse(cLISPlist_copy(l, clsp));
    return cLISPlist_reverse2(l, tail);
}

Lisp cLISPlist_advance(Lisp l, int i)
{
    while (i > 0)
    {
        if (!isPairType(l)) return l;
        l = cLISPcdr(l);
        --i;
    }
    return l;
}

Lisp cLISPlist_ref(Lisp l, int n)
{
    l = cLISPlist_advance(l, n);
    if (isPairType(l)) return cLISPcar(l);
    return cLISPnull();
}

int cLISPlist_length(Lisp l)
{
    int n = 0;
    while (isPairType(l)) { ++n; l = cLISPcdr(l); }
    return n;
}

int cLISPis_list(Lisp l)
{
    if (isNULLType(l))
    {
        return 1;
    }
    else if (isPairType(l))
    {
        return cLISPis_list(cLISPcdr(l));
    }
    else
    {
        return 0;
    }
}

Lisp cLISPalist_ref(Lisp l, Lisp key)
{
    while (isPairType(l))
    {
        Lisp pair = cLISPcar(l);
        if (isPairType(pair) && cLISPeq(cLISPcar(pair), key))
        {
            return pair;
        }
        l = cLISPcdr(l);
    }
    return cLISPfalse();
}

static int vector_len_(const Vector* v) { return v->chunk.d.vector.length; }

// types are stored in an array of bytes at the end of the data.
static char* vector_types_(Vector* v)
{
    char* base = (char*)v;
    return base + sizeof(Vector) + sizeof(cLispVal) * vector_len_(v);
}

//Returns ptrVal
static Vector* vector_get_(Lisp v)
{
    assert(cLISPtype(v) == cLispVECTOR);
    return v.val.ptrVal;
}

Lisp cLISPmake_vector(int n, clispPtrENV clsp)
{
    size_t size = sizeof(Vector) + sizeof(cLispVal) * n + sizeof(char) * n;
    Vector* vector = gc_alloc(size, cLispVECTOR, clsp);
    vector->chunk.d.vector.length = n;

    cLispVal val;
    val.ptrVal = vector;
    return (Lisp) { val, cLispVECTOR };
}

Lisp cLISPmake_vectorFuncPtr(Lisp *x, int n, clispPtrENV clsp)
{
    Lisp v = cLISPmake_vector(n, clsp);
    for (int i = 0; i < n; ++i)
        cLISPvector_set(v, i, x[i]);
    return v;
}

int cLISPvector_length(Lisp v) { return vector_len_(vector_get_(v)); }

Lisp cLISPvector_ref(Lisp v, int i)
{
    Vector* vector = vector_get_(v);
    assert(i < vector_len_(vector));
    Lisp x = { vector->entries[i], (clispType)(vector_types_(vector)[i]) };
    return x;
}

void cLISPvector_set(Lisp v, int i, Lisp x)
{
    Vector* vector = vector_get_(v);
    assert(i < vector_len_(vector));
    vector->entries[i] = x.val;
    vector_types_(vector)[i] = (char)x.type;
}

Lisp cLISPvector_swap(Lisp v, int i, int j)
{
    Lisp tmp = cLISPvector_ref(v, i);
    cLISPvector_set(v, i, cLISPvector_ref(v, j));
    cLISPvector_set(v, j, tmp);
    return v;
}

void cLISPvector_fill(Lisp v, Lisp x)
{
    int n = cLISPvector_length(v);
    Vector* vector = vector_get_(v);
    char* entry_types = vector_types_(vector);

    for (int i = 0; i < n; ++i)
    {
        vector->entries[i] = x.val;
        entry_types[i] = (char)x.type;
    }
}

Lisp cLISPsubvector(Lisp old, int start, int end, clispPtrENV clsp)
{
    assert(start <= end);
    Vector* src = vector_get_(old);

    int m = vector_len_(src);
    if (end > m) end = m;
    
    int n = end - start;
    Lisp new_v = cLISPmake_vector(n, clsp);
    Vector* dst = vector_get_(new_v);
    memcpy(dst->entries, src->entries + start, sizeof(cLispVal) * n);
    memcpy(vector_types_(dst), vector_types_(src) + start, sizeof(char) * n);
    return new_v;
}

Lisp cLISPvector_grow(Lisp v, int n, clispPtrENV clsp)
{
    Vector* src = vector_get_(v);
    int m = vector_len_(src);
    assert(n >= m);

    if (n == m)
    {
        return v;
    }
    else
    {
        Lisp new_v = cLISPmake_vector(n, clsp);
        Vector* dst = vector_get_(new_v);
        memcpy(dst->entries, src->entries, sizeof(cLispVal) * m);
        memcpy(vector_types_(dst), vector_types_(src), sizeof(char) * m);
        return new_v;
    }
}

Lisp cLISPavector_ref(Lisp v, Lisp key)
{
    int n = cLISPvector_length(v);
    for (int i = 0; i < n; ++i)
    {
        Lisp pair = cLISPvector_ref(v, i);
        if (isPairType(pair) && cLISPeq(cLISPcar(pair), key)) return pair;
    }
    return cLISPfalse();
}

static uint64_t hash_uint64(uint64_t x)
{
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 32;
    return x;
}

static uint64_t hashVal(cLispVal x) { return hash_uint64((uint64_t)x.intVal); }

static Table *table_get_(Lisp t)
{
    assert(cLISPtype(t) == cLispTABLE);
    return t.val.ptrVal;
}

Lisp cLISPmake_table(clispPtrENV clsp)
{
    Table *table = gc_alloc(sizeof(Table), cLispTABLE, clsp);
    table->size = 0;
    table->capacity = 0;

    cLispVal x;
    x.ptrVal = table;
    return (Lisp) { x, cLispTABLE };
}

static void  tableExpand(Lisp t, size_t new_capacity, clispPtrENV clsp)
{
    Table *table = table_get_(t);
    if (new_capacity < 16) new_capacity = 16;
    assert(POW2VERIFY(new_capacity));

    int old_capacity = table->capacity;
    Lisp old_keys = { table->keys, cLispVECTOR };
    Lisp oldVals = { table->vals, cLispVECTOR };

    table->capacity = new_capacity;
    int n = table->size;
    table->size = 0;
    Lisp newVals = cLISPmake_vector(new_capacity, clsp);
    Lisp new_keys = cLISPmake_vector(new_capacity, clsp);
    cLISPvector_fill(new_keys, cLISPnull());
    table->vals = newVals.val;
    table->keys = new_keys.val;

    for (int i = 0; i < old_capacity; ++i)
    {
        Lisp key = cLISPvector_ref(old_keys, i);
        if (!isNULLType(key))
            cLISPtable_set(t, key, cLISPvector_ref(oldVals, i), clsp);
    }
    assert(n == table->size);
}

void cLISPtable_set(Lisp t, Lisp key, Lisp x, clispPtrENV clsp)
{ 
    Table *table = table_get_(t);
    if (2 * table->size >= table->capacity)
    {
         tableExpand(t, table->capacity * 2, clsp);
    }
    assert(2 * table->size < table->capacity);

    Lisp keys = { table->keys, cLispVECTOR };
    Lisp vals = { table->vals, cLispVECTOR };

    uint32_t i = hashVal(key.val);
    while (1)
    {
        i &= (table->capacity - 1);

        Lisp saved_key = cLISPvector_ref(keys, i);
        if (isNULLType(saved_key))
        {
            ++table->size;
            cLISPvector_set(keys, i, key);
            cLISPvector_set(vals, i, x);
            return;
        }
        else if (cLISPeq(saved_key, key))
        {
            cLISPvector_set(vals, i, x);
            return;
        }
        ++i;
    }
}

Lisp cLISPtable_get(Lisp t, Lisp key, int* present)
{
    Table *table = table_get_(t);
    int capacity = table->capacity;
    if (capacity == 0)
    {
       *present = 0;
       return cLISPnull();
    }

    Lisp keys = { table->keys, cLispVECTOR };
    Lisp vals = { table->vals, cLispVECTOR };

    uint32_t i = hashVal(key.val);
    while (1)
    {
        i &= (capacity - 1);

        Lisp saved_key = cLISPvector_ref(keys, i);

        if (isNULLType(saved_key))
        {
            *present = 0;
            return cLISPnull();
        }
        else if (cLISPeq(saved_key, key))
        {
            *present = 1;
            return cLISPvector_ref(vals, i);
        }
        ++i;
    }
}

Lisp cLISPtable_to_alist(Lisp t, clispPtrENV clsp)
{
    const Table *table = table_get_(t);
    Lisp result = cLISPnull();

    Lisp keys = { table->keys, cLispVECTOR };
    Lisp vals = { table->vals, cLispVECTOR };
    
    for (int i = 0; i < table->capacity; ++i)
    {
        Lisp key = cLISPvector_ref(keys, i);
        if (!isNULLType(key))
        {
            result = cLISPcons(cLISPcons(key, cLISPvector_ref(vals, i), clsp), result, clsp);
        }
    }
    return result;
}

int cLISPtable_size(Lisp t) { return table_get_(t)->size; }

void cLISPtable_define_funcs(Lisp t, const LispFuncDef* defs, clispPtrENV clsp)
{
    while (defs->name)
    {
        cLISPtable_set(t, createSymbol(defs->name, clsp), cLISPmake_func(defs->func_ptr), clsp);
        ++defs;
    }
}

static String* string_get_(Lisp s)
{
    assert(cLISPtype(s) == cLispSTRING);
    return s.val.ptrVal;
}

Lisp cLISPmake_buffer(int cap, clispPtrENV clsp)
{
    assert(cap >= 0);
    String* string = gc_alloc(sizeof(String) + cap, cLispSTRING, clsp);
    string->chunk.d.string.capacity = cap;
    
    cLispVal val;
    val.ptrVal = string;
    return (Lisp){ val, string->chunk.type };
}

Lisp cLISPbuffer_copy(Lisp s, clispPtrENV clsp)
{
    int cap = cLISPbuffer_capacity(s);
    Lisp b = cLISPmake_buffer(cap, clsp);
    memcpy(cLISPbuffer(b), cLISPbuffer(s), cap);
    return b;
}

int cLISPbuffer_capacity(Lisp s)
{
    return string_get_(s)->chunk.d.string.capacity;
}

void cLISPbuffer_fill(Lisp s, int start, int end, int x)
{
    int n = cLISPbuffer_capacity(s);
    if (start > n) start = n;
    if (end > n) end = n;
    memset(cLISPbuffer(s) + start, x, end - start);
}

char *cLISPbuffer(Lisp s) { return string_get_(s)->string; }
const char *cLISPstring(Lisp s) { return cLISPbuffer(s); }

Lisp cLISPmake_string(int n, clispPtrENV clsp)
{
    Lisp s = cLISPmake_buffer(n + 1, clsp);
    cLISPbuffer(s)[n] = '\0';
    return s;
}

Lisp cLISPmake_string2(const char* c_string, clispPtrENV clsp)
{
    size_t length = strlen(c_string);
    Lisp s = cLISPmake_string(length, clsp);
    memcpy(cLISPbuffer(s), c_string, length);
    return s; 
}

int cLISPstring_length(Lisp s) { return strlen(cLISPstring(s)); }

int cLISPstring_ref(Lisp s, int i) {
    const String* str = string_get_(s);
    assert(i >= 0 && i < cLISPbuffer_capacity(s));
    return (int)str->string[i]; 
}

void cLISPstring_set(Lisp s, int i, int c)
{
    assert(c >= 0 && c <= 127);
    assert(i >= 0 && i < cLISPbuffer_capacity(s));
    string_get_(s)->string[i] = (char)c;
}

Lisp cLISPsubstring(Lisp s, int start, int end, clispPtrENV clsp)
{
    assert(start <= end);

    int count = start;
    const char *first = cLISPstring(s);
    while (*first && count) {
        --count;
        ++first;
    }

    count = (end - start);
    const char *last = first;
    while (*last && count)
    {
        --count;
        ++last;
    }
    Lisp result = cLISPmake_string(last - first, clsp);
    memcpy(cLISPbuffer(result), first, last - first);
    return result;
}

Lisp cLISPmake_char(int c)
{
    Lisp l;
    l.type = cLispCHAR;
    l.val.charVal = c;
    return l;
}

int cLISPchar(Lisp l) { return l.val.charVal; }
Lisp cLISPeof(void) { return cLISPmake_char(-1); }

static uint64_t turnToHashBYTES(const char *buffer, size_t n)
{
    uint64_t x = 0xcbf29ce484222325;
    for (size_t i = 0; i < n; i++)
    {
        x ^= buffer[i];
        x *= 0x100000001b3;
        x ^= x >> 32;
    }
    return x;
}


static Symbol* symbol_get_(Lisp x)
{
    assert(cLISPtype(x) == cLispSYMBOL);
    return x.val.ptrVal;
}
int  cLISPsymbol_length(Lisp l) { return symbol_get_(l)->chunk.d.symbol.length; }
const char* cLISPsymbol_string(Lisp l) { return symbol_get_(l)->text; }

static Lisp symbol_make_(const char* string, int length, clispPtrENV clsp)
{
    Symbol* symbol = gc_alloc(sizeof(Symbol) + (length + 1), cLispSYMBOL, clsp);
    memcpy(symbol->text, string, length);
    symbol->text[length] = '\0';
    symbol->next.ptrVal = NULL; 
    symbol->chunk.d.symbol.length = length;

    cLispVal x;
    x.ptrVal = symbol;
    return (Lisp) { x, cLispSYMBOL };
}

static Lisp insertSymbol(Lisp table, const char* string, size_t length, clispPtrENV clsp)
{
    uint64_t hash = turnToHashBYTES(string, length);
    Lisp key;
    key.type = cLispINT;
    key.val.intVal = (LispInt)hash;
    int present;
    Lisp first_symbol = cLISPtable_get(table, key, &present);
    if (present)
    {
        Lisp it = first_symbol;
        while (it.val.ptrVal != NULL)
        {
            if (cLISPsymbol_length(it) == length &&
                strncmp(cLISPsymbol_string(it), string, length) == 0) {
                return it;
            }
            it.val = symbol_get_(it)->next;
        }
    }

    // New Symbol
    Lisp symbol = symbol_make_(string, length, clsp);

    symbol_get_(symbol)->next = first_symbol.val;
    cLISPtable_set(table, key, symbol, clsp);
    return symbol;
}

Lisp createSymbol(const char* string, clispPtrENV clsp)
{
    assert(string);
    int length = strnlen(string, IDENT_MAX);
    return insertSymbol(clsp.p->symbols_table, string, length, clsp);
}

Lisp cLISPgen_symbol(clispPtrENV clsp)
{
    char text[64];
    int bytes = snprintf(text, 64, ":G%d", clsp.p->symbol_counter++);
    return symbol_make_(text, bytes, clsp);
}

Lisp cLISPmake_ptr(void *ptr)
{
    return (Lisp) { .val = { .ptrVal = ptr }, .type = cLispPTR };
}


void *cLISPptr(Lisp l)
{
    assert(cLISPtype(l) == cLispPTR);
    return l.val.ptrVal;
}

Lisp cLISPmake_port(FILE *file, int input) {
    return (Lisp) {
        .val = { .ptrVal = file },
        .type = (input == 1) ? cLispPORT_IN : cLispPORT_OUT
    };
}

FILE *cLISPport(Lisp l)
{
    assert(cLISPtype(l) == cLispPORT_IN || cLISPtype(l) == cLispPORT_OUT);
    return l.val.ptrVal;
}

Lisp cLISPmake_func(LispCFunc func)
{
    Lisp l;
    l.type = cLispFUNC;
    l.val.funcVal = (void(*)(void))func;
    return l;
}

LispCFunc cLISPfunc(Lisp l)
{
    assert(cLISPtype(l) == cLispFUNC);
    return (LispCFunc)l.val.funcVal;
}


Lisp cLISPmake_lambda(Lisp args, Lisp body, Lisp env, clispPtrENV clsp)
{
    Lambda* lambda = gc_alloc(sizeof(Lambda), cLispLAMBDA, clsp);
    lambda->chunk.d.lambda.body_type = (uint8_t)cLISPtype(body);
    lambda->chunk.d.lambda.args_type = (uint8_t)cLISPtype(args);

    assert(cLISPis_env(env));

    lambda->args = args.val;
    lambda->body = body.val;
    lambda->env = env.val;
    
    cLispVal val;
    val.ptrVal = lambda;
    return (Lisp) { val, cLispLAMBDA };
}

static Lambda* lambda_get_(Lisp l)
{
    assert(l.type == cLispLAMBDA);
    return l.val.ptrVal;
}

Lisp cLISPlambda_body(Lisp l)
{
     const Lambda* lambda = lambda_get_(l);
     return (Lisp) { lambda->body, (clispType)lambda->chunk.d.lambda.body_type };
}

Lisp lambda_args_(Lisp l)
{
     const Lambda* lambda = lambda_get_(l);
     return (Lisp) { lambda->args, (clispType)lambda->chunk.d.lambda.args_type };
}

Lisp cLISPlambda_env(Lisp l)
{
    const Lambda* lambda = lambda_get_(l);
    return val_to_list_(lambda->env);
}

typedef struct
{
    Chunk chunk;
    Lisp result;
    jmp_buf jmp;
    int stack_ptr;
} jumpBuffer;

static jumpBuffer* jump_get_(Lisp x) {
    assert(x.type == cLispJUMP);
    return x.val.ptrVal;
}

static Lisp make_jump_(clispPtrENV clsp)
{
    jumpBuffer* j = gc_alloc(sizeof(jumpBuffer), cLispJUMP, clsp);
    j->result = cLISPfalse();
    return (Lisp) { .val = { .ptrVal = j }, .type = cLispJUMP };
}

static void scannerInit(Scanner* lex, const char* start, const char* end)
{
    lex->start = start;
    lex->end = end;
    lex->token_start = start;
    lex->token_end = start;
    lex->token = TOKEN_NONE;
}

static const char* skipEmptySpace(const char* f, const char* l)
{
    while (1)
    {
        while (f != l && isspace(*f)) ++f;
        if (f != l && *f == ';')
        {
            ++f;
            while (f != l && *f != '\n') ++f;
        }
        else
        {
            return f;
        }
    }
}

static const char* match_char_(const char* f, const char* l)
{
    if (f == l) return NULL;
    if (isalnum(*f)) {
        ++f;
        while (f != l && isalnum(*f)) ++f;
        return f;
    } else if (isprint(*f)) {
        ++f;
        return f;
    } else {
        return NULL;
    }
}

static const char* match_number_(const char* f, const char* l, int* out_has_decimal)
{
    if (f == l) return NULL;
    if (*f == '-' || *f == '+') ++f;
    if (f == l || !isdigit(*f)) return NULL;
    ++f;
    while (f != l && isdigit(*f)) ++f;
    if (f == l || *f != '.') {
        *out_has_decimal = 0;
        return f;
    }

    ++f;
    *out_has_decimal = 1;
    while (f != l && isdigit(*f)) ++f;
    return f;
}

static int is_symbol_(char c)
{
    if (c < '!' || c > 'z') return 0;
    switch (c) {
        case '(':
        case ')':
        case '#':
        case ';':
            return 0;
        default:
            return 1;
    }
}

const char* match_symbol_(const char* f, const char* l)
{   
    // need at least one valid symbol character
    if (f == l || !is_symbol_(*f)) return NULL;
    ++f;
    while (f != l && is_symbol_(*f)) ++f; 
    return f;
}

bool temp_string(const char* f, const char* l,int s){
    //printf("-%s-\n",f);
    //printf("--%s--\n",l);
    bool mat=false;
    for(int j=0;j<s;j++){
    if(f[j]==l[j]){
        mat= true;
    }
    else{
        return false;
    }
    printf("\n--[%c]--[%c]\n",f[j], l[j]);
    }
    
    return mat;
}

static const char* match_string_(const char* f, const char* l)
{
    while (f != l)
    {
        switch (*f)
        {
            case '"':
                ++f;
                return f;
            case '\\':
                ++f;
                if (f == l) return NULL;
                ++f;
                break;
            case '\0':
            case '\n':
                return NULL;
            default:
                ++f;
        }
    }
    return NULL;
}

static
size_t scanPos(Scanner* lex) {
    return lex->token_end - lex->start;
}

static
size_t scanner_copy_token(Scanner* lex, size_t start_index, size_t max_length, char* dest)
{
    size_t size = lex->token_end - lex->token_start;
    assert(size >= start_index);
    size -= start_index;
    if (size > max_length) size = max_length;
    memcpy(dest, lex->token_start + start_index, size);
    return size;
}

static void scanner_nextToken(Scanner* lex) {
    const char* l = lex->end;
    const char* f = skipEmptySpace(lex->token_end, l);
    lex->token_end = f;
    lex->token_start = f;
    lex->token = TOKEN_NONE;

    int has_decimal;

    switch (*f)
    {           
        case '\0': break;
        case '(':
           // printf("\n1.c_END:(|%s", f);
           //printf("\n1.TOKEN_END:(|%s", lex->token_end);
            lex->token = TOKEN_L_PAREN;
            lex->token_end = f + 1;
            //printf("\n1.c_END:(|%s", f);
            //printf("\n2.TOKEN_END:|%s\n", lex->token_end);
           
           // printf("\n))++%s",lex->token_end);
            break;
                
        case ')':
            //printf("\nTOKEN_R_PLACED");
            lex->token = TOKEN_R_PAREN;
            lex->token_end = f + 1;
            break;
        case '.':
            lex->token = TOKEN_PERIOD;
            lex->token_end = f + 1;
            break;
        case '\'':
            lex->token = TOKEN_QUOTE;
            lex->token_end = f + 1;
            break;
        case '`':
            lex->token = TOKEN_BQUOTE;
            lex->token_end = f + 1;
            break;
        case ',':
            lex->token = TOKEN_COMMA;
            lex->token_end = f + 1;
            break;
        case '@':
            lex->token = TOKEN_AT;
            lex->token_end = f + 1;
            break;
     
        case '#':
            // skip #
            //add tepri
            ++f;
            if (f == l) break;

            switch (*f) {
                case '(':
                    // #(
                 
                    lex->token = TOKEN_HASH_L_PAREN;
                    lex->token_end = f + 1;
                  
                    break;
                case '\\':
                    ++f;
                    // #_char
                    f = match_char_(f, l);
                    if (f) {
                        lex->token = TOKEN_CHAR;
                        lex->token_end = f;
                    }
                    break;
                case 't':
                case 'f':
                    // #t or #f
                    lex->token = TOKEN_BOOL;
                    lex->token_end = f + 1;
                    break;
                default:
                    break;
            }
            break;
        case '"':
            ++f;
            // printf("\n5.1.1.1 c_END:sh_f||%s", f);
            // printf("\n5.1.1.2 c_END:sh_l||%s", l);
            // printf("\n5.1.2 lex_END:sh||%s", lex->token_end);
            f = match_string_(f, l);
            // printf("\n5.1.1 c_END:sh_f||%s", f);
            // printf("\n5.1.1.2 c_END:sh_l||%s", l);
            // printf("\n5.1.2 lex_END:sh||%s", lex->token_end);
            if (f) {
                //printf("\nADDING TOKEN_STRING");
                lex->token = TOKEN_STRING;
                lex->token_end = f;
            }
            // printf("\n))-->%s",lex->token_end);
            break; //token_end indicates place
        default:
            // printf("\n3.1.1 c_ENDdef_num:||%s", f);
            // printf("\n3.1.2 lex_ENDdef_num:||%s", lex->token_end);
            f = match_number_(f, l, &has_decimal);
            // printf("\n3.1.1 c_END:def_num||%s", f);
            // printf("\n3.1.2 lex_ENDdef_num:||%s", lex->token_end);
            if (f) {
                lex->token = has_decimal ? TOKEN_FLOAT : TOKEN_INT;
                lex->token_end = f;
            } else {
                f = lex->token_end;
                // printf("\n4.1.1 c_END_symbol_f:||%s", f);
                // printf("\n4.1.1 c_END_symbol_l:||%s", l);
                // printf("\n4.1.2 lex_END_symbol:||%s", lex->token_end);
                f = match_symbol_(f, l);
                // printf("\n4.1.1 c_END_symbol_f:||%s", f);
                // printf("\n4.1.1 c_END_symbol_l:||%s", l);
                // printf("\n5.1.2 lex_END_symbol:||%s", lex->token_end);

                if (f) {
                    lex->token = TOKEN_SYMBOL;
                    lex->token_end = f;
                }
            }
            break;
    }
}

// requires: length(out) >= (last - first)
static
char* string_unescape_(const char* first, const char* last, char* out)
{

    while (first != last)
    {
        if (*first == '\\')
        {
            ++first;
            switch (*first)
            {
                case '\\': *out = '\\'; break;
                case 'n': *out = '\n'; break;
                case 't': *out = '\t'; break;
                case 'f': *out = '\f'; break;
                case '"': *out = '"'; break;
                default: break;
            }
        }
        else
        {
            *out = *first;
        }
        ++out;
        ++first;
    }
    return out;
}

static void print_escaped_(const char* c, FILE* file)
{
    while (*c)
    {
        switch (*c)
        {
            case '\n':
                fputc('\\', file);
                fputc('n', file);
                break;
            case '\t':
                fputc('\\', file);
                fputc('t', file);
                break;
            case '\f':
                fputc('\\', file);
                fputc('f', file);
                break;
            case '\"':
                fputc('\\', file);
                fputc('"', file);
                break;
            default:
                fputc(*c, file);
                break;
        }
        ++c;
    }
}

static Lisp parse_number_(Scanner* lex, clispPtrENV clsp)
{
    char tempArray[128];
    size_t length = scanner_copy_token(lex, 0, 128, tempArray);
    tempArray[length] = '\0';

    switch (lex->token)
    {
        case TOKEN_INT:  return cLISPparse_int(tempArray);
        case TOKEN_FLOAT: return cLISPparse_real(tempArray);
        default: assert(0);
    }
}

static Lisp parse_string_(Scanner* lex, clispPtrENV clsp)
{ 
    size_t size = (lex->token_end - lex->token_start) - 2;
    Lisp l = cLISPmake_buffer(size + 1, clsp);
    char* str = cLISPbuffer(l);
    scanner_copy_token(lex, 1, size, str);
    char* out = string_unescape_(str, str + size, str);
    *out = '\0';
    return l;
}

static Lisp parse_symbol_(Scanner* lex, clispPtrENV clsp)
{
    char tempArray[IDENT_MAX];
    size_t length = scanner_copy_token(lex, 0, IDENT_MAX, tempArray);
    // always convert symbols_table to uppercase
    for (int i = 0; i < length; ++i)
        tempArray[i] = toupper(tempArray[i]);
    return insertSymbol(clsp.p->symbols_table, tempArray, length, clsp);
}

static const char* asciiTable[] =
{
    "EOF",
    "NUL", "SOH", "STX", "ETX", "EOT",
    "ENQ", "ACK", "BEL", "backspace", "tab",
    "newline", "VT", "page", "return", "SO",
    "SI", "DLE", "DC1", "DC2", "DC3",
    "DC4", "NAK", "SYN", "ETB", "CAN",
    "EM", "SUB", "altmode", "FS", "GS", "RS",
    "backnext", "space", NULL
};

static int parse_char_(Scanner* lex)
{
    char tempArray[64];
    size_t length = scanner_copy_token(lex, 2, 64, tempArray);  
    tempArray[length] = '\0';

    if (length == 1)
    {
        return (int)tempArray[0];
    }
    else
    {
        const char** name_it = asciiTable;
        
        int i = 0;
        while (*name_it)
        {
            if (strcmp(*name_it, tempArray) == 0) return i - 1;
            ++name_it;
            ++i;
        }
        return -1;
    }
}

static Lisp parse_list_r(Scanner* lex, jmp_buf error_jmp, clispPtrENV clsp)
{  
    int quote_type = SYM_QUOTE;
    switch (lex->token)
    {
        
        case TOKEN_NONE:
        
            fprintf(clsp.p->errFile, "%lu. expected closing )\n", scanPos(lex));
            longjmp(error_jmp, ERROR_READ_SYNTAX);
        case TOKEN_PERIOD:
          
            fprintf(clsp.p->errFile, "%lu. unexpected .\n", scanPos(lex));
            longjmp(error_jmp, ERROR_READ_SYNTAX);
        case TOKEN_L_PAREN:
        {
         
            Lisp tail = cLISPnull();
            scanner_nextToken(lex);
            while (lex->token != TOKEN_R_PAREN && lex->token != TOKEN_PERIOD)
            {
                tail = cLISPcons(parse_list_r(lex, error_jmp, clsp), tail, clsp);
                scanner_nextToken(lex);
            }
            if (lex->token == TOKEN_PERIOD)
            {
               
                if (isNULLType(tail))
                {
                    fprintf(clsp.p->errFile, "%lu. unexpected .\n", scanPos(lex));
                    longjmp(error_jmp, ERROR_READ_SYNTAX);
                }

                scanner_nextToken(lex);
                if (lex->token != TOKEN_R_PAREN)
                {
                 
                    Lisp x = parse_list_r(lex, error_jmp, clsp);
                    tail = cLISPlist_reverse2(tail, x);
                    scanner_nextToken(lex);
                }
            }
            else
            {
                tail = cLISPlist_reverse(tail);
            }

            if (lex->token != TOKEN_R_PAREN)
            {
                fprintf(clsp.p->errFile, "%lu. expected closing )\n", scanPos(lex));
                longjmp(error_jmp, ERROR_READ_SYNTAX);
            }
            return tail;
        }
        case TOKEN_R_PAREN:
           
            fprintf(clsp.p->errFile, "unexpected )\n");
            longjmp(error_jmp, ERROR_READ_SYNTAX);
        case TOKEN_BOOL:
        {
      
            char c;
            scanner_copy_token(lex, 1, 1, &c);
            return cLISPmake_bool(c == 't' ? 1 : 0);
        }
        case TOKEN_HASH_L_PAREN:
        {
            // #(
            scanner_nextToken(lex);

            Lisp* buffer = NULL;
            size_t buffer_cap = 0;

            int n = 0;
            while (lex->token != TOKEN_R_PAREN)
            {
                Lisp x = parse_list_r(lex, error_jmp, clsp);
                scanner_nextToken(lex);

                if (buffer_cap <= n + 1)
                {
                    buffer_cap *= 2;
                    if (buffer_cap < 16) buffer_cap = 16;
                    buffer = realloc(buffer, buffer_cap * sizeof(Lisp));
                }
                buffer[n] = x;
                ++n;
            }
            // )
            
            Lisp v =  cLISPmake_vectorFuncPtr(buffer, n, clsp);
            if (buffer) free(buffer);
            return v;
        }
        case TOKEN_FLOAT:
         
        case TOKEN_INT:
         
            return parse_number_(lex, clsp);
        case TOKEN_STRING:
         
            return parse_string_(lex, clsp);
        case TOKEN_SYMBOL:
       
            return parse_symbol_(lex, clsp);
        case TOKEN_CHAR:
        {
          
            int c = parse_char_(lex);
            if (c <= 0)
            {
                fprintf(clsp.p->errFile, "%lu. unknown character\n", scanPos(lex));
                longjmp(error_jmp, ERROR_READ_SYNTAX);
            }
            return cLISPmake_char(c);
        }
        case TOKEN_COMMA:
           
            scanner_nextToken(lex);

            if (lex->token == TOKEN_AT)
            {
                quote_type = SYM_UNQUOTE_SPLICE;
                scanner_nextToken(lex);
            }
            else
            {
                quote_type = SYM_UNQUOTE;
            }
            goto quote;
        case TOKEN_BQUOTE:
     
            quote_type = SYM_QUASI_QUOTE;
            scanner_nextToken(lex);
            goto quote;
        case TOKEN_QUOTE:
         
            scanner_nextToken(lex);
            goto quote;
        quote:
        {
             // '
        
             Lisp l = cLISPcons(parse_list_r(lex, error_jmp, clsp), cLISPnull(), clsp);
             return cLISPcons(get_sym(quote_type, clsp), l, clsp);
        }
        default:
            assert(0);
    }
}
/*
//PRINTS TOKEN IN SCANNER
static Lisp parse_list_r_print(Scanner* lex, jmp_buf error_jmp, clispPtrENV clsp)
{  
    int quote_type = SYM_QUOTE;
    switch (lex->token)
    {
        case TOKEN_TEMPRI:
            printf("\nTEMPRI");
        case TOKEN_NONE:
            printf("\nTOKEN_NONE_ERROR");
            fprintf(clsp.p->errFile, "%lu. expected closing )\n", scanPos(lex));
            longjmp(error_jmp, ERROR_READ_SYNTAX);
        case TOKEN_PERIOD:
            printf("\nTOKEN_PERIOD_ERROR");
            fprintf(clsp.p->errFile, "%lu. unexpected .\n", scanPos(lex));
            longjmp(error_jmp, ERROR_READ_SYNTAX);
        case TOKEN_L_PAREN:
        {
            printf("\nTOKEN_LEFT");
            Lisp tail = cLISPnull();
            scanner_nextToken(lex);
            while (lex->token != TOKEN_R_PAREN && lex->token != TOKEN_PERIOD)
            {
                tail = cLISPcons(parse_list_r(lex, error_jmp, clsp), tail, clsp);
                scanner_nextToken(lex);
            }
            if (lex->token == TOKEN_PERIOD)
            {
               
                if (isNULLType(tail))
                {
                    fprintf(clsp.p->errFile, "%lu. unexpected .\n", scanPos(lex));
                    longjmp(error_jmp, ERROR_READ_SYNTAX);
                }

                scanner_nextToken(lex);
                if (lex->token != TOKEN_R_PAREN)
                {
                    printf("\n=>TOKEN_RIGHT");
                    Lisp x = parse_list_r(lex, error_jmp, clsp);
                    tail = cLISPlist_reverse2(tail, x);
                    scanner_nextToken(lex);
                }
            }
            else
            {
                tail = cLISPlist_reverse(tail);
            }

            if (lex->token != TOKEN_R_PAREN)
            {
                fprintf(clsp.p->errFile, "%lu. expected closing )\n", scanPos(lex));
                longjmp(error_jmp, ERROR_READ_SYNTAX);
            }
            return tail;
        }
        case TOKEN_R_PAREN:
            printf("\nTOKEN_RIGHT");
            fprintf(clsp.p->errFile, "unexpected )\n");
            longjmp(error_jmp, ERROR_READ_SYNTAX);
        case TOKEN_BOOL:
        {
            printf("\nBOOL");
            char c;
            scanner_copy_token(lex, 1, 1, &c);
            return cLISPmake_bool(c == 't' ? 1 : 0);
        }
        case TOKEN_HASH_L_PAREN:
        {
                printf("\nTOKEN_HASH_L_PAREN");
            scanner_nextToken(lex);

            Lisp* buffer = NULL;
            size_t buffer_cap = 0;

            int n = 0;
            while (lex->token != TOKEN_R_PAREN)
            {
                Lisp x = parse_list_r(lex, error_jmp, clsp);
                scanner_nextToken(lex);

                if (buffer_cap <= n + 1)
                {
                    buffer_cap *= 2;
                    if (buffer_cap < 16) buffer_cap = 16;
                    buffer = realloc(buffer, buffer_cap * sizeof(Lisp));
                }
                buffer[n] = x;
                ++n;
            }
            
            Lisp v =  cLISPmake_vectorFuncPtr(buffer, n, clsp);
            if (buffer) free(buffer);
            return v;
        }
        case TOKEN_FLOAT:
            printf("\nTOKEN_FLOAT");
        case TOKEN_INT:
            printf("\nTOKEN_INT");
            return parse_number_(lex, clsp);
        case TOKEN_STRING:
            printf("\nTOKEN_FLOAT");
            return parse_string_(lex, clsp);
        case TOKEN_SYMBOL:
            printf("\nTOKEN_SYMBOL");
            return parse_symbol_(lex, clsp);
        case TOKEN_CHAR:
        {
            printf("\nTOKEN_CHAR");
            int c = parse_char_(lex);
            if (c <= 0)
            {
                fprintf(clsp.p->errFile, "%lu. unknown character\n", scanPos(lex));
                longjmp(error_jmp, ERROR_READ_SYNTAX);
            }
            return cLISPmake_char(c);
        }
        case TOKEN_COMMA:
            printf("\nTOKEN_COMMA");
            scanner_nextToken(lex);

            if (lex->token == TOKEN_AT)
            {
                quote_type = SYM_UNQUOTE_SPLICE;
                scanner_nextToken(lex);
            }
            else
            {
                quote_type = SYM_UNQUOTE;
            }
            goto quote;
        case TOKEN_BQUOTE:
            printf("\nTOKEN_BQUOTE");
            quote_type = SYM_QUASI_QUOTE;
            scanner_nextToken(lex);
            goto quote;
        case TOKEN_QUOTE:
            printf("\nTOKEN_QUOTE");
            scanner_nextToken(lex);
            goto quote;
        quote:
        {
             printf("\nquote");
             Lisp l = cLISPcons(parse_list_r(lex, error_jmp, clsp), cLISPnull(), clsp);
             return cLISPcons(get_sym(quote_type, clsp), l, clsp);
        }
        default:
            assert(0);
    }
}
*/
static Lisp parse(Scanner* lex, cLispError* out_error, clispPtrENV clsp)
{
    jmp_buf error_jmp;
    cLispError error = setjmp(error_jmp);

    if (error != ERROR_NONE)
    {
        if (out_error) *out_error = error;
        return cLISPnull();
    }

    scanner_nextToken(lex);
    if (lex->token == TOKEN_NONE) return cLISPeof();

    Lisp result = parse_list_r(lex, error_jmp, clsp);
    scanner_nextToken(lex);
    
    if (lex->token != TOKEN_NONE)
    {
        result = cLISPcons(result, cLISPnull(), clsp);
        
        while (lex->token != TOKEN_NONE)
        {
            result = cLISPcons(parse_list_r(lex, error_jmp, clsp), result, clsp);
            scanner_nextToken(lex);
        } 

        result = cLISPcons(get_sym(SYM_BEGIN, clsp), cLISPlist_reverse(result), clsp);
    }

    if (out_error) *out_error = error;
    return result;
}

Lisp cLISPread(const char *program, cLispError* out_error, clispPtrENV clsp)
{
    Scanner lex;
    scannerInit(&lex, program, NULL);
    Lisp l = parse(&lex, out_error, clsp);
    return l;
}

static void* fread_all_(FILE* file, size_t* out_size) {
    const size_t chunkSize = 32 * 1024;
    const size_t MEDIUM_SIZE = 10 * 1024 * 1024;

    *out_size = 0;

    size_t cap = 0;
    char* data = NULL;

    while (1) {
        if (*out_size + chunkSize > cap) {
            if (cap > MEDIUM_SIZE) {
                cap = (cap * 3) / 2;
            } else if (cap < chunkSize) {
                cap = chunkSize;
            } else {
                cap *= 2;

            }

            void* new_data = realloc(data, cap);
            if (!new_data) {
                *out_size = 0;
                free(data);
                return NULL;
            }
            data = new_data;
        }

        size_t read = fread(data + *out_size, 1, chunkSize, file);
        *out_size += read;
        if (read < chunkSize) {
            if (ferror(file)) {
                *out_size = 0;
                free(data);
                return NULL;
            } else {
                return data;
            }
        }
    }
}

Lisp cLISPread_file(FILE *file, cLispError* out_error, clispPtrENV clsp)
{
    size_t size;
    char* start = fread_all_(file, &size);
    if (!start) {
        *out_error = ERROR_FILE_OPEN;
        return cLISPeof();
    }
    
    Scanner lex;    
    scannerInit(&lex, start, start+size);
    Lisp l = parse(&lex, out_error, clsp);
    free(start);
    return l; 
}


Lisp cLISPread_path(const char *path, cLispError* out_error, clispPtrENV clsp)
{

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        *out_error = ERROR_FILE_OPEN;
        return cLISPeof();
    }
    off_t pos = lseek(fd, 0, SEEK_CUR);
    off_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, pos, SEEK_SET);

    const char* program = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (program == MAP_FAILED) {
        *out_error = ERROR_FILE_OPEN;
        return cLISPeof();
    }
    Scanner lex;
    scannerInit(&lex, program, program+size);
    Lisp l = parse(&lex, out_error, clsp);
    return l;
    munmap((void*)program, size);
    close(fd);

    return l;

    return cLISPeof();
}

Lisp cLISPenv_extend(Lisp l, Lisp table, clispPtrENV clsp) { return cLISPcons(table, l, clsp); }

Lisp cLISPenv_lookup(Lisp l, Lisp key, int *present)
{
    while (isPairType(l))
    {
        Lisp x = cLISPtable_get(cLISPcar(l), key, present);
        if (*present) return x;
        l = cLISPcdr(l);
    }
    
    return cLISPnull();
}

void defineVM(Lisp l, Lisp key, Lisp x, clispPtrENV clsp)
{
    cLISPtable_set(cLISPcar(l), key, x, clsp);
}

int cLISPenv_set(Lisp l, Lisp key, Lisp x, clispPtrENV clsp)
{
    int present;
    while (isPairType(l))
    {
        cLISPtable_get(cLISPcar(l), key, &present);
        if (present)
        {
            cLISPtable_set(cLISPcar(l), key, x, clsp);
            return 1;
        }  
        l = cLISPcdr(l);
    }

    return 0; 
}

int cLISPis_env(Lisp l) { return cLISPis_list(l); }

static void cLISPprint_r(FILE* file, Lisp l, int human_readable, int is_cdr)
{
    switch (cLISPtype(l))
    {   case cLispTERPRI: fprintf(file, "\n"); break;
        case cLispINT: fprintf(file, "%lli", cLISPint(l)); break;
        case cLispREAL: fprintf(file, "%f", cLISPreal(l)); break;
        case cLispNULL:  break;
        case cLispSYMBOL: fputs(cLISPsymbol_string(l), file); break;
        case cLispBOOL:
            if(cLISPbool(l)== 0){
                fprintf(file, "FALSE");
            }
            else{fprintf(file, "TRUE");}
           
            break;
        case cLispSTRING:
            if (human_readable)
            {
                fputs(cLISPstring(l), file);
            }
            else
            {
                fputc('"', file);
                print_escaped_(cLISPstring(l), file);
                fputc('"', file);
            }
            break;
        case cLispCHAR:
        {
            int c = cLISPint(l);

            if (human_readable)
            {
                if (c >= 0) fputc(c, file);
            }
            else
            {
                if (c >= -1 && c < 33)
                {
                    fprintf(file, "#\\%s", asciiTable[c + 1]);
                }
                else if (isprint(c))
                {
                    fprintf(file, "#\\%c", (char)c);
                }
                else
                {
                    fprintf(file, "#\\+%d", c);
                }
            }
            break;
        }
        case cLispJUMP: fputs("<jump>", file); break;
        case cLispLAMBDA: fputs("<lambda>", file); break;
        case cLispPTR: fprintf(file, "<ptr-%p>", l.val.ptrVal); break;
        case cLispPORT_IN: fprintf(file, "<input port-%d>", fileno((FILE*)l.val.ptrVal)); break;
        case cLispPORT_OUT: fprintf(file, "<output port-%d>", fileno((FILE*)l.val.ptrVal)); break;
        case cLispFUNC: fprintf(file, "<c-func-%p>", l.val.ptrVal); break;
        case cLispTABLE:
        {
            const Table* table = table_get_(l);
            fprintf(file, "{");

            Lisp keys = { table->keys, cLispVECTOR };
            Lisp vals = { table->vals, cLispVECTOR };
            for (int i = 0; i < table->capacity; ++i)
            {
                Lisp key = cLISPvector_ref(keys, i);
                if (!isNULLType(key))
                {
                    Lisp val = cLISPvector_ref(vals, i);

                    cLISPprint_r(file, key, human_readable, 0);
                    fprintf(file, ": ");
                    cLISPprint_r(file, val, human_readable, 0);
                    fprintf(file, " ");
                }
            }
            fprintf(file, "}");
            break;
        }
        case cLispVECTOR:
        {
            fprintf(file, "#(");
            int N = cLISPvector_length(l);
            for (int i = 0; i < N; ++i)
            {
                cLISPprint_r(file, cLISPvector_ref(l, i), human_readable, 0);
                if (i + 1 < N)
                {
                    fprintf(file, " ");
                }
            }
            fprintf(file, ")");
            break;
        }
        case cLispPAIR:
        {
            if (!is_cdr) fprintf(file, "(");
            cLISPprint_r(file, cLISPcar(l), human_readable, 0);

            if (cLISPtype(cLISPcdr(l)) != cLispPAIR)
            {
                if (!isNULLType(cLISPcdr(l)))
                { 
                    fprintf(file, " . ");
                    cLISPprint_r(file, cLISPcdr(l), human_readable, 0);
                }

                fprintf(file, ")");
            }
            else
            {
                fprintf(file, " ");
                cLISPprint_r(file, cLISPcdr(l), human_readable, 1);
            } 
            break;
        }
        default:
            
            break;
    }
}

void cLISPprintf(FILE* file, Lisp l) { cLISPprint_r(file, l, 0, 0);  }

void cLISPset_stderr(FILE* file, clispPtrENV clsp) { clsp.p->errFile = file; }
FILE *cLISPstderr(clispPtrENV clsp) { return clsp.p->errFile; }

static void cLISPstack_push(Lisp x, clispPtrENV clsp)
{
    clsp.p->stack[clsp.p->stack_ptr] = x;
    ++clsp.p->stack_ptr;
}

static Lisp cLISPstack_pop(clispPtrENV clsp)
{
    clsp.p->stack_ptr--;
    return clsp.p->stack[clsp.p->stack_ptr];
}

static Lisp* cLISPstack_peek(size_t i, clispPtrENV clsp)
{
    return clsp.p->stack + (clsp.p->stack_ptr - i);
}

Lisp cLISPcall_cc(Lisp proc, cLispError* out_error, clispPtrENV clsp)
{
    Lisp j = make_jump_(clsp);
    jumpBuffer* jump = jump_get_(j);
    jump->stack_ptr = clsp.p->stack_ptr;

    int has_result = setjmp(jump->jmp);
    if (has_result)
    {
        jump = jump_get_(cLISPstack_pop(clsp));
        clsp.p->stack_ptr = jump->stack_ptr;
        return jump->result;
    }
    else
    {
        return cLISPapply(proc, cLISPcons(j, cLISPnull(), clsp), out_error, clsp);
    }
}

static int apply(Lisp operator, Lisp args, Lisp* out_result, Lisp* out_env, cLispError* error, clispPtrENV clsp)
{
    switch (cLISPtype(operator))
    {
        case cLispLAMBDA: 
        {
            Lisp slot_names = lambda_args_(operator);
            *out_env = cLISPlambda_env(operator);

            // make a new environment
            Lisp new_table = cLISPmake_table(clsp);
            
            // bind parameters to arguments
            // to pass into function call
            while (isPairType(slot_names) && isPairType(args))
            {
                cLISPtable_set(new_table, cLISPcar(slot_names), cLISPcar(args), clsp);
                slot_names = cLISPcdr(slot_names);
                args = cLISPcdr(args);
            }

            if (cLISPtype(slot_names) == cLispSYMBOL)
            {
                // variable length arguments
                cLISPtable_set(new_table, slot_names, args, clsp);
            }
            else if (!isNULLType(slot_names))
            {
                *error = ERROR_TOO_FEW_ARGS;
                return 0;
            }
            else if (!isNULLType(args))
            {
                *error = ERROR_TOO_MANY_ARGS;
                return 0;
            }

            *out_env = cLISPenv_extend(*out_env, new_table, clsp);

          
            *out_result = cLISPlambda_body(operator);
            return 1;
        }
        case cLispFUNC: 
        {
            LispCFunc f = cLISPfunc(operator);
            *out_result = f(args, error, clsp);
            return 0;
        }
        case cLispJUMP:
        {
            jumpBuffer* jump = jump_get_(operator);
            jump->result = cLISPcar(args);
            cLISPstack_push(operator, clsp);
            longjmp(jump->jmp, 1);
        }
        default:
        {
            cLISPprintf(clsp.p->errFile, operator);
            fprintf(clsp.p->errFile, " is not an operator.\n");

            *error = ERROR_BAD_OP;
            return 0;
        }
    }
}

static Lisp eval_r(jmp_buf error_jmp, clispPtrENV clsp)
{
    Lisp* env = cLISPstack_peek(2, clsp);
    Lisp* x = cLISPstack_peek(1, clsp);
    
    while (1)
    {
        switch (cLISPtype(*x))
        {
            case cLispSYMBOL:
            {
                int present = 0;
                Lisp val = cLISPenv_lookup(*env, *x, &present);
                if (!present)
                {
                    fprintf(clsp.p->errFile, "%s is not defined.\n", cLISPsymbol_string(*x));
                    longjmp(error_jmp, ERROR_UNDEFINED_VAR); 
                    return cLISPnull();
                }
                return val;
            }
            case cLispPAIR:
            {
                Lisp op_sym = cLISPcar(*x);
                int opValid = cLISPtype(op_sym) == cLispSYMBOL;

                if (cLISPeq(op_sym, get_sym(SYM_IF, clsp)) && opValid) 
                {
                
                    Lisp predicate = cLISPlist_ref(*x, 1);
                    
                    cLISPstack_push(*env, clsp);
                    cLISPstack_push(predicate, clsp);
 
                    if (isboolTrue(eval_r(error_jmp, clsp)))
                    {
                        *x = cLISPlist_ref(*x, 2);

                    }
                    else
                    {
                   
                        *x = cLISPlist_ref(*x, 3);
               
                    }
                    
                    cLISPstack_pop(clsp);
                    cLISPstack_pop(clsp);
                }
                else if (cLISPeq(op_sym, get_sym(SYM_BEGIN, clsp)) && opValid)
                {
                    Lisp it = cLISPcdr(*x);
                    if (isNULLType(it)) return it;
                
                    while (isPairType(cLISPcdr(it)))
                    {
                        // save next thing
                        cLISPstack_push(cLISPcdr(it), clsp);
                        
                        cLISPstack_push(*env, clsp);
                        cLISPstack_push(cLISPcar(it), clsp);
                        
                        eval_r(error_jmp, clsp);
        
                        cLISPstack_pop(clsp);
                        cLISPstack_pop(clsp);
                        
                        it = cLISPstack_pop(clsp);
                        //it = cLISPcdr(it);
                    }
                    
                    *x = cLISPcar(it);
        
                }
                else if (cLISPeq(op_sym, get_sym(SYM_QUOTE, clsp)) && opValid)
                {
                    return cLISPlist_ref(*x, 1);
                }
                else if (cLISPeq(op_sym, get_sym(SYM_DEFINE, clsp))) 
                {
                    // variable definitions
                    cLISPstack_push(*env, clsp);
                    cLISPstack_push(cLISPlist_ref(*x, 2), clsp);
                    
                    Lisp value = eval_r(error_jmp, clsp);
                    
                    cLISPstack_pop(clsp);
                    cLISPstack_pop(clsp);
                    
                    Lisp symbol = cLISPlist_ref(*x, 1);
                    defineVM(*env, symbol, value, clsp);
                    return cLISPnull();
                }
                else if (cLISPeq(op_sym, get_sym(SYM_SET, clsp)) && opValid)
                {
                    assert(!isNULLType(*env));
                    cLISPstack_push(*env, clsp);
                    cLISPstack_push(cLISPlist_ref(*x, 2), clsp);
                    
                    Lisp value = eval_r(error_jmp, clsp);
                    
                    cLISPstack_pop(clsp);
                    cLISPstack_pop(clsp);
                    
                    Lisp symbol = cLISPlist_ref(*x, 1);
                    if (!cLISPenv_set(*env, symbol, value, clsp))
                    { 
                        fprintf(clsp.p->errFile, "error: unknown variable: %s\n", cLISPsymbol_string(symbol));
                    }
                    return cLISPnull();
                }
                else if (cLISPeq(op_sym, get_sym(SYM_LAMBDA, clsp)) && opValid) 
                {
                    Lisp args = cLISPlist_ref(*x, 1);
                    Lisp body = cLISPlist_ref(*x, 2);
                    return cLISPmake_lambda(args, body, *env, clsp);
                }
                else 
                {
                    cLISPstack_push(*env, clsp);
                    cLISPstack_push(cLISPcar(*x), clsp);
                    
                    Lisp operator = eval_r(error_jmp, clsp);
                    
                    Lisp operator_expr = cLISPstack_pop(clsp);
                    cLISPstack_pop(clsp);
                    
                    cLISPstack_push(operator, clsp);
                    cLISPstack_push(operator_expr, clsp);
                    
                    Lisp arg_expr = cLISPcdr(*x);
                    
                    Lisp args = cLISPnull();
                    
                    while (isPairType(arg_expr))
                    {
                        cLISPstack_push(cLISPcdr(arg_expr), clsp);
                        cLISPstack_push(args, clsp);

                        cLISPstack_push(*env, clsp);
                        cLISPstack_push(cLISPcar(arg_expr), clsp);
                        Lisp new_arg = eval_r(error_jmp, clsp);
                        cLISPstack_pop(clsp);
                        cLISPstack_pop(clsp);

                        args = cLISPcons(new_arg, cLISPstack_pop(clsp), clsp);
                        arg_expr = cLISPstack_pop(clsp);
                    }
                    
                    operator_expr = cLISPstack_pop(clsp);
                    operator = cLISPstack_pop(clsp);
                    
                    cLispError error = ERROR_NONE;
                    int needs_to_eval = apply(operator, cLISPlist_reverse(args), x, env, &error, clsp);
                    if (error != ERROR_NONE)
                    {
                        if (cLISPtype(operator_expr) == cLispSYMBOL)
                        {
                            fprintf(clsp.p->errFile, "operator: %s\n", cLISPsymbol_string(operator_expr));
                        }

                        longjmp(error_jmp, error);
                    }

                    if (!needs_to_eval)
                    {
                        return *x;
                    }
               
               


                }
                break;
            }
            default:
               return *x; 
        }
    }
}

static Lisp expand_quasi_r(Lisp l, jmp_buf error_jmp, clispPtrENV clsp)
{
    if (cLISPtype(l) != cLispPAIR)
    {
        Lisp terms[] = { get_sym(SYM_QUOTE, clsp), l };
        return cLISPmake_list2(terms, 2, clsp);
    }

    Lisp op = cLISPcar(l);
    int opValid = cLISPtype(op) == cLispSYMBOL;

    if (cLISPeq(op, get_sym(SYM_UNQUOTE, clsp)) && opValid)
    {
        return cLISPcar(cLISPcdr(l));
    }
    else if (cLISPeq(op, get_sym(SYM_UNQUOTE_SPLICE, clsp)) && opValid)
    {
        fprintf(clsp.p->errFile, "slicing ,@ must be in a backquoted list.\n");
        longjmp(error_jmp, ERROR_FORM_SYNTAX);
    }
    else
    {
        Lisp terms[] = {
            get_sym(SYM_CONS, clsp),
            expand_quasi_r(cLISPcar(l), error_jmp, clsp),
            expand_quasi_r(cLISPcdr(l), error_jmp, clsp),
        };
        return cLISPmake_list2(terms, 3, clsp);
    }
}

static Lisp expand_r(Lisp l, jmp_buf error_jmp, clispPtrENV clsp)
{
    if (cLISPtype(l) != cLispPAIR) return l;

    Lisp op = cLISPcar(l);
    if (cLISPtype(op) == cLispSYMBOL)
    {
        if (cLISPeq(op, get_sym(SYM_QUOTE, clsp)))
        {
            if (cLISPlist_length(l) != 2)
            {
                fprintf(clsp.p->errFile, "(quote x)\n");
                longjmp(error_jmp, ERROR_FORM_SYNTAX);
            }
            return l;
        }
        else if (cLISPeq(op, get_sym(SYM_QUASI_QUOTE, clsp)))
        {
            return expand_quasi_r(cLISPcar(cLISPcdr(l)), error_jmp, clsp);
        }
        else if (cLISPeq(op, get_sym(SYM_DEFINE_MACRO, clsp)))
        {
            if (cLISPlist_length(l) != 3)
            {
                fprintf(clsp.p->errFile, "(define-macro name proc)\n");
                longjmp(error_jmp, ERROR_FORM_SYNTAX);
            } 

            Lisp symbol = cLISPlist_ref(l, 1);
            Lisp body = cLISPlist_ref(l, 2);

            cLispError e;
            Lisp lambda = cLISPeval(body, &e, clsp);

            if (e != ERROR_NONE)
            {
                longjmp(error_jmp, e);
            }
            if (cLISPtype(lambda) != cLispLAMBDA)
            {
                fprintf(clsp.p->errFile, "(define-macro name proc) not a procedure\n");
                longjmp(error_jmp, ERROR_FORM_SYNTAX);
            } 

            cLISPtable_set(clsp.p->macros, symbol, lambda, clsp);
            return cLISPnull();
        }
        else 
        {
            int present;
            Lisp proc = cLISPtable_get(clsp.p->macros, op, &present);

            if (present)
            {
                Lisp result;
                Lisp calling_env;
                cLispError error = ERROR_NONE;
                if (apply(proc, cLISPcdr(l), &result, &calling_env, &error, clsp) == 1)
                {
                    result = cLISPevalPtr(result, calling_env, &error, clsp);
                }

                if (error != ERROR_NONE)
                {
                    fprintf(clsp.p->errFile, "macroexpand failed: %s\n", cLISPsymbol_string(op));
                    longjmp(error_jmp, error);
                }
                return expand_r(result, error_jmp, clsp);
            }
        }
    }
    Lisp it = l;
    while (isPairType(it))
    {
        cLISPset_car(it, expand_r(cLISPcar(it), error_jmp, clsp));
        it = cLISPcdr(it);
    }
    return l;

}

Lisp cLISPmacroexpand(Lisp lisp, cLispError* out_error, clispPtrENV clsp)
{
    jmp_buf error_jmp;
    cLispError error = setjmp(error_jmp);

    if (error == ERROR_NONE)
    {
        Lisp result = expand_r(lisp, error_jmp, clsp);
        *out_error = error;
        return result;
    }
    else
    {
        *out_error = error;
        return cLISPnull();
    }
}

Lisp cLISPevalPtr(Lisp l, Lisp env, cLispError* out_error, clispPtrENV clsp)
{
    cLispError error;
    Lisp expanded = cLISPmacroexpand(l, &error, clsp);
    
    if (error != ERROR_NONE)
    {
        if (out_error) *out_error = error;
        return cLISPnull();
    }
    
    size_t save_stack = clsp.p->stack_ptr;
    
    jmp_buf error_jmp;
    error = setjmp(error_jmp);

    if (error == ERROR_NONE)
    {
        cLISPstack_push(env, clsp);
        cLISPstack_push(expanded, clsp);
        
        Lisp result = eval_r(error_jmp, clsp);
        
        cLISPstack_pop(clsp);
        cLISPstack_pop(clsp);

        if (out_error)
        {
            *out_error = error;
        }

        return result; 
    }
    else
    {
        if (out_error)
        {
            clsp.p->stack_ptr = save_stack;
            *out_error = error;
        }

        return cLISPnull();
    }
}

Lisp cLISPeval(Lisp expr, cLispError* out_error, clispPtrENV clsp)
{
    return cLISPevalPtr(expr, cLISPenv(clsp), out_error, clsp);
}

Lisp cLISPapply(Lisp op, Lisp args, cLispError* out_error, clispPtrENV clsp)
{
    Lisp x;
    Lisp env;
    int needs_to_eval = apply(op, args, &x, &env, out_error, clsp);
    if (*out_error != ERROR_NONE) return cLISPfalse();
    return needs_to_eval ? cLISPevalPtr(x, env, out_error, clsp) : x;
}

static Lisp garbageMove(Lisp x, clispPtrENV clsp)
{
    switch (x.type)
    {
        case cLispLAMBDA:
        case cLispPAIR:
        case cLispSTRING:
        case cLispVECTOR:
        case cLispTABLE:
        case cLispSYMBOL:
        {
            Chunk* chunk = x.val.ptrVal;
            if (chunk->gc_state == garbageClear)
            {
                Chunk* dest = heapAlloc(chunk->link.size, chunk->type, &clsp.p->heap);
                memcpy(dest, chunk, chunk->link.size);
                dest->gc_state = garbageToVisit;
                chunk->link.forward = dest;
                chunk->gc_state = garbageNonExist;
            }

            assert(chunk->gc_state == garbageNonExist);
            x.val.ptrVal = chunk->link.forward;
            return x;
        }
        default:
            return x;
    }
}

static cLispVal garbageMoveVal(cLispVal val, clispType type, clispPtrENV clsp)
{
    return garbageMove( (Lisp) { val, type}, clsp).val;
}

static void garbageMove_v(Lisp* start, int n, clispPtrENV clsp)
{
    for (int i = 0; i < n; ++i) start[i] = garbageMove(start[i], clsp);
}

static Lisp garbageMoveSymbols(Lisp old_table, clispPtrENV clsp)
{
    Table* from = table_get_(old_table);
    Lisp to_table = cLISPmake_table(clsp);
    int cap = from->capacity;
     tableExpand(to_table, cap, clsp);

    Lisp hashes = { from->keys, cLispVECTOR };
    Lisp symbols_table = { from->vals, cLispVECTOR };

    for (int i = 0; i < cap; ++i)
    {
        Lisp hash = cLISPvector_ref(hashes, i); 
        if (!isNULLType(hash))
        {
            Lisp old_symbol = cLISPvector_ref(symbols_table, i);
            while (old_symbol.val.ptrVal != NULL)
            {
                if (symbol_get_(old_symbol)->chunk.gc_state == garbageNonExist)
                {
                    Lisp to_insert = garbageMove(old_symbol, clsp);
                    int present;
                    Lisp existing = cLISPtable_get(to_table, hash, &present);
                    symbol_get_(to_insert)->next = existing.val;
                    cLISPtable_set(to_table, hash, to_insert, clsp);
                }
                old_symbol.val = symbol_get_(old_symbol)->next;
            }
        }
    }
    return to_table;
}

Lisp cLISPcollect(Lisp root_to_save, clispPtrENV clsp)
{
    Heap from = clsp.p->heap;
    heap_init(&clsp.p->heap);
    clsp.p->env = garbageMove(clsp.p->env, clsp);
    clsp.p->macros = garbageMove(clsp.p->macros, clsp);

    garbageMove_v(clsp.p->symbol_cache, SYM_COUNT, clsp);
    garbageMove_v(clsp.p->stack, clsp.p->stack_ptr, clsp);

    Lisp result = garbageMove(root_to_save, clsp);

    const Page* page = clsp.p->heap.bottom;
    int page_counter = 0;
    while (page)
    {
        size_t offset = 0;
        while (offset < page->size)
        {
            Chunk* chunk = (Chunk*)(page->buffer + offset);
            if (chunk->gc_state == garbageToVisit)
            {
                switch (chunk->type)
                {
                    case cLispPAIR:
                    {
                        
                        Pair* p = (Pair*)chunk;
                        p->car = garbageMoveVal(p->car, p->chunk.d.pair.car_type, clsp);
                        p->cdr = garbageMoveVal(p->cdr, p->chunk.d.pair.cdr_type, clsp);
                        break;
                    }
                    case cLispVECTOR:
                    {
                        Lisp vector;
                        vector.val.ptrVal = chunk;
                        vector.type = cLispVECTOR;

                        Vector* v = (Vector*)chunk;
                        int n = vector_len_(v);
                        for (int i = 0; i < n; ++i)
                            v->entries[i] = garbageMove(cLISPvector_ref(vector, i), clsp).val;
                        break;
                    }
                    case cLispLAMBDA:
                    {
                        
                        Lambda* l = (Lambda*)chunk;
                        l->args = garbageMoveVal(l->args, (clispType)l->chunk.d.lambda.args_type, clsp);
                        l->body = garbageMoveVal(l->body, (clispType)l->chunk.d.lambda.body_type, clsp);
                        l->env = garbageMoveVal(l->env, l->env.ptrVal == NULL ? cLispNULL : cLispPAIR, clsp);
                        break;
                    }
                    
                    case cLispTABLE:
                    {
                        
                        Lisp table;
                        table.val.ptrVal = chunk;
                        table.type = cLispTABLE;

                        Table* t = (Table*)chunk;
                        int n = t->capacity;

                        Lisp keys = { t->keys, cLispVECTOR };
                        Lisp vals = { t->vals, cLispVECTOR };

                        for (int i = 0; i < n; ++i)
                        {
                         
                         
                            Lisp key = cLISPvector_ref(keys, i); 
                            if (!isNULLType(key))
                            {
                                cLISPvector_set(keys, i, garbageMove(key, clsp));
                                cLISPvector_set(vals, i, garbageMove(cLISPvector_ref(vals, i), clsp));
                            }
                        }
                      
                      
                         tableExpand(table, n, clsp);
                        break;
                     }
                    default: break;
                }
                chunk->gc_state = garbageClear;
            }
            offset += chunk->link.size;
        }
        page = page->next;
        ++page_counter;
    }
    
    assert(page_counter == clsp.p->heap.page_count);
    clsp.p->symbols_table = garbageMoveSymbols(clsp.p->symbols_table, clsp);

    
    size_t diff = from.size - clsp.p->heap.size;

    heapEnd(&from);
    
    clsp.p->gc_stat_freed = diff;

    return result;
}

void cLISPprint_collect_stats(clispPtrENV clsp)
{
    Page* page = clsp.p->heap.bottom;
    while (page)
    {
        printf("%lu/%lu ", page->size, page->capacity);
        page = page->next;
    }
    fprintf(clsp.p->errFile, "\ngc collected: %lu\t time: %lu us\n", clsp.p->gc_stat_freed, clsp.p->gc_stat_time);
    fprintf(clsp.p->errFile, "heap size: %lu\t pages: %lu\n", clsp.p->heap.size, clsp.p->heap.page_count);
    fprintf(clsp.p->errFile, "symbols_table: %lu \n", (size_t)cLISPtable_size(clsp.p->symbols_table));
}

Lisp cLISPenv(clispPtrENV clsp) { return clsp.p->env; }

void cLISPset_env(Lisp env, clispPtrENV clsp)
{
    assert(cLISPis_env(env));
    clsp.p->env = env;
}

Lisp cLISPmacro_table(clispPtrENV clsp) { return clsp.p->macros; }

void cLISPset_macro_table(Lisp table, clispPtrENV clsp)
{
    assert(cLISPtype(table) == cLispTABLE);
    clsp.p->macros = table;
}

const char* cLISPerror_string(cLispError error)
{
    switch (error)
    {
        case ERROR_NONE:
            return "none";
        case ERROR_FILE_OPEN:
            return "file error: could not open file";
        case ERROR_MMAP:
            return "mmap error.";
        case ERROR_READ_SYNTAX:
            return "read/syntax error.";
        case ERROR_FORM_SYNTAX:
            return "expand error: bad special form";
        case ERROR_UNDEFINED_VAR:
            return "eval error: undefined variable";
        case ERROR_BAD_OP:
            return "eval error: attempt to apply something which was not an operator";
        case ERROR_ARG_TYPE:
            return "eval error: invalid argument type";
        case ERROR_TOO_MANY_ARGS:
            return "eval error: too many arguments";
         case ERROR_TOO_FEW_ARGS:
            return "eval error: missing arguments";
        case ERROR_OUT_OF_BOUNDS:
            return "eval error: index out of bounds";
        case ERROR_RUNTIME:
            return "evaluation called (error) and it was not handled";
        default:
            return "unknown error code";
    }
}

clispPtrENV cLispInit(void)
{
    clispPtrENV clsp;
    clsp.p = malloc(sizeof(struct LispStruct));
    if (!clsp.p) return clsp;

    clsp.p->errFile = stderr;

    clsp.p->symbol_counter = 0;
    clsp.p->stack_ptr = 0;
    clsp.p->stack_depth = STACK_DEPTH;
    clsp.p->stack = malloc(sizeof(Lisp) * STACK_DEPTH);
    clsp.p->gc_stat_freed = 0;
    clsp.p->gc_stat_time = 0;
    
    heap_init(&clsp.p->heap);

    clsp.p->symbols_table = cLISPmake_table(clsp);
    clsp.p->env = cLISPnull();
    clsp.p->macros = cLISPmake_table(clsp);

    Lisp* c = clsp.p->symbol_cache;
    c[SYM_IF] = createSymbol("IF", clsp);
    c[SYM_BEGIN] = createSymbol("BEGIN", clsp);
    c[SYM_QUOTE] = createSymbol("QUOTE", clsp);
    c[SYM_QUASI_QUOTE] = createSymbol("QUASIQUOTE", clsp);
    c[SYM_UNQUOTE] = createSymbol("UNQUOTE", clsp);
    c[SYM_UNQUOTE_SPLICE] = createSymbol("UNQUOTESPLICE", clsp);
    c[SYM_DEFINE] = createSymbol("DEF", clsp);
    c[SYM_DEFINE_MACRO] = createSymbol("DEFINE-MACRO", clsp);
    c[SYM_SET] = createSymbol("SET!", clsp);
    c[SYM_LAMBDA] = createSymbol("/\\_", clsp);
    c[SYM_CONS] = createSymbol("CONS", clsp);
    c[SYM_TERPRI] = createSymbol("TERPRI", clsp);
    return clsp;
}

void cLISPshutdown(clispPtrENV clsp)
{
    heapEnd(&clsp.p->heap);
    free(clsp.p->stack);
    free(clsp.p);
}

#endif

