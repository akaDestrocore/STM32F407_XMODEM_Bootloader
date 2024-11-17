#include <janpatch.h>

/**
 * Read a buffer off the stream
 */
static size_t jp_fread(janpatch_ctx *ctx, void *ptr, size_t size, size_t count, janpatch_buffer *buffer)
{
    ctx->fseek(buffer->stream, buffer->position, SEEK_SET);

    size_t bytes_read = ctx->fread(ptr, size, count, buffer->stream);

    buffer->position += bytes_read;

    return bytes_read;
}

/**
 * Write a buffer to the stream
 */
static size_t jp_fwrite(janpatch_ctx *ctx, const void *ptr, size_t size, size_t count, janpatch_buffer *buffer)
{
    ctx->fseek(buffer->stream, buffer->position, SEEK_SET);

    size_t bytes_written = ctx->fwrite(ptr, size, count, buffer->stream);

    buffer->position += bytes_written;

    return bytes_written;
}

/**
 * Set position of the stream
 */
static int jp_fseek(janpatch_buffer *buffer, long int offset, int origin)
{
    if (origin == SEEK_SET)
    {
        buffer->position = offset;
    }
    else if (origin == SEEK_CUR)
    {
        buffer->position += offset;
    }
    else
    {
        JANPATCH_ERROR("Origin %d not supported in jp_fseek (only SEEK_CUR,SEEK_SET)\n", origin);
        return -1;
    }
    return 0;
}


/**
 * Get a character from the stream
 */
static int jp_getc(janpatch_ctx* ctx, janpatch_buffer* buffer)
{
    long position = buffer->position;
    if (position < 0) return -1;

    // calculate the current page...
    uint32_t page = ((unsigned long)position) / buffer->size;

    if (page != buffer->current_page)
    {
        jp_fseek(buffer, page * buffer->size, SEEK_SET);
        buffer->current_page_size = jp_fread(ctx, buffer->buffer, 1, buffer->size, buffer);
        buffer->current_page = page;
    }

    int position_in_page = position % buffer->size;

    if ((size_t)position_in_page >= buffer->current_page_size)
    {
        return EOF;
    }

    unsigned char b = buffer->buffer[position_in_page];
    jp_fseek(buffer, position + 1, SEEK_SET);
    return b;
}

/**
 * Write a character to a stream
 */
static int jp_putc(int c, janpatch_ctx* ctx, janpatch_buffer* buffer)
{
    long position = buffer->position;
    if (position < 0) {
        return -1;
    }

    // calculate the current page...
    uint32_t page = ((unsigned long)position) / buffer->size;

    if (page != buffer->current_page)
    {
        // flush the page buffer...
        if (buffer->current_page != 0xFFFFFFFF)
        {
            jp_fseek(buffer, buffer->current_page * buffer->size, SEEK_SET);
            jp_fwrite(ctx, buffer->buffer, 1, buffer->current_page_size, buffer);

            if (ctx->progress)
            {
                ctx->progress(position * 100 / ctx->max_file_size);
            }
        }

        // and read the next page...
        jp_fseek(buffer, page * buffer->size, SEEK_SET);
        jp_fread(ctx, buffer->buffer, 1, buffer->size, buffer);
        buffer->current_page_size = buffer->size;
        buffer->current_page = page;
    }

    int position_in_page = position % buffer->size;

    buffer->buffer[position_in_page] = (unsigned char)c;
    jp_fseek(buffer, position + 1, SEEK_SET);

    return 0;
}

static void jp_final_flush(janpatch_ctx* ctx, janpatch_buffer* buffer)
{
    long position = buffer->position;
    int position_in_page = position % buffer->size;

    uint32_t page = ((unsigned long)position) / buffer->size;

    // if the page has changed we also need to flush the previous page
    // this can happen when the last operation (e.g. jp_putc) has just crossed page boundary
    if (page != buffer->current_page) {
        // flush the page buffer...
        if (buffer->current_page != 0xFFFFFFFF) {
            jp_fseek(buffer, buffer->current_page * buffer->size, SEEK_SET);
            jp_fwrite(ctx, buffer->buffer, 1, buffer->current_page_size, buffer);
        }
    }

    // flush the new page buffer
    jp_fseek(buffer, page * buffer->size, SEEK_SET);
    jp_fwrite(ctx, buffer->buffer, 1, position_in_page, buffer);

    if (ctx->progress) {
        ctx->progress(100);
    }
}

