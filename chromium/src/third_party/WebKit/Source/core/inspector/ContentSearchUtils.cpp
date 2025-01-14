/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY GOOGLE INC. AND ITS CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GOOGLE INC.
 * OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "core/inspector/ContentSearchUtils.h"

#include "core/platform/text/RegularExpression.h"
#include <wtf/StdLibExtras.h>

using namespace std;

namespace WebCore {
namespace ContentSearchUtils {

namespace {
// This should be kept the same as the one in front-end/utilities.js
static const char regexSpecialCharacters[] = "[](){}+-*.,?\\^$|";
}

static String createSearchRegexSource(const String& text)
{
    String result;
    const UChar* characters = text.characters();
    String specials(regexSpecialCharacters);

    for (unsigned i = 0; i < text.length(); i++) {
        if (specials.find(characters[i]) != notFound)
            result.append("\\");
        result.append(characters[i]);
    }

    return result;
}

static inline size_t sizetExtractor(const size_t* value)
{
    return *value;
}

TextPosition textPositionFromOffset(size_t offset, const Vector<size_t>& lineEndings)
{
    const size_t* foundLineEnding = approximateBinarySearch<size_t, size_t>(lineEndings, lineEndings.size(), offset, sizetExtractor);
    size_t lineIndex = foundLineEnding - &lineEndings.at(0);
    if (offset > *foundLineEnding)
        ++lineIndex;
    size_t lineStartOffset = lineIndex > 0 ? lineEndings.at(lineIndex - 1) + 1 : 0;
    size_t column = offset - lineStartOffset;
    return TextPosition(OrdinalNumber::fromZeroBasedInt(lineIndex), OrdinalNumber::fromZeroBasedInt(column));
}

static Vector<pair<int, String> > getRegularExpressionMatchesByLines(const RegularExpression* regex, const String& text)
{
    Vector<pair<int, String> > result;
    if (text.isEmpty())
        return result;

    OwnPtr<Vector<size_t> > endings(lineEndings(text));
    size_t size = endings->size();
    unsigned start = 0;
    for (size_t lineNumber = 0; lineNumber < size; ++lineNumber) {
        size_t lineEnd = endings->at(lineNumber);
        String line = text.substring(start, lineEnd - start);
        if (line.endsWith('\r'))
            line = line.left(line.length() - 1);

        int matchLength;
        if (regex->match(line, 0, &matchLength) != -1)
            result.append(pair<int, String>(lineNumber, line));

        start = lineEnd + 1;
    }
    return result;
}

PassOwnPtr<Vector<size_t> > lineEndings(const String& text)
{
    OwnPtr<Vector<size_t> > result(adoptPtr(new Vector<size_t>()));

    unsigned start = 0;
    while (start < text.length()) {
        size_t lineEnd = text.find('\n', start);
        if (lineEnd == notFound)
            break;

        result->append(lineEnd);
        start = lineEnd + 1;
    }
    result->append(text.length());

    return result.release();
}

static PassRefPtr<TypeBuilder::Page::SearchMatch> buildObjectForSearchMatch(int lineNumber, const String& lineContent)
{
    return TypeBuilder::Page::SearchMatch::create()
        .setLineNumber(lineNumber)
        .setLineContent(lineContent)
        .release();
}

PassOwnPtr<RegularExpression> createSearchRegex(const String& query, bool caseSensitive, bool isRegex)
{
    String regexSource = isRegex ? query : createSearchRegexSource(query);
    return adoptPtr(new RegularExpression(regexSource, caseSensitive ? TextCaseSensitive : TextCaseInsensitive));
}

int countRegularExpressionMatches(const RegularExpression* regex, const String& content)
{
    if (content.isEmpty())
        return 0;

    int result = 0;
    int position;
    unsigned start = 0;
    int matchLength;
    while ((position = regex->match(content, start, &matchLength)) != -1) {
        if (start >= content.length())
            break;
        if (matchLength > 0)
            ++result;
        start = position + 1;
    }
    return result;
}

PassRefPtr<TypeBuilder::Array<TypeBuilder::Page::SearchMatch> > searchInTextByLines(const String& text, const String& query, const bool caseSensitive, const bool isRegex)
{
    RefPtr<TypeBuilder::Array<TypeBuilder::Page::SearchMatch> > result = TypeBuilder::Array<TypeBuilder::Page::SearchMatch>::create();

    OwnPtr<RegularExpression> regex = ContentSearchUtils::createSearchRegex(query, caseSensitive, isRegex);
    Vector<pair<int, String> > matches = getRegularExpressionMatchesByLines(regex.get(), text);

    for (Vector<pair<int, String> >::const_iterator it = matches.begin(); it != matches.end(); ++it)
        result->addItem(buildObjectForSearchMatch(it->first, it->second));

    return result;
}

static String findMagicComment(const String& content, const String& name, MagicCommentType commentType, bool* deprecated = 0)
{
    ASSERT(name.find("=") == notFound);
    if (deprecated)
        *deprecated = false;
    String pattern;
    String deprecatedPattern;
    switch (commentType) {
    case JavaScriptMagicComment:
        pattern = "//#[\040\t]" + createSearchRegexSource(name) + "=[\040\t]*([^\\s\'\"]*)[\040\t]*$";
        deprecatedPattern = "//@[\040\t]" + createSearchRegexSource(name) + "=[\040\t]*([^\\s\'\"]*)[\040\t]*$";
        break;
    case CSSMagicComment:
        pattern = "/\\*#[\040\t]" + createSearchRegexSource(name) + "=[\040\t]*([^\\s]*)[\040\t]*\\*/[\040\t]*$";
        deprecatedPattern = "/\\*@[\040\t]" + createSearchRegexSource(name) + "=[\040\t]*([^\\s]*)[\040\t]*\\*/[\040\t]*$";
        break;
    default:
        ASSERT_NOT_REACHED();
        return String();
    }
    RegularExpression regex(pattern, TextCaseSensitive, MultilineEnabled);
    RegularExpression deprecatedRegex(deprecatedPattern, TextCaseSensitive, MultilineEnabled);

    int matchLength;
    int offset = regex.match(content, 0, &matchLength);
    if (offset == -1) {
        offset = deprecatedRegex.match(content, 0, &matchLength);
        if (offset != -1 && deprecated)
            *deprecated = true;
    }
    if (offset == -1)
        return String();

    String match = content.substring(offset, matchLength);
    size_t separator = match.find("=");
    ASSERT(separator != notFound);
    match = match.substring(separator + 1);

    switch (commentType) {
    case JavaScriptMagicComment:
        return match.stripWhiteSpace();
    case CSSMagicComment: {
        size_t lastStarIndex = match.reverseFind('*');
        ASSERT(lastStarIndex != notFound);
        return match.substring(0, lastStarIndex).stripWhiteSpace();
    }
    default:
        ASSERT_NOT_REACHED();
        return String();
    }
}

String findSourceURL(const String& content, MagicCommentType commentType, bool* deprecated)
{
    return findMagicComment(content, "sourceURL", commentType, deprecated);
}

String findSourceMapURL(const String& content, MagicCommentType commentType, bool* deprecated)
{
    return findMagicComment(content, "sourceMappingURL", commentType, deprecated);
}

} // namespace ContentSearchUtils
} // namespace WebCore

