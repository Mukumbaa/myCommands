#include <stdio.h>
#include <assert.h>
#include "../include/libstr.h"

#define TEST_START(name) printf("[RUNNING] %-35s", name)
#define TEST_PASSED()     printf(" \033[0;32m[PASS]\033[0m\n")

void test_init_and_destroy(void) {
    TEST_START("Init & Destroy");

    String s1 = str_init("Hello World");
    assert(s1.str != NULL);
    assert(s1.len == 11);
    assert(s1.capacity >= 12);
    assert(str_cmpr(&s1, "Hello World") == 0);
    str_destroy(&s1);
    assert(s1.str == NULL);
    assert(s1.len == 0);

    String s2 = str_init("");
    assert(s2.str != NULL);
    assert(s2.len == 0);
    assert(str_cmpr(&s2, "") == 0);
    str_destroy(&s2);

    String s3 = str_init(NULL);
    assert(s3.str != NULL);
    assert(s3.len == 0);
    str_destroy(&s3);

    String s4 = str_init_with_cap(128);
    assert(s4.str != NULL);
    assert(s4.len == 0);
    assert(s4.capacity >= 128);
    str_destroy(&s4);

    str_destroy(NULL); // does not crash
    TEST_PASSED();
}

void test_reset(void) {
    TEST_START("Reset");

    String s = str_init("Da resettare");
    size_t prev_cap = s.capacity;
    str_reset(&s);

    assert(s.len == 0);
    assert(s.str != NULL);
    assert(s.str[0] == '\0');
    assert(s.capacity == prev_cap);

    str_destroy(&s);
    TEST_PASSED();
}

void test_cat_and_append(void) {
    TEST_START("Cat & Append (Auto-growth)");

    String s = str_empty();
    const char *alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (size_t i = 0; alphabet[i] != '\0'; i++) {
        bool ok = str_char(&s, alphabet[i]);
        assert(ok);
    }
    assert(str_cmpr(&s, alphabet) == 0);

    bool ok = str_catr(&s, " - 12345");
    assert(ok);
    assert(str_cmpr(&s, "abcdefghijklmnopqrstuvwxyz - 12345") == 0);

    String target = str_init("ac");
    String b = str_init("b");
    assert(str_cat(&target, &b, 1));
    assert(str_cmpr(&target, "abc") == 0);

    str_destroy(&b);
    str_destroy(&target);
    str_destroy(&s);
    TEST_PASSED();
}

void test_copy(void) {
    TEST_START("Copy (str_cpy)");

    String original = str_init("Test copia stringa");
    String copy = str_empty();

    bool ok = str_cpy(&copy, &original);
    assert(ok);
    assert(copy.len == original.len);
    assert(str_equals(&copy, &original));
    assert(copy.str != original.str); // indipendent buffers

    // secure self-copy
    assert(str_cpy(&copy, &copy));
    assert(str_cmpr(&copy, "Test copia stringa") == 0);

    str_destroy(&original);
    str_destroy(&copy);
    TEST_PASSED();
}

void test_compare_and_equals(void) {
    TEST_START("Compare & Equals");

    String s1 = str_init("mela");
    String s2 = str_init("mela");
    String s3 = str_init("pero");
    String s4 = str_init("melanzana");

    assert(str_equals(&s1, &s2) == true);
    assert(str_equals(&s1, &s3) == false);
    assert(str_cmp(&s1, &s2) == 0);
    assert(str_cmp(&s1, &s3) < 0);
    assert(str_cmp(&s3, &s1) > 0);
    assert(str_cmp(&s1, &s4) < 0);
    assert(str_cmpr(&s1, "mela") == 0);

    str_destroy(&s1);
    str_destroy(&s2);
    str_destroy(&s3);
    str_destroy(&s4);
    TEST_PASSED();
}

void test_contains(void) {
    TEST_START("Contains (Substrings & Sensitivity)");

    String hay = str_init("Il linguaggio C e fantastico!");
    String needle1 = str_init("linguaggio");
    String needle2 = str_init("LINGUAGGIO");
    String needle3 = str_init("inesistente");

    // case Sensitive
    ptrdiff_t idx = str_contains(&hay, &needle1, CASE_SENSITIVE);
    assert(idx == 3);
    assert(str_contains(&hay, &needle2, CASE_SENSITIVE) == -1);
    assert(str_contains(&hay, &needle3, CASE_SENSITIVE) == -1);

    // case Insensitive
    assert(str_contains(&hay, &needle2, NO_CASE_SENSITIVE) == 3);

    // needle longer than the text
    String long_needle = str_init("Il linguaggio C e fantastico! Ma davvero tanto lungo!!");
    assert(str_contains(&hay, &long_needle, CASE_SENSITIVE) == -1);

    // raw version str_containsr  
    assert(str_containsr(&hay, "fantastico", CASE_SENSITIVE) == 18);
    assert(str_containsr(&hay, "FANTASTICO", NO_CASE_SENSITIVE) == 18);

    str_destroy(&long_needle);
    str_destroy(&needle3);
    str_destroy(&needle2);
    str_destroy(&needle1);
    str_destroy(&hay);
    TEST_PASSED();
}

