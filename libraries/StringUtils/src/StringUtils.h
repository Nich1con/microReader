#pragma once

#include "utils/PrintString.h"
#include "utils/String.h"
#include "utils/Text.h"
#include "utils/TextList.h"
#include "utils/TextParser.h"
#include "utils/Value.h"
#include "utils/convert/b64.h"
#include "utils/convert/convert.h"
#include "utils/convert/qwerty.h"
#include "utils/convert/unicode.h"
#include "utils/convert/url.h"
#include "utils/hash.h"
#include "utils/list.h"
#include "utils/parser.h"
#include "utils/splitter.h"

// legacy
namespace sutil = su;
namespace su {
using AnyText = Text;
using AnyValue = Value;
using AnyTextList = TextList;
}  // namespace su