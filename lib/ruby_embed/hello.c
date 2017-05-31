/**
 * gcc -o hello hello.c -I/disk4/local/ruby/include/ruby-2.3.0/ -I/disk4/local/ruby/include/ruby-2.3.0/x86_64-linux -L/disk4/local/ruby/lib -lruby
 */

#include "ruby.h"

int main(void)
{
	ruby_init();
	ruby_init_loadpath();
	ruby_script("embedded");
	rb_eval_string("puts 'Hello, world!'");
	ruby_cleanup(0);
	return 0;
}
