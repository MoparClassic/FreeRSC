#include "EncryptionTests.h"

#include <gmp.h>
#include <string.h>

static void
test_encryption_decryption(CuTest *tc)
{
    char enc_vals[] = {
        19, -58, -55, 97, 70, -119, -44, 91, -106, -105, -83, -43, -49,
        11, -101, -104, -12, -98, -9, -112, 23, -119, -99, -111, -2, 37,
        0, 74, -67, -124, 77, -6, 5, 118, 14, -104, -77, 11, -33, 14, -8,
        106, 38, 32, 44, -32, -51, 115, 63, -113, 119, 7, -6, -71, -91,
        -94, -84, -98, 51, 13, 10, -125, -117, 46
    };
    /*
        char enc_vals[] = {
            0, 1, 2, 3
        };
    */
    char dec_vals[] = {
        3, 51, -89, -80, 4, -47, 98, 107, -20, 122, -74, 21, 3, 121, -113,
        27, 0, 0, 0, 0, 76, 111, 116, 104, 121, 32, 32, 32, 32, 32, 32, 32,
        32, 32, 32, 32, 32, 32, 32, 32, 10, 100, 97, 110, 116, 104, 101,
        109, 97, 110, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 10
    };
    char buf[200];
    const char *priv_key_str = "730546719878348732291497161314617369560443701473303681965331739205703475535302276087891130348991033265134162275669215460061940182844329219743687403068279";
    const char *mod_str = "1549611057746979844352781944553705273443228154042066840514290174539588436243191882510185738846985723357723362764835928526260868977814405651690121789896823";
    const char *pub_key_str = "1370158896620336158431733257575682136836100155721926632321599369132092701295540721504104229217666225601026879393318399391095704223500673696914052239029335";
    const char *test_str = "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF";
    mpz_t pubkey, privkey, mod, testval, rofl;
    mpz_t encrypted_vals, decrypted_vals;
    mp_bitcnt_t bitcnt;
    int i;
    unsigned int result;

    mpz_init2(pubkey, 1024);
    mpz_init2(privkey, 1024);
    mpz_init2(mod, 1024);
    mpz_init2(testval, 512);
    mpz_init(encrypted_vals);
    mpz_init(decrypted_vals);

    mpz_init(rofl);

    /*
        if (mpz_set_str(pubkey, pub_key_str, 10) == -1) {
            printf("Public key is invalid base 10 integer\n");
        }
        gmp_printf("Public key: %Zx\n", pubkey);

        if (mpz_set_str(privkey, priv_key_str, 10) == -1) {
            printf("Private key is invalid base 10 integer\n");
        }
        gmp_printf("Private key: %Zx\n", privkey);

        if (mpz_set_str(mod, mod_str, 10) == -1) {
            printf("Modulo is invalid base 10 integer\n");
        }
        gmp_printf("Modulo: %Zx\n", mod);

        if (mpz_set_str(testval, test_str, 16) == -1) {
            printf("Testval is invalid base 16 integer\n");
        }
        gmp_printf("Testval: %Zx\n", testval);
    */

    mpz_set_ui(rofl, 0);
    for (i = 0; i < sizeof(enc_vals); ++i) {
        mpz_mul_2exp(rofl, rofl, 8);
        mpz_add_ui(rofl, rofl, enc_vals[i]);
    }
    gmp_printf("rofl = %Zd\n", rofl);
    result = mpz_get_ui(rofl);
    printf("%d\n", result);
    result = mpz_get_ui(rofl);
    printf("%d\n", result);

    /*
        mpz_powm(encrypted_vals, testval, pubkey, mod);
        gmp_printf("Encrypted testval: %Zd\n", encrypted_vals);
        mpz_powm(decrypted_vals, encrypted_vals, privkey, mod);
        gmp_printf("Decrypted testval: %Zx\n", decrypted_vals);
    */

    /*memset(buf, 0, sizeof (buf));
    mpz_get_str(buf, 10, decrypted_vals);
    for (i = 0; i < sizeof (buf); ++i) {
        printf("%d, ", buf[i]);
    }
    printf("\n");*/

    /*
        if (mpz_cmp(testval, decrypted_vals) == 0) {
            printf("Initial testval is equal to decrypted value\n");
        }
    */

    mpz_clear(pubkey);
    mpz_clear(privkey);
    mpz_clear(mod);
    mpz_clear(testval);
    mpz_clear(encrypted_vals);
    mpz_clear(decrypted_vals);
}

CuSuite * get_encryption_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    //SUITE_ADD_TEST(suite, &test_encryption_decryption);
    return suite;
}
