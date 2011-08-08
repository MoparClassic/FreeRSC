#include "packethandler.h"
#include "dataoperations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int discard(client_t *, uint32_t, uint32_t);
static int session_request(client_t *, uint32_t, uint32_t);
static int login(client_t *, uint32_t, uint32_t);
static int logout_request(client_t *, uint32_t, uint32_t);
static int logout(client_t *, uint32_t, uint32_t);
static int register_account(client_t *, uint32_t, uint32_t);
static int ping(client_t *, uint32_t, uint32_t);
static int its_a_trap(client_t *, uint32_t, uint32_t); /* Trololol :D */
static int walk_request(client_t *, uint32_t, uint32_t);
static int chat(client_t *, uint32_t, uint32_t);
static int privacy_settings(client_t *, uint32_t, uint32_t);
static int game_settings(client_t *, uint32_t, uint32_t);
static int drop_item(client_t *, uint32_t, uint32_t);
static int activate_prayer(client_t *, uint32_t, uint32_t);
static int deactivate_prayer(client_t *, uint32_t, uint32_t);
static int command(client_t *, uint32_t, uint32_t);
static int wield_item(client_t *, uint32_t, uint32_t);
static int unwield_item(client_t *, uint32_t, uint32_t);
static int player_appearances(client_t *, uint32_t, uint32_t);
static int spell_on_self(client_t *, uint32_t, uint32_t);
static int spell_on_player(client_t *, uint32_t, uint32_t);
static int spell_on_npc(client_t *, uint32_t, uint32_t);
static int spell_on_inventory_item(client_t *, uint32_t, uint32_t);
static int spell_on_door(client_t *, uint32_t, uint32_t);
static int spell_on_object(client_t *, uint32_t, uint32_t);
static int spell_on_ground_item(client_t *, uint32_t, uint32_t);
static int spell_on_ground(client_t *, uint32_t, uint32_t);
static int appearance_changed(client_t *, uint32_t, uint32_t);
static int send_trade_request(client_t *, uint32_t, uint32_t);
static int accept_trade(client_t *, uint32_t, uint32_t);
static int confirm_accept_trade(client_t *, uint32_t, uint32_t);
static int decline_trade(client_t *, uint32_t, uint32_t);
static int receive_trade_offer(client_t *, uint32_t, uint32_t);
static int send_duel_request(client_t *, uint32_t, uint32_t);
static int accept_duel(client_t *, uint32_t, uint32_t);
static int confirm_accept_duel(client_t *, uint32_t, uint32_t);
static int decline_duel(client_t *, uint32_t, uint32_t);
static int receive_duel_offer(client_t *, uint32_t, uint32_t);
static int set_duel_options(client_t *, uint32_t, uint32_t);
static int add_friend(client_t *, uint32_t, uint32_t);
static int remove_friend(client_t *, uint32_t, uint32_t);
static int add_ignore(client_t *, uint32_t, uint32_t);
static int remove_ignore(client_t *, uint32_t, uint32_t);
static int send_pm(client_t *, uint32_t, uint32_t);
static int use_item(client_t *, uint32_t, uint32_t);
static int use_item_on_player(client_t *, uint32_t, uint32_t);
static int use_item_on_door(client_t *, uint32_t, uint32_t);
static int use_item_on_object(client_t *, uint32_t, uint32_t);
static int use_item_on_ground_item(client_t *, uint32_t, uint32_t);
static int use_item_on_item(client_t *, uint32_t, uint32_t);
static int use_item_on_npc(client_t *, uint32_t, uint32_t);
static int pickup_item(client_t *, uint32_t, uint32_t);
static int object_action(client_t *, uint32_t, uint32_t);
static int wall_object_action(client_t *, uint32_t, uint32_t);
static int attack_player(client_t *, uint32_t, uint32_t);
static int attack_npc(client_t *, uint32_t, uint32_t);
static int change_attack_style(client_t *, uint32_t, uint32_t);
static int talk_to_npc(client_t *, uint32_t, uint32_t);
static int close_bank(client_t *, uint32_t, uint32_t);
static int deposit_item(client_t *, uint32_t, uint32_t);
static int withdraw_item(client_t *, uint32_t, uint32_t);
static int menu_response(client_t *, uint32_t, uint32_t);
static int exception_handler(client_t *, uint32_t, uint32_t);
static int close_shop(client_t *, uint32_t, uint32_t);
static int buy_item(client_t *, uint32_t, uint32_t);
static int sell_item(client_t *, uint32_t, uint32_t);
static int follow_player(client_t *, uint32_t, uint32_t);
static int npc_command(client_t *, uint32_t, uint32_t);