static void process_mod(janpatch_ctx *ctx, janpatch_buffer *source, janpatch_buffer *patch, janpatch_buffer *target, bool up_source_stream) {
    // it can be that ESC character is actually in the data, but then it's prefixed with another ESC
    // so... we're looking for a lone ESC character
    size_t cnt = 0;
    while (1) {
        cnt++;
        int m = jp_getc(ctx, patch);
        if (m == -1) {
            // End of file stream... rewind 1 character and return, this will yield back to janpatch main function, which will exit
            jp_fseek(source, -1, SEEK_CUR);
            return;
        }
        // JANPATCH_DEBUG("%02x ", m);
        // so... if it's *NOT* an ESC character, just write it to the target stream
        if (m != JANPATCH_OPERATION_ESC)
        {
            // JANPATCH_DEBUG("NOT ESC\n");
            jp_putc(m, ctx, target);
            if (up_source_stream)
            {
                jp_fseek(source, 1, SEEK_CUR); // and up source
            }
            continue;
        }

        // read the next character to see what we should do
        m = jp_getc(ctx, patch);
        // JANPATCH_DEBUG("%02x ", m);

        if (m == -1) {
            // End of file stream... rewind 1 character and return, this will yield back to janpatch main function, which will exit
            jp_fseek(source, -1, SEEK_CUR);
            return;
        }

        // if the character after this is *not* an operator (except ESC)
        if (m == JANPATCH_OPERATION_ESC)
        {
            // JANPATCH_DEBUG("ESC, NEXT CHAR ALSO ESC\n");
            jp_putc(m, ctx, target);
            if (up_source_stream) {
                jp_fseek(source, 1, SEEK_CUR);
            }
        }
        else if (m >= 0xA2 && m <= 0xA6) { // character after this is an operator? Then roll back two characters and exit
            // JANPATCH_DEBUG("ESC, THEN OPERATOR\n");
            JANPATCH_DEBUG("%lu bytes\n", cnt);
            jp_fseek(patch, -2, SEEK_CUR);
            break;
        }
        else { // else... write both the ESC and m
            // JANPATCH_DEBUG("ESC, BUT NO OPERATOR\n");
            jp_putc(JANPATCH_OPERATION_ESC, ctx, target);
            jp_putc(m, ctx, target);
            if (up_source_stream) {
                jp_fseek(source, 2, SEEK_CUR); // up source by 2
            }
        }
    }
}

static int find_length(janpatch_ctx *ctx, janpatch_buffer *buffer)
{
    /* So... the EQL/BKT length thing works like this:
    *
    * If byte[0] is between 1..251 => use byte[0] + 1
    * If byte[0] is 252 => use ???
    * If byte[0] is 253 => use (byte[1] << 8) + byte[2]
    * If byte[0] is 254 => use (byte[1] << 16) + (byte[2] << 8) + byte[3] (NOT VERIFIED)
    */
    uint8_t l = jp_getc(ctx, buffer);
    if (l <= 251)
    {
        return l + 1;
    }
    else if (l == 252)
    {
        return l + jp_getc(ctx, buffer) + 1;
    }
    else if (l == 253)
    {
        return (jp_getc(ctx, buffer) << 8) + jp_getc(ctx, buffer);
    }
    else if (l == 254)
    {
        return (jp_getc(ctx, buffer) << 24) + (jp_getc(ctx, buffer) << 16) + (jp_getc(ctx, buffer) << 8) + (jp_getc(ctx, buffer));
    }
    else
    {
        JANPATCH_ERROR("EQL followed by unexpected byte %02x %02x\n", JANPATCH_OPERATION_EQL, l);
        return -1;
    }

    // it's fine if we get over the end of the stream here, will be caught by the next function
}

