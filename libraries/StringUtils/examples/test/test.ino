#include <Arduino.h>
#include <StringUtils.h>

void testParse(const char* list);
void testList(const su::List& list);
void testString();
void testCompare();
void testCompareValue();
void testValue();
template <typename T>
void printValue(T val);
void testB64_cstr(const char* str);
void testB64_str(const char* str);
void testQwerty(const char* str);
void testSize();
void testUrl(const char* str);
void testUnicode();
void testCast(const char* str);

void setup() {
    Serial.begin(115200);
    Serial.println();

    testParse("");
    testParse("1234;1;abcdef");
    testParse(";;45;;;abcd;;");

    testList("");
    testList("1234;1;abcdef");
    testList(";;45;;;abcd;;");
    testList(F("abc;123;456"));
    testList("123;456;abc");

    testCast("abcd");
    testCast("1234");
    testCast("-1234");
    testCast("3.14");
    testCast("true");

    testString();
    testCompare();
    testCompareValue();
    testValue();
    testUnicode();
    testQwerty("123 abc Привет");

    testUrl("hello 123! !@#$%^&*()_= 321");

    testB64_str("some text!");
    testB64_cstr("some text!");

    testSize();

    Serial.println(su::strlenRu("hello привет"));
}

void loop() {
}

void testCast(const char* str) {
    Serial.println("========= CAST =========");
    su::Text txt(str);

    // explicit
    Serial.println(String("toBool: ") + txt.toBool());
    Serial.println(String("toInt16: ") + txt.toInt16());
    Serial.println(String("toInt32: ") + txt.toInt32());
    // Serial.println(String("toInt64: ") + txt.toInt64());
    Serial.println(String("asFloat: ") + txt.toFloat());
    Serial.println(String("toStr: ") + txt.str());
    Serial.println(String("toString: ") + txt.toString());
    char buf[txt.length() + 1];
    txt.toStr(buf);
    Serial.println(String("to char: ") + buf);

    // implicit
    int8_t i8 = txt;
    uint8_t u8 = txt;
    int16_t i16 = txt;
    uint16_t u16 = txt;
    int32_t i32 = txt;
    uint32_t u32 = txt;
    // int64_t i64 = txt;
    // uint64_t u64 = txt;
    bool b = txt;
    float f = txt;
    double d = txt;
    String S = txt;

    Serial.println(String("int8_t: ") + i8);
    Serial.println(String("uint8_t: ") + u8);
    Serial.println(String("int16_t: ") + i16);
    Serial.println(String("uint16_t: ") + u16);
    Serial.println(String("int32_t: ") + i32);
    Serial.println(String("uint32_t: ") + u32);
    // Serial.println(String("int64_t: ") + i64);
    // Serial.println(String("uint64_t: ") + u64);
    Serial.println(String("bool: ") + b);
    Serial.println(String("float: ") + f);
    Serial.println(String("double: ") + d);
    Serial.println(String("String: ") + S);
    Serial.println();
    Serial.println();
}

void testParse(const char* list) {
    Serial.println("=== PARSE TEST ===");
    Serial.println(list);

    uint16_t len = strlen(list);
    char buf[len + 1];
    strcpy(buf, list);

    Serial.println("=== SPLITTER ===");
    su::Splitter spl(buf);
    for (uint8_t i = 0; i < spl.length(); i++) {
        Serial.print(i);
        Serial.print(": ");
        Serial.print(spl[i].str());
        Serial.print(" == ");
        Serial.print(spl[i]);
        Serial.print(" [");
        Serial.print(spl[i].length());
        Serial.println("]");
    }
    spl.restore();

    Serial.println("done!");

    Serial.println("=== PARSER ===");
    su::Parser p(buf);
    while (p.next()) {
        Serial.print(p.index());
        Serial.print(": ");
        Serial.print(p.str());
        Serial.print(", ");
        Serial.print(p.get());
        Serial.print(" [");
        Serial.print(p.get().length());
        Serial.println("]");
    }
    Serial.println("done!");

    Serial.print("buffer: ");
    Serial.println(list);
    Serial.println();
    Serial.println();
}

