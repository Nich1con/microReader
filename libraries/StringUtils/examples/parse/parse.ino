#include <Arduino.h>
#include <StringUtils.h>

// тест всех Text парсеров и разделителей

void setup() {
    Serial.begin(115200);
    Serial.println();

    // ================ DOUBLE PARSER ================
    su::Text t("123;456\nabc;def;ghk\n333;444");

    for (su::TextParser row(t, '\n'); row.parse();) {
        Serial.print("row #");
        Serial.println(row.index());
        for (su::TextParser col(row, ';'); col.parse();) {
            Serial.print(col);
            Serial.print(',');
        }
        Serial.println();
    }

    // ================ SINGLE PARSER ================

    su::Text txtC(";123;;456;");
    su::Text txtT("::123::::456::");

    // ================ TEXTLIST ================
    // char
    {
        Serial.print("TextList: ");
        su::TextList list(txtC, ';');
        for (int i = 0; i < list.length(); i++) {
            Serial.print(list[i]);
            Serial.print(',');
        }
        Serial.println();
    }

    // text
    {
        Serial.print("TextList: ");
        su::TextList list(txtT, "::");
        for (int i = 0; i < list.length(); i++) {
            Serial.print(list[i]);
            Serial.print(',');
        }
        Serial.println();
    }

    // ================ TEXTPARSER ================
    // char
    Serial.print("TextParser: ");
    for (su::TextParser p(txtC, ';'); p.parse();) {
        Serial.print(p);
        Serial.print(',');
    }
    Serial.println();

    // text
    Serial.print("TextParser: ");
    for (su::TextParser p(txtT, "::"); p.parse();) {
        Serial.print(p);
        Serial.print(',');
    }
    Serial.println();

    // ================ SPLIT ================
    // ================ char div ================
    {
        int count = txtC.count(';');
        Serial.println(count);

        // get sub
        Serial.print("sub: ");
        for (int i = 0; i < count; i++) {
            Serial.print(txtC.getSub(i, ';'));
            Serial.print(',');
        }
        Serial.println();

        // parse to text
        {
            su::Text arr[5];
            int am = txtC.split(arr, 5, ';');
            Serial.println(am);
            Serial.print("text: ");
            for (int i = 0; i < am; i++) {
                Serial.print(arr[i]);
                Serial.print(',');
            }
            Serial.println();
        }

        // parse to ints
        {
            int arr[5];
            int am = txtC.split(arr, 5, ';');
            Serial.println(am);
            Serial.print("ints: ");
            for (int i = 0; i < am; i++) {
                Serial.print(arr[i]);
                Serial.print(',');
            }
            Serial.println();
        }

        // parse to vars
        {
            int a, b, c, d, e;
            int* p[] = {&a, &b, &c, &d, &e};
            txtC.split(p, 5, ';');
            Serial.print("vars: ");
            Serial.print(String() + a + ',' + b + ',' + c + ',' + d + ',' + e);
        }
    }
    Serial.println();

    // ================ text div ================
    {
        int count = txtT.count("::");
        Serial.println(count);

        // get sub
        Serial.print("sub: ");
        for (int i = 0; i < count; i++) {
            Serial.print(txtT.getSub(i, "::"));
            Serial.print(',');
        }
        Serial.println();

        // parse to text
        {
            su::Text arr[5];
            int am = txtT.split(arr, 5, "::");
            Serial.println(am);
            Serial.print("text: ");
            for (int i = 0; i < am; i++) {
                Serial.print(arr[i]);
                Serial.print(',');
            }
            Serial.println();
        }

        // parse to ints
        {
            int arr[5];
            int am = txtT.split(arr, 5, "::");
            Serial.println(am);
            Serial.print("ints: ");
            for (int i = 0; i < am; i++) {
                Serial.print(arr[i]);
                Serial.print(',');
            }
            Serial.println();
        }

        // parse to vars
        {
            int a, b, c, d, e;
            int* p[] = {&a, &b, &c, &d, &e};
            txtT.split(p, 5, "::");
            Serial.print("vars: ");
            Serial.print(String() + a + ',' + b + ',' + c + ',' + d + ',' + e);
        }
    }
    Serial.println();
}

void loop() {
}