int (*packet_decoder_table[256])(client_t *client, uint32_t opcode,
                                 uint32_t psiz) = {
    &login, /* 0 */
    &discard, /* 1 */
    &discard, /* 2 */
    &its_a_trap, /* 3 */
    &discard, /* 4 */
    &ping, /* 5 */
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
    &use_item_on_player, /* 16 */
    &spell_on_object, /* 17 */
    &discard, /* 18 */
    &discard, /* 19 */
    &discard, /* 20 */
    &discard, /* 21 */
    &discard, /* 22 */
    &discard, /* 23 */
    &discard, /* 24 */
    &add_ignore, /* 25 */
    &discard, /* 26 */
    &use_item_on_item, /* 27 */
    &discard, /* 28 */
    &discard, /* 29 */
    &discard, /* 30 */
    &discard, /* 31 */
    &session_request, /* 32 */
    &discard, /* 33 */
    &use_item_on_ground_item, /* 34 */
    &decline_duel, /* 35 */
    &use_item_on_door, /* 36 */
    &discard, /* 37 */
    &discard, /* 38 */
    &logout, /* 39 */
    &object_action, /* 40 */
    &discard, /* 41 */
    &change_attack_style, /* 42 */
    &discard, /* 43 */
    &discard, /* 44 */
    &discard, /* 45 */
    &discard, /* 46 */
    &discard, /* 47 */
    &close_bank, /* 48 */
    &spell_on_inventory_item, /* 49 */
    &discard, /* 50 */
    &object_action, /* 51 */
    &remove_friend, /* 52 */
    &confirm_accept_trade, /* 53 */
    &discard, /* 54 */
    &spell_on_player, /* 55 */
    &activate_prayer, /* 56 */
    &attack_player, /* 57 */
    &discard, /* 58 */
    &discard, /* 59 */
    &discard, /* 60 */
    &discard, /* 61 */
    &discard, /* 62 */
    &discard, /* 63 */
    &discard, /* 64 */
    &discard, /* 65 */
    &discard, /* 66 */
    &spell_on_door, /* 67 */
    &follow_player, /* 68 */
    &discard, /* 69 */
    &receive_trade_offer, /* 70 */
    &spell_on_npc, /* 71 */
    &discard, /* 72 */
    &attack_npc, /* 73 */
    &npc_command, /* 74 */
    &discard, /* 75 */
    &discard, /* 76 */
    &discard, /* 77 */
    &discard, /* 78 */
    &discard, /* 79 */
    &discard, /* 80 */
    &discard, /* 81 */
    &discard, /* 82 */
    &player_appearances, /* 83 */
    &discard, /* 84 */
    &discard, /* 85 */
    &discard, /* 86 */
    &confirm_accept_duel, /* 87 */
    &discard, /* 88 */
    &use_item, /* 89 */
    &command, /* 90 */
    &discard, /* 91 */
    &unwield_item, /* 92 */
    &discard, /* 93 */
    &use_item_on_object, /* 94 */
    &discard, /* 95 */
    &discard, /* 96 */
    &discard, /* 97 */
    &discard, /* 98 */
    &discard, /* 99 */
    &discard, /* 100 */
    &discard, /* 101 */
    &discard, /* 102 */
    &discard, /* 103 */
    &spell_on_ground_item, /* 104 */
    &discard, /* 105 */
    &discard, /* 106 */
    &discard, /* 107 */
    &remove_ignore, /* 108 */
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
    &receive_duel_offer, /* 123 */
    &discard, /* 124 */
    &discard, /* 125 */
    &wall_object_action, /* 126 */
    &discard, /* 127 */
    &buy_item, /* 128 */
    &logout_request, /* 129 */
    &discard, /* 130 */
    &discard, /* 131 */
    &walk_request, /* 132 */
    &discard, /* 133 */
    &discard, /* 134 */
    &discard, /* 135 */
    &discard, /* 136 */
    &discard, /* 137 */
    &discard, /* 138 */
    &discard, /* 139 */
    &discard, /* 140 */
    &discard, /* 141 */
    &use_item_on_npc, /* 142 */
    &discard, /* 143 */
    &discard, /* 144 */
    &chat, /* 145 */
    &discard, /* 146 */
    &discard, /* 147 */
    &discard, /* 148 */
    &discard, /* 149 */
    &discard, /* 150 */
    &register_account, /* 151 */
    &discard, /* 152 */
    &discard, /* 153 */
    &menu_response, /* 154 */
    &discard, /* 155 */
    &exception_handler, /* 156 */
    &game_settings, /* 157 */
    &discard, /* 158 */
    &discard, /* 159 */
    &discard, /* 160 */
    &discard, /* 161 */
    &discard, /* 162 */
    &discard, /* 163 */
    &discard, /* 164 */
    &discard, /* 165 */
    &send_trade_request, /* 166 */
    &discard, /* 167 */
    &add_friend, /* 168 */
    &discard, /* 169 */
    &discard, /* 170 */
    &discard, /* 171 */
    &discard, /* 172 */
    &discard, /* 173 */
    &discard, /* 174 */
    &discard, /* 175 */
    &privacy_settings, /* 176 */
    &talk_to_npc, /* 177 */
    &discard, /* 178 */
    &discard, /* 179 */
    &discard, /* 180 */
    &wield_item, /* 181 */
    &discard, /* 182 */
    &withdraw_item, /* 183 */
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
    &deposit_item, /* 198 */
    &discard, /* 199 */
    &discard, /* 200 */
    &discard, /* 201 */
    &discard, /* 202 */
    &discard, /* 203 */
    &discard, /* 204 */
    &discard, /* 205 */
    &spell_on_self, /* 206 */
    &discard, /* 207 */
    &discard, /* 208 */
    &discard, /* 209 */
    &discard, /* 210 */
    &accept_trade, /* 211 */
    &discard, /* 212 */
    &discard, /* 213 */
    &discard, /* 214 */
    &discard, /* 215 */
    &decline_trade, /* 216 */
    &discard, /* 217 */
    &appearance_changed, /* 218 */
    &discard, /* 219 */
    &discard, /* 220 */
    &discard, /* 221 */
    &send_duel_request, /* 222 */
    &discard, /* 223 */
    &discard, /* 224 */
    &set_duel_options, /* 225 */
    &discard, /* 226 */
    &discard, /* 227 */
    &discard, /* 228 */
    &discard, /* 229 */
    &discard, /* 230 */
    &discard, /* 231 */
    &spell_on_ground, /* 232 */
    &discard, /* 233 */
    &discard, /* 234 */
    &wall_object_action, /* 235 */
    &discard, /* 236 */
    &discard, /* 237 */
    &discard, /* 238 */
    &discard, /* 239 */
    &discard, /* 240 */
    &discard, /* 241 */
    &discard, /* 242 */
    &discard, /* 243 */
    &discard, /* 244 */
    &pickup_item, /* 245 */
    &walk_request, /* 246 */
    &discard, /* 247 */
    &deactivate_prayer, /* 248 */
    &discard, /* 249 */
    &discard, /* 250 */
    &discard, /* 251 */
    &accept_duel, /* 252 */
    &close_shop, /* 253 */
    &send_pm, /* 254 */
    &sell_item, /* 255 */
};

