#include "loki_main.hpp"
#include <stdio.h>
#include <iostream>
#include <chibi/eval.h>



#ifdef __ANDROID__
    #include <android/log.h>
    #define APPNAME "Loki"
#endif


#define BUF_SIZE 1024 * 16
static char text_out[BUF_SIZE] = "<outputs here>";


static sexp check_exception(sexp ctx, sexp res) {
    sexp_gc_var4(err, advise, sym, tmp);
    if (res && sexp_exceptionp(res)) {
        sexp_gc_preserve4(ctx, err, advise, sym, tmp);
        tmp = res;
        err = sexp_current_error_port(ctx);
        if (! sexp_oportp(err))
            err = sexp_make_output_port(ctx, stderr, SEXP_FALSE);
        sexp_print_exception(ctx, res, err);
        sexp_print_exception_stack_trace(ctx, res, err);
        sexp_gc_release4(ctx);
        //exit(70);
    }
    return res;
}





char buf_out[40960] = { 0 };
char buf_err[40960] = { 0 };

static sexp ctx;
static sexp env;




void scheme_init() {

    #ifdef __ANDROID__
        fclose(stdout);
        fclose(stderr);
        stdout = fmemopen(buf_out, sizeof(buf_out), "w");
        stderr = fmemopen(buf_err, sizeof(buf_err), "w");
        setbuf(stdout, NULL);
        setbuf(stderr, NULL);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "LOKI Hello world from C.\n");
    #endif

    sexp_scheme_init();
    ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    env = sexp_context_env(ctx);


    sexp tmp;
    #ifdef __ANDROID__
        char append_path[] = "/sdcard/Loki/chibi-lib";
        sexp_add_module_directory(ctx, sexp_c_string(ctx,append_path,-1), SEXP_TRUE);

        char include_path1[] = "/sdcard/Loki/chibi-ffi";
        sexp_add_module_directory(ctx, tmp=sexp_c_string(ctx,include_path1,-1), SEXP_FALSE);
        
        char include_path2[] = "/sdcard/Loki/chibi-scm";
        sexp_add_module_directory(ctx, tmp=sexp_c_string(ctx,include_path2,-1), SEXP_FALSE);

    #else
        char include_path3[] = "/home/nawal/data/chibi-droid/deps/chibi-scheme-static/lib";
        sexp_add_module_directory(ctx, tmp=sexp_c_string(ctx,include_path3,-1), SEXP_FALSE);
    #endif

    sexp e_ = sexp_load_standard_env(ctx, env, SEXP_SEVEN);
    sexp_load_standard_ports(ctx, e_, stdin, stdout, stderr, 0);
    check_exception(ctx, e_);
}












void loki_init() {
    scheme_init();

    #ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "LOKI ----------\n");
    #endif
    printf("foo");
    fprintf(stderr, "%s", "test stderr worked!\n");

    #ifdef __ANDROID__    
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "stdout: %s", buf_out);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "stderr: %s", buf_err);
    #endif

    sexp obj1;
    sexp_eval_string(ctx, "(display \"LOKI scheme prints... called from C code.\")", -1, NULL);
    sexp_eval_string(ctx, "(newline)", -1, NULL);
    sexp_eval_string(ctx, "(define lucky-number 666)", -1, NULL);
    sexp ret = sexp_eval_string(ctx, "lucky-number", -1, NULL);

    int lucky_number = -1;
    if (sexp_integerp(ret)) {
        lucky_number = sexp_unbox_fixnum(ret);
    }
    #ifdef __ANDROID__    
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Lucky number: %d\n", lucky_number);
    #else
        printf("Lucky number: %d\n", lucky_number);
    #endif

    printf("list directory:\n");
    obj1 = sexp_eval_string(ctx, "(import (chibi filesystem))", -1, NULL);
    sexp_debug(ctx, "obj1: ", obj1);
    check_exception(ctx, obj1);

    obj1 = sexp_eval_string(ctx, "(directory-files \"/\")", -1, NULL);
    sexp_debug(ctx, "obj1: ", obj1);
    check_exception(ctx, obj1);

    #ifdef __ANDROID__          
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "stdout: %s", buf_out);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "stderr: %s", buf_err);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "LOKI ----------\n");
    #endif
    sexp_eval_string(ctx, "(define (scm_loop) '()", -1, NULL);
}





void loki_main() {
    sexp_eval_string(ctx, "(scm_loop)", -1, NULL);
}



void loki_destroy() {
    sexp_destroy_context(ctx);
}



