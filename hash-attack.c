#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "byteops.h"

typedef struct
{
	unsigned char **bytes;
	int capacity;
	int length;
} byte_set;

void sha1(unsigned char *input, unsigned char *output, int input_length, int output_length);
int collision_attack(int digest_length);
int preimage_attack(unsigned char *digest, int digest_length);
int byte_set_insert(unsigned char *bytes, int byte_count, byte_set *set);

const int NUM_TESTS = 50;
const int INPUT_LENGTH = 80;

int main(int argc, char **argv)
{
	int digest_length;
	int test_counter;
	srand(time(0));

	for (digest_length = 8; digest_length <= 24; digest_length += 3)
	{
		int total_attempts = 0;
		for (test_counter = 0; test_counter < NUM_TESTS; test_counter++)
		{
			total_attempts += collision_attack(digest_length);
		}

		printf("\nAVG ATTEMPTS FOR DIGEST LENGTH %d: %f\n", digest_length, total_attempts / (float) NUM_TESTS);
		printf("ANTICIPATED AVG: %f\n\n", pow(2, digest_length / 2));
	}

	unsigned char *input = calloc(INPUT_LENGTH, 1);
	for (digest_length = 8; digest_length <= 24; digest_length += 3)
	{
		int total_attempts = 0;
		// unsigned char *input = calloc(INPUT_LENGTH, 1);
		unsigned char *digest = calloc(digest_length, 1);
		for (test_counter = 0; test_counter < NUM_TESTS; test_counter++)
		{
			generate_random_bytes(input, INPUT_LENGTH);
			sha1(input, digest, INPUT_LENGTH, digest_length);
			total_attempts += preimage_attack(digest, digest_length);
		}

		printf("\nAVG ATTEMPTS FOR DIGEST LENGTH %d: %f\n", digest_length, total_attempts / (float) NUM_TESTS);
		printf("ANTICIPATED AVG: %f\n\n", pow(2, digest_length));
		free(digest);
	}
	free(input);
}

int collision_attack(int digest_length)
{
	int byte_count = digest_length / 8;
	if (digest_length % 8 != 0)
	{
		byte_count += 1;
	}

	byte_set digests;
	digests.bytes = malloc(40 * sizeof(byte *));
	digests.capacity = 40;
	digests.length = 0;

	unsigned char *digest = calloc(digest_length, 1);

	int input_length = digest_length * 2;
	unsigned char *input = calloc(input_length, 1);
	int attempt_count = 0;
	do
	{
		attempt_count++;
		generate_random_bytes(input, input_length);
		sha1(input, digest, input_length, digest_length);
	} while (byte_set_insert(input, byte_count, &digests) && attempt_count < pow(2, input_length));

	printf("%d\n", attempt_count);
	// printf("COLLISION ATTACK SUCCEEDED. %d ATTEMPTS\n", attempt_count);

	for (int i = 0; i < digests.length; i++)
	{
		free(digests.bytes[i]);
	}
	free(digests.bytes);
	free(input);
	return attempt_count;
}

int preimage_attack(unsigned char *digest, int digest_length)
{
	int byte_count = digest_length / 8;
	if (digest_length % 8 != 0)
	{
		byte_count += 1;
	}
	// printf("Preimage Collision Test\t->\tDigest: ");
	// print_bits(digest, digest_length);
	// printf("\n");
	unsigned char *input = calloc(8, 1);
	int input_length = 8;
	unsigned char *check_digest = calloc(byte_count, 1);
	int attempt_count = 0;
	do 
	{
		attempt_count++;
		sha1(input, check_digest, input_length, digest_length);
		input_length = increment_bytes(input, input_length);
	} while (memcmp(check_digest, digest, byte_count));
	
	printf("%d\n", attempt_count);
	// printf("Preimage Collision --\tINPUT: ");
	// print_bytes(input, input_length);
	// printf("\tDIGEST: ");
	// print_bits(check_digest, digest_length);
	// printf("\nAttempts: %d\n", attempt_count);
	return attempt_count;
}

void sha1(unsigned char *input, unsigned char *output, int input_length, int output_length)
{
	int byte_count = output_length / 8;
	unsigned char mask = 255;
	if (output_length % 8 != 0)
	{
		byte_count++;
		mask = 255 << (8 - output_length % 8);
	}

	unsigned char digest[20];
	SHA1(input, byte_count, digest);

	memcpy(output, digest, byte_count);
	output[byte_count - 1] &= mask;
}

int byte_set_insert(byte *bytes, int byte_count, byte_set *set)
{
	byte *bytes_copy = calloc(byte_count, 1);
	memcpy(bytes_copy, bytes, byte_count);
	int bytes_index = 0;
	while (bytes_index < set->length && memcmp(bytes_copy, set->bytes[bytes_index], byte_count))
	{
		bytes_index++;
	}
	if (bytes_index != set->length)
	{
		return 0;
	}
	set->length++;
	if (set->length > set->capacity)
	{
		byte **new_bytes = malloc(set->capacity * 2 * sizeof(byte *));
		memcpy(new_bytes, set->bytes, set->capacity * sizeof(byte *));
		set->capacity *= 2;
		free(set->bytes);
		set->bytes = new_bytes;
	}
	set->bytes[set->length - 1] = bytes_copy;
	return 1;
}

int output_byte_set(byte_set *set, char *file_name)
{
	FILE f;
	("", "w")
}