int janpatch(janpatch_ctx ctx, JANPATCH_STREAM *source, JANPATCH_STREAM *patch, JANPATCH_STREAM *target)
{
	char posinfo[80];

    ctx.source_buffer.current_page = 0xffffffff;
    ctx.patch_buffer.current_page = 0xffffffff;
    ctx.target_buffer.current_page = 0xffffffff;

    ctx.source_buffer.position = 0;
    ctx.patch_buffer.position = 0;
    ctx.target_buffer.position = 0;

    ctx.source_buffer.stream = source;
    ctx.patch_buffer.stream = patch;
    ctx.target_buffer.stream = target;

    // look at the size of the source file...
    if (ctx.progress != NULL && ctx.ftell != NULL)
    {
        ctx.fseek(source, 0, SEEK_END);
        ctx.max_file_size = ctx.ftell(source);
        char src_size[50];
		sprintf(src_size,"Source file size is %ld\r\n", ctx.max_file_size);
		USART_SendString((uint8_t*)src_size);
        ctx.fseek(source, 0, SEEK_SET);

        // and at the size of the patch file
        ctx.fseek(patch, 0, SEEK_END);
        ctx.max_file_size += ctx.ftell(patch);
        char ptch_size[50];
		sprintf(ptch_size,"Now max file size is %ld\r\n", ctx.max_file_size);
		USART_SendString((uint8_t*)ptch_size);
        ctx.fseek(patch, 0, SEEK_SET);
    }
    else
    {
        ctx.progress = NULL;
    }

    int c;
    while ((c = jp_getc(&ctx, &ctx.patch_buffer)) != EOF)
    {
        if (c == JANPATCH_OPERATION_ESC)
        {
            c = jp_getc(&ctx, &ctx.patch_buffer);
        }
        else if (c != -1)
        {
            // Rewind 1 character, for the one we just consummed, and set a default operation of MOD
            jp_fseek(&ctx.patch_buffer, -1, SEEK_CUR);
            c = JANPATCH_OPERATION_MOD;
        }
        switch (c)
        {
            case JANPATCH_OPERATION_EQL:
            {
                int length = find_length(&ctx, &ctx.patch_buffer);
                if (length == -1)
                {
                	USART_SendString((uint8_t*)"EQL length invalid\r\n");
					sprintf(posinfo,"Positions are, source=%ld patch=%ld new=%ld\r\n", ctx.source_buffer.position, ctx.patch_buffer.position, ctx.target_buffer.position);
					USART_SendString((uint8_t*)posinfo);
					return 1;
                }

                char eql_len[50];
				sprintf(eql_len,"EQL: %d bytes\r\n", length);
				USART_SendString((uint8_t*)eql_len);

                for (int ix = 0; ix < length; ix++)
                {
                    int r = jp_getc(&ctx, &ctx.source_buffer);
                    if (r < -1)
                    {
                    	char ret_fread[80];
						sprintf(ret_fread,"fread returned %d, but expected character\r\n", r);
						USART_SendString((uint8_t*)ret_fread);
						char posinfo[80] = "\0";
						sprintf(posinfo,"Positions are, source=%ld patch=%ld new=%ld\r\n", ctx.source_buffer.position, ctx.patch_buffer.position, ctx.target_buffer.position);
						USART_SendString((uint8_t*)posinfo);
                        return 1;
                    }

                    jp_putc(r, &ctx, &ctx.target_buffer);
                }

                break;
            }
            case JANPATCH_OPERATION_MOD:
            {
                JANPATCH_DEBUG("MOD: ");

                // MOD means to modify the next series of bytes
                // so just write everything (until the next ESC sequence) to the target JANPATCH_STREAM
                // but also up the position in the source JANPATCH_STREAM every time
                process_mod(&ctx, &ctx.source_buffer, &ctx.patch_buffer, &ctx.target_buffer, true);
                break;
            }
            case JANPATCH_OPERATION_INS:
            {
                JANPATCH_DEBUG("INS: ");
                // INS inserts the sequence in the new JANPATCH_STREAM, but does not up the position of the source JANPATCH_STREAM
                // so just write everything (until the next ESC sequence) to the target JANPATCH_STREAM

                process_mod(&ctx, &ctx.source_buffer, &ctx.patch_buffer, &ctx.target_buffer, false);
                break;
            }
            case JANPATCH_OPERATION_BKT:
            {
                // BKT = backtrace, seek back in source JANPATCH_STREAM with X bytes...
                int length = find_length(&ctx, &ctx.patch_buffer);
                if (length == -1)
                {
					USART_SendString((uint8_t*)"BKT length invalid\r\n");
					char posinfo[80] = "\0";
					sprintf(posinfo,"Positions are, source=%ld patch=%ld new=%ld\r\n", ctx.source_buffer.position, ctx.patch_buffer.position, ctx.target_buffer.position);
					USART_SendString((uint8_t*)posinfo);
					return 1;
                }

                char btk[60];
                sprintf(btk,"BKT: %d bytes\r\n", -length);
				USART_SendString((uint8_t*)btk);
                JANPATCH_DEBUG("BKT: %d bytes\r\n", -length);

                jp_fseek(&ctx.source_buffer, -length, SEEK_CUR);

                break;
            }
            case JANPATCH_OPERATION_DEL:
            {
                // DEL deletes bytes, so up the source stream with X bytes
                int length = find_length(&ctx, &ctx.patch_buffer);
                if (length == -1)
                {
                	USART_SendString((uint8_t*)"DEL length invalid\r\n");
                	char posinfo[80] = "\0";
					sprintf(posinfo,"Positions are, source=%ld patch=%ld new=%ld\r\n", ctx.source_buffer.position, ctx.patch_buffer.position, ctx.target_buffer.position);
					USART_SendString((uint8_t*)posinfo);
					return 1;
                }

                char del[50];
                sprintf(del,"DEL: %d bytes\r\n", length);
				USART_SendString((uint8_t*)del);

                jp_fseek(&ctx.source_buffer, length, SEEK_CUR);
                break;
            }
            case -1: {
                // End of file stream... rewind 1 character and break, this will yield back to main loop
                jp_fseek(&ctx.source_buffer, -1, SEEK_CUR);
                break;
            }
            default:
            {
            	char c_op[50];
				sprintf(c_op,"Unsupported operator %02x\r\n", c);
				USART_SendString((uint8_t*)c_op);
				char posinfo[80] = "\0";
				sprintf(posinfo,"Positions are, source=%ld patch=%ld new=%ld\r\n", ctx.source_buffer.position, ctx.patch_buffer.position, ctx.target_buffer.position);
				USART_SendString((uint8_t*)posinfo);
				return 1;
            }
        }
    }

    jp_final_flush(&ctx, &ctx.target_buffer);

    return 0;
}
