#include <msgpack.h>
#include <stdio.h>

int main(void)
{
	msgpack_sbuffer* buffer = msgpack_sbuffer_new();
	msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

	int j;

	for (j = 0; j < 23; j++){
		msgpack_sbuffer_clear(buffer);

		msgpack_pack_array(pk, 3);
		msgpack_pack_raw(pk, 5);
		msgpack_pack_raw_body(pk, "hello", 5);
		msgpack_pack_raw(pk, 11);
		msgpack_pack_raw_body(pk, "messagepack", 11);
		msgpack_pack_int(pk, j);

		msgpack_unpacked msg;
		msgpack_unpacked_init(&msg);
		bool success = msgpack_unpack_next(&msg, buffer->data, buffer->size, NULL);

		msgpack_object obj = msg.data;
		msgpack_object_print(stdout, obj);
		puts("");
	}

	msgpack_sbuffer_free(buffer);
	msgpack_packer_free(pk);
}
