#include "byteops.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int increment_bytes(unsigned char *bytes, int length)
{
	int increment_completed = 0;
	int byte_index = 0;
	while (!increment_completed && byte_index < length)
	{
		if (bytes[byte_index] == 255)
		{
			bytes[byte_index] = 0;
		}
		else
		{
			bytes[byte_index]++;
			increment_completed = 1;
		}
		byte_index++;
	}
	if (byte_index == length)
	{
		unsigned char *new_byte_array = calloc(length * 2, 1);
		memset(new_byte_array, 0, length * 2);
		memcpy(new_byte_array, bytes, length);

		free (bytes);
		bytes = new_byte_array;
		new_byte_array[length] = 1;

		return length * 2;
	}
	return length;
}

void print_bytes(byte *bytes, int byte_count)
{
	int byte_counter;
	printf("0x");
	for (byte_counter = 0; byte_counter < byte_count; byte_counter++)
	{
		printf("%02X", bytes[byte_counter]);
	}
}

void print_bits(byte *bytes, int bit_count)
{
	int byte_counter;
	int bit_counter;
	int byte_count = bit_count / 8;
	int extra_bits = bit_count % 8;
	if (extra_bits != 0)
	{
		byte_count += 1;
	}
	else
	{
		extra_bits = 8;
	}

	byte cur_byte;
	int num_bits;

	printf("0b");

	for (byte_counter = 0; byte_counter < byte_count; byte_counter++)
	{
		cur_byte = bytes[byte_counter];
		// if (byte_counter == byte_count - 1)
		// {
		// 	num_bits = extra_bits;
		// }
		// else
		// {
			num_bits = 8;
		// }
		for (bit_counter = 0; bit_counter < num_bits; bit_counter++)
		{
			printf("%d", cur_byte & 0x01);
			cur_byte = cur_byte >> 1;
		}
	}
}

void generate_random_bytes(unsigned char *bytes, int length)
{
	int byte_array_index;
	for (byte_array_index = 0; byte_array_index < length; byte_array_index++)
	{
		bytes[byte_array_index] = rand() % 256;
	}
}