void test_format(void) {
    TEST_START("Formatting (str_from_fmt, append)");
    String s = str_from_fmt("Valore: %d, Float: %.2f", 42, 3.14);
    assert(str_cmpr(&s, "Valore: 42, Float: 3.14") == 0);

    bool ok = str_append_fmt(&s, " -> fine: %s", "OK");
    assert(ok);
    assert(str_cmpr(&s, "Valore: 42, Float: 3.14 -> fine: OK") == 0);

    str_destroy(&s);
    TEST_PASSED();
}

void test_trim_and_case(void) {
    TEST_START("Trim, Case & Reverse");
    String s = str_init("   \t  test di testo  \n  ");

    str_ltrim(&s);
    assert(str_cmpr(&s, "test di testo  \n  ") == 0);

    str_rtrim(&s);
    assert(str_cmpr(&s, "test di testo") == 0);

    str_to_upper(&s);
    assert(str_cmpr(&s, "TEST DI TESTO") == 0);

    str_to_lower(&s);
    assert(str_cmpr(&s, "test di testo") == 0);

    String rev = str_init("radar1");
    str_reverse(&rev);
    assert(str_cmpr(&rev, "1radar") == 0);

    str_destroy(&rev);
    str_destroy(&s);
    TEST_PASSED();
}

void test_substr_and_remove(void) {
    TEST_START("Substr & Remove");
    String s = str_init("0123456789");

    String sub = str_substr(&s, 3, 4);
    assert(str_cmpr(&sub, "3456") == 0);
    assert(sub.len == 4);

    bool ok = str_remove(&s, 3, 4);
    assert(ok);
    assert(str_cmpr(&s, "012789") == 0);
    assert(s.len == 6);

    char popped;
    ok = str_pop(&s, &popped);
    assert(ok && popped == '9');
    assert(str_cmpr(&s, "01278") == 0);

    str_destroy(&sub);
    str_destroy(&s);
    TEST_PASSED();
}

void test_replace(void) {
    TEST_START("Replace");
    String s = str_init("mela pera mela banana mela");

    bool ok = str_replace(&s, "mela", "kiwi");
    assert(ok);
    assert(str_cmpr(&s, "kiwi pera kiwi banana kiwi") == 0);

    ok = str_replace(&s, "kiwi", "fragola");
    assert(ok);
    assert(str_cmpr(&s, "fragola pera fragola banana fragola") == 0);

    str_destroy(&s);
    TEST_PASSED();
}

void test_predicates(void) {
    TEST_START("Predicates (starts, ends, empty, last)");
    String s = str_init("/var/log/syslog.tar.gz");

    assert(str_starts_with(&s, "/var"));
    assert(!str_starts_with(&s, "/etc"));
    assert(str_ends_with(&s, ".tar.gz"));
    assert(!str_ends_with(&s, ".zip"));
    assert(!str_is_empty(&s));

    String empty = str_empty();
    assert(str_is_empty(&empty));

    ptrdiff_t last_dot = str_find_last(&s, ".", CASE_SENSITIVE);
    assert(last_dot == 19);

    ptrdiff_t last_slash = str_find_last(&s, "/", CASE_SENSITIVE);
    assert(last_slash == 8);

    str_destroy(&empty);
    str_destroy(&s);
    TEST_PASSED();
}

void test_split_and_join(void) {
    TEST_START("Split & Join");
    String csv = str_init("admin,user,,guest");
    String *tokens = NULL;

    int count = str_split(&tokens, &csv, ',');
    assert(count == 4);
    assert(str_cmpr(&tokens[0], "admin") == 0);
    assert(str_cmpr(&tokens[1], "user") == 0);
    assert(str_cmpr(&tokens[2], "") == 0);
    assert(str_cmpr(&tokens[3], "guest") == 0);

    String joined = str_empty();
    bool ok = str_join(&joined, tokens, count, " | ");
    assert(ok);
    assert(str_cmpr(&joined, "admin | user |  | guest") == 0);

    str_free_split(tokens, count);
    str_destroy(&joined);
    str_destroy(&csv);
    TEST_PASSED();
}

int main(void) {
    printf("==================================================\n");
    printf("        SUITE DI TEST COMPLETA: libstr            \n");
    printf("==================================================\n");

    test_init_and_destroy();
    test_reset();
    test_cat_and_append();
    test_copy();
    test_compare_and_equals();
    test_contains();
    test_format();
    test_trim_and_case();
    test_substr_and_remove();
    test_replace();
    test_predicates();
    test_split_and_join();

    printf("==================================================\n");
    printf("\033[0;32mTUTTI I TEST SONO STATI SUPERATI CON SUCCESSO!\033[0m\n");
    printf("==================================================\n");
    return 0;
}
