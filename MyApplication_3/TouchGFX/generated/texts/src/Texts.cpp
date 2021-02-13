/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#include <stdarg.h>
#include <texts/TypedTextDatabase.hpp>
#include <touchgfx/TextProvider.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>

uint16_t touchgfx::Font::getStringWidth(const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(TEXT_DIRECTION_LTR, text, pArg);
    va_end(pArg);
    return width;
}

uint16_t touchgfx::Font::getStringWidth(touchgfx::TextDirection textDirection, const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(textDirection, text, pArg);
    va_end(pArg);
    return width;
}

touchgfx::Unicode::UnicodeChar touchgfx::TextProvider::getNextLigature(TextDirection direction)
{
    if (fontGsubTable && nextCharacters.peekChar())
    {
        substituteGlyphs();
        if (nextCharacters.peekChar(1) == 0x093F) //Hindi I-matra
        {
            nextCharacters.replaceAt1(nextCharacters.peekChar());
            nextCharacters.replaceAt0(0x093F);
        }
    }
    return getNextChar();
}

void touchgfx::TextProvider::initializeInternal()
{
    fillInputBuffer();
}

void touchgfx::LCD::drawString(touchgfx::Rect widgetArea, const touchgfx::Rect& invalidatedArea, const touchgfx::LCD::StringVisuals& stringVisuals, const touchgfx::Unicode::UnicodeChar* format, ...)
{
    va_list pArg;
    va_start(pArg, format);
    drawStringLTR(widgetArea, invalidatedArea, stringVisuals, format, pArg);
    va_end(pArg);
}

//Default typed text database
extern const touchgfx::TypedText::TypedTextData* const typedTextDatabaseArray[];

TEXT_LOCATION_FLASH_PRAGMA
KEEP extern const touchgfx::Unicode::UnicodeChar texts_all_languages[] TEXT_LOCATION_FLASH_ATTRIBUTE =
{
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x36, 0x32, 0x2e, 0x35, 0x20, 0x20, 0x20, 0x20, 0x31, 0x32, 0x35, 0x20, 0x20, 0x20, 0x31, 0x38, 0x37, 0x2e, 0x35, 0x20, 0x20, 0x32, 0x35, 0x30, 0x20, 0x20, 0x20, 0x33, 0x31, 0x32, 0x2e, 0x35, 0x20, 0x20, 0x20, 0x33, 0x37, 0x35, 0x20, 0x20, 0x20, 0x34, 0x33, 0x37, 0x2e, 0x35, 0x20, 0x20, 0x20, 0x35, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20, 0x35, 0x36, 0x32, 0x2e, 0x35, 0x20, 0x20, 0x36, 0x32, 0x35, 0x20, 0x20, 0x20, 0x36, 0x37, 0x38, 0x2e, 0x35, 0x20, 0x20, 0x20, 0x37, 0x35, 0x30, 0x20, 0x20, 0x20, 0x38, 0x31, 0x32, 0x2e, 0x35, 0x20, 0x20, 0x20, 0x38, 0x37, 0x35, 0x20, 0x20, 0x20, 0x39, 0x33, 0x37, 0x2e, 0x35, 0x20, 0x20, 0x20, 0x31, 0x30, 0x30, 0x30, 0x20, 0x48, 0x7a, 0x0, // @0 "       62.5    125   187.5  250   312.5   375   437.5   500    562.5  625   678.5   750   812.5   875   937.5   1000 Hz"
    0x45, 0x32, 0x20, 0x2d, 0x3e, 0x20, 0x20, 0x20, 0x38, 0x32, 0x2e, 0x34, 0x20, 0x48, 0x7a, 0xa, 0x41, 0x32, 0x20, 0x2d, 0x3e, 0x20, 0x31, 0x31, 0x30, 0x2e, 0x30, 0x20, 0x48, 0x7a, 0xa, 0x44, 0x33, 0x20, 0x2d, 0x3e, 0x20, 0x31, 0x34, 0x36, 0x2e, 0x38, 0x20, 0x48, 0x7a, 0x20, 0xa, 0x47, 0x33, 0x20, 0x2d, 0x3e, 0x20, 0x31, 0x39, 0x36, 0x2e, 0x30, 0x20, 0x48, 0x7a, 0xa, 0x48, 0x33, 0x20, 0x2d, 0x3e, 0x20, 0x32, 0x34, 0x36, 0x2e, 0x39, 0x20, 0x48, 0x7a, 0xa, 0x45, 0x34, 0x20, 0x2d, 0x3e, 0x20, 0x33, 0x32, 0x39, 0x2e, 0x36, 0x20, 0x20, 0x48, 0x7a, 0x0, // @120 "E2 ->   82.4 Hz?A2 -> 110.0 Hz?D3 -> 146.8 Hz ?G3 -> 196.0 Hz?H3 -> 246.9 Hz?E4 -> 329.6  Hz"
    0x46, 0x75, 0x6e, 0x64, 0x61, 0x6d, 0x65, 0x6e, 0x74, 0x61, 0x6c, 0x20, 0xa, 0x66, 0x72, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x63, 0x79, 0x3a, 0xa, 0x2, 0x20, 0x20, 0x48, 0x7a, 0x0, // @213 "Fundamental ?frequency:?<>  Hz"
    0x47, 0x75, 0x69, 0x74, 0x61, 0x72, 0x20, 0x54, 0x55, 0x4e, 0x45, 0x52, 0x20, 0x0, // @243 "Guitar TUNER "
    0x2, 0x0, // @257 "<>"
    0x30, 0x0 // @259 "0"
};
TEXT_LOCATION_FLASH_PRAGMA
KEEP extern uint32_t const indicesGb[] TEXT_LOCATION_FLASH_ATTRIBUTE;

