#include "EncryptionTests.h"

#include <gmp.h>
#include <string.h>

/*
static void
test_encryption_decryption(CuTest *tc)
{
    char vals[] = {
        '9', '99', '42', '0', '50', '152', '96',
        '228', '252', '88', '138', '31', '225',
        '157', '67', '101', '181', '82', '3', '69',
        '162', '210', '145', '107', '61', '93',
        '225', '201', '42', '186', '121', '164',
        '155', '241', '187', '164', '61', '68',
        '168', '65', '87', '38', '198', '214',
        '233', '192', '77', '108', '171', '21',
        '64', '73', '213', '231', '76', '69', '6',
        '34', '188', '181', '203', '161', '79', '160'
    };
    char buf[200];
    const char *priv_key_str = "730546719878348732291497161314617369560443701473303681965331739205703475535302276087891130348991033265134162275669215460061940182844329219743687403068279";
    const char *mod_str = "1549611057746979844352781944553705273443228154042066840514290174539588436243191882510185738846985723357723362764835928526260868977814405651690121789896823";
    const char *pub_key_str = "1370158896620336158431733257575682136836100155721926632321599369132092701295540721504104229217666225601026879393318399391095704223500673696914052239029335";
    const char *test_str = "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF00";
    mpz_t pubkey, privkey, mod, testval;
    mpz_t encrypted, decrypted;
    int i;

    mpz_init2(pubkey, 1024);
    mpz_init2(privkey, 1024);
    mpz_init2(mod, 1024);
    mpz_init2(testval, 512);
    mpz_init(encrypted);
    mpz_init(decrypted);

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

    if (mpz_set_str(testval, vals, 16) == -1) {
        printf("Testval is invalid base 16 integer\n");
    }
    gmp_printf("Testval: %Zx\n", testval);

    mpz_powm(encrypted, testval, pubkey, mod);
    gmp_printf("Encrypted testval: %Zd\n", encrypted);
    mpz_powm(decrypted, testval, privkey, mod);
    gmp_printf("Decrypted testval: %Zx\n", decrypted);

    memset(buf, 0, sizeof (buf));
    mpz_get_str(buf, 10, decrypted);
    for (i = 0; i < sizeof (buf); ++i) {
        printf("%d, ", buf[i]);
    }
    printf("\n");

    if (mpz_cmp(testval, decrypted) == 0) {
        printf("Initial testval is equal to decrypted value\n");
    }

    mpz_clear(pubkey);
    mpz_clear(privkey);
    mpz_clear(mod);
    mpz_clear(testval);
    mpz_clear(encrypted);
    mpz_clear(decrypted);
}
*/

CuSuite * get_encryption_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    //SUITE_ADD_TEST(suite, &test_encryption_decryption);
    return suite;
}
