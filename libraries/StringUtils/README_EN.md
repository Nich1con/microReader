This is an automatic translation, may be incorrect in some places. See sources and examples!

# Stringutils
A set of tools for working with lines
- Fast conversion functions
- Parsing, dividing by dividers
- Several classes-converters of data into a line and vice versa for use in other libraries
- Coding and decoding Base64, URL, Unicode, Yzukeen/QWERTYS BACK

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [documentation] (#docs)
- [versions] (#varsions)
- [installation] (# Install)
- [bugs and feedback] (#fedback)

<a id="docs"> </a>

## Documentation
## SU :: Text
Class-wrapping for all types of lines.Can be created in the designer from:
- `" Consta Char "` - lines
- `char []` - lines
- `f (" f-string ")`
- `proGMEM` - lines
- `string` - lines

Peculiarities:
- Keeps the type and length of the line
- allows ** print **, ** to convert ** into any integer format and ** compare ** with the variables of all standard types, and also compare with any other lines
- Conclusion in tuning in different ways, search and separation
- ** cannot change the original line **, all operations are only "for reading"
- ** does not create a copy of the line ** and works with the original line, i.e.*The original line should be in memory during the existence of Text*
- If created from string lines, then the original string line should not change during the work of the Text copy

`` `CPP
// ====== Designer =========
SU :: Text (String & Str);
SU :: Text (Constation String & Str);
SU :: text (const __flashstringhelper* str, int16_t len = 0);
SU :: Text (const char* str, int16_t len = 0, bool pgm = 0);

// ======== System ==============
Bool Valid ();// Line status, exists or not
Bool pgm ();// Line from Flash memory
uint16_t Length ();// Line length
uint16_t Lengthonicode (); // Line length taking into account unicode symbols
uint16_t Readlen ();// Count and return the length of the line (const)
Void Calclen ();// Counting and remembering the length of the line (Non-CST)
Type type ();// Type of line
const char* str ();// Get the pointer to the line.Will always return the non -nuclear pointer
const char* end ();// Indicator at the end of the line.Will always return the non -nuclear pointer
Bool Terminated ();// The line is valid and ends \ 0

// ======== Hash ============
size_t hash ();// Hash lines size_t
uint32_t hash32 ();// Hash lines 32 bits

// ======== Print ==========
Size_t Printto (Print & P);// Print in Print (from length)

// ======== Comparison and search ===========
// compared with all types of lines through ==

// Compare with the line starting from the index
Bool Compare (Text S, Uint16_T from = 0);

// Compare with the line starting from the index, indicating the number of characters
Bool Comparen (Text S, Uint16_t Amount, Uint16_T From = 0);

// Find the position of the symbol in the line starting from the index
Int16_t Indexof (char, uint16_t from = 0);

// Find the position of the line in the line
Int16_t Indexof (Text Txt, Uint16_t From = 0);

// Find the position of the symbol in the line from the end
Int16_T Lastindexof (char system);

// Find the position of the line in the line from the end
Int16_t Lastindexof (Text Txt);

// Find a symbol and get a pointer to the first entry
COST Char* Find (Char Sym, Uint16_T FROM = 0);

// starts from the line
Bool Startswith (Const Text & Txt);

// ends with a line
Bool Endswith (Const Text & Txt);

// ======== Separation and Parsing ============
// will return a new line with cleaned test symbols from the beginning and end
Text trim ();

// Count the number of settings separated by the symbol (the number of separators +1)
Uint16_T Count (Char Sym);

// Count the number of tips separated by lines (number of separators +1)
Uint16_T Count (Text Txt);

// divide by the symbol-under the array of any type
uint16_t split (t* arr, uint16_t len, char div);

// Get a setting from the list on the index
Text getsub (Uint16_T IDX, Char Div);

// Get a setting from the list on the index
Text getsub (Uint16_T IDX, Text Div);

// Highlight the tuning (beginning, the end is not including).Negative indices work from the end of the line
Text Substring (Int16_T Start, Int16_t End = 0);

// Get a symbol by index
Char charat (Uint16_T IDX);

// ======== conclusion.Lines =========
// Get like string string
String Tostring (Bool Decodeunicode = False);

// Bring out a string line.Will return FALSE in case of failure
Bool Tostring (String & S, Bool Decodeunicode = False);

// Add to string line.Will return FALSE in case of failure
Bool Addstring (String & S, Bool Decodeunicode = False);

// Bring out an array.Return the length of the line.Terminate - complete the line with zero
uint16_t Tostr (char* buf, int16_t bufsize = -1, bool terminate = true);

// ======== conclusion.B64 =========
// data size (byte) if they are b64
Size_T SIZEB64 ();

// Bend from b64
Bool Decodeb64 (Void* var, size_t size);

// ======== conclusion.Numbers ========
Bool Tobool ();// get a value as bool
int16_t toint16 ();// get a value as int16
int32_t toint32 ();// get a value as int32
int64_t toint64 ();// get a value as int64
uint32_t toint32hex ();// get a value as uint 32 from HEX line
Float Tofloat ();// get a value as Float

// is also automatically converted and compared with
Bool
Char + Unsigned
Short + Unsigned
Int + Unsigned
Long + Unsigned
Long Long + Unsigned
Float
Double
String

// for manual line control
const Char* _str;// indicator to the line
uint16_t _len;// length
`` `

#### Example
`` `CPP
// Designer
SU :: Text V0 ("-123456");
SU :: text v1 = "-123456";
v1 = f ("-123456");
String S ("ABCD");
SU :: text v2 (s);
v2 = s;

// Comparison
v2 == v1;
v2 == F ("Text");
v1 == -123456;

// Auto Convertation
int V = v0;
String S2 = V2;

// Conclusion in the array
Char Buf [20];
v1.tostr (buf);

// Parsing and separation
SU :: TEXT LIST ("ABC/123/Def");
Serial.println (libetsub (0, '/'));// ABC
Serial.println (libetsub (2, '/'));// Def

Serial.println (list.substring (4, 6));// 123

SU :: Text Arr [3];
list.split (arr, 3, '/');
Serial.println (arr [0]);
Serial.println (arr [1]);
Serial.println (arr [2]);

// you can parse in any type
int arr2 [3];// Float, byte ...
list.split (arr2, 3, '/');
Serial.println (arr2 [0]);
Serial.println (arr2 [1]);
Serial.println (arr2 [2]);

// you can't do this
Text T1 (String ("123"));// The line will be unloaded from memory!
// t1 .... the program will break

String S;
Text T1 (S);
S += string ("123");// The line address will change!
// t1 .... the program will break

// at the same time like this - you can
VOID FOO (COST Text & Text) {
    // String exists here
    Print (Text);
}
FOO (String ("123"));
`` `

The built-in separator and hash functions allow you to very simply and effectively disassemble various text protocols.For example, a package of the type `key = value`, where` key` can be sent to a variable in the code.The package can be divided, the key has shit and interviewed through Switch to assign an alive variable:
`` `CPP
Text Txt ("Key1 = 1234");
int val = txt.getsub (1, '=');// Value in int

switch (txt.getsub (0, '='). Hash ()) {// Key hash
    Case Su :: SH ("Key1"):
        VAR1 = Val;
        Break;
    Case Su :: SH ("Key2"):
        Var2 = Val;
        Break;
    Case Su :: SH ("Key3"):
        Var2 = Val;Break;
}
`` `

Or a protocol of the type `name/index/value`, where` name` is a text key, `index` - serial number:
`` `CPP
Text Txt ("Key/3/1234");

into val = txt.getsub (2, '/');

switch (txt.getsub (0, '/'). Hash ()) {
    Case Su :: SH ("Key"):
        switch (txt.getsub (1, '/'). Toint16 ()) {
            Case 0: Break;
            Case 1: Break;
            Case 2: Break;
            // .....
        }
        Break;
    Case Su :: SH ("KeyKey"):
        // ...
        Break;
    Case Su :: SH ("Anotherkey"):
        // ...
        Break;
}
`` `

## SU :: Value
The additive to `text` supports all other standard data types.It has a 22 byte buffer, when creating it converts the number into it:
`` `CPP
SU :: Value (Bool Value);
SU :: Value (Char + Unsigned Value, Uint8_t Base = Dec);
SU :: Value (Short + Unsigned Value, Uint8_t Base = Dec);
SU :: value (int + Unsigned Value, Uint8_t Base = Dec);
SU :: Value (Long + Unsigned Value, Uint8_t Base = Dec);
SU :: Value (Long Long + Unsigned Value, Uint8_t Base = Dec);
SU :: Value (Double Value, Uint8_t Dec = 2);

// similarly with a manual sizes of the buffer
SU :: valuet <buffer size> ();
`` `

#### Example
`` `CPP
SU :: Value V0 ("-123456");// all lines can also be
SU :: Value V1 (123);
SU :: Value V2 (3.14);
SU :: Value V3 ((Uint64_T) 12345678987654321);

// converted from among the text
v1 = 10;
v1 = 3.14;
V1 = 12345654321ULL;

Serial.println (v0);// printed in Serial
Serial.println (v1 == v2);// compared

// compared with any line
SU :: Text S ("123");
String ss = "123";
Serial.println (s == "123");
Serial.println (s == f ("123"));
Serial.println (s == SS);

// converted into any type
Float F = V2;// f == 3.14
int i = v1;// I = 123

// displayed in string
String S;
v0.tostring (s);

// displayed in Char []
Char buf [v1.length () + 1];// +1 for '\ 0'
v1.tostr (buf);
`` `

#### Use in libraries
The following libraries are built on the basis of `text`:
- [gson] (https://github.com/gyverlibs/gson)
- [gyverhub] (https://github.com/gyverlibs/gyverhub)
- [pairs] (https://github.com/gyverlibs/pars)
- [Fastbot2] (https://github.com/gyverlibs/fastbot2)

##### The Transfer of Text to Function
- lines of any type
- without allocations, which is extremely critical when assembling string
- without creating a dozen overloaded functions

For example, we need a function taking a line in any form.In the vanilla framework Arduino you can do this:
`` `CPP
VOID Settext
    // and for example add to the line
    S += str;
}
`` `

Such a function can take any lines:
`` `CPP
Settext ("Constal");
Settext (F ("F-String"));

Char str [] = "Buffer string";
Settext (str);

String s = "arduino string";
settext (s);
`` `
But this line will be * duplicated * in the constructor `string`, and the worst thing is in dynamic memory!Thus, when adding to conditionally-global string in this area of definition, re-alolocation and fragmentation of memory will occur.`Text` allows you to completely avoid this:

`` `CPP
VOID Settext
    // and for example add to the line
    str.Addstring (s);
}
`` `
Now this function also knows how to take lines in any format, but ** does not create their copies **, and for example, adding to the line becomes quick and safe.

##### the text of the text
TEXT is also convenient for withdrawal, for example, in the class that stores the buffer and fills it with data and knows their length:

`` `CPP
Class myclass {
    Public:
    SU :: text get () {
        Return Su :: Text (Buffer, Len);
    }

    Private:
    Char Buffer [20];
    int len;
};

Myclass S;
Serial.println (S.Get ());
`` `

Inheritance option:
`` `CPP
Class Myclass: Public Su :: Text {
    Public:
    VOID FOO () {
        SU :: Text :: _ str = buffer;
        SU :: Text :: _ len = somelen;
    }

    Private:
    Char Buffer [20];
};

Myclass S;
Serial.println (s);
`` `

If instead of `Text` use` value`, the function can also take any numerical data.

## SU :: Textlist
Diviner of `Text` lists on` Text` tuning.

### static
`` `CPP
Textlistt <int16_t cap> (Text List, Char Div);
Textlistt<int16_t cap> (Text List, Text Div);

// number of construction
uint16_t Length ();

// buffer size
Uint16_T Capacy ();

// Get the tuning under the index
COST Text & Get (Uint16_T IDX);
constaT & operator [] (int IDX);
`` `

### dynamic
`` `CPP
Textlist (Text List, Char Div);
Textlist (Text List, Text Div);
// number of construction
uint16_t Length ();

// Get the tuning under the index
COST Text & Get (Uint16_T IDX);
constaT & operator [] (int IDX);
`` `

### outdated parsers
<details>
<summary> Expand </summary>

## SU :: Parser
Separation of the line into tips according to the separator in the cycle.** Changes ** the initial line, but after completing it returns the separators to the place.

`` `CPP
SU :: Parser P (String & Str, Char Div = ';');
SU :: Parser p (const char* str, char div = ';');

Bool Next ();// Parish the next tuning.Will return FALSE if Parsing is over
uint8_t index ();// current tuning index
const char* str ();// Get tuning
Text get ();// Get a tone as a Text
`` `

#### Example
`` `CPP
Char Buf [] = "123; 456; ABC";

SU :: Parser P (BUF);
While (p.next ()) {
    Serial.print (P.index ());
    Serial.print (":");
    Serial.println (P.Get ());
}
`` `

## SU :: Splitter
Separation of the line into tips according to the separator in the cycle.** Changes ** the original line!After removing the object, the line is restored, or manually call `RESTORE ()`
`` `CPP
SU :: Splittert <Max.Settings> SPL (string & str, char div = ';');
SU :: Splittert <Max.Settings> SPL (const char* str, char div = ';');

SU :: Splitter Spl (String & Str, Char Div = ';');// Auto-size (released in Heap)
SU :: Splitter SPL (Consta Char* str, char div = ';');// Auto-size (released in Heap)

VOID Setdiv (Char Div);// Install the separator
VOID restore ();// Restore the line (return the dividers)
uint8_t leength ();// Number of tuning
const char* str (uint16_t IDX);// Get the tuning on the index
Text Get (Uint16_T IDX);// Get a t in the index as a Text
`` `

#### Example
`` `CPP
Char Buf [] = "123; 456; ABC";

SU :: Splitter SPL (BUF);
for (uint8_t i = 0; i <spl.length (); i ++) {
    Serial.print (i);
    Serial.print (":");
    Serial.println (spl [i]);
}

Spl.Restore ();
`` `

### SU :: List Functions
`` `CPP
// Get the number of tuning in the list
Uint16_T SU :: list :: Length (Text List, Char Div = ';');

// Get the tuning index in the list
Int16_T SU :: list :: indexof (Text List, Text Str, Char Div = ';');

// Checking whether the list contains the list
Bool Su :: List :: includes (Text List, Text Str, Char Div = ';');

// Get a setting from the list on the index
TEXT SU :: list :: Get (Text List, Uint16_T IDX, Char Div = ';');

// Parrow in an array of the specified type and size.Will return the number of recorded tuning
TEMPLATE <TYPENAME T>
uint16_t SU :: list :: Parse (Text List, T* Buf, Uint16_t Len, Char Div = ';');
`` `

#### Example
`` `CPP
Serial.println (SU :: list :: Length ("123; 456; 333"));// 3
Serial.println (SU :: list :: includes ("123; 456; 333", "456"));// True
Serial.println (SU :: list :: indexof ("123; 456; 333", "333"));// 2
Serial.println (SU :: list :: get ("123; 456; 333", 1));// 456

// Parish in an array
Float Arr [3];
SU :: list :: parse ("3.14; 2.54; 15.15"), Arr, 3);
`` `

## SU :: LIST CLASS
Obtaining a settings by dividers ** without modifying the original line ** also works with Progmem lines.
`` `CPP
List (Text);

// Install the separator
VOID Setdiv (Char Div);

// get the size of the list
uint16_t Length ();

// Get the tuning index in the list or -1 if it is not
Int16_t Indexof (Text Str);

// Check the availability of tuning in the list
Bool Includes (Text Str);

// Get the tuning under the index
Text Get (Uint16_T IDX);

// Parrow in an array of the specified type and size.Will return the number of recorded tuning
TEMPLATE <TYPENAME T>
uint16_t Parse (t* buf, uint16_t len);
`` `

#### Example
`` `CPP
SU :: List List ("" 123; 456; 333 "));
Serial.print ("len:");
Serial.println (list.length ());// 3
Serial.print ("index 2:");
Serial.println (list [2]);// 333
Serial.print ("index of '456':");
Serial.println (list.indexof (f ("456")));// 1
Serial.print ("Index of '789':");
Serial.println (List.indexof ("789"));// -1

// rewrite in an array
int arr [3];
List.parse (Arr, 3);
`` `
</details>

## SU :: Printstring
`` `CPP
// line in which you can make Print/Println
SU :: Printstring PRS;
PRS += "As a regular string";

PRS.Print (10);
PRS.println ("Hello");
PRS.Print (F ("Print!"));

Serial.println (PRS);
`` `

## qwerty
`` `CPP
// change the layout (ru in Qwerty) - String
String SU :: TOQWERTY (COST String & RU);

// Change the layout (ru in QWERTY) - Char* (QW length as ru + 1, the function will add '\ 0')
Char* SU :: Toqwerty (Const Char* Ru, Char* QW);
`` `

### Base64
`` `CPP
// The size of coded data in terms of source
Size_T SU :: B64 :: Encodedlen (Size_t Len);

// Future size of decoded data on line b64 and its length
Size_T SU :: b64 :: decodedlen (const char* b64, size_t len);

// encode data in string
VOID SU :: B64 :: Encode (String* B64, Uint8_T* DATA, SIZE_T LEN, BOL PGM = FALSE);

// encode data in char [] (the library does not add '\ 0' to the end)
VOID SU :: B64 :: Encode (Char* B64, Uint8_T* DATA, SIZE_T LEN, BOOL PGM = FALSE);

// Express data from line B64 to DATA buffer
VOID SU :: b64 :: decode (uint8_t* data, constel* b64, size_t len);
VOID SU :: b64 :: decode (uint8_t* Data, Consta String & B64);
`` `

### Unicode
Decider of a line containing unicode symbols of the type `\ u0abc`.Also makes UNESCAPE symbols `\ t \ r \ n`!
`` `CPP
// Decoding the line. Remember the line for LEN length.Otherwise - along the length of the line
String SU :: unicode :: decode (const char* str, uint16_t len = 0);

// Decoding the line
String Su :: Unicode :: Decode (Const String & Str);

// Cod the Unicode symbol by its code.There should be 5 cells in the array
VOID SU :: unicode :: Encode (char* str, uint32_t c);

// Coding Unicode symbol by its code
String Su :: Unicode :: Encode (uint32_t code);
`` `

### url
`` `CPP
// Symbol should be urlencode
Bool Su :: url :: NEEDSENCODE (Char C);

// encode in the URL
VOID SU :: url :: Encode (Consta Char* SRC, Uint16_t Len, String & Dest);
VOID SU :: url :: Encode (Const String & SRC, String & Dest);
String SU :: url :: Encode (Const String & SRC);

// Report the URL
VOID SU :: url :: decode (Consta Char* SRC, UINT16_T LEN, String & DEST);
VOID SU :: url :: decode (Const String & SRC, String & Dest);
String SU :: url :: decode (Const String & SRC);
`` `

### Hash
Instead of comparing the lines, you can compare the hash of these lines, which makes the program more compact, lighter and in most cases faster.The functions indicated below as “is considered a compiler” are considered a compiler, that is, ** the line does not even get into the program ** - a hash -piece is substituted instead:

`` `CPP
// is considered a compiler
Contexpr SU :: Size_t Su :: SH (const chaar* str);// (String Hash) Size_t size
Contexpr SU :: size_t SH32 (COST Char* str);// (String Hash) Size 32 bits

// is considered in the radim
Size_t Su :: Hash (const char* str, int16_t len = -1);// size depends on the platform and corresponds to size_t
uint32_t SU :: hash32 (const char* str, int16_t len = -1);// size 32 bits

Size_t Su :: hash_p (pgm_p str, int16_t len = -1);// Progmem line, size size_t
uint32_t SU :: hash32_p (pgm_p str, int16_t len = -1);// Progmem line, size 32 bits
`` `

> On the ESP board `sh`,` hash` and `hash_p` are 32-bit by default!

The 32 -bit version of the hash has 7 collisions of 234450 English words, 16 -bit version - 170723 collisions (which is 73% - a purely statistical number of conflicts from the calculation of 16 bits - 65536)

#### Example
Determine which line was "received".Classic method with comparing rows from Progmem:

`` `CPP
Char buf [] = "some_text";

if (! strcmp_p (buf, pstr ("abcDef"))) serial.println (0);
Else if (! Strcmp_p (BUF, PSTR ("12345"))) serial.println (1);
Else if (! Strcmp_p (BUF, PSTR ("Wrong Text"))) serial.println (2);
Else if (! Strcmp_p (BUF, PSTR ("SOME Text"))) serial.println (3);
Else if (! Strcmp_p (Buf, Pstr ("Hello"))) SErial.println (4);
Else if (! Strcmp_p (BUF, PSTR ("SOME_TEXT"))) serial.PRINTLN (5);
`` `

Method with Hash Line:
`` `CPP
Using SU :: SH;
Using SU :: HASH;

Char buf [] = "some_text";

switch (hash (buf)) {
    Case Su :: SH ("ABCDEF"): Serial.Println (0);Break;
    Case Su :: SH ("12345"): serial.println (1);Break;
    Case Su :: SH ("Wrong Text"): Serial.println (2);Break;
    Case Su :: SH ("Some Text"): Serial.println (3);Break;
    Case Su :: SH ("Hello"): serial.println (4);Break;
    Case Su :: SH ("SOME_TEXT"): Serial.PRINTLN (5);Break;
}
`` `
> One calculation of the hash takes a little more time than a comparison with a line.But the final design from the example is 2 times faster (on ESP).

> `SH (" lines ")` in this example do not fall into the program code at all - their hash is substituted instead of them

### other utilities
`` `CPP
// Line length with Russian symbols
uint16_t SU :: strlenru (const chaar* str);

// Get the length of the whole number
uint8_t SU :: intlen (int32_t val);

// Get the length of Float numbers
uint8_t SU :: floatlen (Double Val, Uint8_t Dec);

// Transform the line into a whole number
TEMPLATE <TYPENAME T>
T Su :: strtoint (const char* str, uint8_t len = 0);

// Transforming ProgmemMem a line into a whole number
TEMPLATE <TYPENAME T>
T Su :: Strtoint_p (Consta Char* str, uint8_t len = 0);

// Transform Float into a line indicating the number of signs after the point
uint8_t SU :: floattostr (Double val, char* buf, uint8_t dec = 2);

// Transform HEX line into a whole number.Optional length
Uint32_T SU :: Strtointhex (const char* str, int8_t len = -1);

// The length of the symbol in the number of bytes
uint8_t SU :: Charsize (Char Sym);

// Conversion of the number in Char* array.Writes from the beginning of the array, adds '\ 0', will return the length of the line
// for int64 max.Boofer length - 22 characters, for Int32 - 12
uint8_t SU :: uinttostr (uint32_t n, char* buf, uint8_t base = dec);
uint8_t SU :: inttostr (int32_t n, char* buf, uint8_t base = dec);
uint8_t SU :: uint64tostr (uint64_t n, char* buf, uint8_t base = dec);
uint8_t SU :: int64tostr (int64_t n, char* buf, uint8_t base = dec);

// Convertation from a line in Float
Float Su :: Strtofloat (Consta Char* S);

// Convertation from Progemem Lines to Float
Float SU :: strtofloat_p (pgm_p s);

// Quick integer logarithm 10 (length of the number in the number of symbols)
uint8_t SU :: getlog10 (uint32_t value);
uint8_t SU :: getlog10 (int32_t value);

// quick construction 10 to degree
uint32_t SU :: getpow10 (uint8_t value);
`` `

<a id="versions"> </a>

## versions
- V1.0
- v1.1.0 - optimization, features have been added, vulnerabilities are fixed
- V1.2 - Added hash functions
- v1.2.x - minor corrections and improvements
- V1.3 - Optimization COST, FICHES, comparisons have been added
- v1.3.1 - added substring
- v1.3.2 - support ESP8266 versions 2.x
- v1.3.5 - uinttostr: HEX is now in the lower register like Si -functions
- V1.3.6 - TOINT32HEX (), Count (), Split () and Getsub () are added to Text.Added Parser Textlist
- v1.3.7 - Boarins are fixed on avr
- v1.4.0 - Anytext is renamed Text, Sutil Names Space - SU, added functions in Text, the result of converting and substring are given to SI and JS functions

<a id="install"> </a>

## Installation
- The library can be found by the name ** stringutils ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/stringutils/archive/refs/heads/main.zip) .Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I always recommend an updateLibrary: In the new versions, errors and bugs are corrected, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code