//array holding dynamically installed languages
struct TranslationHeader
{
    uint32_t offset_to_texts;
    uint32_t offset_to_indices;
    uint32_t offset_to_typedtext;
};
static const TranslationHeader* languagesArray[1] = { 0 };

//Compiled and linked in languages
static const uint32_t* const staticLanguageIndices[] =
{
    indicesGb
};

touchgfx::LanguageId touchgfx::Texts::currentLanguage = static_cast<touchgfx::LanguageId>(0);
static const touchgfx::Unicode::UnicodeChar* currentLanguagePtr = 0;
static const uint32_t* currentLanguageIndices = 0;

void touchgfx::Texts::setLanguage(touchgfx::LanguageId id)
{
    const touchgfx::TypedText::TypedTextData* currentLanguageTypedText = 0;
    if (id < 1)
    {
        if (languagesArray[id] != 0)
        {
            //dynamic translation is added
            const TranslationHeader* translation = languagesArray[id];
            currentLanguagePtr = (const touchgfx::Unicode::UnicodeChar*)(((const uint8_t*)translation) + translation->offset_to_texts);
            currentLanguageIndices = (const uint32_t*)(((const uint8_t*)translation) + translation->offset_to_indices);
            currentLanguageTypedText = (const touchgfx::TypedText::TypedTextData*)(((const uint8_t*)translation) + translation->offset_to_typedtext);
        }
        else
        {
            //compiled and linked in languages
            currentLanguagePtr = texts_all_languages;
            currentLanguageIndices = staticLanguageIndices[id];
            currentLanguageTypedText = typedTextDatabaseArray[id];
        }
    }

    if (currentLanguageTypedText)
    {
        currentLanguage = id;
        touchgfx::TypedText::registerTypedTextDatabase(currentLanguageTypedText,
                                                       TypedTextDatabase::getFonts(), TypedTextDatabase::getInstanceSize());
    }
}

void touchgfx::Texts::setTranslation(touchgfx::LanguageId id, const void* translation)
{
    languagesArray[id] = (const TranslationHeader*)translation;
}

const touchgfx::Unicode::UnicodeChar* touchgfx::Texts::getText(TypedTextId id) const
{
    return &currentLanguagePtr[currentLanguageIndices[id]];
}