/* #define PRINT_PAYLOAD */
static int
print_payload(client_t *client, unsigned int opcode, int packet_size)
{
#ifdef PRINT_PAYLOAD
    cbuffer_t *cbuf = &client->in_buffer;
    int i;

    cbuffer_mark_read_position(cbuf);
    printf("Packet details: opcode=%d, size=%d\n", opcode, packet_size);
    printf("Payload: ");
    for (i = 0; i < packet_size; ++i) {
        printf("%d ", cbuf->buffer[cbuf->read_offset++]);
    }
    cbuffer_rewind_read_position(cbuf);
    printf("\n");
#endif
    return 0;
}

static int
discard(client_t *client, uint32_t opcode, uint32_t psiz)
{
    cbuffer_t *cbuf = &client->in_buffer;

    print_payload(client, opcode, psiz);
    cbuf->read_offset += psiz;
    return 1;
}

static int
session_request(client_t *client, uint32_t opcode, uint32_t psiz)
{
    cbuffer_t *inbuf = &client->in_buffer;
    cbuffer_t *outbuf = &client->out_buffer;
    uint64_t randval = 0LL;
    int ps = psiz - 1; /* We skip the first byte; we don't use it */
    char str[32];

    print_payload(client, opcode, psiz);

    memset(str, '\0', sizeof(str));
    cbuffer_skip(inbuf, 1); /* First byte skipped */
    cbuffer_read_fixedlen_string(inbuf, ps, str, sizeof(str));

    printf("String received: '%s'\n", str);

    /* Generate a random 64 bit integer to send to the client */
    randval = rand();
    randval <<= 32;
    randval |= rand();

    cbuffer_send_data(outbuf, &randval, sizeof(randval));
    return 1;
}

