#include <Arduino.h>
#include <StringUtils.h>

template <typename T>
void __print(const T& arg) {
    Serial.print(arg);
    Serial.print('\t');
}

template <typename... Args>
void LOG(const Args&... args) {
    (__print(args), ...);
    Serial.println();
}

#define TEST_TEXT "1234.5abcd"
#define TEST_TEXT_SH "1234.5abc"
#define TEST_TEXT_LNG "1234.5abcde"

void test(const su::Text& text, const char* type) {
    LOG(F("============== TESTING =============="));
    LOG("text type:", type);
    LOG("pgm:", text.pgm());
    LOG("length:", text.length());
    LOG("readLen:", text.readLen());
    LOG("str:", text.str());
    LOG("end:", text.end());
    LOG("valid:", text.valid());
    LOG("terminated:", text.terminated());

    su::PrintString pr;
    pr.print(text);
    LOG("print:", pr == TEST_TEXT);

    {
        const char* cmpc = TEST_TEXT;
        const __FlashStringHelper* cmpf = F(TEST_TEXT);
        String cmps(TEST_TEXT);
        const su::Text txtc(cmpc);
        const su::Text txtf(cmpf);
        const su::Text txts(cmps);

        LOG("compare cstr:", text.compare(cmpc));
        LOG("compare f:", text.compare(cmpf));
        LOG("compare s:", text.compare(cmps));
        LOG("compare cstr:", text.compare(txtc));
        LOG("compare f:", text.compare(txtf));
        LOG("compare s:", text.compare(txts));

        LOG("compare N cstr:", text.compareN(cmpc, 5));
        LOG("compare N f:", text.compareN(cmpf, 5));
        LOG("compare N s:", text.compareN(cmps, 5));
        LOG("compare N cstr:", text.compareN(txtc, 5));
        LOG("compare N f:", text.compareN(txtf, 5));
        LOG("compare N s:", text.compareN(txts, 5));
    }
    {
        const char* cmpc = TEST_TEXT_SH;
        const __FlashStringHelper* cmpf = F(TEST_TEXT_SH);
        String cmps(TEST_TEXT_SH);
        const su::Text txtc(cmpc);
        const su::Text txtf(cmpf);
        const su::Text txts(cmps);

        LOG("compare SH cstr:", !text.compare(cmpc));
        LOG("compare SH f:", !text.compare(cmpf));
        LOG("compare SH s:", !text.compare(cmps));
        LOG("compare SH cstr:", !text.compare(txtc));
        LOG("compare SH f:", !text.compare(txtf));
        LOG("compare SH s:", !text.compare(txts));
    }
    {
        const char* cmpc = TEST_TEXT_LNG;
        const __FlashStringHelper* cmpf = F(TEST_TEXT_LNG);
        String cmps(TEST_TEXT_LNG);
        const su::Text txtc(cmpc);
        const su::Text txtf(cmpf);
        const su::Text txts(cmps);

        LOG("compare LN cstr:", !text.compare(cmpc));
        LOG("compare LN f:", !text.compare(cmpf));
        LOG("compare LN s:", !text.compare(cmps));
        LOG("compare LN cstr:", !text.compare(txtc));
        LOG("compare LN f:", !text.compare(txtf));
        LOG("compare LN s:", !text.compare(txts));
    }

    LOG("indexOf '3':", text.indexOf('3') == 2);
    LOG("charAt 3:", text.charAt(3) == '4');
    String s("some String+");
    text.addString(s);
    LOG("addString:", s == (String("some String+") + TEST_TEXT));
    LOG("toString:", text.toString() == TEST_TEXT);
    char buf[text.length() + 1];
    text.toStr(buf);
    LOG("toStr:", !strcmp(buf, TEST_TEXT));

    LOG("toBool:", text.toBool() == 1);
    LOG("toInt16:", text.toInt16() == 1234);
    LOG("toInt32:", text.toInt32() == 1234);
    LOG("toFloat:", text.toFloat() == 1234.5);

    // 1234.5abcd
    LOG("substring:", text.substring(0) == TEST_TEXT);
    LOG("substring:", text.substring(-2) == "cd");
    LOG("substring:", text.substring(3, 6) == "4.5");
    LOG("substring:", text.substring(3, -3) == "4.5a");
    LOG("substring:", text.substring(-3, 3) == "4.5a");

    su::Text arr[2];
    text.split(arr, 2, '.');
    LOG("split:", arr[0] == "1234");
    LOG("split:", arr[1] == "5abcd");
    LOG("count:", text.count('.') == 2);

    text.split(arr, 2, "4.5");
    LOG("split:", arr[0] == "123");
    LOG("split:", arr[1] == "abcd");
    LOG("count:", text.count("4.5") == 2);

    LOG("getSub:", text.getSub(0, '.') == "1234");
    LOG("getSub:", text.getSub(1, '.') == "5abcd");

    LOG("getSub:", text.getSub(0, "4.5") == "123");
    LOG("getSub:", text.getSub(1, "4.5") == "abcd");

    LOG("startsWith:", text.startsWith(TEST_TEXT_LNG) == 0);
    LOG("startsWith:", text.startsWith(TEST_TEXT) == 1);
    LOG("startsWith:", text.startsWith(su::Text(TEST_TEXT)) == 1);
    LOG("startsWith:", text.startsWith("1234.") == 1);
    LOG("startsWith:", text.startsWith("abc") == 0);
    LOG("startsWith f:", text.startsWith(F("1234.")) == 1);
    LOG("startsWith f:", text.startsWith(F("abc")) == 0);
    LOG("startsWith t:", text.startsWith(su::Text("1234.")) == 1);
    LOG("startsWith t:", text.startsWith(su::Text("abc")) == 0);

    LOG("endsWith:", text.endsWith(TEST_TEXT_LNG) == 0);
    LOG("endsWith:", text.endsWith(TEST_TEXT) == 1);
    LOG("endsWith:", text.endsWith(su::Text(TEST_TEXT)) == 1);
    LOG("endsWith:", text.endsWith("abcd") == 1);
    LOG("endsWith:", text.endsWith("123") == 0);
    LOG("endsWith f:", text.endsWith(F("abcd")) == 1);
    LOG("endsWith f:", text.endsWith(F("123")) == 0);
    LOG("endsWith t:", text.endsWith(su::Text("abcd")) == 1);
    LOG("endsWith t:", text.endsWith(su::Text("123")) == 0);

    LOG("indexOf:", text.indexOf(TEST_TEXT_LNG) == -1);
    LOG("indexOf:", text.indexOf(TEST_TEXT) == 0);
    LOG("indexOf:", text.indexOf(su::Text(TEST_TEXT)) == 0);
    LOG("indexOf:", text.indexOf(".5") == 4);
    LOG("indexOf:", text.indexOf("ff") == -1);
    LOG("indexOf f:", text.indexOf(F(".5")) == 4);
    LOG("indexOf f:", text.indexOf(F("ff")) == -1);
    LOG("indexOf t:", text.indexOf(su::Text(".5")) == 4);
    LOG("indexOf t:", text.indexOf(su::Text("ff")) == -1);

    LOG("lastIndexOf:", text.lastIndexOf(TEST_TEXT_LNG) == -1);
    LOG("lastIndexOf:", text.lastIndexOf(TEST_TEXT) == 0);
    LOG("lastIndexOf:", text.lastIndexOf(su::Text(TEST_TEXT)) == 0);
    LOG("lastIndexOf:", text.lastIndexOf(".5ab") == 4);
    LOG("lastIndexOf:", text.lastIndexOf("ff") == -1);
    LOG("lastIndexOf f:", text.lastIndexOf(F(".5ab")) == 4);
    LOG("lastIndexOf f:", text.lastIndexOf(F("ff")) == -1);
    LOG("lastIndexOf t:", text.lastIndexOf(su::Text(".5ab")) == 4);
    LOG("lastIndexOf t:", text.lastIndexOf(su::Text("ff")) == -1);
}

void setup() {
    Serial.begin(115200);
    test(su::Text(), "null");
    test("", "empty string");

    test(TEST_TEXT, "cstr");
    test(su::Text(TEST_TEXT "plus", strlen(TEST_TEXT)), "long cstr");

    test(F(TEST_TEXT), "f str");
    test(su::Text(F(TEST_TEXT "plus"), strlen(TEST_TEXT)), "long fstr");

    String s(TEST_TEXT);
    test(s, "String");
    test(String(TEST_TEXT), "const String");
    Serial.println(F("========== DONE =========="));
}

void loop() {
}