void testList(const su::List& list) {
    Serial.println("=== LIST ===");
    Serial.print("list: ");
    Serial.print(list);
    Serial.print(", len: ");
    Serial.println(list.length());
    Serial.print("index of '123':");
    Serial.println(list.indexOf(F("123")));
    Serial.print("index of '456':");
    Serial.println(list.indexOf("456"));
    // for (int i = 0; i < list.length(); i++) {
    //     Serial.println(list[i]);
    // }
    Serial.println();
    Serial.println();
}
void testString() {
    Serial.println("=== PrintString ===");
    su::PrintString ps;
    ps.print(123);
    ps.print(F(" test"));
    Serial.println(ps);
    Serial.println();
}
void testCompare() {
    // true
    Serial.println(su::Text("123") == su::Text(F("123")));
    Serial.println(su::Text("abc") == su::Text(F("abc")));
    Serial.println(su::Text("11111") == su::Text(F("11111")));
    Serial.println(su::Text("11111") == su::Text(F("11111")));

    // false
    Serial.println(su::Text(F("abc")) == su::Text(("123")));
    Serial.println(su::Text(("1234")) == su::Text(F("123")));
    Serial.println(su::Text(F("abc")) == su::Text(("abcd")));
}
void testCompareValue() {
    // true
    Serial.println(su::Value(12345) == su::Text(F("12345")));
    Serial.println(su::Value(3.14) == su::Text("3.14"));
    Serial.println(su::Value('c') == su::Text("c"));
    Serial.println(su::Value((uint64_t)12345654321) == su::Text("12345654321"));

    // false
    Serial.println(su::Value(123456) == su::Text(F("12345")));
    Serial.println(su::Value(3.0) == su::Text(F("3.14")));
    Serial.println(su::Value(0) == su::Text(F("")));
}
template <typename T>
void printValue(T val) {
    su::Value v(val);
    Serial.print(v);
    Serial.print(", len: ");
    Serial.println(v.length());
}
void testValue() {
    printValue("cstr");
    printValue(F("f str"));
    printValue(-1234);
    printValue((uint16_t)1234);
    printValue((uint32_t)123456789);
    printValue((uint64_t)12345678987654321);
    printValue('a');
    printValue(3.14);
}
void testUrl(const char* str) {
    String s;
    su::url::encode(str, s);
    Serial.println(s);
    Serial.println(su::url::decode(s));
}

void testSize() {
    Serial.println(su::charSize('f'));
    Serial.println(su::charSize("й"[0]));
    Serial.println(su::charSize("Ё"[0]));
}

void testQwerty(const char* str) {
    Serial.println(su::toQwerty(str));

    char qw[strlen(str) + 1];
    su::toQwerty(str, qw);
    Serial.println(qw);
}
void testB64_str(const char* str) {
    // encode
    String b64;
    su::b64::encode(&b64, (uint8_t*)str, strlen(str));
    Serial.println(b64);

    // decode
    uint16_t dlen = su::b64::decodedLen(b64.c_str(), b64.length());
    char dec[dlen + 1];
    su::b64::decode((uint8_t*)dec, b64.c_str(), b64.length());
    dec[dlen] = 0;
    Serial.println(dec);
}
void testB64_cstr(const char* str) {
    // encode
    uint16_t slen = strlen(str);
    uint16_t elen = su::b64::encodedLen(slen);
    char b64[elen + 1];
    su::b64::encode(b64, (uint8_t*)str, strlen(str));
    b64[elen] = 0;
    Serial.println(b64);

    // decode
    uint16_t dlen = su::b64::decodedLen(b64, strlen(b64));
    char dec[dlen + 1];
    su::b64::decode((uint8_t*)dec, b64, strlen(b64));
    dec[dlen] = 0;
    Serial.println(dec);
}
void testUnicode() {
    Serial.println(su::unicode::decode("hello \u0421\u0430\u043d\u044f!"));
    char sym[5];
    su::unicode::encode(sym, 0x2605);
    Serial.println(sym);
    Serial.println(su::unicode::encode(0x23F0));
}