static int
login(client_t *client, uint32_t opcode, uint32_t psiz)
{
    cbuffer_t *inbuf = &client->in_buffer;
    cbuffer_t *outbuf = &client->out_buffer;
    uint8_t reconnecting = 0;
    uint16_t client_version = 0;
    uint32_t uid;
    uint8_t buf[psiz - 4];
    uint32_t session_keys[4];
    int ofs = 0;
    int ps = psiz;
    int encrypted_siz;
    int i;
    int return_code = 22;
    char username_dirty[21];
    char password_dirty[21];
    char username[13];
    char password[21];

    memset(username_dirty, 0, sizeof(username_dirty));
    memset(password_dirty, 0, sizeof(password_dirty));
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));

    cbuffer_mark_read_position(inbuf);
    reconnecting = cbuffer_read_byte(inbuf);
    client_version = cbuffer_read_short(inbuf);
    //encrypted_siz = cbuffer_read_byte(inbuf);
    for (i = 0; i < 4; ++i) {
        session_keys[i] = cbuffer_read_int(inbuf);
    }
    uid = cbuffer_read_int(inbuf);
    cbuffer_read_fixedlen_string(inbuf, 20, username_dirty, sizeof(username_dirty));
    cbuffer_read_fixedlen_string(inbuf, 20, password_dirty, sizeof(password_dirty));

    printf("'%s', '%s'\n", username_dirty, password_dirty);
    trim(username, username_dirty, sizeof(username));
    trim(password, password_dirty, sizeof(password));
    printf("'%s', '%s'\n", username, password);

    return_code = 0; /* Successful login */
    cbuffer_send_data(outbuf, &return_code, sizeof(return_code));

    cbuffer_rewind_read_position(inbuf);
    discard(client, opcode, psiz);
    return 0;
}

static int
logout_request(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
logout(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
register_account(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
ping(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
its_a_trap(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
walk_request(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
chat(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
privacy_settings(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
game_settings(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
drop_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
activate_prayer(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
deactivate_prayer(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
command(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
wield_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
unwield_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
player_appearances(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
spell_on_self(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
spell_on_player(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
spell_on_npc(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
spell_on_inventory_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
spell_on_door(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
spell_on_object(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}
static int
spell_on_ground_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
spell_on_ground(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
appearance_changed(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
send_trade_request(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
accept_trade(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
confirm_accept_trade(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
decline_trade(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
receive_trade_offer(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
send_duel_request(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
accept_duel(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
confirm_accept_duel(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
decline_duel(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
receive_duel_offer(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
set_duel_options(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
add_friend(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
remove_friend(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
add_ignore(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
remove_ignore(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
send_pm(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
use_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
use_item_on_player(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
use_item_on_door(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
use_item_on_object(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
use_item_on_ground_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
use_item_on_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
use_item_on_npc(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
pickup_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
object_action(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
wall_object_action(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
attack_player(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
attack_npc(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
change_attack_style(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
talk_to_npc(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
close_bank(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
deposit_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
withdraw_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
menu_response(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
exception_handler(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
close_shop(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
buy_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
sell_item(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
follow_player(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}

static int
npc_command(client_t *client, uint32_t opcode, uint32_t psiz)
{
    discard(client, opcode, psiz);
    return 0;
}