/*
 * ngx_service.c
 *
 *  Created on: Oct 27, 2015
 *      Author: ygao@sse.com.cn
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_service.h"

#define PB_SIZE 1024
#define CONTENT_TYPE "application/json;charset=GB2312"
#define SERVER_NAME	 "mqws/1.0"

static char* ngx_http_mqws(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t  ngx_http_mqws_commands[] = {
    { ngx_string("mqws"),
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
      ngx_http_mqws,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },
      ngx_null_command
};

static ngx_http_module_t  ngx_http_mqws_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */
    NULL, /* create main configuration */
    NULL, /* init main configuration */
    NULL, /* create server configuration */
    NULL, /* merge server configuration */
    NULL, /* create location configuration */
    NULL /* merge location configuration */
};

ngx_module_t  ngx_http_mqws_module = {
    NGX_MODULE_V1,
    &ngx_http_mqws_module_ctx, /* module context */
    ngx_http_mqws_commands,   /* module directives */
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
ngx_http_mqws_handler(ngx_http_request_t *r)
{
    ngx_int_t     rc;
    ngx_buf_t    *b;
    ngx_chain_t   out;

    static char uri[PB_SIZE];
    static char decode[PB_SIZE];
    static char args[PB_SIZE];

    char* src;
    char* dst;
    int status=NGX_HTTP_OK;
    int reply_len=0;
    char* reply=0;

    if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    rc = ngx_http_discard_request_body(r);

    if (rc != NGX_OK && rc != NGX_AGAIN) {
        return rc;
    }
    //uri
    if (r->uri.len>=PB_SIZE)
    	return NGX_HTTP_NOT_ALLOWED;
    ngx_memcpy(uri,r->uri.data,r->uri.len);
    uri[r->uri.len]=0;
    //decode uri
    src = uri;
    dst = decode;
    ngx_unescape_uri((u_char**)&dst, (u_char**)&src, r->uri.len, 0);
    ngx_memcpy(uri,decode,dst - decode);
    uri[dst - decode] = '\0';
    //args
    if (r->args.len>=PB_SIZE)
    	return NGX_HTTP_NOT_ALLOWED;
    ngx_memcpy(args,r->args.data,r->args.len);
    args[r->args.len]=0;
    //decode args
    src = args;
    dst =decode;
    ngx_unescape_uri((u_char**)&dst, (u_char**)&src, r->args.len, 0);
    ngx_memcpy(args,decode,dst - decode);
    args[dst - decode] = '\0';

    reply=request(uri,args,&status,&reply_len);

    if (status!=NGX_HTTP_OK)
    {
    	return status;
    }
    ngx_str_t type=ngx_string(CONTENT_TYPE);
    //ngx_str_t server=ngx_string(SERVER_NAME);
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_type=type;
    //r->headers_out.server=server;

    r->headers_out.content_length_n = reply_len;

    b = ngx_create_temp_buf(r->pool, reply_len);
    if (b == NULL) {
    	ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate response buffer.");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }


    ngx_memcpy(b->pos, reply, reply_len);

    b->last=b->pos+reply_len;
    b->last_buf=1;

    out.buf = b;
    out.next = NULL;


    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    return ngx_http_output_filter(r, &out);
}

static char *
ngx_http_mqws(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t  *clcf;
    //ngx_http_circle_gif_loc_conf_t *cglcf = conf;

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_mqws_handler;
    //cglcf->enable = 1;

    return NGX_CONF_OK;
}
