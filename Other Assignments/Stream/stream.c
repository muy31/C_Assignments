#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stream.h"

struct stream {
	size_t position;
	size_t length;
	int* things;
	Stream* stream1;
	Stream* stream2;
};

// Return the next character in stream
int streamNext(Stream* stream) {
	int val;
	if (stream->length > stream->position) {
		val = stream->things[stream->position];
	}
	else {
		stream->position = 0;
		val = stream->things[0];
	}
	stream->position++;
	return val;
}

// Return a stream that repeats the contents of s
// over and over.
// For empty string, returns infinite stream of '\0'
// This should copy s in case s is changed or freed.
//
// streamFromString("abc") -> "abcabcabcabcabcabc..."
Stream* streamFromString(const char* s) {

	size_t len = strlen(s);

	Stream* str = malloc(sizeof(Stream));
	str->position = 0;
	str->length = len;
	str->things = malloc((len + 1) * sizeof(int));
	str->stream1 = NULL;
	str->stream2 = NULL;

	if (str->length == 0) {
		str->things[0] = 0;
	}
	else {

		for (size_t index = 0; index < str->length; index++) {
			str->things[index] = s[index];
		}
	}

	return str;

}

// Return contents of even interleaved with contents of odd.
//
// streamInterleave(streamFromString("a"), streamFromString("bc"))
//  -> "abacabacabac..."
Stream* streamInterleave(Stream* even, Stream* odd) {

	size_t len = even->length * odd->length * 2;

	Stream* str = malloc(sizeof(Stream));

	str->position = 0;
	str->length = len;

	str->things = malloc((len + 1) * sizeof(int));

	for (size_t index = 0; index < len; index++) {
		if (index % 2 == 0) {
			str->things[index] = streamNext(even);
		}
		else {
			str->things[index] = streamNext(odd);
		}
	}

	str->stream1 = even;
	str->stream2 = odd;

	return str;

}
// Return stream where each character c is replaced
// by f(c). Both c and f(c) should be in the range 0..255.
Stream* streamMap(int (*f)(int), Stream* stream) {

	Stream* str = malloc(sizeof(Stream));
	str->position = 0;
	str->length = stream->length;

	str->things = malloc(sizeof(int) * (str->length + 1));

	for (size_t index = 0; index < str->length; index++) {

		str->things[index] = (*f)(stream->things[index]);

	}

	str->stream1 = stream;
	str->stream2 = NULL;

	return str;
}

// Free stream and any streams used to construct stream.
void streamDestroy(Stream* stream) {
	free(stream->things);

	if (stream->stream1 != NULL) {
		streamDestroy(stream->stream1);
	}

	if (stream->stream2 != NULL) {
		streamDestroy(stream->stream2);
	}

	free(stream);
}
