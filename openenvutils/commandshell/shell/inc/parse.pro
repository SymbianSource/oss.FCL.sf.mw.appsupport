/* Generated automatically */
static int par_event _((void));
static void set_list_code _((int p,int type,int complex));
static void set_sublist_code _((int p,int type,int flags,int skip,int complex));
static int par_list _((int*complex));
static int par_list1 _((int*complex));
static int par_sublist _((int*complex));
static int par_sublist2 _((int*complex));
static int par_pline _((int*complex));
static int par_cmd _((int*complex));
static void par_for _((int*complex));
static void par_case _((int*complex));
static void par_if _((int*complex));
static void par_while _((int*complex));
static void par_repeat _((int*complex));
static void par_subsh _((int*complex));
static void par_funcdef _((void));
static void par_time _((void));
static void par_dinbrack _((void));
static int par_simple _((int*complex,int nr));
static void par_redir _((int*rp));
static int par_wordlist _((void));
static int par_nl_wordlist _((void));
static int par_cond _((void));
static int par_cond_1 _((void));
static int par_cond_2 _((void));
static int par_cond_double _((char*a,char*b));
static int get_cond_num _((char*tst));
static int par_cond_triple _((char*a,char*b,char*c));
static int par_cond_multi _((char*a,LinkList l));
static void yyerror _((int noerr));
static Wordcode load_dump_header _((char*nam,char*name,int err));
static int build_dump _((char*nam,char*dump,char**files,int ali,int map,int flags));
static int build_cur_dump _((char*nam,char*dump,char**names,int match,int map,int what));
static int zwcstat _((char*filename,struct stat*buf,FuncDump dumps));
static Eprog check_dump_file _((char*file,struct stat*sbuf,char*name,int*ksh));
