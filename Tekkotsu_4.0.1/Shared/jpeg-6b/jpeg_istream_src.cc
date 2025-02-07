#include "jpeg_istream_src.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Theoretically, could be used to load several consecutive
 *  JPEGs from memory, but not really tested, some tweaks
 *  would probably be required */
	
//#include <jpeglib.h> 
#include <jerror.h> 

typedef struct {
	struct jpeg_source_mgr  pub;
	std::istream * is;
	char c;
} JPEGStreamSource;

static void init_sourceFunc(j_decompress_ptr cinfo) {
	JPEGStreamSource  *src = (JPEGStreamSource*)cinfo->src;
	src->is->get(src->c);
	src->pub.next_input_byte = (JOCTET*)&src->c;
	src->pub.bytes_in_buffer = sizeof(char);
}
static boolean fill_input_bufferFunc(j_decompress_ptr cinfo) {
	JPEGStreamSource  *src = (JPEGStreamSource*)cinfo->src;
	if(!src->is->get(src->c))
		return false;
	src->pub.next_input_byte = (JOCTET*)&src->c;
	src->pub.bytes_in_buffer = 1;
	return true;
}
static void skip_input_dataFunc(j_decompress_ptr cinfo, long num_bytes) {
	JPEGStreamSource  *src = (JPEGStreamSource*)cinfo->src;
	if (num_bytes > 0) {
		if ((size_t)num_bytes > src->pub.bytes_in_buffer) {
			num_bytes -= src->pub.bytes_in_buffer;
			src->pub.bytes_in_buffer = 0;
			src->is->ignore(num_bytes);
		} else {
			src->pub.bytes_in_buffer -= num_bytes;
		}
	}
}
static void term_sourceFunc(j_decompress_ptr) {}

GLOBAL(void) 
jpeg_istream_src (j_decompress_ptr cinfo, std::istream& inStream) {
	JPEGStreamSource *src; 
	
	if (cinfo->src == NULL) /* first time for this JPEG object? */ 
		cinfo->src = (struct jpeg_source_mgr *) (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(JPEGStreamSource)); 
	
	src = (JPEGStreamSource*) cinfo->src;
	src->pub.init_source = init_sourceFunc; 
	src->pub.fill_input_buffer = fill_input_bufferFunc; 
	src->pub.skip_input_data = skip_input_dataFunc; 
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */ 
	src->pub.term_source = term_sourceFunc; 
	src->is = &inStream;
	src->pub.next_input_byte = (JOCTET*)&src->c;
	src->pub.bytes_in_buffer = 0;
} 

#ifdef __cplusplus
}
#endif
