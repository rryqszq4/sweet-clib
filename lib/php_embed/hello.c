/**
 *
 * gcc -o hello hello.c `/usr/local/php/bin/php-config --includes` -I/usr/local/php/include/php/sapi/embed `/usr/local/php/bin/php-config --ldflags` -L/usr/local/php/lib -lphp5 `/usr/local/php/bin/php-config --libs` -liconv
 *
 */

#include <stdio.h>
#include <php_embed.h>

int main(int argc, char *argv[])
{
	zend_file_handle file_handle;
	
	file_handle.type = ZEND_HANDLE_FP;
	file_handle.opened_path = NULL;
	file_handle.free_filename = 0;
	file_handle.filename = "test.php";
	
	file_handle.handle.fp = fopen(file_handle.filename, "rb");

	PHP_EMBED_START_BLOCK(argc, argv);
		php_execute_script(&file_handle TSRMLS_CC);
	PHP_EMBED_END_BLOCK();

	return 0;
}
