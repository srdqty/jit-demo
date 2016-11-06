#ifndef __STRUCT_H__
#define __STRUCT_H__

#define TRUE 1
#define FALSE 0

#define FIXNUM_ENCODING 1

enum {
  num_type,
  func_type,
  sym_type,
  plus_type,
  minus_type,
  times_type,
  app_type,
  lambda_type,
  if0_type
};

enum {
  env_type = 100
};

typedef struct env env;

typedef struct tagged {
  int type;
} tagged;

#if !FIXNUM_ENCODING
# define TAGGED_TYPE(t) ((t)->type)
#else
# define TAGGED_TYPE(t) ((((intptr_t)(t)) & 0x1) ? num_type : ((t)->type))
#endif

#if !FIXNUM_ENCODING
typedef struct num_val {
  tagged t;
  int n;
} num_val;
# define NUM_VAL(v) (((num_val *)(v))->n)
#else
# define NUM_VAL(v) (((intptr_t)(v)) >> 1)
#endif

typedef struct func_val {
  tagged t;
  struct symbol *arg_name;
  tagged *body;
  env* e;
} func_val;

typedef struct symbol {
  tagged t;
  char *s;
} symbol;

typedef struct bin_op_expr {
  tagged t;
  tagged *left, *right;
} bin_op_expr;

typedef struct lambda_expr {
  tagged t;
  symbol *arg_name;
  tagged *body;
} lambda_expr;

typedef struct if0_expr {
  tagged t;
  tagged *tst;
  tagged *thn;
  tagged *els;
} if0_expr;

struct env {
  int type;
  symbol *id;
  tagged *val;
  struct env *rest;
};

#if !FIXNUM_ENCODING
tagged* make_num(int n);
#else
# define make_num(n) ((tagged *)(intptr_t)(((n) << 1) | 0x1))
#endif

tagged* make_func(symbol *arg_name, tagged *body, env *e);
symbol* make_symbol(char *s);
tagged* make_plus(tagged *left, tagged *right);
tagged* make_minus(tagged *left, tagged *right);
tagged* make_times(tagged *left, tagged *right);
tagged* make_app(tagged *left, tagged *right);
tagged* make_lambda(symbol *arg_name, tagged *body);
tagged* make_if0(tagged *tst, tagged *thn, tagged *els);

env* make_env(symbol *id, tagged *val, env *rest);

#endif