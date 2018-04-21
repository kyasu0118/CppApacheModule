#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include <apr_hash.h>
#include <apr_strings.h>
#include "GazoShori.hpp"

class MultipartFormData
{
public:
    std::string                boundary;
    std::vector< std::string > content;
    std::vector< uint8_t >     binary_data;
    
    MultipartFormData( const std::vector< uint8_t >& data  )
    {
        std::vector< uint8_t > buffer = data;
        size_t begin = 0;
        size_t i = 0;
        
        for( i=0; i<data.size(); ++i )
        {
            if( buffer[i] == '\r' && buffer[i+1] == '\n' )
            {
                buffer[i] = '\0';
                boundary = (const char*)&buffer[ begin ];
                i = i+2;
                begin = i;
                break;
            }
        }
        
        for( ; i<data.size(); ++i )
        {
            if( buffer[i] == '\r' && buffer[i+1] == '\n' )
            {
                buffer[i] = '\0';
                content.emplace_back( (const char*)&buffer[ begin ] );
                i += 2;
                begin = i;
                
                // next binary data.
                if( content.back().find("Content-Type:") != std::string::npos )
                {
                    begin += 2;
                    break;
                }
            }
        }
        
        binary_data.insert(
            binary_data.end(), (uint8_t*)&buffer[ begin ], (uint8_t*)&buffer[ buffer.size() - boundary.size() - 6] );
    }
};

static int gazo_shori_handler(request_rec *r)
{
    if( strcmp(r->handler, "gazo_shori") )
    {
        return DECLINED;
    }
    if( r->method_number != M_POST )
    {
        return HTTP_METHOD_NOT_ALLOWED;
    }

    int seen_eos = 0;
    apr_bucket_brigade* bb = apr_brigade_create( r->pool, r->connection->bucket_alloc );

    std::vector< uint8_t > post;
    
    do
    {
        apr_bucket* bucket = nullptr;
        apr_status_t rv = ap_get_brigade(r->input_filters, bb, AP_MODE_READBYTES, APR_BLOCK_READ, HUGE_STRING_LEN);
        for (bucket = APR_BRIGADE_FIRST(bb); bucket != APR_BRIGADE_SENTINEL(bb); bucket = APR_BUCKET_NEXT(bucket))
        {
            if (APR_BUCKET_IS_EOS(bucket))
            {
                seen_eos = 1;
                break;
            }
            if (bucket->length != 0)
            {
                const char* p_data = nullptr;
                apr_size_t length = 0;
                rv = apr_bucket_read( bucket, &p_data, &length, APR_BLOCK_READ );

                const char *dup_data = apr_pstrmemdup(r->pool, p_data, length);
                post.insert(post.end(), dup_data, dup_data + length);
                apr_bucket_delete(bucket);
            }
        }
        apr_brigade_cleanup( bb );
    } while ( seen_eos == 0 );

    r->content_type = "image/bmp";

    if ( !r->header_only )
    {
        gs::ImageRGB image;
        MultipartFormData form( post );
        
        image.read( form.binary_data );
        image = image.gaussian( 10 );

        std::stringstream bitmap;
        image.write( bitmap );
        ap_rwrite( bitmap.str().data(), bitmap.str().size(), r );
    }
    return OK;
}

static void gazo_shori_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(gazo_shori_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

extern "C"
{
/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA gazo_shori_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    gazo_shori_register_hooks  /* register hooks                      */
};
}
