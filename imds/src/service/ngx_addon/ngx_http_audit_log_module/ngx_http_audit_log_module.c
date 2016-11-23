
/*
 *  * Copyright (C) Igor Sysoev
 *   * Copyright (C) Nginx, Inc.
 *    */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_service_audit.h"

static ngx_int_t ngx_http_audit_log_config(ngx_conf_t *cf);
static ngx_int_t ngx_http_brokerNo_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_infoNo_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_clientId_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_userName_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_YYYY_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_MM_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_DD_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_HH_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_min_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ss_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_ms_config(ngx_http_request_t *r,ngx_http_variable_value_t *v, uintptr_t data);

static ngx_http_module_t  ngx_http_audit_log_module_ctx = {
    NULL,														        /* preconfiguration */
    ngx_http_audit_log_config,          	/* postconfiguration */
    NULL,                                  	/* create main configuration */
    NULL,                                  	/* init main configuration */
    NULL,                                  	/* create server configuration */
    NULL,                                  	/* merge server configuration */
    NULL,       														/* create location configuration */
    NULL         														/* merge location configuration */
};


ngx_module_t  ngx_http_audit_log_module = {
    NGX_MODULE_V1,
    &ngx_http_audit_log_module_ctx,           	 /* module context */
    NULL										,              /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_http_variable_t  ngx_http_audit_vars[] = {
    { ngx_string("brokerNo"), NULL,
      ngx_http_brokerNo_config, 0, 0, 0 },
    { ngx_string("infoNo"), NULL,
      ngx_http_infoNo_config, 0, 0, 0 },
    { ngx_string("clientId"), NULL,
      ngx_http_clientId_config, 0, 0, 0 },
    { ngx_string("userName"), NULL,
      ngx_http_userName_config, 0, 0, 0 },
    { ngx_string("YYYY"), NULL,
    	ngx_http_YYYY_config, 0, 0, 0},
    { ngx_string("MM"), NULL,
    	ngx_http_MM_config, 0, 0, 0},
    { ngx_string("DD"), NULL,
    	ngx_http_DD_config, 0, 0, 0},
    { ngx_string("HH"), NULL,
    	ngx_http_HH_config, 0, 0, 0},
   	{ ngx_string("min"), NULL,
    	ngx_http_min_config, 0, 0, 0},	
    { ngx_string("ss"), NULL,
    	ngx_http_ss_config, 0, 0, 0},	
    { ngx_string("ms"), NULL,
    	ngx_http_ms_config, 0, 0, 0},
    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};

static ngx_int_t
ngx_http_audit_log_config(ngx_conf_t *cf)
{
    ngx_http_variable_t  *var, *v;

    for (v = ngx_http_audit_vars; v->name.len; v++) {
        var = ngx_http_add_variable(cf, &v->name, v->flags);
        if (var == NULL) {
            return NGX_ERROR;
        }

        var->get_handler = v->get_handler;
        var->data = v->data;
    }

    return NGX_OK;
}

static ngx_int_t
ngx_http_brokerNo_config(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{  
		int ret = -1;
		u_char encode[ENCODELEN] = {0};
		ngx_table_elt_t **cookies;
		u_char *brokerNo;
		
		brokerNo = ngx_pnalloc(r->pool, BROKERNOLEN);
		if(brokerNo == NULL) {
				return NGX_ERROR;
		}
		
		if(r->headers_in.cookies.nelts){
				cookies = r->headers_in.cookies.elts;
				ngx_snprintf(encode,sizeof(encode),"%V",&cookies[0]->value);
		}
		
		ret = generateAuditElement(r->headers_in.cookies.nelts, (char *)encode+6, (char *)r->args.data, 1,(char *)brokerNo);
		if(ret){
			//ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "brokerNo = [%s][%d]", brokerNo,ngx_strlen(brokerNo));
			
			v->len = ngx_strlen(brokerNo);
			v->valid = 1;
      v->no_cacheable = 0;  
      v->not_found = 0; 
      v->data = brokerNo;
		}else{
			v->not_found = 1;
		}
		
		return  NGX_OK; 
} 


static ngx_int_t
ngx_http_infoNo_config(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{  
		int ret = -1;
		u_char encode[ENCODELEN] = {0};
		ngx_table_elt_t **cookies;
		u_char *infoNo;
		
		infoNo = ngx_pnalloc(r->pool, INFONOLEN);
		if(infoNo == NULL) {
				return NGX_ERROR;
		}
		
		if(r->headers_in.cookies.nelts){
				cookies = r->headers_in.cookies.elts;
				ngx_snprintf(encode,sizeof(encode),"%V",&cookies[0]->value);
		}
		
		ret = generateAuditElement(r->headers_in.cookies.nelts, (char *)encode+6, (char *)r->args.data, 2,(char *)infoNo);
		if(ret){
			//ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "infoNo = [%s][%d]", infoNo,ngx_strlen(infoNo));
			
			v->len = ngx_strlen(infoNo);
			v->valid = 1;
      v->no_cacheable = 0;  
      v->not_found = 0; 
      v->data = infoNo;
		}else{
			v->not_found = 1;
		}
		
		return  NGX_OK; 
} 

static ngx_int_t
ngx_http_clientId_config(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{  
		int ret = -1;
		u_char encode[ENCODELEN] = {0};
		ngx_table_elt_t **cookies;
		u_char *clientId;
		
		clientId = ngx_pnalloc(r->pool, CLINETIDLEN);
		if(clientId == NULL) {
				return NGX_ERROR;
		}
		
		if(r->headers_in.cookies.nelts){
				cookies = r->headers_in.cookies.elts;
				ngx_snprintf(encode,sizeof(encode),"%V",&cookies[0]->value);
		}
		
		ret = generateAuditElement(r->headers_in.cookies.nelts, (char *)encode+6, (char *)r->args.data, 3,(char *)clientId);
		if(ret){
			//ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "clientId = [%s][%d]", clientId,ngx_strlen(clientId));
			
			v->len = ngx_strlen(clientId);
			v->valid = 1;
      v->no_cacheable = 0;  
      v->not_found = 0; 
      v->data = clientId;
		}else{
			v->not_found = 1;
		}
		
		return  NGX_OK; 
} 

static ngx_int_t
ngx_http_userName_config(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data)
{  
		int ret = -1;
		u_char encode[ENCODELEN] = {0};
		ngx_table_elt_t **cookies;
		u_char *userName;
		
		userName = ngx_pnalloc(r->pool, USERNAMELEN);
		if(userName == NULL) {
				return NGX_ERROR;
		}
		
		if(r->headers_in.cookies.nelts){
				cookies = r->headers_in.cookies.elts;
				ngx_snprintf(encode,sizeof(encode),"%V",&cookies[0]->value);
		}
		
		ret = generateAuditElement(r->headers_in.cookies.nelts, (char *)encode+6, (char *)r->args.data, 4,(char *)userName);
		if(ret){
			//ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "userName = [%s][%d]", userName,ngx_strlen(userName));
			
			v->len = ngx_strlen(userName);
			v->valid = 1;
      v->no_cacheable = 0;  
      v->not_found = 0; 
      v->data = userName;
		}else{
			v->not_found = 1;
		}
		
		return  NGX_OK; 
}

static ngx_int_t
ngx_http_YYYY_config(ngx_http_request_t *r,
ngx_http_variable_value_t *v, uintptr_t data)
{
		/*1970-09-28T12:00:00+06:00*/
    u_char  *p;

    p = ngx_pnalloc(r->pool, 5);
    if (p == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(p, ngx_cached_http_log_iso8601.data,4);

    v->len = 4;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}

static ngx_int_t
ngx_http_MM_config(ngx_http_request_t *r,
ngx_http_variable_value_t *v, uintptr_t data)
{
		/*1970-09-28T12:00:00+06:00*/
    u_char  *p;

    p = ngx_pnalloc(r->pool, 3);
    if (p == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(p, ngx_cached_http_log_iso8601.data+5,2);

    v->len = 2;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}

static ngx_int_t
ngx_http_DD_config(ngx_http_request_t *r,
ngx_http_variable_value_t *v, uintptr_t data)
{
		/*1970-09-28T12:00:00+06:00*/
    u_char  *p;

    p = ngx_pnalloc(r->pool, 3);
    if (p == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(p, ngx_cached_http_log_iso8601.data+8,2);

    v->len = 2;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}

static ngx_int_t
ngx_http_HH_config(ngx_http_request_t *r,
ngx_http_variable_value_t *v, uintptr_t data)
{
		/*1970-09-28T12:00:00+06:00*/
    u_char  *p;

    p = ngx_pnalloc(r->pool, 3);
    if (p == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(p, ngx_cached_http_log_iso8601.data+11,2);

    v->len = 2;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}

static ngx_int_t
ngx_http_min_config(ngx_http_request_t *r,
ngx_http_variable_value_t *v, uintptr_t data)
{
		/*1970-09-28T12:00:00+06:00*/
    u_char  *p;

    p = ngx_pnalloc(r->pool, 3);
    if (p == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(p, ngx_cached_http_log_iso8601.data+14,2);

    v->len = 2;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}

static ngx_int_t
ngx_http_ss_config(ngx_http_request_t *r,
ngx_http_variable_value_t *v, uintptr_t data)
{
		/*1970-09-28T12:00:00+06:00*/
    u_char  *p;

    p = ngx_pnalloc(r->pool, 3);
    if (p == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(p, ngx_cached_http_log_iso8601.data+17,2);

    v->len = 2;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}

static ngx_int_t
ngx_http_ms_config(ngx_http_request_t *r,
ngx_http_variable_value_t *v, uintptr_t data)
{
    u_char      *p;
    ngx_time_t  *tp;

    p = ngx_pnalloc(r->pool, 4);
    if (p == NULL) {
        return NGX_ERROR;
    }

    tp = ngx_timeofday();

    v->len = ngx_sprintf(p, "%03M",tp->msec) - p;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}
