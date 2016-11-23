/*
 * ngx_http_add_auth_module.c
 *
 *  Created on: 2016年11月7日
 *      Author: mds
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <string.h>
#include "ngx_auth.h"

typedef struct
{
	ngx_int_t auth_check;
}ngx_http_auth_loc_conf_t;

static ngx_int_t ngx_http_auth_init(ngx_conf_t *cf);

static ngx_int_t ngx_http_auth_handler(ngx_http_request_t *r);

static void *ngx_http_auth_create_loc_conf(ngx_conf_t *cf);

static char *ngx_http_auth_check(ngx_conf_t *cf, ngx_command_t *cmd,void *conf);

static ngx_command_t ngx_http_auth_commands[] = {
        {
                ngx_string("auth_check"),
                NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
                ngx_http_auth_check,
                NGX_HTTP_LOC_CONF_OFFSET,
                0,
                NULL },

        ngx_null_command
};


static ngx_http_module_t ngx_http_auth_module_ctx = {
        NULL,                          /* preconfiguration */
        ngx_http_auth_init,           /* postconfiguration */

        NULL,                          /* create main configuration */
        NULL,                          /* init main configuration */

        NULL,                          /* create server configuration */
        NULL,                          /* merge server configuration */

        ngx_http_auth_create_loc_conf, /* create location configuration */
        NULL                            /* merge location configuration */
};


ngx_module_t ngx_http_add_auth_module = {
        NGX_MODULE_V1,
        &ngx_http_auth_module_ctx,    /* module context */
        ngx_http_auth_commands,       /* module directives */
        NGX_HTTP_MODULE,               /* module type */
        NULL,                          /* init master */
        NULL,                          /* init module */
        NULL,                          /* init process */
        NULL,                          /* init thread */
        NULL,                          /* exit thread */
        NULL,                          /* exit process */
        NULL,                          /* exit master */
        NGX_MODULE_V1_PADDING
};


static ngx_int_t
ngx_http_auth_handler(ngx_http_request_t *r)
{
		u_char encode[ENCODELEN] = {0};
		char sessionID[SESSIONLEN+1] = {0};
		
		char *rsp_cookie;
		ngx_table_elt_t **cookies;
		ngx_table_elt_t *set_cookie;
  	ngx_http_auth_loc_conf_t* my_conf;
  	
  	ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "ngx_http_add_auth_module is called!");
  	
  	rsp_cookie = ngx_pnalloc(r->pool, SESSIONLEN+16);
  	if(rsp_cookie == NULL){
  		return NGX_ERROR;
  	}
  	
		my_conf = ngx_http_get_module_loc_conf(r, ngx_http_add_auth_module);
		/* on-off */
		if(my_conf->auth_check == 1){
			if(r->headers_in.cookies.nelts) {
				cookies = r->headers_in.cookies.elts;
				ngx_snprintf(encode,sizeof(encode),"%V", &cookies[0]->value);
			}
			if(!authCheck(r->headers_in.cookies.nelts, (char *)r->request_line.data, (char *)encode+6, sessionID,(char *)r->args.data))
					return NGX_HTTP_FORBIDDEN;
			if(r->headers_in.cookies.nelts){
				sprintf(rsp_cookie, "sessionID=%s",sessionID);
				set_cookie = ngx_list_push(&r->headers_out.headers);
				if (set_cookie == NULL) {
					return NGX_ERROR;
    		}
    		
    		set_cookie->hash = 1;
    		set_cookie->key.len = sizeof("Set-Cookie") - 1;
    		set_cookie->key.data = (u_char *) "Set-Cookie";
    		set_cookie->value.len = ngx_strlen(rsp_cookie);
    		set_cookie->value.data = (u_char *)rsp_cookie;
      	
				return NGX_OK;
			}else{
				return NGX_OK;
			}
		}else{
			return NGX_OK;
		}
	
}

static char *ngx_http_auth_check(ngx_conf_t *cf, ngx_command_t *cmd,
        void *conf)
{
        ngx_http_auth_loc_conf_t* local_conf;

        local_conf = conf;

        char* rv = NULL;

        rv = ngx_conf_set_flag_slot(cf, cmd, conf);


        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "auth_check:%d", local_conf->auth_check);
        return rv;
}

static void *ngx_http_auth_create_loc_conf(ngx_conf_t *cf)
{
        ngx_http_auth_loc_conf_t* local_conf = NULL;
        local_conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_auth_loc_conf_t));
        if (local_conf == NULL)
        {
                return NULL;
        }

        local_conf->auth_check = NGX_CONF_UNSET;

        return local_conf;
}
static ngx_int_t
ngx_http_auth_init(ngx_conf_t *cf)
{
        ngx_http_handler_pt        *h;
        ngx_http_core_main_conf_t  *cmcf;

        cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

        h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
        if (h == NULL) {
                return NGX_ERROR;
        }

        *h = ngx_http_auth_handler;

        return NGX_OK;
}
