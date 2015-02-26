#include <msgpack.h>
#include <stdio.h>

int main(void)
{
	msgpack_sbuffer* buffer = msgpack_sbuffer_new();
	msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

	msgpack_pack_array(pk, 2);
	msgpack_pack_raw(pk, 5);
	msgpack_pack_raw_body(pk, "hello", 5);
	msgpack_pack_raw(pk, 11);
	msgpack_pack_raw_body(pk, "messagepack", 11);

	msgpack_unpacked msg;
	msgpack_unpacked_init(&msg);
	
	char buf[32];
	memset(buf, 0, 32);
	strcpy(buf, "hello I'm memware client\n");
	
	bool success = msgpack_unpack_next(&msg, buf, sizeof(buf), NULL);

	printf("%d\n", success);

	msgpack_object obj = msg.data;
	msgpack_object_print(stdout, obj);
	printf("\n");

	msgpack_sbuffer_free(buffer);
	msgpack_packer_free(pk);

}
