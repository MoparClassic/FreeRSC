#include "packethandler.h"

#include <stdio.h>

int (*packet_decoder_table[256])(client_t *client, unsigned int opcode,
                                 int packet_size) = {
    &discard, /* 0 */
    &discard, /* 1 */
    &discard, /* 2 */
    &discard, /* 3 */
    &discard, /* 4 */
    &discard, /* 5 */
    &discard, /* 6 */
    &discard, /* 7 */
    &discard, /* 8 */
    &discard, /* 9 */
    &discard, /* 10 */
    &discard, /* 11 */
    &discard, /* 12 */
    &discard, /* 13 */
    &discard, /* 14 */
    &discard, /* 15 */
    &discard, /* 16 */
    &discard, /* 17 */
    &discard, /* 18 */
    &discard, /* 19 */
    &discard, /* 20 */
    &discard, /* 21 */
    &discard, /* 22 */
    &discard, /* 23 */
    &discard, /* 24 */
    &discard, /* 25 */
    &discard, /* 26 */
    &discard, /* 27 */
    &discard, /* 28 */
    &discard, /* 29 */
    &discard, /* 30 */
    &discard, /* 31 */
    &discard, /* 32 */
    &discard, /* 33 */
    &discard, /* 34 */
    &discard, /* 35 */
    &discard, /* 36 */
    &discard, /* 37 */
    &discard, /* 38 */
    &discard, /* 39 */
    &discard, /* 40 */
    &discard, /* 41 */
    &discard, /* 42 */
    &discard, /* 43 */
    &discard, /* 44 */
    &discard, /* 45 */
    &discard, /* 46 */
    &discard, /* 47 */
    &discard, /* 48 */
    &discard, /* 49 */
    &discard, /* 50 */
    &discard, /* 51 */
    &discard, /* 52 */
    &discard, /* 53 */
    &discard, /* 54 */
    &discard, /* 55 */
    &discard, /* 56 */
    &discard, /* 57 */
    &discard, /* 58 */
    &discard, /* 59 */
    &discard, /* 60 */
    &discard, /* 61 */
    &discard, /* 62 */
    &discard, /* 63 */
    &discard, /* 64 */
    &discard, /* 65 */
    &discard, /* 66 */
    &discard, /* 67 */
    &discard, /* 68 */
    &discard, /* 69 */
    &discard, /* 70 */
    &discard, /* 71 */
    &discard, /* 72 */
    &discard, /* 73 */
    &discard, /* 74 */
    &discard, /* 75 */
    &discard, /* 76 */
    &discard, /* 77 */
    &discard, /* 78 */
    &discard, /* 79 */
    &discard, /* 80 */
    &discard, /* 81 */
    &discard, /* 82 */
    &discard, /* 83 */
    &discard, /* 84 */
    &discard, /* 85 */
    &discard, /* 86 */
    &discard, /* 87 */
    &discard, /* 88 */
    &discard, /* 89 */
    &discard, /* 90 */
    &discard, /* 91 */
    &discard, /* 92 */
    &discard, /* 93 */
    &discard, /* 94 */
    &discard, /* 95 */
    &discard, /* 96 */
    &discard, /* 97 */
    &discard, /* 98 */
    &discard, /* 99 */
    &discard, /* 100 */
    &discard, /* 101 */
    &discard, /* 102 */
    &discard, /* 103 */
    &discard, /* 104 */
    &discard, /* 105 */
    &discard, /* 106 */
    &discard, /* 107 */
    &discard, /* 108 */
    &discard, /* 109 */
    &discard, /* 110 */
    &discard, /* 111 */
    &discard, /* 112 */
    &discard, /* 113 */
    &discard, /* 114 */
    &discard, /* 115 */
    &discard, /* 116 */
    &discard, /* 117 */
    &discard, /* 118 */
    &discard, /* 119 */
    &discard, /* 120 */
    &discard, /* 121 */
    &discard, /* 122 */
    &discard, /* 123 */
    &discard, /* 124 */
    &discard, /* 125 */
    &discard, /* 126 */
    &discard, /* 127 */
    &discard, /* 128 */
    &discard, /* 129 */
    &discard, /* 130 */
    &discard, /* 131 */
    &discard, /* 132 */
    &discard, /* 133 */
    &discard, /* 134 */
    &discard, /* 135 */
    &discard, /* 136 */
    &discard, /* 137 */
    &discard, /* 138 */
    &discard, /* 139 */
    &discard, /* 140 */
    &discard, /* 141 */
    &discard, /* 142 */
    &discard, /* 143 */
    &discard, /* 144 */
    &discard, /* 145 */
    &discard, /* 146 */
    &discard, /* 147 */
    &discard, /* 148 */
    &discard, /* 149 */
    &discard, /* 150 */
    &discard, /* 151 */
    &discard, /* 152 */
    &discard, /* 153 */
    &discard, /* 154 */
    &discard, /* 155 */
    &discard, /* 156 */
    &discard, /* 157 */
    &discard, /* 158 */
    &discard, /* 159 */
    &discard, /* 160 */
    &discard, /* 161 */
    &discard, /* 162 */
    &discard, /* 163 */
    &discard, /* 164 */
    &discard, /* 165 */
    &discard, /* 166 */
    &discard, /* 167 */
    &discard, /* 168 */
    &discard, /* 169 */
    &discard, /* 170 */
    &discard, /* 171 */
    &discard, /* 172 */
    &discard, /* 173 */
    &discard, /* 174 */
    &discard, /* 175 */
    &discard, /* 176 */
    &discard, /* 177 */
    &discard, /* 178 */
    &discard, /* 179 */
    &discard, /* 180 */
    &discard, /* 181 */
    &discard, /* 182 */
    &discard, /* 183 */
    &discard, /* 184 */
    &discard, /* 185 */
    &discard, /* 186 */
    &discard, /* 187 */
    &discard, /* 188 */
    &discard, /* 189 */
    &discard, /* 190 */
    &discard, /* 191 */
    &discard, /* 192 */
    &discard, /* 193 */
    &discard, /* 194 */
    &discard, /* 195 */
    &discard, /* 196 */
    &discard, /* 197 */
    &discard, /* 198 */
    &discard, /* 199 */
    &discard, /* 200 */
    &discard, /* 201 */
    &discard, /* 202 */
    &discard, /* 203 */
    &discard, /* 204 */
    &discard, /* 205 */
    &discard, /* 206 */
    &discard, /* 207 */
    &discard, /* 208 */
    &discard, /* 209 */
    &discard, /* 210 */
    &discard, /* 211 */
    &discard, /* 212 */
    &discard, /* 213 */
    &discard, /* 214 */
    &discard, /* 215 */
    &discard, /* 216 */
    &discard, /* 217 */
    &discard, /* 218 */
    &discard, /* 219 */
    &discard, /* 220 */
    &discard, /* 221 */
    &discard, /* 222 */
    &discard, /* 223 */
    &discard, /* 224 */
    &discard, /* 225 */
    &discard, /* 226 */
    &discard, /* 227 */
    &discard, /* 228 */
    &discard, /* 229 */
    &discard, /* 230 */
    &discard, /* 231 */
    &discard, /* 232 */
    &discard, /* 233 */
    &discard, /* 234 */
    &discard, /* 235 */
    &discard, /* 236 */
    &discard, /* 237 */
    &discard, /* 238 */
    &discard, /* 239 */
    &discard, /* 240 */
    &discard, /* 241 */
    &discard, /* 242 */
    &discard, /* 243 */
    &discard, /* 244 */
    &discard, /* 245 */
    &discard, /* 246 */
    &discard, /* 247 */
    &discard, /* 248 */
    &discard, /* 249 */
    &discard, /* 250 */
    &discard, /* 251 */
    &discard, /* 252 */
    &discard, /* 253 */
    &discard, /* 254 */
    &discard, /* 255 */
};

//#define PRINT_PAYLOAD
static int
print_payload(client_t *client, unsigned int opcode, int packet_size)
{
#ifdef PRINT_PAYLOAD
    cbuffer_t *cbuf = &client->in_buffer;
    int i;

    cbuffer_mark_read_position(cbuf);
    printf("Payload: ");
    for (i = 0; i < packet_size; ++i) {
        printf("%d ", cbuf->buffer[cbuf->read_offset++]);
    }
    cbuffer_rewind_read_position(cbuf);
    printf("\n");
#endif
}

int discard(client_t *client, unsigned int opcode, int packet_size)
{
    cbuffer_t *cbuf = &client->in_buffer;

    printf("Discard handler: opcode %d size %d\n", opcode, packet_size);
    print_payload(client, opcode, packet_size);
    cbuf->read_offset += packet_size;
    return 1;
}

int login(client_t *client, unsigned int opcode, int packet_size)
{
    return 0;
}

int logout(client_t *client, unsigned int opcode, int packet_size)
{
    return 0;
}