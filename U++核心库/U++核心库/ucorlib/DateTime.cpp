#include "DateTime.h"
#include "DateTimeFormat.h"
#include "BuiltInAssembly.h"
#include "CultureInfo_impl.h"
#include "StringHelper.h"
#include "TimeZoneInfo.h"
#include "TimeZoneInfo_impl.h"
#include "TimeSpan.h"
#include "CalendarHelper.h"
#include "WindowsEnvironment.h"
using namespace System;
using namespace System::OS;

namespace System
{
	enum class ParseFailureKind
	{
		None = 0,
		ArgumentNull = 1,
		Format = 2,
		FormatWithParameter = 3,
		FormatBadDateTimeCalendar = 4,  
	};

	ENUM_OPERATORS(ParseFlags)
	enum class ParseFlags
	{
		HaveYear = 0x00000001,
		HaveMonth = 0x00000002,
		HaveDay = 0x00000004,
		HaveHour = 0x00000008,
		HaveMinute = 0x00000010,
		HaveSecond = 0x00000020,
		HaveTime = 0x00000040,
		HaveDate = 0x00000080,
		TimeZoneUsed = 0x00000100,
		TimeZoneUtc = 0x00000200,
		ParsedMonthName = 0x00000400,
		CaptureOffset = 0x00000800,
		YearDefault = 0x00001000,
		Rfc1123Pattern = 0x00002000,
		UtcSortPattern = 0x00004000,
	};

	enum class DTSubStringType
	{
		Unknown = 0,
		Invalid = 1,
		Number = 2,
		End = 3,
		Other = 4,
	};

	struct DTSubString
	{
		std::wstring s;
		int index;
		int length;
		DTSubStringType type;
		int value;

		wchar_t operator[](int relativeIndex)
		{
			return s[index + relativeIndex];
		}

		DTSubString() : index(0), length(0), value(0), type(DTSubStringType::Unknown)
		{

		}

		DTSubString(const DTSubString& str)
		{
			s = str.s;
			index = str.index;
			length = str.length;
			type = str.type;
			value = str.value;
		}
	};

	enum class  DTT : int
	{

		End = 0,    // '\0'
		NumEnd = 1,    // Num[ ]*[\0]
		NumAmpm = 2,    // Num[ ]+AmPm
		NumSpace = 3,    // Num[ ]+^[Dsep|Tsep|'0\']
		NumDatesep = 4,    // Num[ ]*Dsep
		NumTimesep = 5,    // Num[ ]*Tsep
		MonthEnd = 6,    // Month[ ]*'\0'
		MonthSpace = 7,    // Month[ ]+^[Dsep|Tsep|'\0']
		MonthDatesep = 8,    // Month[ ]*Dsep
		NumDatesuff = 9,    // Month[ ]*DSuff
		NumTimesuff = 10,   // Month[ ]*TSuff
		DayOfWeek = 11,   // Day of week name
		YearSpace = 12,   // Year+^[Dsep|Tsep|'0\']
		YearDateSep = 13,  // Year+Dsep
		YearEnd = 14,  // Year+['\0']
		TimeZone = 15,  // timezone name
		Era = 16,  // era name
		NumUTCTimeMark = 17,      // Num + 'Z'
		// When you add a new token which will be in the
		// state table, add it after NumLocalTimeMark.
		Unk = 18,   // unknown
		NumLocalTimeMark = 19,    // Num + 'T'
		Max = 20,   // marker
	};

	enum class TM
	{
		NotSet = -1,
		AM = 0,
		PM = 1,
	};

	enum class DS
	{
		BEGIN = 0,
		N = 1,        // have one number
		NN = 2,        // have two numbers

		// The following are known to be part of a date

		D_Nd = 3,        // date string: have number followed by date separator
		D_NN = 4,        // date string: have two numbers
		D_NNd = 5,        // date string: have two numbers followed by date separator

		D_M = 6,        // date string: have a month
		D_MN = 7,        // date string: have a month and a number
		D_NM = 8,        // date string: have a number and a month
		D_MNd = 9,        // date string: have a month and number followed by date separator
		D_NDS = 10,       // date string: have one number followed a date suffix.

		D_Y = 11,        // date string: have a year.
		D_YN = 12,        // date string: have a year and a number
		D_YNd = 13,        // date string: have a year and a number and a date separator
		D_YM = 14,        // date string: have a year and a month
		D_YMd = 15,        // date string: have a year and a month and a date separator
		D_S = 16,       // have numbers followed by a date suffix.
		T_S = 17,       // have numbers followed by a time suffix.

		// The following are known to be part of a time

		T_Nt = 18,          // have num followed by time separator
		T_NNt = 19,       // have two numbers followed by time separator


		Error = 20,

		// The following are terminal states. These all have an action
		// associated with them; and transition back to BEGIN.

		DX_NN = 21,       // day from two numbers
		DX_NNN = 22,       // day from three numbers
		DX_MN = 23,       // day from month and one number
		DX_NM = 24,       // day from month and one number
		DX_MNN = 25,       // day from month and two numbers
		DX_DS = 26,       // a set of date suffixed numbers.
		DX_DSN = 27,       // day from date suffixes and one number.
		DX_NDS = 28,       // day from one number and date suffixes .
		DX_NNDS = 29,       // day from one number and date suffixes .

		DX_YNN = 30,       // date string: have a year and two number
		DX_YMN = 31,       // date string: have a year, a month, and a number.
		DX_YN = 32,       // date string: have a year and one number
		DX_YM = 33,       // date string: have a year, a month.
		TX_N = 34,       // time from one number (must have ampm)
		TX_NN = 35,       // time from two numbers
		TX_NNN = 36,       // time from three numbers
		TX_TS = 37,       // a set of time suffixed numbers.
		DX_NNY = 38,
	};

	struct DateTimeToken
	{
		DTT dtt;    // Store the token
		DateTokenType suffix; // Store the CJK Year/Month/Day suffix (if any)
		int num;    // Store the number that we are parsing (if any)

		DateTimeToken() : dtt(DTT::Unk), suffix(DateTokenType::UnknownToken), num(0)
		{

		}

		DateTimeToken(const DateTimeToken& token)
		{
			dtt = token.dtt;
			suffix = token.suffix;
			num = token.num;
		}
	};

	struct DateTimeRawInfo
	{
		std::vector<int> num;
		int numCount;
		int month;
		int year;
		int dayOfWeek;
		int era;
		TM timeMark;
		double fraction;

		bool timeZone;

		DateTimeRawInfo() : numCount(0), month(1), year(1), dayOfWeek(0), era(0), timeMark(TM::NotSet), fraction(0)
		{

		}

		DateTimeRawInfo(const DateTimeRawInfo& info)
		{
			num = info.num;
			numCount = info.numCount;
			month = info.month;
			year = info.year;
			dayOfWeek = info.dayOfWeek;
			era = info.era;
			timeMark = info.timeMark;
			fraction = info.fraction;
		}

		void Init(const std::vector<int>& numberBuffer)
		{
			month = -1;
			year = -1;
			dayOfWeek = -1;
			era = -1;
			timeMark = TM::NotSet;
			fraction = -1;
			num = numberBuffer;
			numCount = 0;
		}

		void AddNumber(int value)
		{
			if ((int)num.size() > numCount)
				num[numCount++] = value;
			else
			{
				num.push_back(value);
				numCount++;
			}
		}

		int GetNumber(int index)
		{
			return num[index];
		}
	};

	static std::wstring WhiteSpaceChecks = { ' ', L'\x00A0' };

	struct __DTString
	{

		//
		// Value propery: stores the real string to be parsed.
		//
		std::wstring Value;

		//
		// Index property: points to the character that we are currently parsing.
		//
		int Index;

		// The length of Value string.
		int len;

		// The current chracter to be looked at.
		wchar_t m_current;

		CultureInfo_impl* m_info;
		// Flag to indicate if we encouter an digit, we should check for token or not.
		// In some cultures, such as mn-MN, it uses "\x0031\x00a0\x0434\x04af\x0433\x044d\x044d\x0440\x00a0\x0441\x0430\x0440" in month names.
		bool m_checkDigitToken;

		__DTString();

		__DTString(const std::wstring& str, CultureInfo_impl* dtfi, bool checkDigitToken);

		__DTString(const std::wstring& str, CultureInfo_impl* dtfi);

		~__DTString();

		//
		// Advance the Index.
		// Return true if Index is NOT at the end of the string.
		//
		// Typical usage:
		// while (str.GetNext())
		// {
		//     char ch = str.GetChar()
		// }
		bool GetNext();

		bool Advance(int count);


		void GetRegularToken(DateTokenType& tokenType, int& tokenValue, const CultureInfo& dtfi);

		DateTokenType GetSeparatorToken(const CultureInfo& dtfi, int& indexBeforeSeparator, wchar_t& charBeforeSeparator);

		bool MatchSpecifiedWord(const std::wstring& target);

		bool MatchSpecifiedWord(const std::wstring& target, int endIndex);

		bool MatchSpecifiedWords(const std::wstring& target, bool checkWordBoundary, int& matchLength);

		//
		// Check to see if the string starting from Index is a prefix of
		// str.
		// If a match is found, true value is returned and Index is updated to the next character to be parsed.
		// Otherwise, Index is unchanged.
		//
		bool Match(const std::wstring& str);

		bool Match(wchar_t ch);

		//
		//  Actions: From the current position, try matching the longest word in the specified string array.
		//      E.g. words[] = {"AB", "ABC", "ABCD"}, if the current position points to a substring like "ABC DEF",
		//          MatchLongestWords(words, ref MaxMatchStrLen) will return 1 (the index), and maxMatchLen will be 3.
		//  Returns:
		//      The index that contains the longest word to match
		//  Arguments:
		//      words   The string array that contains words to search.
		//      maxMatchStrLen  [in/out] the initailized maximum length.  This parameter can be used to
		//          find the longest match in two string arrays.
		//
		int MatchLongestWords(const std::vector<std::wstring>& words, int& maxMatchStrLen);

		//
		// Get the number of repeat character after the current character.
		// For a string "hh:mm:ss" at Index of 3. GetRepeatCount() = 2, and Index
		// will point to the second ':'.
		//
		int GetRepeatCount();

		// Return false when end of string is encountered or a non-digit character is found.
		bool GetNextDigit();

		//
		// Get the current character.
		//
		wchar_t GetChar();

		//
		// Convert the current character to a digit, and return it.
		//
		int GetDigit();

		//
		// Eat White Space ahead of the current position
		//
		// Return false if end of string is encountered.
		//
		void SkipWhiteSpaces();

		//
		// Skip white spaces from the current position
		//
		// Return false if end of string is encountered.
		//
		bool SkipWhiteSpaceCurrent();

		void TrimTail();

		// Trim the trailing spaces within a quoted string.
		// Call this after TrimTail() is done.
		void RemoveTrailingInQuoteSpaces();

		// Trim the leading spaces within a quoted string.
		// Call this after the leading spaces before quoted string are trimmed.
		void RemoveLeadingInQuoteSpaces();

		DTSubString GetSubString();

		void ConsumeSubString(DTSubString sub);
	};

	static bool Tokenize(DateTokenType TokenMask, DateTokenType& tokenType, int& tokenValue, __DTString& str, CultureInfo_impl* loc);

#pragma region __DTString

	__DTString::__DTString()
	{

	}

	__DTString::__DTString(const std::wstring& str, CultureInfo_impl* dtfi, bool checkDigitToken) : __DTString(str, dtfi)
	{
		m_checkDigitToken = checkDigitToken;
	}

	__DTString::__DTString(const std::wstring& str, CultureInfo_impl* dtfi)
	{
		Index = -1;
		Value = str;
		len = Value.length();

		m_current = '\0';
		m_info = dtfi;
		m_checkDigitToken = ((DateTimeFormatFlags)m_info->_data->formatFlags & DateTimeFormatFlags::UseDigitPrefixInTokens) != DateTimeFormatFlags::None;
	}

	__DTString::~__DTString()
	{

	}

	//
	// Advance the Index.
	// Return true if Index is NOT at the end of the string.
	//
	// Typical usage:
	// while (str.GetNext())
	// {
	//     char ch = str.GetChar()
	// }
	bool __DTString::GetNext()
	{
		Index++;
		if (Index < len)
		{
			m_current = Value[Index];
			return (true);
		}
		return (false);
	}

	bool __DTString::Advance(int count)
	{
		Index += count;
		if (Index < len)
		{
			m_current = Value[Index];
			return (true);
		}
		return (false);
	}


	void __DTString::GetRegularToken(DateTokenType& tokenType, int& tokenValue, const CultureInfo& dtfi)
	{
		tokenValue = 0;
		if (Index >= len) {
			tokenType = DateTokenType::EndOfString;
			return;
		}

		tokenType = DateTokenType::UnknownToken;

	Start:

		if (isdigit(m_current, m_info->_locale))
		{
			// This is a digit.
			tokenValue = m_current - '0';
			int value;
			int start = Index;

			//
			// Collect other digits.
			//
			while (++Index < len)
			{
				m_current = Value[Index];
				value = m_current - '0';
				if (value >= 0 && value <= 9) {
					tokenValue = tokenValue * 10 + value;
				}
				else {
					break;
				}
			}
			if (Index - start > 8) {
				tokenType = DateTokenType::NumberToken;
				tokenValue = -1;
			}
			else if (Index - start < 3) {
				tokenType = DateTokenType::NumberToken;
			}
			else {
				// If there are more than 3 digits, assume that it's a year value.
				tokenType = DateTokenType::YearNumberToken;
			}
			if (m_checkDigitToken)
			{
				int save = Index;
				wchar_t saveCh = m_current;
				// Re-scan using the staring Index to see if this is a token.
				Index = start;  // To include the first digit.
				m_current = Value[Index];
				DateTokenType tempType;
				int tempValue;
				// This DTFI has tokens starting with digits.
				// E.g. mn-MN has month name like "\x0031\x00a0\x0434\x04af\x0433\x044d\x044d\x0440\x00a0\x0441\x0430\x0440"
				if (Tokenize(DateTokenType::RegularTokenMask, tempType, tempValue, *this, m_info))
				{
					tokenType = tempType;
					tokenValue = tempValue;
					// This is a token, so the Index has been advanced propertly in DTFI.Tokenizer().
				}
				else
				{
					// Use the number token value.
					// Restore the index.
					Index = save;
					m_current = saveCh;
				}

			}

		}
		else if (iswspace(m_current))
		{
			// Just skip to the next character.
			while (++Index < len) {
				m_current = Value[Index];
				if (!(iswspace(m_current))) {
					goto Start;
				}
			}
			// We have reached the end of string.
			tokenType = DateTokenType::EndOfString;
		}
		else {
			Tokenize(DateTokenType::RegularTokenMask, tokenType, tokenValue, *this, m_info);
		}
	}

	DateTokenType __DTString::GetSeparatorToken(const CultureInfo& dtfi, int& indexBeforeSeparator, wchar_t& charBeforeSeparator)
	{
		indexBeforeSeparator = Index;
		charBeforeSeparator = m_current;
		DateTokenType tokenType;
		if (!SkipWhiteSpaceCurrent()) {
			// Reach the end of the string.
			return (DateTokenType::SEP_End);
		}
		if (!isdigit(m_current, m_info->_locale))
		{
			// Not a digit.  Tokenize it.
			int tokenValue=0;
			bool found = Tokenize(DateTokenType::SeparatorTokenMask, tokenType, tokenValue, *this, m_info);
			if (!found) {
				tokenType = DateTokenType::SEP_Space;
			}
		}
		else {
			// Do nothing here.  If we see a number, it will not be a separator. There is no need wasting time trying to find the
			// separator token.
			tokenType = DateTokenType::SEP_Space;
		}
		return (tokenType);
	}

	bool __DTString::MatchSpecifiedWord(const std::wstring& target)
	{
		return MatchSpecifiedWord(target, (int)target.length() + Index);
	}

	bool __DTString::MatchSpecifiedWord(const std::wstring& target, int endIndex)
	{
		int count = endIndex - Index;

		if (count != target.length()) {
			return false;
		}

		if (Index + count > len) {
			return false;
		}

		return  StringHelper::Compare(Value, Index, count, target, 0, count, m_info->_localeplus, true) == 0;
	}

	bool __DTString::MatchSpecifiedWords(const std::wstring& target, bool checkWordBoundary, int& matchLength)
	{
		int valueRemaining = Value.length() - Index;
		matchLength = target.length();

		if (matchLength > valueRemaining || StringHelper::Compare(Value, Index, matchLength, target, 0, matchLength, m_info->_localeplus, true) != 0) {
			// Check word by word
			int targetPosition = 0;                 // Where we are in the target string
			int thisPosition = Index;         // Where we are in this string
			int wsIndex = StringHelper::IndexOfAny(target, WhiteSpaceChecks, targetPosition);
			if (wsIndex == -1) {
				return false;
			}
			do {
				int segmentLength = wsIndex - targetPosition;
				if (thisPosition >= (int)Value.length() - segmentLength) { // Subtraction to prevent overflow.
					return false;
				}
				if (segmentLength == 0) {
					// If segmentLength == 0, it means that we have leading space in the target string.
					// In that case, skip the leading spaces in the target and this string.
					matchLength--;
				}
				else {
					// Make sure we also have whitespace in the input string
					if (!iswspace(Value[thisPosition + segmentLength])) {
						return false;
					}
					if (StringHelper::Compare(Value, thisPosition, segmentLength, target, targetPosition, segmentLength, m_info->_localeplus, true) != 0) {
						return false;
					}
					// Advance the input string
					thisPosition = thisPosition + segmentLength + 1;
				}
				// Advance our target string
				targetPosition = wsIndex + 1;


				// Skip past multiple whitespace
				while (thisPosition < (int)Value.length() && iswspace(Value[thisPosition])) {
					thisPosition++;
					matchLength++;
				}
			} while ((wsIndex = StringHelper::IndexOfAny(target, WhiteSpaceChecks, targetPosition)) >= 0);
			// now check the last segment;
			if (targetPosition < (int)target.length()) {
				int segmentLength = (int)target.length() - targetPosition;
				if (thisPosition >(int)Value.length() - segmentLength) {
					return false;
				}
				if (StringHelper::Compare(Value, thisPosition, segmentLength, target, targetPosition, segmentLength, m_info->_localeplus, true) != 0) {
					return false;
				}
			}
		}

		if (checkWordBoundary)
		{
			int nextCharIndex = Index + matchLength;
			if (nextCharIndex < (int)Value.length()) {
				if (isalpha(Value[nextCharIndex], m_info->_locale))
				{
					return (false);
				}
			}
		}
		return (true);
	}

	//
	// Check to see if the string starting from Index is a prefix of
	// str.
	// If a match is found, true value is returned and Index is updated to the next character to be parsed.
	// Otherwise, Index is unchanged.
	//
	bool __DTString::Match(const std::wstring& str)
	{
		if (++Index >= len)
		{
			return (false);
		}

		if (str.length() > (Value.length() - Index)) {
			return false;
		}

		if (StringHelper::Compare(Value, Index, str.length(), str, 0, str.length(), m_info->_localeplus, false) == 0)
		{
			// Update the Index to the end of the matching string.
			// So the following GetNext()/Match() opeartion will get
			// the next character to be parsed.
			Index += (str.length() - 1);
			return (true);
		}
		return (false);
	}

	bool __DTString::Match(wchar_t ch)
	{
		if (++Index >= len)
		{
			return (false);
		}
		if (Value[Index] == ch)
		{
			m_current = ch;
			return (true);
		}
		Index--;
		return (false);
	}

	//
	//  Actions: From the current position, try matching the longest word in the specified string array.
	//      E.g. words[] = {"AB", "ABC", "ABCD"}, if the current position points to a substring like "ABC DEF",
	//          MatchLongestWords(words, ref MaxMatchStrLen) will return 1 (the index), and maxMatchLen will be 3.
	//  Returns:
	//      The index that contains the longest word to match
	//  Arguments:
	//      words   The string array that contains words to search.
	//      maxMatchStrLen  [in/out] the initailized maximum length.  This parameter can be used to
	//          find the longest match in two string arrays.
	//
	int __DTString::MatchLongestWords(const std::vector<std::wstring>& words, int& maxMatchStrLen)
	{
		int result = -1;
		for (int i = 0; i < (int)words.size(); i++)
		{
			const std::wstring& word = words[i];
			int matchLength = word.length();
			if (MatchSpecifiedWords(word, false, matchLength))
			{
				if (matchLength > maxMatchStrLen)
				{
					maxMatchStrLen = matchLength;
					result = i;
				}
			}
		}

		return (result);
	}

	//
	// Get the number of repeat character after the current character.
	// For a string "hh:mm:ss" at Index of 3. GetRepeatCount() = 2, and Index
	// will point to the second ':'.
	//
	int __DTString::GetRepeatCount()
	{
		wchar_t repeatChar = Value[Index];
		int pos = Index + 1;
		while ((pos < len) && (Value[pos] == repeatChar))
		{
			pos++;
		}
		int repeatCount = (pos - Index);
		// Update the Index to the end of the repeated characters.
		// So the following GetNext() opeartion will get
		// the next character to be parsed.
		Index = pos - 1;
		return (repeatCount);
	}

	// Return false when end of string is encountered or a non-digit character is found.
	bool __DTString::GetNextDigit()
	{
		if (++Index >= len)
		{
			return (false);
		}
		return (iswdigit(Value[Index])!=0);
	}

	//
	// Get the current character.
	//
	wchar_t __DTString::GetChar()
	{
		return (Value[Index]);
	}

	//
	// Convert the current character to a digit, and return it.
	//
	int __DTString::GetDigit()
	{
		return (Value[Index] - '0');
	}

	//
	// Eat White Space ahead of the current position
	//
	// Return false if end of string is encountered.
	//
	void __DTString::SkipWhiteSpaces()
	{
		// Look ahead to see if the next character
		// is a whitespace.
		while (Index + 1 < len)
		{
			wchar_t ch = Value[Index + 1];
			if (!iswspace(ch))
			{
				return;
			}
			Index++;
		}
		return;
	}

	//
	// Skip white spaces from the current position
	//
	// Return false if end of string is encountered.
	//
	bool __DTString::SkipWhiteSpaceCurrent()
	{
		if (Index >= len)
		{
			return (false);
		}

		if (!iswspace(m_current))
		{
			return (true);
		}

		while (++Index < len)
		{
			m_current = Value[Index];
			if (!iswspace(m_current))
			{
				return (true);
			}
		}
		return (false);
	}

	void __DTString::TrimTail()
	{
		int i = len - 1;
		while (i >= 0 && iswspace(Value[i])) {
			i--;
		}
		Value = Value.substr(0, i + 1);
		len = Value.length();
	}

	// Trim the trailing spaces within a quoted string.
	// Call this after TrimTail() is done.
	void __DTString::RemoveTrailingInQuoteSpaces()
	{
		int i = len - 1;
		if (i <= 1) {
			return;
		}
		wchar_t ch = Value[i];
		// Check if the last character is a quote.
		if (ch == '\'' || ch == '\"')
		{
			if (iswspace(Value[i - 1]))
			{
				i--;
				while (i >= 1 && iswspace(Value[i - 1]))
				{
					i--;
				}
				Value = Value.erase(i, Value.length() - 1 - i);
				len = Value.length();
			}
		}
	}

	// Trim the leading spaces within a quoted string.
	// Call this after the leading spaces before quoted string are trimmed.
	void __DTString::RemoveLeadingInQuoteSpaces()
	{
		if (len <= 2)
		{
			return;
		}
		int i = 0;
		wchar_t ch = Value[i];
		// Check if the last character is a quote.
		if (ch == '\'' || ch == '\"')
		{
			while ((i + 1) < len && iswspace(Value[i + 1]))
			{
				i++;
			}
			if (i != 0) {
				Value = Value.erase(1, i);
				len = Value.length();
			}
		}
	}

	DTSubString __DTString::GetSubString()
	{
		DTSubString sub = DTSubString();
		sub.index = Index;
		sub.s = Value;
		while (Index + sub.length < len) {
			DTSubStringType currentType;
			wchar_t ch = Value[Index + sub.length];
			if (ch >= '0' && ch <= '9') {
				currentType = DTSubStringType::Number;
			}
			else {
				currentType = DTSubStringType::Other;
			}

			if (sub.length == 0)
			{
				sub.type = currentType;
			}
			else
			{
				if (sub.type != currentType)
				{
					break;
				}
			}
			sub.length++;
			if (currentType == DTSubStringType::Number)
			{
				// Incorporate the number into the value
				// Limit the digits to prevent overflow
				if (sub.length > 8)
				{
					sub.type = DTSubStringType::Invalid;
					return sub;
				}
				int number = ch - '0';
				sub.value = sub.value * 10 + number;
			}
			else {
				// For non numbers, just return this length 1 token. This should be expanded
				// to more types of thing if this parsing approach is used for things other
				// than numbers and single characters
				break;
			}
		}
		if (sub.length == 0) {
			sub.type = DTSubStringType::End;
			return sub;
		}

		return sub;
	}

	void __DTString::ConsumeSubString(DTSubString sub)
	{
		Index = sub.index + sub.length;
		if (Index < len) {
			m_current = Value[Index];
		}
	}
#pragma endregion

	enum class  HebrewNumberParsingState
	{
		InvalidHebrewNumber,
		NotHebrewDigit,
		FoundEndOfHebrewNumber,
		ContinueParsing
	};

	enum class HS
	{
		_err = -1,
		Start = 0,
		S400 = 1,
		S400_400 = 2,
		S400_X00 = 3,
		S400_X0 = 4,
		X00_DQ = 5,
		S400_X00_X0 = 6,
		X0_DQ = 7,
		X = 8,
		X0 = 9,
		X00 = 10,
		S400_DQ = 11,
		S400_400_DQ = 12,
		S400_400_100 = 13,
		S9 = 14,
		X00_S9 = 15,
		S9_DQ = 16,
		END = 100
	};

	struct HebrewNumberParsingContext
	{
		HS state;

		int result;

		HebrewNumberParsingContext(int result)
		{
			state = HS::Start;
			this->result = result;
		}
	};

	class HebrewNumber
	{
	public:
		enum class HebrewToken
		{
			Invalid = -1,
			Digit400 = 0,
			Digit200_300 = 1,
			Digit100 = 2,
			Digit10 = 3,
			Digit1 = 4,
			Digit6_7 = 5,
			Digit7 = 6,
			Digit9 = 7,
			SingleQuote = 8,
			DoubleQuote = 9
		};

		class HebrewValue
		{
		public:
			HebrewToken token;

			int value;

			HebrewValue(HebrewToken token, int value)
			{
				this->token = token;
				this->value = value;
			}
		};

		static HebrewValue HebrewValues[];

		static wchar_t maxHebrewNumberCh;

		static std::vector<HS> NumberPasingState[17];

		HebrewNumber()
		{
		}

		static bool IsDigit(wchar_t ch)
		{
			if (ch < L'\x05D0' || ch > maxHebrewNumberCh)
			{
				if (ch == '\'')
				{
					return true;
				}
				return ch == '\"';
			}
			return HebrewValues[ch - 1488].value >= 0;
		}

		static HebrewNumberParsingState ParseByChar(wchar_t  ch, HebrewNumberParsingContext& context)
		{
			HebrewToken hebrewValues;
			if (ch == '\'')
			{
				hebrewValues = HebrewToken::SingleQuote;
			}
			else if (ch != '\"')
			{
				int num = ch - 1488;
				if (num < 0 || num >= 27)
				{
					return HebrewNumberParsingState::NotHebrewDigit;
				}
				hebrewValues = HebrewValues[num].token;
				if (hebrewValues == HebrewToken::Invalid)
				{
					return HebrewNumberParsingState::NotHebrewDigit;
				}
				context.result = context.result + HebrewValues[num].value;
			}
			else
			{
				hebrewValues = HebrewToken::DoubleQuote;
			}
			context.state = NumberPasingState[(int)context.state][(int)hebrewValues];
			if (context.state == HS::_err)
			{
				return HebrewNumberParsingState::InvalidHebrewNumber;
			}
			if (context.state == HS::END)
			{
				return HebrewNumberParsingState::FoundEndOfHebrewNumber;
			}
			return HebrewNumberParsingState::ContinueParsing;
		}

		static std::wstring ToString(int Number)
		{
			int number;
			wchar_t chr = '\0';
			std::wstring stringBuilder;
			if (Number > 5000)
			{
				Number = Number - 5000;
			}
			int num = Number / 100;
			if (num > 0)
			{
				Number = Number - num * 100;
				for (int i = 0; i < num / 4; i++)
				{
					stringBuilder.push_back(L'\x05EA');
				}
				int num1 = num % 4;
				if (num1 > 0)
				{
					stringBuilder.push_back((wchar_t)(1510 + num1));
				}
			}
			int number1 = Number / 10;
			Number = Number % 10;
			switch (number1)
			{
			case 0:
			{
				chr = '\0';
				break;
			}
			case 1:
			{
				chr = L'\x05D9';
				break;
			}
			case 2:
			{
				chr = L'\x05DB';
				break;
			}
			case 3:
			{
				chr = L'\x05DC';
				break;
			}
			case 4:
			{
				chr = L'\x05DE';
				break;
			}
			case 5:
			{
				chr = L'\x05E0';
				break;
			}
			case 6:
			{
				chr = L'\x05E1';
				break;
			}
			case 7:
			{
				chr = L'\x05E2';
				break;
			}
			case 8:
			{
				chr = L'\x05E4';
				break;
			}
			case 9:
			{
				chr = L'\x05E6';
				break;
			}
			}
			if (Number > 0)
			{
				number = 1488 + Number - 1;
			}
			else
			{
				number = 0;
			}
			wchar_t chr1 = (wchar_t)number;
			if (chr1 == L'\x05D4' && chr == L'\x05D9')
			{
				chr1 = L'\x05D5';
				chr = L'\x05D8';
			}
			if (chr1 == L'\x05D5' && chr == L'\x05D9')
			{
				chr1 = L'\x05D6';
				chr = L'\x05D8';
			}
			if (chr != 0)
			{
				stringBuilder.push_back(chr);
			}
			if (chr1 != 0)
			{
				stringBuilder.push_back(chr1);
			}
			if (stringBuilder.length() <= 1)
			{
				stringBuilder.push_back('\'');
			}
			else
			{
				stringBuilder.push_back('\"');
			}
			return stringBuilder;
		}
	};

	HebrewNumber::HebrewValue HebrewNumber::HebrewValues[] = { HebrewValue(HebrewToken::Digit1, 1), HebrewValue(HebrewToken::Digit1, 2), HebrewValue(HebrewToken::Digit1, 3), HebrewValue(HebrewToken::Digit1, 4), HebrewValue(HebrewToken::Digit1, 5), HebrewValue(HebrewToken::Digit6_7, 6), HebrewValue(HebrewToken::Digit6_7, 7), HebrewValue(HebrewToken::Digit1, 8), HebrewValue(HebrewToken::Digit9, 9), HebrewValue(HebrewToken::Digit10, 10), HebrewValue(HebrewToken::Invalid, -1), HebrewValue(HebrewToken::Digit10, 20), HebrewValue(HebrewToken::Digit10, 30), HebrewValue(HebrewToken::Invalid, -1), HebrewValue(HebrewToken::Digit10, 40), HebrewValue(HebrewToken::Invalid, -1), HebrewValue(HebrewToken::Digit10, 50), HebrewValue(HebrewToken::Digit10, 60), HebrewValue(HebrewToken::Digit10, 70), HebrewValue(HebrewToken::Invalid, -1), HebrewValue(HebrewToken::Digit10, 80), HebrewValue(HebrewToken::Invalid, -1), HebrewValue(HebrewToken::Digit10, 90), HebrewValue(HebrewToken::Digit100, 100), HebrewValue(HebrewToken::Digit200_300, 200), HebrewValue(HebrewToken::Digit200_300, 300), HebrewValue(HebrewToken::Digit400, 400) };

	wchar_t HebrewNumber::maxHebrewNumberCh = (wchar_t)(1514);

	std::vector<HS> HebrewNumber::NumberPasingState[17] = { std::vector < HS > {HS::S400, HS::X00, HS::X00, HS::X0, HS::X, HS::X, HS::X, HS::S9, HS::_err, HS::_err},
		std::vector < HS > { HS::S400_400, HS::S400_X00, HS::S400_X00, HS::S400_X0, HS::_err, HS::_err, HS::_err, HS::X00_S9, HS::END, HS::S400_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::S400_400_100, HS::S400_X0, HS::_err, HS::_err, HS::_err, HS::X00_S9, HS::_err, HS::S400_400_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::S400_X00_X0, HS::_err, HS::_err, HS::_err, HS::X00_S9, HS::_err, HS::X00_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::X0_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::END, HS::END, HS::END, HS::END, HS::END, HS::_err, HS::_err },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::X0_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::_err, HS::END, HS::END, HS::END, HS::END, HS::_err, HS::_err },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::END, HS::_err },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::END, HS::X0_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::S400_X0, HS::_err, HS::_err, HS::_err, HS::X00_S9, HS::END, HS::X00_DQ },
		std::vector < HS > { HS::END, HS::END, HS::END, HS::END, HS::END, HS::END, HS::END, HS::END, HS::_err, HS::_err },
		std::vector < HS > { HS::_err, HS::_err, HS::END, HS::END, HS::END, HS::END, HS::END, HS::END, HS::_err, HS::_err },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::S400_X00_X0, HS::_err, HS::_err, HS::_err, HS::X00_S9, HS::_err, HS::X00_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::END, HS::S9_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::S9_DQ },
		std::vector < HS > { HS::_err, HS::_err, HS::_err, HS::_err, HS::_err, HS::END, HS::END, HS::_err, HS::_err, HS::_err } };


	__forceinline bool IsHebrewChar(wchar_t ch)
	{
		if (ch < L'\x0590')
		{
			return false;
		}
		return ch <= L'\x05FF';
	}

	static bool TryParseHebrewNumber(__DTString& str, bool& badFormat, int& number)
	{
		HebrewNumberParsingState hebrewNumberParsingState;
		number = -1;
		badFormat = false;
		int index = str.Index;
		if (!HebrewNumber::IsDigit(str.Value[index]))
		{
			return false;
		}
		HebrewNumberParsingContext hebrewNumberParsingContext = HebrewNumberParsingContext(0);
		do
		{
			int num = index;
			index = num + 1;
			hebrewNumberParsingState = HebrewNumber::ParseByChar(str.Value[num], hebrewNumberParsingContext);
			switch (hebrewNumberParsingState)
			{
			case HebrewNumberParsingState::InvalidHebrewNumber:
			case HebrewNumberParsingState::NotHebrewDigit:
			{
				return false;
			}
			default:
			{
				continue;
			}
			}
		} while (index < (int)str.Value.length() && hebrewNumberParsingState != HebrewNumberParsingState::FoundEndOfHebrewNumber);
		if (hebrewNumberParsingState != HebrewNumberParsingState::FoundEndOfHebrewNumber)
		{
			return false;
		}
		str.Advance(index - str.Index);
		number = hebrewNumberParsingContext.result;
		return true;
	}

	static bool Tokenize(DateTokenType TokenMask, DateTokenType& tokenType, int& tokenValue, __DTString& str, CultureInfo_impl* loc)
	{
		bool flag;
		tokenType = DateTokenType::UnknownToken;
		tokenValue = 0;
		wchar_t mCurrent = str.m_current;
		bool flag1 = iswalpha(mCurrent) != 0;
		if (flag1)
		{
			mCurrent = tolower(mCurrent, loc->_locale);
			if (IsHebrewChar(mCurrent) && TokenMask == DateTokenType::RegularTokenMask && TryParseHebrewNumber(str, flag, tokenValue))
			{
				if (flag)
				{
					tokenType = DateTokenType::UnknownToken;
					return false;
				}
				tokenType = DateTokenType::HebrewNumber;
				return true;
			}
		}
		int num = mCurrent % L'Ç';
		int num1 = 1 + mCurrent % L'Å';
		int num2 = str.len - str.Index;
		int num3 = 0;

		bool HasSpacesInMonthNames = loc->_data->HasSpacesInMonthNames;
		bool HasSpacesInDayNames = loc->_data->HasSpacesInDayNames;
		do
		{
			DateTokenHashValue* tokenHashValue = loc->_dateTokens[num];
			if (!tokenHashValue)
			{
				break;
			}
			if ((int)(tokenHashValue->tokenType & TokenMask) > 0 && (int)tokenHashValue->tokenString.length() <= num2)
			{
				if (StringHelper::Compare(str.Value, str.Index, tokenHashValue->tokenString, 0, tokenHashValue->tokenString.length(), loc->_localeplus, true) == 0)
				{
					if (flag1)
					{
						int index = str.Index + tokenHashValue->tokenString.length();
						int num4 = index;
						if (index < str.len && iswalpha(str.Value[num4]))
						{
							return false;
						}
					}
					tokenType = tokenHashValue->tokenType & TokenMask;
					tokenValue = tokenHashValue->tokenValue;
					str.Advance((int)tokenHashValue->tokenString.length());
					return true;
				}
				if (tokenHashValue->tokenType == DateTokenType::MonthToken && HasSpacesInMonthNames)
				{
					int num5 = 0;
					if (str.MatchSpecifiedWords(tokenHashValue->tokenString, true, num5))
					{
						tokenType = tokenHashValue->tokenType & TokenMask;
						tokenValue = tokenHashValue->tokenValue;
						str.Advance(num5);
						return true;
					}
				}
				else if (tokenHashValue->tokenType == DateTokenType::DayOfWeekToken && HasSpacesInDayNames)
				{
					int num6 = 0;
					if (str.MatchSpecifiedWords(tokenHashValue->tokenString, true, num6))
					{
						tokenType = tokenHashValue->tokenType & TokenMask;
						tokenValue = tokenHashValue->tokenValue;
						str.Advance(num6);
						return true;
					}
				}
			}
			num3++;
			num = num + num1;
			if (num < 199)
			{
				continue;
			}
			num = num - 199;
		} while (num3 < 199);
		return false;
	}

	struct DateTimeResult
	{
		int Year;
		int Month;
		int Day;
		//
		// Set time defualt to 00:00:00.
		//
		int Hour;
		int Minute;
		int Second;
		double fraction;

		int era;

		ParseFlags flags;

		TimeSpan timeZoneOffset;

		const Calendar* calendar;

		DateTime parsedDate;

		ParseFailureKind failure;

		std::string failureMessageID;

		DateTimeResult() :flags((ParseFlags)0) {}

		void Init()
		{
			Year = -1;
			Month = -1;
			Day = -1;
			fraction = -1;
			era = -1;
		}

		void SetDate(int year, int month, int day)
		{
			Year = year;
			Month = month;
			Day = day;
		}

		void SetFailure(ParseFailureKind failure, const std::string& failureMessageID)
		{
			this->failure = failure;
			this->failureMessageID = failureMessageID;
		}

	};

	// This is the helper data structure used in ParseExact().
	struct ParsingInfo
	{

		const Calendar* calendar;
		int dayOfWeek;
		TM timeMark;

		bool fUseHour12;
		bool fUseTwoDigitYear;
		bool fAllowInnerWhite;
		bool fAllowTrailingWhite;
		bool fCustomNumberParser;
		//DateTimeParse.MatchNumberDelegate parseNumberDelegate;

		void Init()
		{
			dayOfWeek = -1;
			timeMark = TM::NotSet;
		}

	};

	class DateTimeParse
	{
	public:

		static std::vector<DS> dateParsingStates[25];

		//delegate bool MatchNumberDelegate(__DTString& str, int digitLen, int& result);

		//static MatchNumberDelegate m_hebrewNumberParser = new MatchNumberDelegate(DateTimeParse.MatchHebrewDigits);


		static std::exception GetDateTimeParseException(DateTimeResult& result)
		{
			switch (result.failure)
			{
			case ParseFailureKind::ArgumentNull:
			{
				return std::exception(result.failureMessageID.c_str());
			}
			case ParseFailureKind::Format:
			{
				return std::exception(result.failureMessageID.c_str());
			}
			case ParseFailureKind::FormatWithParameter:
			{
				return std::exception(result.failureMessageID.c_str());
			}
			case ParseFailureKind::FormatBadDateTimeCalendar:
			{
				return std::exception(result.failureMessageID.c_str());
			}
			}
			return std::exception();
		}

		static DateTime ParseExact(const std::wstring& s, const std::wstring& format, CultureInfo& dtfi, DateTimeStyles style)
		{
			DateTimeResult result;       // The buffer to store the parsing result.
			result.Init();
			if (TryParseExact(s, format, dtfi, style, result)) {
				return result.parsedDate;
			}
			else {
				throw GetDateTimeParseException(result);
			}
		}

		static DateTime ParseExact(const std::wstring& s, const std::wstring& format, CultureInfo& dtfi, DateTimeStyles style, TimeSpan& offset)
		{
			DateTimeResult result = DateTimeResult();       // The buffer to store the parsing result.
			offset = TimeSpan();
			result.Init();
			result.flags |= ParseFlags::CaptureOffset;
			if (TryParseExact(s, format, dtfi, style, result)) {
				offset = result.timeZoneOffset;
				return result.parsedDate;
			}
			else {
				throw GetDateTimeParseException(result);
			}
		}

		static bool TryParseExact(const std::wstring& s, const std::wstring& format, CultureInfo& dtfi, DateTimeStyles style, DateTime& result)
		{
			result = DateTime::MinValue;
			DateTimeResult resultData = DateTimeResult();       // The buffer to store the parsing result.
			resultData.Init();
			if (TryParseExact(s, format, dtfi, style, resultData)) {
				result = resultData.parsedDate;
				return true;
			}
			return false;
		}

		static bool TryParseExact(const std::wstring& s, const std::wstring& format, CultureInfo& dtfi, DateTimeStyles style, DateTime& result, TimeSpan& offset)
		{
			result = DateTime::MinValue;
			offset = TimeSpan();
			DateTimeResult resultData = DateTimeResult();       // The buffer to store the parsing result.
			resultData.Init();
			resultData.flags |= ParseFlags::CaptureOffset;
			if (TryParseExact(s, format, dtfi, style, resultData)) {
				result = resultData.parsedDate;
				offset = resultData.timeZoneOffset;
				return true;
			}
			return false;
		}

		static bool TryParseExact(const std::wstring& s, const std::wstring& format, CultureInfo& dtfi, DateTimeStyles style, DateTimeResult& result)
		{
			if (s.empty()) {
				result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
				return false;
			}
			if (format.empty()) {
				result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
				return false;
			}
			if (s.length() == 0) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			if (format.length() == 0) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
				return false;
			}

			return DoStrictParse(s, format, style, dtfi, result);
		}

		static DateTime ParseExactMultiple(const std::wstring& s, const wstringvector& formats,
			CultureInfo& dtfi, DateTimeStyles style)
		{
			DateTimeResult result = DateTimeResult();       // The buffer to store the parsing result.
			result.Init();
			if (TryParseExactMultiple(s, formats, dtfi, style, result)) {
				return result.parsedDate;
			}
			else {
				throw GetDateTimeParseException(result);
			}
		}


		static DateTime ParseExactMultiple(const std::wstring& s, const wstringvector& formats,
			CultureInfo& dtfi, DateTimeStyles style, TimeSpan& offset)
		{
			DateTimeResult result = DateTimeResult();       // The buffer to store the parsing result.
			offset = TimeSpan();
			result.Init();
			result.flags |= ParseFlags::CaptureOffset;
			if (TryParseExactMultiple(s, formats, dtfi, style, result)) {
				offset = result.timeZoneOffset;
				return result.parsedDate;
			}
			else {
				throw GetDateTimeParseException(result);
			}
		}

		static bool TryParseExactMultiple(const std::wstring& s, const wstringvector& formats,
			CultureInfo& dtfi, DateTimeStyles style, DateTime& result, TimeSpan& offset)
		{
			result = DateTime::MinValue;
			offset = TimeSpan();
			DateTimeResult resultData = DateTimeResult();       // The buffer to store the parsing result.
			resultData.Init();
			resultData.flags |= ParseFlags::CaptureOffset;
			if (TryParseExactMultiple(s, formats, dtfi, style, resultData)) {
				result = resultData.parsedDate;
				offset = resultData.timeZoneOffset;
				return true;
			}
			return false;
		}


		static bool TryParseExactMultiple(const std::wstring& s, const wstringvector& formats,
			CultureInfo& dtfi, DateTimeStyles style, DateTime& result)
		{
			result = DateTime::MinValue;
			DateTimeResult resultData = DateTimeResult();       // The buffer to store the parsing result.
			resultData.Init();
			if (TryParseExactMultiple(s, formats, dtfi, style, resultData)) {
				result = resultData.parsedDate;
				return true;
			}
			return false;
		}

		static bool TryParseExactMultiple(const std::wstring& s, const wstringvector& formats,
			CultureInfo& dtfi, DateTimeStyles style, DateTimeResult& result)
		{
			if (s.empty()) {
				result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
				return false;
			}
			if (formats.empty())
			{
				result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
				return false;
			}

			if (s.length() == 0) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			if (formats.size() == 0) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
				return false;
			}

			//
			// Do a loop through the provided formats and see if we can parse succesfully in
			// one of the formats.
			//
			for (int i = 0; i < (int)formats.size(); i++) {
				if (formats[i].empty() || formats[i].length() == 0) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
					return false;
				}
				// Create a new result each time to ensure the runs are independent. Carry through
				// flags from the caller and return the result.
				DateTimeResult innerResult = DateTimeResult();       // The buffer to store the parsing result.
				innerResult.Init();
				innerResult.flags = result.flags;
				if (TryParseExact(s, formats[i], dtfi, style, innerResult)) {
					result.parsedDate = innerResult.parsedDate;
					result.timeZoneOffset = innerResult.timeZoneOffset;
					return (true);
				}
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return (false);
		}

		////////////////////////////////////////////////////////////////////////////
		//
		// NOTE: The following state machine table is dependent on the order of the
		// DS and DTT enumerations.
		//
		// For each non terminal state, the following table defines the next state
		// for each given date token type.
		//
		////////////////////////////////////////////////////////////////////////////

		//          End       NumEnd      NumAmPm     NumSpace    NumDaySep   NumTimesep  MonthEnd    MonthSpace  MonthDSep   NumDateSuff NumTimeSuff     DayOfWeek     YearSpace   YearDateSep YearEnd     TimeZone   Era         UTCTimeMark   
		
		//          End       NumEnd      NumAmPm     NumSpace    NumDaySep   NumTimesep  MonthEnd    MonthSpace  MonthDSep   NumDateSuff NumTimeSuff     DayOfWeek     YearSpace   YearDateSep YearEnd     TimeZone    Era        UTCMark

		static const std::wstring GMTName;
		static const std::wstring ZuluName;

		//
		// Search from the index of str at str.Index to see if the target string exists in the str.
		//
		static bool MatchWord(__DTString& str, const std::wstring& target)
		{
			int length = target.length();
			if (length > ((int)str.Value.length() - str.Index)) {
				return false;
			}

			if (StringHelper::Compare(str.Value, str.Index, length, target, 0, length, str.m_info->_localeplus,  true) != 0) 
			{
				return (false);
			}

			int nextCharIndex = str.Index + (int)target.length();

			if (nextCharIndex < (int)str.Value.length()) {
				wchar_t nextCh = str.Value[nextCharIndex];
				if (iswalpha(nextCh)) {
					return (false);
				}
			}
			str.Index = nextCharIndex;
			if (str.Index < str.len) {
				str.m_current = str.Value[str.Index];
			}

			return (true);
		}


		//
		// Check the word at the current index to see if it matches GMT name or Zulu name.
		//
		static bool GetTimeZoneName(__DTString& str)
		{
			//
			// <

			if (MatchWord(str, GMTName)) {
				return (true);
			}

			if (MatchWord(str, ZuluName)) {
				return (true);
			}

			return (false);
		}

		static bool IsDigit(wchar_t ch)
		{
			return (ch >= '0' && ch <= '9');
		}


		/*=================================ParseFraction==========================
		**Action: Starting at the str.Index, which should be a decimal symbol.
		** if the current character is a digit, parse the remaining
		**      numbers as fraction.  For example, if the sub-string starting at str.Index is "123", then
		**      the method will return 0.123
		**Returns:      The fraction number.
		**Arguments:
		**      str the parsing string
		**std::exceptions:
		============================================================================*/

		static bool ParseFraction(__DTString& str, double& result)
		{
			result = 0;
			double decimalBase = 0.1;
			int digits = 0;
			wchar_t ch;
			while (str.GetNext()
				&& IsDigit(ch = str.m_current)) {
				result += (ch - '0') * decimalBase;
				decimalBase *= 0.1;
				digits++;
			}
			return (digits > 0);
		}

		/*=================================ParseTimeZone==========================
		**Action: Parse the timezone offset in the following format:
		**          "+8", "+08", "+0800", "+0800"
		**        This method is used by DateTime::Parse().
		**Returns:      The TimeZone offset.
		**Arguments:
		**      str the parsing string
		**std::exceptions:
		**      Formatstd::exception if invalid timezone format is found.
		============================================================================*/

		static bool ParseTimeZone(__DTString& str, TimeSpan& result)
		{
			// The hour/minute offset for timezone.
			int hourOffset = 0;
			int minuteOffset = 0;
			DTSubString sub;

			// Consume the +/- character that has already been read
			sub = str.GetSubString();
			if (sub.length != 1) {
				return false;
			}
			wchar_t offsetChar = sub[0];
			if (offsetChar != '+' && offsetChar != '-') {
				return false;
			}
			str.ConsumeSubString(sub);

			sub = str.GetSubString();
			if (sub.type != DTSubStringType::Number) {
				return false;
			}
			int value = sub.value;
			int length = sub.length;
			if (length == 1 || length == 2) {
				// Parsing "+8" or "+08"
				hourOffset = value;
				str.ConsumeSubString(sub);
				// See if we have minutes
				sub = str.GetSubString();
				if (sub.length == 1 && sub[0] == ':') {
					// Parsing "+8:00" or "+08:00"
					str.ConsumeSubString(sub);
					sub = str.GetSubString();
					if (sub.type != DTSubStringType::Number || sub.length < 1 || sub.length > 2) {
						return false;
					}
					minuteOffset = sub.value;
					str.ConsumeSubString(sub);
				}
			}
			else if (length == 3 || length == 4) {
				// Parsing "+800" or "+0800"
				hourOffset = value / 100;
				minuteOffset = value % 100;
				str.ConsumeSubString(sub);
			}
			else {
				// Wrong number of digits
				return false;
			}
			if (minuteOffset < 0 || minuteOffset >= 60) {
				return false;
			}

			result = TimeSpan(hourOffset, minuteOffset, 0);
			if (offsetChar == '-') {
				result = result.Negate;
			}
			return true;
		}

		// This is the helper function to handle timezone in string in the format like +/-0800
		static bool HandleTimeZone(__DTString& str, DateTimeResult& result)
		{
			if ((str.Index < str.len - 1)) {
				wchar_t nextCh = str.Value[str.Index];
				// Skip whitespace, but don't update the index unless we find a time zone marker
				int whitespaceCount = 0;
				while (iswspace(nextCh) && str.Index + whitespaceCount < str.len - 1) {
					whitespaceCount++;
					nextCh = str.Value[str.Index + whitespaceCount];
				}
				if (nextCh == '+' || nextCh == '-') {
					str.Index += whitespaceCount;
					if ((result.flags & ParseFlags::TimeZoneUsed) != (ParseFlags)0)
					{
						// Should not have two timezone offsets.
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
					result.flags |= ParseFlags::TimeZoneUsed;
					if (!ParseTimeZone(str, result.timeZoneOffset)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
				}
			}
			return true;
		}

		//
		// This is the lexer. Check the character at the current index, and put the found token in dtok and
		// some raw date/time information in raw.
		//

		static bool Lex(DS dps, __DTString& str, DateTimeToken& dtok, DateTimeRawInfo& raw, DateTimeResult& result, const CultureInfo& dtfi, DateTimeStyles styles)
		{

			DateTokenType tokenType;
			int tokenValue;
			int indexBeforeSeparator;
			wchar_t charBeforeSeparator;

			DateTokenType sep;
			dtok.dtt = DTT::Unk;     // Assume the token is unkown.

			str.GetRegularToken(tokenType, tokenValue, dtfi);


			// Look at the regular token.
			switch (tokenType) {
			case DateTokenType::NumberToken:
			case DateTokenType::YearNumberToken:
				if (raw.numCount == 3 || tokenValue == -1) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				//
				// This is a digit.
				//
				// If the previous parsing state is DS::T_NNt (like 12:01), and we got another number,
				// so we will have a terminal state DS::TX_NNN (like 12:01:02).
				// If the previous parsing state is DS::T_Nt (like 12:), and we got another number,
				// so we will have a terminal state DS::TX_NN (like 12:01).
				//
				// Look ahead to see if the following character is a decimal point or timezone offset.
				// This enables us to parse time in the forms of:
				//  "11:22:33.1234" or "11:22:33-08".
				if (dps == DS::T_NNt) {
					if ((str.Index < str.len - 1)) {
						wchar_t nextCh = str.Value[str.Index];
						if (nextCh == '.') {
							// While ParseFraction can fail, it just means that there were no digits after
							// the dot. In this case ParseFraction just removes the dot. This is actually
							// valid for cultures like Albanian, that join the time marker to the time with
							// with a dot: e.g. "9:03.MD"
							ParseFraction(str, raw.fraction);
						}
					}
				}
				if (dps == DS::T_NNt || dps == DS::T_Nt) {
					if ((str.Index < str.len - 1)) {
						if (false == HandleTimeZone(str, result))
						{
							return false;
						}
					}
				}

				dtok.num = tokenValue;
				if (tokenType == DateTokenType::YearNumberToken)
				{
					if (raw.year == -1)
					{
						raw.year = tokenValue;
						//
						// If we have number which has 3 or more digits (like "001" or "0001"),
						// we assume this number is a year. Save the currnet raw.numCount in
						// raw.year.
						//
						switch (sep = str.GetSeparatorToken(dtfi, indexBeforeSeparator, charBeforeSeparator)) {
						case DateTokenType::SEP_End:
							dtok.dtt = DTT::YearEnd;
							break;
						case DateTokenType::SEP_Am:
						case DateTokenType::SEP_Pm:
							if (raw.timeMark == TM::NotSet) {
								raw.timeMark = (sep == DateTokenType::SEP_Am ? TM::AM : TM::PM);
								dtok.dtt = DTT::YearSpace;
							}
							else {
								result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							}
							break;
						case DateTokenType::SEP_Space:
							dtok.dtt = DTT::YearSpace;
							break;
						case DateTokenType::SEP_Date:
							dtok.dtt = DTT::YearDateSep;
							break;
						case DateTokenType::SEP_DateOrOffset:
							// The separator is either a date separator or the start of a time zone offset. If the token will complete the date then
							// process just the number and roll back the index so that the outer loop can attempt to parse the time zone offset.
							if ((dateParsingStates[(int)dps][(int)DTT::YearDateSep] == DS::Error)
								&& (dateParsingStates[(int)dps][(int)DTT::YearSpace] > DS::Error)) {
								str.Index = indexBeforeSeparator;
								str.m_current = charBeforeSeparator;
								dtok.dtt = DTT::YearSpace;
							}
							else {
								dtok.dtt = DTT::YearDateSep;
							}
							break;
						case DateTokenType::SEP_YearSuff:
						case DateTokenType::SEP_MonthSuff:
						case DateTokenType::SEP_DaySuff:
							dtok.dtt = DTT::NumDatesuff;
							dtok.suffix = sep;
							break;
						case DateTokenType::SEP_HourSuff:
						case DateTokenType::SEP_MinuteSuff:
						case DateTokenType::SEP_SecondSuff:
							dtok.dtt = DTT::NumTimesuff;
							dtok.suffix = sep;
							break;
						default:
							// Invalid separator after number number.
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return false;
						}
						//
						// Found the token already. Return now.
						//
						return true;
					}
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				switch (sep = str.GetSeparatorToken(dtfi, indexBeforeSeparator, charBeforeSeparator))
				{
					//
					// Note here we check if the numCount is less than three.
					// When we have more than three numbers, it will be caught as error in the state machine.
					//
				case DateTokenType::SEP_End:
					dtok.dtt = DTT::NumEnd;
					raw.AddNumber(dtok.num);
					break;
				case DateTokenType::SEP_Am:
				case DateTokenType::SEP_Pm:
					if (raw.timeMark == TM::NotSet) {
						raw.timeMark = (sep == DateTokenType::SEP_Am ? TM::AM : TM::PM);
						dtok.dtt = DTT::NumAmpm;
						raw.AddNumber(dtok.num);
					}
					else {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						break;
					}
					if (dps == DS::T_NNt || dps == DS::T_Nt) {
						if (false == HandleTimeZone(str, result))
						{
							return false;
						}
					}
					break;
				case DateTokenType::SEP_Space:
					dtok.dtt = DTT::NumSpace;
					raw.AddNumber(dtok.num);
					break;
				case DateTokenType::SEP_Date:
					dtok.dtt = DTT::NumDatesep;
					raw.AddNumber(dtok.num);
					break;
				case DateTokenType::SEP_DateOrOffset:
					// The separator is either a date separator or the start of a time zone offset. If the token will complete the date then
					// process just the number and roll back the index so that the outer loop can attempt to parse the time zone offset.
					if ((dateParsingStates[(int)dps][(int)DTT::NumDatesep] == DS::Error)
						&& (dateParsingStates[(int)dps][(int)DTT::NumSpace] > DS::Error)) {
						str.Index = indexBeforeSeparator;
						str.m_current = charBeforeSeparator;
						dtok.dtt = DTT::NumSpace;
					}
					else {
						dtok.dtt = DTT::NumDatesep;
					}
					raw.AddNumber(dtok.num);
					break;
				case DateTokenType::SEP_Time:
					dtok.dtt = DTT::NumTimesep;
					raw.AddNumber(dtok.num);
					break;
				case DateTokenType::SEP_YearSuff:
					try {
						dtok.num = dtfi.Calendar->ToFourDigitYear(tokenValue);
					}
					catch (...) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
					dtok.dtt = DTT::NumDatesuff;
					dtok.suffix = sep;
					break;
				case DateTokenType::SEP_MonthSuff:
				case DateTokenType::SEP_DaySuff:
					dtok.dtt = DTT::NumDatesuff;
					dtok.suffix = sep;
					break;
				case DateTokenType::SEP_HourSuff:
				case DateTokenType::SEP_MinuteSuff:
				case DateTokenType::SEP_SecondSuff:
					dtok.dtt = DTT::NumTimesuff;
					dtok.suffix = sep;
					break;
				case DateTokenType::SEP_LocalTimeMark:
					dtok.dtt = DTT::NumLocalTimeMark;
					raw.AddNumber(dtok.num);
					break;
				default:
					// Invalid separator after number number.
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case DateTokenType::HebrewNumber:
				if (tokenValue >= 100) {
					// This is a year number
					if (raw.year == -1) {
						raw.year = tokenValue;
						//
						// If we have number which has 3 or more digits (like "001" or "0001"),
						// we assume this number is a year. Save the currnet raw.numCount in
						// raw.year.
						//
						switch (sep = str.GetSeparatorToken(dtfi, indexBeforeSeparator, charBeforeSeparator)) {
						case DateTokenType::SEP_End:
							dtok.dtt = DTT::YearEnd;
							break;
						case DateTokenType::SEP_Space:
							dtok.dtt = DTT::YearSpace;
							break;
						case DateTokenType::SEP_DateOrOffset:
							// The separator is either a date separator or the start of a time zone offset. If the token will complete the date then
							// process just the number and roll back the index so that the outer loop can attempt to parse the time zone offset.
							if (dateParsingStates[(int)dps][(int)DTT::YearSpace] > DS::Error) {
								str.Index = indexBeforeSeparator;
								str.m_current = charBeforeSeparator;
								dtok.dtt = DTT::YearSpace;
								break;
							}
						default:
							// Invalid separator after number number.
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return false;
						}
					}
					else {
						// Invalid separator after number number.
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
				}
				else {
					// This is a day number
					dtok.num = tokenValue;
					raw.AddNumber(dtok.num);

					switch (sep = str.GetSeparatorToken(dtfi, indexBeforeSeparator, charBeforeSeparator)) {
						//
						// Note here we check if the numCount is less than three.
						// When we have more than three numbers, it will be caught as error in the state machine.
						//
					case DateTokenType::SEP_End:
						dtok.dtt = DTT::NumEnd;
						break;
					case DateTokenType::SEP_Space:
					case DateTokenType::SEP_Date:
						dtok.dtt = DTT::NumDatesep;
						break;
					case DateTokenType::SEP_DateOrOffset:
						// The separator is either a date separator or the start of a time zone offset. If the token will complete the date then
						// process just the number and roll back the index so that the outer loop can attempt to parse the time zone offset.
						if ((dateParsingStates[(int)dps][(int)DTT::NumDatesep] == DS::Error)
							&& (dateParsingStates[(int)dps][(int)DTT::NumSpace] > DS::Error)) {
							str.Index = indexBeforeSeparator;
							str.m_current = charBeforeSeparator;
							dtok.dtt = DTT::NumSpace;
						}
						else {
							dtok.dtt = DTT::NumDatesep;
						}
						break;
					default:
						// Invalid separator after number number.
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
				}
				break;
			case DateTokenType::DayOfWeekToken:
				if (raw.dayOfWeek == -1)
				{
					//
					// This is a day of week name.
					//
					raw.dayOfWeek = tokenValue;
					dtok.dtt = DTT::DayOfWeek;
				}
				else {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case DateTokenType::MonthToken:
				if (raw.month == -1)
				{
					//
					// This is a month name
					//
					switch (sep = str.GetSeparatorToken(dtfi, indexBeforeSeparator, charBeforeSeparator))
					{
					case DateTokenType::SEP_End:
						dtok.dtt = DTT::MonthEnd;
						break;
					case DateTokenType::SEP_Space:
						dtok.dtt = DTT::MonthSpace;
						break;
					case DateTokenType::SEP_Date:
						dtok.dtt = DTT::MonthDatesep;
						break;
					case DateTokenType::SEP_DateOrOffset:
						// The separator is either a date separator or the start of a time zone offset. If the token will complete the date then
						// process just the number and roll back the index so that the outer loop can attempt to parse the time zone offset.
						if ((dateParsingStates[(int)dps][(int)DTT::MonthDatesep] == DS::Error)
							&& (dateParsingStates[(int)dps][(int)DTT::MonthSpace] > DS::Error)) {
							str.Index = indexBeforeSeparator;
							str.m_current = charBeforeSeparator;
							dtok.dtt = DTT::MonthSpace;
						}
						else {
							dtok.dtt = DTT::MonthDatesep;
						}
						break;
					default:
						//Invalid separator after month name
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
					raw.month = tokenValue;
				}
				else {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case DateTokenType::EraToken:
				if (result.era != -1) {
					result.era = tokenValue;
					dtok.dtt = DTT::Era;
				}
				else {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case DateTokenType::JapaneseEraToken:
				// Special case for Japanese.  We allow Japanese era name to be used even if the calendar is not Japanese Calendar.
				if (result.era != -1) {
					result.era = tokenValue;
					dtok.dtt = DTT::Era;
				}
				else {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case DateTokenType::TEraToken:
				/*  */
				if (result.era != -1) {
					result.era = tokenValue;
					dtok.dtt = DTT::Era;
				}
				else {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case DateTokenType::TimeZoneToken:
				//
				// This is a timezone designator
				//
				// NOTENOTE : for now, we only support "GMT" and "Z" (for Zulu time).
				//
				if ((result.flags & ParseFlags::TimeZoneUsed) != (ParseFlags)0) {
					// Should not have two timezone offsets.
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				dtok.dtt = DTT::TimeZone;
				result.flags |= ParseFlags::TimeZoneUsed;
				result.timeZoneOffset = TimeSpan(0);
				result.flags |= ParseFlags::TimeZoneUtc;
				break;
			case DateTokenType::EndOfString:
				dtok.dtt = DTT::End;
				break;
			case DateTokenType::DateWordToken:
			case DateTokenType::IgnorableSymbol:
				// Date words and ignorable symbols can just be skipped over
				break;
			case DateTokenType::Am:
			case DateTokenType::Pm:
				if (raw.timeMark == TM::NotSet) {
					raw.timeMark = (TM)tokenValue;
				}
				else {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case DateTokenType::UnknownToken:
				if (iswalpha(str.m_current)) {
					result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_UnknowDateTimeWord");
					return (false);
				}


				if ((str.m_current == '-' || str.m_current == '+') && ((result.flags & ParseFlags::TimeZoneUsed) == (ParseFlags)0))
				{
					int originalIndex = str.Index;
					if (ParseTimeZone(str, result.timeZoneOffset)) {
						result.flags |= ParseFlags::TimeZoneUsed;
						return true;
					}
					else {
						// Time zone parse attempt failed. Fall through to punctuation handling.
						str.Index = originalIndex;
					}
				}

				// Visual Basic implements string to date conversions on top of DateTime::Parse:
				//   CDate("#10/10/95#")
				//
				if (VerifyValidPunctuation(str)) {
					return true;
				}

				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			return true;
		}

		static bool VerifyValidPunctuation(__DTString& str)
		{
			// Compatability Behavior. Allow trailing nulls and surrounding hashes
			wchar_t ch = str.Value[str.Index];
			if (ch == '#') {
				bool foundStart = false;
				bool foundEnd = false;
				for (int i = 0; i < str.len; i++) {
					ch = str.Value[i];
					if (ch == '#') {
						if (foundStart) {
							if (foundEnd) {
								// Having more than two hashes is invalid
								return false;
							}
							else {
								foundEnd = true;
							}
						}
						else {
							foundStart = true;
						}
					}
					else if (ch == '\0') {
						// Allow nulls only at the end
						if (!foundEnd) {
							return false;
						}
					}
					else if ((!iswspace(ch))) {
						// Anthyhing other than whitespace outside hashes is invalid
						if (!foundStart || foundEnd) {
							return false;
						}
					}
				}
				if (!foundEnd) {
					// The has was un-paired
					return false;
				}
				// Valid Hash usage: eat the hash and continue. 
				str.GetNext();
				return true;
			}
			else if (ch == '\0') {
				for (int i = str.Index; i < str.len; i++) {
					if (str.Value[i] != '\0') {
						// Nulls are only valid if they are the only trailing character
						return false;
					}
				}
				// Move to the end of the string
				str.Index = str.len;
				return true;
			}
			return false;
		}

		static const int ORDER_YMD;     // The order of date is Year/Month/Day.
		static const int ORDER_MDY;     // The order of date is Month/Day/Year.
		static const int ORDER_DMY;     // The order of date is Day/Month/Year.
		static const int ORDER_YDM;     // The order of date is Year/Day/Month
		static const int ORDER_YM;     // Year/Month order.
		static const int ORDER_MY;     // Month/Year order.
		static const int ORDER_MD;     // Month/Day order.
		static const int ORDER_DM;     // Day/Month order.

		//
		// Decide the year/month/day order from the datePattern.
		//
		// Return 0 for YMD, 1 for MDY, 2 for DMY, otherwise -1.
		//
		static bool GetYearMonthDayOrder(const std::wstring& datePattern, const CultureInfo& dtfi, int& order)
		{
			int yearOrder = -1;
			int monthOrder = -1;
			int dayOrder = -1;
			int orderCount = 0;

			bool inQuote = false;

			for (int i = 0; i <(int)datePattern.length() && orderCount < 3; i++)
			{
				wchar_t ch = datePattern[i];
				if (ch == '\\' || ch == '%')
				{
					i++;
					continue;  // Skip next character that is escaped by this backslash
				}

				if (ch == '\'' || ch == '"')
				{
					inQuote = !inQuote;
				}

				if (!inQuote)
				{
					if (ch == 'y')
					{
						yearOrder = orderCount++;

						//
						// Skip all year pattern charaters.
						//
						for (; i + 1 < (int)datePattern.length() && datePattern[i + 1] == 'y'; i++)
						{
							// Do nothing here.
						}
					}
					else if (ch == 'M')
					{
						monthOrder = orderCount++;
						//
						// Skip all month pattern characters.
						//
						for (; i + 1 < (int)datePattern.length() && datePattern[i + 1] == 'M'; i++)
						{
							// Do nothing here.
						}
					}
					else if (ch == 'd')
					{

						int patternCount = 1;
						//
						// Skip all day pattern characters.
						//
						for (; i + 1 < (int)datePattern.length() && datePattern[i + 1] == 'd'; i++)
						{
							patternCount++;
						}
						//
						// Make sure this is not "ddd" or "dddd", which means day of week.
						//
						if (patternCount <= 2)
						{
							dayOrder = orderCount++;
						}
					}
				}
			}

			if (yearOrder == 0 && monthOrder == 1 && dayOrder == 2)
			{
				order = ORDER_YMD;
				return true;
			}
			if (monthOrder == 0 && dayOrder == 1 && yearOrder == 2)
			{
				order = ORDER_MDY;
				return true;
			}
			if (dayOrder == 0 && monthOrder == 1 && yearOrder == 2)
			{
				order = ORDER_DMY;
				return true;
			}
			if (yearOrder == 0 && dayOrder == 1 && monthOrder == 2)
			{
				order = ORDER_YDM;
				return true;
			}
			order = -1;
			return false;
		}

		//
		// Decide the year/month order from the pattern.
		//
		// Return 0 for YM, 1 for MY, otherwise -1.
		//
		static bool GetYearMonthOrder(const std::wstring& pattern, const CultureInfo& dtfi, int& order)
		{
			int yearOrder = -1;
			int monthOrder = -1;
			int orderCount = 0;

			bool inQuote = false;
			for (int i = 0; i < (int)pattern.length() && orderCount < 2; i++)
			{
				wchar_t ch = pattern[i];
				if (ch == '\\' || ch == '%')
				{
					i++;
					continue;  // Skip next character that is escaped by this backslash
				}

				if (ch == '\'' || ch == '"')
				{
					inQuote = !inQuote;
				}

				if (!inQuote)
				{
					if (ch == 'y')
					{
						yearOrder = orderCount++;

						//
						// Skip all year pattern charaters.
						//
						for (; i + 1 < (int)pattern.length() && pattern[i + 1] == 'y'; i++)
						{
						}
					}
					else if (ch == 'M')
					{
						monthOrder = orderCount++;
						//
						// Skip all month pattern characters.
						//
						for (; i + 1 < (int)pattern.length() && pattern[i + 1] == 'M'; i++)
						{
						}
					}
				}
			}

			if (yearOrder == 0 && monthOrder == 1)
			{
				order = ORDER_YM;
				return true;
			}
			if (monthOrder == 0 && yearOrder == 1)
			{
				order = ORDER_MY;
				return true;
			}
			order = -1;
			return false;
		}

		//
		// Decide the month/day order from the pattern.
		//
		// Return 0 for MD, 1 for DM, otherwise -1.
		//
		static bool GetMonthDayOrder(const std::wstring& pattern, const CultureInfo& dtfi, int& order)
		{
			int monthOrder = -1;
			int dayOrder = -1;
			int orderCount = 0;

			bool inQuote = false;
			for (int i = 0; i < (int)pattern.length() && orderCount < 2; i++)
			{
				wchar_t ch = pattern[i];
				if (ch == '\\' || ch == '%')
				{
					i++;
					continue;  // Skip next character that is escaped by this backslash
				}

				if (ch == '\'' || ch == '"')
				{
					inQuote = !inQuote;
				}

				if (!inQuote)
				{
					if (ch == 'd')
					{
						int patternCount = 1;
						//
						// Skip all day pattern charaters.
						//
						for (; i + 1 < (int)pattern.length() && pattern[i + 1] == 'd'; i++)
						{
							patternCount++;
						}

						//
						// Make sure this is not "ddd" or "dddd", which means day of week.
						//
						if (patternCount <= 2)
						{
							dayOrder = orderCount++;
						}

					}
					else if (ch == 'M')
					{
						monthOrder = orderCount++;
						//
						// Skip all month pattern characters.
						//
						for (; i + 1 < (int)pattern.length() && pattern[i + 1] == 'M'; i++)
						{
						}
					}
				}
			}

			if (monthOrder == 0 && dayOrder == 1)
			{
				order = ORDER_MD;
				return true;
			}
			if (dayOrder == 0 && monthOrder == 1)
			{
				order = ORDER_DM;
				return true;
			}
			order = -1;
			return false;
		}

		//
		// Adjust the two-digit year if necessary.
		//
		static bool TryAdjustYear(DateTimeResult& result, int year, int& adjustedYear)
		{
			if (year < 100)
			{
				try {
					// the Calendar classes need some real work.  Many of the calendars that throw
					// don't implement a fast/non-allocating (and non-throwing) IsValid{Year|Day|Month} method.
					// we are making a targeted try/catch fix in the in-place release but will revisit this code
					// in the next side-by-side release.
					year = result.calendar->ToFourDigitYear(year);
				}
				catch (...) {
					adjustedYear = -1;
					return false;
				}
			}
			adjustedYear = year;
			return true;
		}

		static bool SetDateYMD(DateTimeResult& result, int year, int month, int day)
		{
			// Note, longer term these checks should be done at the end of the parse. This current
			// way of checking creates order dependence with parsing the era name.
			if (result.calendar->IsValidDay(year, month, day, result.era))
			{
				result.SetDate(year, month, day);                           // YMD
				return (true);
			}
			return (false);
		}

		static bool SetDateMDY(DateTimeResult& result, int month, int day, int year)
		{
			return (SetDateYMD(result, year, month, day));
		}

		static bool SetDateDMY(DateTimeResult& result, int day, int month, int year)
		{
			return (SetDateYMD(result, year, month, day));
		}

		static bool SetDateYDM(DateTimeResult& result, int year, int day, int month)
		{
			return (SetDateYMD(result, year, month, day));
		}

		static void GetDefaultYear(DateTimeResult& result, DateTimeStyles& styles)
		{
			result.Year = result.calendar->GetYear(GetDateTimeNow(result, styles));
			result.flags |= ParseFlags::YearDefault;
		}

		// Processing teriminal case: DS::DX_NN
		static bool GetDayOfNN(DateTimeResult& result, DateTimeStyles& styles, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{

			if ((result.flags & ParseFlags::HaveDate) != (ParseFlags)0)
			{
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			int n1 = raw.GetNumber(0);
			int n2 = raw.GetNumber(1);

			GetDefaultYear(result, styles);

			int order;
			if (!GetMonthDayOrder(dtfi.GetPattern(CultureInfoPatterns::MonthDayPattern), dtfi, order)) {
				result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
				return false;
			}

			if (order == ORDER_MD)
			{
				if (SetDateYMD(result, result.Year, n1, n2))                           // MD
				{
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}
			else {
				// ORDER_DM
				if (SetDateYMD(result, result.Year, n2, n1))                           // DM
				{
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		// Processing teriminal case: DS::DX_NNN
		static bool GetDayOfNNN(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{
			if ((result.flags & ParseFlags::HaveDate) != (ParseFlags)0)
			{
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			int n1 = raw.GetNumber(0);
			int n2 = raw.GetNumber(1);;
			int n3 = raw.GetNumber(2);

			int order;
			if (!GetYearMonthDayOrder(dtfi.GetPattern(CultureInfoPatterns::ShortDatePattern), dtfi, order)) {
				result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
				return false;
			}
			int year;

			if (order == ORDER_YMD) {
				if (TryAdjustYear(result, n1, year) && SetDateYMD(result, year, n2, n3))         // YMD
				{
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}
			else if (order == ORDER_MDY) {
				if (TryAdjustYear(result, n3, year) && SetDateMDY(result, n1, n2, year))         // MDY
				{
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}
			else if (order == ORDER_DMY) {
				if (TryAdjustYear(result, n3, year) && SetDateDMY(result, n1, n2, year))         // DMY
				{
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}
			else if (order == ORDER_YDM) {
				if (TryAdjustYear(result, n1, year) && SetDateYDM(result, year, n2, n3))         // YDM
				{
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		static bool GetDayOfMN(DateTimeResult& result, DateTimeStyles& styles, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{

			if ((result.flags & ParseFlags::HaveDate) != (ParseFlags)0) {
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			// The interpretation is based on the MonthDayPattern and YearMonthPattern
			//
			//    MonthDayPattern   YearMonthPattern  Interpretation
			//    ---------------   ----------------  ---------------
			//    MMMM dd           MMMM yyyy         Day
			//    MMMM dd           yyyy MMMM         Day
			//    dd MMMM           MMMM yyyy         Year
			//    dd MMMM           yyyy MMMM         Day
			//
			// In the first and last cases, it could be either or neither, but a day is a better default interpretation
			// than a 2 digit year.

			int monthDayOrder;
			if (!GetMonthDayOrder(dtfi.GetPattern(CultureInfoPatterns::MonthDayPattern), dtfi, monthDayOrder)) {
				result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
				return false;
			}
			if (monthDayOrder == ORDER_DM) {
				int yearMonthOrder;
				if (!GetYearMonthOrder(dtfi.GetPattern(CultureInfoPatterns::YearMonthPattern), dtfi, yearMonthOrder)) {
					result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
					return false;
				}
				if (yearMonthOrder == ORDER_MY) {
					int year;
					if (!TryAdjustYear(result, raw.GetNumber(0), year) || !SetDateYMD(result, year, raw.month, 1)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
					return true;
				}
			}

			GetDefaultYear(result, styles);
			if (!SetDateYMD(result, result.Year, raw.month, raw.GetNumber(0))) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			return true;

		}

		////////////////////////////////////////////////////////////////////////
		//  Actions:
		//  Deal with the terminal state for Hebrew Month/Day pattern
		//
		////////////////////////////////////////////////////////////////////////

		static bool GetHebrewDayOfNM(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{
			int monthDayOrder;
			if (!GetMonthDayOrder(dtfi.GetPattern(CultureInfoPatterns::MonthDayPattern), dtfi, monthDayOrder)) {
				result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
				return false;
			}
			result.Month = raw.month;
			if (monthDayOrder == ORDER_DM || monthDayOrder == ORDER_MD)
			{
				if (result.calendar->IsValidDay(result.Year, result.Month, raw.GetNumber(0), result.era))
				{
					result.Day = raw.GetNumber(0);
					return true;
				}
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		static bool GetDayOfNM(DateTimeResult& result, DateTimeStyles& styles, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{
			if ((result.flags & ParseFlags::HaveDate) != (ParseFlags)0) {
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			// The interpretation is based on the MonthDayPattern and YearMonthPattern
			//
			//    MonthDayPattern   YearMonthPattern  Interpretation
			//    ---------------   ----------------  ---------------
			//    MMMM dd           MMMM yyyy         Day
			//    MMMM dd           yyyy MMMM         Year
			//    dd MMMM           MMMM yyyy         Day
			//    dd MMMM           yyyy MMMM         Day
			//
			// In the first and last cases, it could be either or neither, but a day is a better default interpretation
			// than a 2 digit year.

			int monthDayOrder;
			if (!GetMonthDayOrder(dtfi.GetPattern(CultureInfoPatterns::MonthDayPattern), dtfi, monthDayOrder)) {
				result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
				return false;
			}
			if (monthDayOrder == ORDER_MD) {
				int yearMonthOrder;
				if (!GetYearMonthOrder(dtfi.GetPattern(CultureInfoPatterns::YearMonthPattern), dtfi, yearMonthOrder)) {
					result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
					return false;
				}
				if (yearMonthOrder == ORDER_YM) {
					int year;
					if (!TryAdjustYear(result, raw.GetNumber(0), year) || !SetDateYMD(result, year, raw.month, 1)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
					return true;
				}
			}

			GetDefaultYear(result, styles);
			if (!SetDateYMD(result, result.Year, raw.month, raw.GetNumber(0))) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			return true;
		}

		static bool GetDayOfMNN(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{
			if ((result.flags & ParseFlags::HaveDate) != (ParseFlags)0) {
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			int n1 = raw.GetNumber(0);
			int n2 = raw.GetNumber(1);

			int order;
			if (!GetYearMonthDayOrder(dtfi.GetPattern(CultureInfoPatterns::ShortDatePattern), dtfi, order)) {
				result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
				return false;
			}
			int year;

			if (order == ORDER_MDY)
			{
				if (TryAdjustYear(result, n2, year) && result.calendar->IsValidDay(year, raw.month, n1, result.era))
				{
					result.SetDate(year, raw.month, n1);      // MDY
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
				else if (TryAdjustYear(result, n1, year) && result.calendar->IsValidDay(year, raw.month, n2, result.era))
				{
					result.SetDate(year, raw.month, n2);      // YMD
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}
			else if (order == ORDER_YMD)
			{
				if (TryAdjustYear(result, n1, year) && result.calendar->IsValidDay(year, raw.month, n2, result.era))
				{
					result.SetDate(year, raw.month, n2);      // YMD
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
				else if (TryAdjustYear(result, n2, year) && result.calendar->IsValidDay(year, raw.month, n1, result.era))
				{
					result.SetDate(year, raw.month, n1);      // DMY
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}
			else if (order == ORDER_DMY)
			{
				if (TryAdjustYear(result, n2, year) && result.calendar->IsValidDay(year, raw.month, n1, result.era))
				{
					result.SetDate(year, raw.month, n1);      // DMY
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
				else if (TryAdjustYear(result, n1, year) && result.calendar->IsValidDay(year, raw.month, n2, result.era))
				{
					result.SetDate(year, raw.month, n2);      // YMD
					result.flags |= ParseFlags::HaveDate;
					return true;
				}
			}

			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		static bool GetDayOfYNN(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{

			if ((result.flags & ParseFlags::HaveDate) != (ParseFlags)0) {
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			int n1 = raw.GetNumber(0);
			int n2 = raw.GetNumber(1);
			std::wstring pattern = dtfi.GetPattern(CultureInfoPatterns::ShortDatePattern);

			// For compatability, don't throw if we can't determine the order, but default to YMD instead
			int order;
			if (GetYearMonthDayOrder(pattern, dtfi, order) && order == ORDER_YDM) {
				if (SetDateYMD(result, raw.year, n2, n1)) {
					result.flags |= ParseFlags::HaveDate;
					return true; // Year + DM
				}
			}
			else {
				if (SetDateYMD(result, raw.year, n1, n2)) {
					result.flags |= ParseFlags::HaveDate;
					return true; // Year + MD
				}
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		static bool GetDayOfNNY(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{

			if ((result.flags  & ParseFlags::HaveDate) != (ParseFlags)0) {
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			int n1 = raw.GetNumber(0);
			int n2 = raw.GetNumber(1);

			int order;
			if (!GetYearMonthDayOrder(dtfi.GetPattern(CultureInfoPatterns::ShortDatePattern), dtfi, order)) {
				result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
				return false;
			}

			if (order == ORDER_MDY || order == ORDER_YMD) {
				if (SetDateYMD(result, raw.year, n1, n2)) {
					result.flags |= ParseFlags::HaveDate;
					return true; // MD + Year
				}
			}
			else {
				if (SetDateYMD(result, raw.year, n2, n1)) {
					result.flags |= ParseFlags::HaveDate;
					return true; // DM + Year
				}
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}


		static bool GetDayOfYMN(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{

			if ((result.flags  & ParseFlags::HaveDate) != (ParseFlags)0) {
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			if (SetDateYMD(result, raw.year, raw.month, raw.GetNumber(0))) {
				result.flags |= ParseFlags::HaveDate;
				return true;
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		static bool GetDayOfYN(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{
			if ((result.flags  & ParseFlags::HaveDate) != (ParseFlags)0) {
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			if (SetDateYMD(result, raw.year, raw.GetNumber(0), 1))
			{
				result.flags |= ParseFlags::HaveDate;
				return true;
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		static bool GetDayOfYM(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{
			if ((result.flags  & ParseFlags::HaveDate) != (ParseFlags)0) {
				// Multiple dates in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			if (SetDateYMD(result, raw.year, raw.month, 1))
			{
				result.flags |= ParseFlags::HaveDate;
				return true;
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		static void AdjustTimeMark(const CultureInfo& dtfi, DateTimeRawInfo& raw)
		{
			// Specail case for culture which uses AM as empty string.
			// E.g. af-ZA (0x0436)
			//    S1159                  \x0000
			//    S2359                  nm
			// In this case, if we are parsing a string like "2005/09/14 12:23", we will assume this is in AM.

			if (raw.timeMark == TM::NotSet) 
			{
				std::wstring am = dtfi.GetConstsName(CultureInfoConstNames::AMDesignator);
				std::wstring pm = dtfi.GetConstsName(CultureInfoConstNames::AMDesignator);
				if (!am.empty() && !pm.empty()) {
					if (am.length() == 0 && pm.length() != 0) {
						raw.timeMark = TM::AM;
					}
					if (pm.length() == 0 && am.length() != 0) {
						raw.timeMark = TM::PM;
					}
				}
			}
		}

		//
		// Adjust hour according to the time mark.
		//
		static bool AdjustHour(int& hour, TM timeMark)
		{
			if (timeMark != TM::NotSet) {

				if (timeMark == TM::AM) {
					if (hour < 0 || hour > 12) {
						return false;
					}
					hour = (hour == 12) ? 0 : hour;
				}
				else {
					if (hour < 0 || hour > 23) {
						return false;
					}
					if (hour < 12) {
						hour += 12;
					}
				}
			}
			return true;
		}

		static bool GetTimeOfN(const CultureInfo& dtfi, DateTimeResult& result, DateTimeRawInfo& raw)
		{
			if ((result.flags & ParseFlags::HaveTime) != (ParseFlags)0) {
				// Multiple times in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			//
			// In this case, we need a time mark. Check if so.
			//
			if (raw.timeMark == TM::NotSet)
			{
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			result.Hour = raw.GetNumber(0);
			result.flags |= ParseFlags::HaveTime;
			return true;
		}

		static bool GetTimeOfNN(const CultureInfo& dtfi, DateTimeResult& result, DateTimeRawInfo& raw)
		{
			if ((result.flags & ParseFlags::HaveTime) != (ParseFlags)0) {
				// Multiple times in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			result.Hour = raw.GetNumber(0);
			result.Minute = raw.GetNumber(1);
			result.flags |= ParseFlags::HaveTime;
			return true;
		}

		static bool GetTimeOfNNN(const CultureInfo& dtfi, DateTimeResult& result, DateTimeRawInfo& raw)
		{
			if ((result.flags & ParseFlags::HaveTime) != (ParseFlags)0) {
				// Multiple times in the input string
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			result.Hour = raw.GetNumber(0);
			result.Minute = raw.GetNumber(1);
			result.Second = raw.GetNumber(2);
			result.flags |= ParseFlags::HaveTime;
			return true;
		}

		//
		// Processing terminal state: A Date suffix followed by one number.
		//
		static bool GetDateOfDSN(DateTimeResult& result, DateTimeRawInfo& raw)
		{
			if (raw.numCount != 1 || result.Day != -1)
			{
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			result.Day = raw.GetNumber(0);
			return true;
		}

		static bool GetDateOfNDS(DateTimeResult& result, DateTimeRawInfo& raw)
		{
			if (result.Month == -1)
			{
				//Should have a month suffix
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			if (result.Year != -1)
			{
				// Aleady has a year suffix
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			if (!TryAdjustYear(result, raw.GetNumber(0), result.Year))
			{
				// the year value is out of range
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			result.Day = 1;
			return true;
		}

		static bool GetDateOfNNDS(DateTimeResult& result, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{

			// For partial CJK Dates, the only valid formats are with a specified year, followed by two numbers, which
			// will be the Month and Day, and with a specified Month, when the numbers are either the year and day or
			// day and year, depending on the short date pattern.

			if ((result.flags & ParseFlags::HaveYear) != (ParseFlags) 0) {
				if (((result.flags & ParseFlags::HaveMonth) == (ParseFlags)0) && ((result.flags & ParseFlags::HaveDay) == (ParseFlags)0)) {
					if (TryAdjustYear(result, raw.year, result.Year) && SetDateYMD(result, result.Year, raw.GetNumber(0), raw.GetNumber(1))) {
						return true;
					}
				}
			}
			else if ((result.flags & ParseFlags::HaveMonth) != (ParseFlags)0) {
				if (((result.flags & ParseFlags::HaveYear) == (ParseFlags)0) && ((result.flags & ParseFlags::HaveDay) == (ParseFlags)0)) {
					int order;
					if (!GetYearMonthDayOrder(dtfi.GetPattern(CultureInfoPatterns::ShortDatePattern), dtfi, order)) {
						result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadDatePattern");
						return false;
					}
					int year;
					if (order == ORDER_YMD) {
						if (TryAdjustYear(result, raw.GetNumber(0), year) && SetDateYMD(result, year, result.Month, raw.GetNumber(1))) {
							return true;
						}
					}
					else {
						if (TryAdjustYear(result, raw.GetNumber(1), year) && SetDateYMD(result, year, result.Month, raw.GetNumber(0))){
							return true;
						}
					}
				}
			}
			result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
			return false;
		}

		//
		// A date suffix is found, use this method to put the number into the result.
		//
		static bool ProcessDateTimeSuffix(DateTimeResult& result, DateTimeRawInfo& raw, DateTimeToken& dtok)
		{
			switch (dtok.suffix)
			{
			case DateTokenType::SEP_YearSuff:
				if ((result.flags & ParseFlags::HaveYear) != (ParseFlags)0) {
					return false;
				}
				result.flags |= ParseFlags::HaveYear;
				result.Year = raw.year = dtok.num;
				break;
			case DateTokenType::SEP_MonthSuff:
				if ((result.flags & ParseFlags::HaveMonth) != (ParseFlags)0) {
					return false;
				}
				result.flags |= ParseFlags::HaveMonth;
				result.Month = raw.month = dtok.num;
				break;
			case DateTokenType::SEP_DaySuff:
				if ((result.flags & ParseFlags::HaveDay) != (ParseFlags)0) {
					return false;
				}
				result.flags |= ParseFlags::HaveDay;
				result.Day = dtok.num;
				break;
			case DateTokenType::SEP_HourSuff:
				if ((result.flags & ParseFlags::HaveHour) != (ParseFlags)0) {
					return false;
				}
				result.flags |= ParseFlags::HaveHour;
				result.Hour = dtok.num;
				break;
			case DateTokenType::SEP_MinuteSuff:
				if ((result.flags & ParseFlags::HaveMinute) != (ParseFlags)0) {
					return false;
				}
				result.flags |= ParseFlags::HaveMinute;
				result.Minute = dtok.num;
				break;
			case DateTokenType::SEP_SecondSuff:
				if ((result.flags & ParseFlags::HaveSecond) != (ParseFlags)0) {
					return false;
				}
				result.flags |= ParseFlags::HaveSecond;
				result.Second = dtok.num;
				break;
			}
			return true;

		}

		////////////////////////////////////////////////////////////////////////
		//
		// Actions:
		// This is used by DateTime::Parse().
		// Process the terminal state for the Hebrew calendar parsing.
		//
		////////////////////////////////////////////////////////////////////////

		


		static bool ProcessHebrewTerminalState(DS dps, DateTimeResult& result, DateTimeStyles& styles, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{
			// The following are accepted terminal state for Hebrew date.
			switch (dps) {
			case DS::DX_MNN:
				// Deal with the default long/short date format when the year number is ambigous (i.e. year < 100).
				raw.year = raw.GetNumber(1);
				if (!CultureInfo_impl::YearMonthAdjustment(dtfi, raw.year, raw.month, true)) {
					result.SetFailure(ParseFailureKind::FormatBadDateTimeCalendar, "Format_BadDateTimeCalendar");
					return false;
				}
				if (!GetDayOfMNN(result, raw, dtfi)) {
					return false;
				}
				break;
			case DS::DX_YMN:
				// Deal with the default long/short date format when the year number is NOT ambigous (i.e. year >= 100).
				if (!CultureInfo_impl::YearMonthAdjustment(dtfi, raw.year, raw.month, true)) {
					result.SetFailure(ParseFailureKind::FormatBadDateTimeCalendar, "Format_BadDateTimeCalendar");
					return false;
				}
				if (!GetDayOfYMN(result, raw, dtfi)) {
					return false;
				}
				break;
			case DS::DX_NM:
			case DS::DX_MN:
				// Deal with Month/Day pattern.
				GetDefaultYear(result, styles);
				if (!CultureInfo_impl::YearMonthAdjustment(dtfi, result.Year,raw.month, true)) {
					result.SetFailure(ParseFailureKind::FormatBadDateTimeCalendar, "Format_BadDateTimeCalendar");
					return false;
				}
				if (!GetHebrewDayOfNM(result, raw, dtfi)) {
					return false;
				}
				break;
			case DS::DX_YM:
				// Deal with Year/Month pattern.
				if (!CultureInfo_impl::YearMonthAdjustment(dtfi, raw.year, raw.month, true)) {
					result.SetFailure(ParseFailureKind::FormatBadDateTimeCalendar, "Format_BadDateTimeCalendar");
					return false;
				}
				if (!GetDayOfYM(result, raw, dtfi)) {
					return false;
				}
				break;
			case DS::TX_N:
				// Deal hour + AM/PM
				if (!GetTimeOfN(dtfi, result, raw)) {
					return false;
				}
				break;
			case DS::TX_NN:
				if (!GetTimeOfNN(dtfi, result, raw)) {
					return false;
				}
				break;
			case DS::TX_NNN:
				if (!GetTimeOfNNN(dtfi, result, raw)) {
					return false;
				}
				break;
			default:
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;

			}
			if (dps > DS::Error)
			{
				//
				// We have reached a terminal state. Reset the raw num count.
				//
				raw.numCount = 0;
			}
			return true;
		}

		//
		// A terminal state has been reached, call the appropriate function to fill in the parsing result.
		// Return true if the state is a terminal state.
		//
		static bool ProcessTerminaltState(DS dps, DateTimeResult& result, DateTimeStyles& styles, DateTimeRawInfo& raw, const CultureInfo& dtfi)
		{

			bool passed = true;
			switch (dps)
			{
			case DS::DX_NN:
				passed = GetDayOfNN(result, styles, raw, dtfi);
				break;
			case DS::DX_NNN:
				passed = GetDayOfNNN(result, raw, dtfi);
				break;
			case DS::DX_MN:
				passed = GetDayOfMN(result, styles, raw, dtfi);
				break;
			case DS::DX_NM:
				passed = GetDayOfNM(result, styles, raw, dtfi);
				break;
			case DS::DX_MNN:
				passed = GetDayOfMNN(result, raw, dtfi);
				break;
			case DS::DX_DS:
				// The result has got the correct value. No need to process.
				passed = true;
				break;
			case DS::DX_YNN:
				passed = GetDayOfYNN(result, raw, dtfi);
				break;
			case DS::DX_NNY:
				passed = GetDayOfNNY(result, raw, dtfi);
				break;
			case DS::DX_YMN:
				passed = GetDayOfYMN(result, raw, dtfi);
				break;
			case DS::DX_YN:
				passed = GetDayOfYN(result, raw, dtfi);
				break;
			case DS::DX_YM:
				passed = GetDayOfYM(result, raw, dtfi);
				break;
			case DS::TX_N:
				passed = GetTimeOfN(dtfi, result, raw);
				break;
			case DS::TX_NN:
				passed = GetTimeOfNN(dtfi, result, raw);
				break;
			case DS::TX_NNN:
				passed = GetTimeOfNNN(dtfi, result, raw);
				break;
			case DS::TX_TS:
				// The result has got the correct value. Nothing to do.
				passed = true;
				break;
			case DS::DX_DSN:
				passed = GetDateOfDSN(result, raw);
				break;
			case DS::DX_NDS:
				passed = GetDateOfNDS(result, raw);
				break;
			case DS::DX_NNDS:
				passed = GetDateOfNNDS(result, raw, dtfi);
				break;
			}

			if (!passed) {
				return false;
			}

			if (dps > DS::Error)
			{
				//
				// We have reached a terminal state. Reset the raw num count.
				//
				raw.numCount = 0;
			}
			return true;
		}

		static DateTime Parse(const std::wstring& s, const CultureInfo& dtfi, DateTimeStyles styles)
		{
			DateTimeResult result = DateTimeResult();       // The buffer to store the parsing result.
			result.Init();
			if (TryParse(s, dtfi, styles, result)) {
				return result.parsedDate;
			}
			else {
				throw GetDateTimeParseException(result);
			}
		}

		static DateTime Parse(const std::wstring& s, const CultureInfo& dtfi, DateTimeStyles styles, TimeSpan& offset)
		{
			DateTimeResult result = DateTimeResult();       // The buffer to store the parsing result.
			result.Init();
			result.flags |= ParseFlags::CaptureOffset;
			if (TryParse(s, dtfi, styles, result)) {
				offset = result.timeZoneOffset;
				return result.parsedDate;
			}
			else {
				throw GetDateTimeParseException(result);
			}
		}


		static bool TryParse(const std::wstring& s, const CultureInfo& dtfi, DateTimeStyles styles, DateTime& result)
		{
			result = DateTime::MinValue;
			DateTimeResult resultData = DateTimeResult();       // The buffer to store the parsing result.
			resultData.Init();
			if (TryParse(s, dtfi, styles, resultData)) {
				result = resultData.parsedDate;
				return true;
			}
			return false;
		}

		static bool TryParse(const std::wstring& s, const CultureInfo& dtfi, DateTimeStyles styles, DateTime& result, TimeSpan& offset)
		{
			result = DateTime::MinValue;
			offset = TimeSpan();
			DateTimeResult parseResult = DateTimeResult();       // The buffer to store the parsing result.
			parseResult.Init();
			parseResult.flags |= ParseFlags::CaptureOffset;
			if (TryParse(s, dtfi, styles, parseResult)) {
				result = parseResult.parsedDate;
				offset = parseResult.timeZoneOffset;
				return true;
			}
			return false;
		}


		//
		// This is the real method to do the parsing work.
		//
		static bool TryParse(const std::wstring& s, const CultureInfo& dtfi, DateTimeStyles styles, DateTimeResult& result)
		{
			if (s.empty()) {
				result.SetFailure(ParseFailureKind::ArgumentNull, "ArgumentNull_String");
				return false;
			}
			if (s.length() == 0) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}


			DateTime time;
			//
			// First try the predefined format.
			//
			//<


			DS dps = DS::BEGIN;     // Date Parsing State.
			bool reachTerminalState = false;

			DateTimeToken   dtok = DateTimeToken();      // The buffer to store the parsing token.
			dtok.suffix = DateTokenType::SEP_Unk;
			DateTimeRawInfo raw = DateTimeRawInfo();    // The buffer to store temporary parsing information.

			raw.Init(std::vector < int > {0, 0, 0});

			result.calendar = dtfi.Calendar;
			result.era = 0;

			//
			// The string to be parsed. Use a __DTString wrapper so that we can trace the index which
			// indicates the begining of next token.
			//
			__DTString str =__DTString(s, CultureInfo_impl::GetImpl(dtfi));

			str.GetNext();

			//
			// The following loop will break out when we reach the end of the str.
			//
			do {
				//
				// Call the lexer to get the next token.
				//
				// If we find a era in Lex(), the era value will be in raw.era.
				if (!Lex(dps, str, dtok, raw, result, dtfi, styles))           
				{
					return false;
				}

				//
				// If the token is not unknown, process it.
				// Otherwise, just discard it.
				//
				if (dtok.dtt != DTT::Unk)
				{
					//
					// Check if we got any CJK Date/Time suffix.
					// Since the Date/Time suffix tells us the number belongs to year/month/day/hour/minute/second,
					// store the number in the appropriate field in the result.
					//
					if (dtok.suffix != DateTokenType::SEP_Unk)
					{
						if (!ProcessDateTimeSuffix(result, raw, dtok)) {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return false;
						}

						dtok.suffix = DateTokenType::SEP_Unk;  // Reset suffix to SEP_Unk;
					}

					if (dtok.dtt == DTT::NumLocalTimeMark) {
						if (dps == DS::D_YNd || dps == DS::D_YN) {
							// Consider this as ISO 8601 format:
							// "yyyy-MM-dd'T'HH:mm:ss"                 1999-10-31T02:00:00
							return (ParseISO8601(raw, str, styles, result));
						}
						else {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return false;
						}
					}

					//
					// Advance to the next state, and continue
					//
					dps = dateParsingStates[(int)dps][(int)dtok.dtt];

					if (dps == DS::Error)
					{
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
					else if (dps > DS::Error)
					{
						if (((DateTimeFormatFlags)CultureInfo_impl::GetImpl(dtfi)->_data->formatFlags & DateTimeFormatFlags::UseHebrewRule) != DateTimeFormatFlags::None)
						{
							if (!ProcessHebrewTerminalState(dps, result, styles, raw, dtfi)) {
								return false;
							}
						}
						else {
							if (!ProcessTerminaltState(dps, result, styles, raw, dtfi)) {
								return false;
							}
						}
						reachTerminalState = true;

						//
						// If we have reached a terminal state, start over from DS::BEGIN again.
						// For example, when we parsed "1999-12-23 13:30", we will reach a terminal state at "1999-12-23",
						// and we start over so we can continue to parse "12:30".
						//
						dps = DS::BEGIN;
					}
				}
			} while (dtok.dtt != DTT::End && dtok.dtt != DTT::NumEnd && dtok.dtt != DTT::MonthEnd);

			if (!reachTerminalState) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			AdjustTimeMark(dtfi, raw);
			if (!AdjustHour(result.Hour, raw.timeMark)) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			// Check if the parased string only contains hour/minute/second values.
			bool bTimeOnly = (result.Year == -1 && result.Month == -1 && result.Day == -1);

			//
			// Check if any year/month/day is missing in the parsing string.
			// If yes, get the default value from today's date.
			//
			if (!CheckDefaultDateTime(result, result.calendar, styles)) {
				return false;
			}

			if (!result.calendar->TryToDateTime(result.Year, result.Month, result.Day,
				result.Hour, result.Minute, result.Second, 0, result.era, time)) {
				result.SetFailure(ParseFailureKind::FormatBadDateTimeCalendar, "Format_BadDateTimeCalendar");
				return false;
			}
			if (raw.fraction > 0) {
				time = time.AddTicks((Int64)round(raw.fraction * 10000000LL));
			}

			//
			// We have to check day of week before we adjust to the time zone.
			// Otherwise, the value of day of week may change after adjustting to the time zone.
			//
			if (raw.dayOfWeek != -1) {
				//
				// Check if day of week is correct.
				//
				if (raw.dayOfWeek != (int)result.calendar->GetDayOfWeek(time)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDayOfWeek");
					return false;
				}
			}

			result.parsedDate = time;

			if (!DetermineTimeZoneAdjustments(result, styles, bTimeOnly)) {
				return false;
			}
			return true;
		}


		// Handles time zone adjustments and sets DateTimeKind values as required by the styles
		static bool DetermineTimeZoneAdjustments(DateTimeResult& result, DateTimeStyles styles, bool bTimeOnly)
		{

			if ((result.flags & ParseFlags::CaptureOffset) != (ParseFlags)0)
			{
				// This is a DateTimeOffset parse, so the offset will actually be captured directly, and 
				// no adjustment is required in most cases
				return DateTimeOffsetTimeZonePostProcessing(result, styles);
			}
			// The flags AssumeUniveral and AssumeLocal only apply when the input does not have a time zone
			if ((result.flags & ParseFlags::TimeZoneUsed) == (ParseFlags)0)
			{
				// If AssumeLocal or AssumeLocal is used, there will always be a kind specified. As in the
				// case when a time zone is present, it will default to being local unless AdjustToUniversal
				// is present. These comparisons determine whether setting the kind is sufficient, or if a
				// time zone adjustment is required. For consistentcy with the rest of parsing, it is desirable
				// to fall through to the Adjust methods below, so that there is consist handling of boundary
				// cases like wrapping around on time-only dates and temporarily allowing an adjusted date
				// to exceed DateTime::MaxValue
				if ((styles & DateTimeStyles::AssumeLocal) != 0) {
					if ((styles & DateTimeStyles::AdjustToUniversal) != 0) {
						result.flags |= ParseFlags::TimeZoneUsed;
						result.timeZoneOffset = TimeZoneInfo_impl::GetLocalUtcOffset(result.parsedDate);
					}
					else {
						result.parsedDate = DateTime::SpecifyKind(result.parsedDate, DateTimeKind::Local);
						return true;
					}
				}
				else if ((styles & DateTimeStyles::AssumeUniversal) != 0) {
					if ((styles & DateTimeStyles::AdjustToUniversal) != 0) {
						result.parsedDate = DateTime::SpecifyKind(result.parsedDate, DateTimeKind::Utc);
						return true;
					}
					else {
						result.flags |= ParseFlags::TimeZoneUsed;
						result.timeZoneOffset = TimeSpan();
					}
				}
				else {
					// No time zone and no Assume flags, so DateTimeKind::Unspecified is fine
					return true;
				}
			}

			if (((styles & DateTimeStyles::RoundtripKind) != 0) && ((result.flags & ParseFlags::TimeZoneUtc) != (ParseFlags)0))
			{
				result.parsedDate = DateTime::SpecifyKind(result.parsedDate, DateTimeKind::Utc);
				return true;
			}

			if ((styles & DateTimeStyles::AdjustToUniversal) != 0) {
				return (AdjustTimeZoneToUniversal(result));
			}
			return (AdjustTimeZoneToLocal(result, bTimeOnly));
		}

		// Apply validation and adjustments specific to DateTimeOffset
		static bool DateTimeOffsetTimeZonePostProcessing(DateTimeResult& result, DateTimeStyles styles)
		{

			// For DateTimeOffset, default to the Utc or Local offset when an offset was not specified by 
			// the input string.
			if ((result.flags & ParseFlags::TimeZoneUsed) == (ParseFlags)0) {
				if ((styles & DateTimeStyles::AssumeUniversal) != 0) {
					// AssumeUniversal causes the offset to default to zero (0)
					result.timeZoneOffset = TimeSpan();
				}
				else {
					// AssumeLocal causes the offset to default to Local.  This flag is on by default for DateTimeOffset.
					result.timeZoneOffset = TimeZoneInfo_impl::GetLocalUtcOffset(result.parsedDate);
				}
			}

			Int64 offsetTicks = result.timeZoneOffset.Ticks;

			// there should be no overflow, because the offset can be no more than -+100 hours and the date already
			// fits within a DateTime::
			Int64 utcTicks = result.parsedDate.Ticks - offsetTicks;

			// For DateTimeOffset, both the parsed time and the corresponding UTC value must be within the boundaries
			// of a DateTime instance.            
			if (utcTicks < 0LL || utcTicks > 3155378975999999999LL) {
				result.SetFailure(ParseFailureKind::Format, "Format_UTCOutOfRange");
				return false;
			}

			// the offset must be within +- 14:00 hours.
			if (offsetTicks < -504000000000LL || offsetTicks > 504000000000LL) {
				result.SetFailure(ParseFailureKind::Format, "Format_OffsetOutOfRange");
				return false;
			}

			// DateTimeOffset should still honor the AdjustToUniversal flag for consistency with DateTime:: It means you
			// want to return an adjusted UTC value, so store the utcTicks in the DateTime and set the offset to zero
			if ((styles & DateTimeStyles::AdjustToUniversal) != 0) {
				if (((result.flags & ParseFlags::TimeZoneUsed) == (ParseFlags)0) && ((styles & DateTimeStyles::AssumeUniversal) == 0))
				{
					// Handle the special case where the timeZoneOffset was defaulted to Local
					bool toUtcResult = AdjustTimeZoneToUniversal(result);
					result.timeZoneOffset = TimeSpan();
					return toUtcResult;
				}

				// The constructor should always succeed because of the range check earlier in the function
				// Althought it is UTC, internally DateTimeOffset does not use this flag
				result.parsedDate = DateTime(utcTicks, DateTimeKind::Utc);
				result.timeZoneOffset = TimeSpan();
			}

			return true;
		}


		//
		// Adjust the specified time to universal time based on the supplied timezone.
		// E.g. when parsing "2001/06/08 14:00-07:00",
		// the time is 2001/06/08 14:00, and timeZoneOffset = -07:00.
		// The result will be "2001/06/08 21:00"
		//
		static bool AdjustTimeZoneToUniversal(DateTimeResult& result)
		{
			long long resultTicks = result.parsedDate.Ticks;
			resultTicks -= result.timeZoneOffset.Ticks;
			if (resultTicks < 0) {
				resultTicks += 864000000000LL;
			}

			if (resultTicks < 0LL || resultTicks > 3155378975999999999LL) {
				result.SetFailure(ParseFailureKind::Format, "Format_DateOutOfRange");
				return false;
			}
			result.parsedDate = DateTime(resultTicks, DateTimeKind::Utc);
			return true;
		}

		//
		// Adjust the specified time to universal time based on the supplied timezone,
		// and then convert to local time.
		// E.g. when parsing "2001/06/08 14:00-04:00", and local timezone is GMT-7.
		// the time is 2001/06/08 14:00, and timeZoneOffset = -05:00.
		// The result will be "2001/06/08 11:00"
		//
		static bool AdjustTimeZoneToLocal(DateTimeResult& result, bool bTimeOnly)
		{
			long long resultTicks = result.parsedDate.Ticks;
			// Convert to local ticks
			const TimeZoneInfo_impl* tz = TimeZoneInfo_impl::Local();
			bool isAmbiguousLocalDst = false;
			if (resultTicks < 864000000000LL) {
				//
				// This is time of day.
				//

				// Adjust timezone.
				resultTicks -= result.timeZoneOffset.Ticks;
				// If the time is time of day, use the current timezone offset.
				resultTicks += tz->GetLocalUtcOffset(bTimeOnly ? DateTime::Now() : result.parsedDate).Ticks;

				if (resultTicks < 0) {
					resultTicks += 864000000000LL;
				}
			}
			else {
				// Adjust timezone to GMT.
				resultTicks -= result.timeZoneOffset.Ticks;
				if (resultTicks < 0LL || resultTicks > 3155378975999999999LL) {
					// If the result ticks is greater than DateTime::MaxValue, we can not create a DateTime from this ticks.
					// In this case, keep using the old code.
					resultTicks += tz->GetUtcOffset(result.parsedDate).Ticks;
				}
				else {
					// Convert the GMT time to local time.
					DateTime utcDt = DateTime(resultTicks, DateTimeKind::Utc);
					bool isDaylightSavings = false;
					resultTicks += TimeZoneInfo_impl::GetUtcOffsetFromUtc(utcDt, TimeZoneInfo_impl::Local(), isDaylightSavings, isAmbiguousLocalDst).Ticks;
				}
			}
			if (resultTicks < 0LL || resultTicks > 3155378975999999999LL) {
				result.parsedDate = DateTime::MinValue;
				result.SetFailure(ParseFailureKind::Format, "Format_DateOutOfRange");
				return false;
			}
			result.parsedDate = DateTime(resultTicks, isAmbiguousLocalDst);
			return true;
		}

		//
		// Parse the ISO8601 format string found during Parse();
		//
		//
		static bool ParseISO8601(DateTimeRawInfo& raw, __DTString& str, DateTimeStyles styles, DateTimeResult& result)
		{
			if (raw.year < 0 || raw.GetNumber(0) < 0 || raw.GetNumber(1) < 0) {
			}
			str.Index--;
			int hour, minute;
			int second = 0;
			double partSecond = 0;

			str.SkipWhiteSpaces();
			if (!ParseDigits(str, 2, hour)) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			str.SkipWhiteSpaces();
			if (!str.Match(':')) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			str.SkipWhiteSpaces();
			if (!ParseDigits(str, 2, minute)) {
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}
			str.SkipWhiteSpaces();
			if (str.Match(':')) {
				str.SkipWhiteSpaces();
				if (!ParseDigits(str, 2, second)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				if (str.Match('.')) {
					if (!ParseFraction(str, partSecond)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
					str.Index--;
				}
				str.SkipWhiteSpaces();
			}
			if (str.GetNext()) {
				wchar_t ch = str.GetChar();
				if (ch == '+' || ch == '-') {
					result.flags |= ParseFlags::TimeZoneUsed;
					if (!ParseTimeZone(str, result.timeZoneOffset)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
				}
				else if (ch == 'Z' || ch == 'z') {
					result.flags |= ParseFlags::TimeZoneUsed;
					result.timeZoneOffset = TimeSpan();
					result.flags |= ParseFlags::TimeZoneUtc;
				}
				else {
					str.Index--;
				}
				str.SkipWhiteSpaces();
				if (str.Match('#')) {
					if (!VerifyValidPunctuation(str)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
					str.SkipWhiteSpaces();
				}
				if (str.Match('\0')) {
					if (!VerifyValidPunctuation(str)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
				}
				if (str.GetNext()) {
					// If this is true, there were non-white space characters remaining in the DateTime
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
			}

			DateTime time;
			if (!CalendarHelper::GregorianTryToDateTime(raw.year, raw.GetNumber(0), raw.GetNumber(1),
				hour, minute, second, 0, result.era, time)) {
				result.SetFailure(ParseFailureKind::FormatBadDateTimeCalendar, "Format_BadDateTimeCalendar");
				return false;
			}

			time = time.AddTicks((Int64)round(partSecond * 10000000LL));
			result.parsedDate = time;
			if (!DetermineTimeZoneAdjustments(result, styles, false)) {
				return false;
			}
			return true;
		}


		////////////////////////////////////////////////////////////////////////
		//
		// Actions:
		//    Parse the current word as a Hebrew number.
		//      This is used by DateTime::ParseExact().
		//
		////////////////////////////////////////////////////////////////////////

		static bool MatchHebrewDigits(__DTString& str, int digitLen, int& number)
		{
			number = 0;

			// Create a context object so that we can parse the Hebrew number text character by character.
			HebrewNumberParsingContext context = HebrewNumberParsingContext(0);

			// Set this to ContinueParsing so that we will run the following while loop in the first time.
			HebrewNumberParsingState state = HebrewNumberParsingState::ContinueParsing;

			while (state == HebrewNumberParsingState::ContinueParsing && str.GetNext()) {
				state = HebrewNumber::ParseByChar(str.GetChar(), context);
			}

			if (state == HebrewNumberParsingState::FoundEndOfHebrewNumber) {
				// If we have reached a terminal state, update the result and returns.
				number = context.result;
				return (true);
			}

			// If we run out of the character before reaching FoundEndOfHebrewNumber, or
			// the state is InvalidHebrewNumber or ContinueParsing, we fail to match a Hebrew number.
			// Return an error.
			return false;
		}

		/*=================================ParseDigits==================================
		**Action: Parse the number string in __DTString that are formatted using
		**        the following patterns:
		**        "0", "00", and "000..0"
		**Returns: the integer value
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if error in parsing number.
		==============================================================================*/

		static bool ParseDigits(__DTString& str, int digitLen, int& result)
		{
			if (digitLen == 1) {
				// 1 really means 1 or 2 for this call
				return ParseDigits(str, 1, 2, result);
			}
			else {
				return ParseDigits(str, digitLen, digitLen, result);
			}
		}

		static bool ParseDigits(__DTString& str, int minDigitLen, int maxDigitLen, int& result)
		{
			result = 0;
			int startingIndex = str.Index;
			int tokenLength = 0;
			while (tokenLength < maxDigitLen) {
				if (!str.GetNextDigit()) {
					str.Index--;
					break;
				}
				result = result * 10 + str.GetDigit();
				tokenLength++;
			}
			if (tokenLength < minDigitLen) {
				str.Index = startingIndex;
				return false;
			}
			return true;
		}

		/*=================================ParseFractionExact==================================
		**Action: Parse the number string in __DTString that are formatted using
		**        the following patterns:
		**        "0", "00", and "000..0"
		**Returns: the fraction value
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if error in parsing number.
		==============================================================================*/

		static bool ParseFractionExact(__DTString& str, int maxDigitLen, double& result)
		{
			if (!str.GetNextDigit()) {
				str.Index--;
				return false;
			}
			result = str.GetDigit();

			int digitLen = 1;
			for (; digitLen < maxDigitLen; digitLen++) {
				if (!str.GetNextDigit()) {
					str.Index--;
					break;
				}
				result = result * 10 + str.GetDigit();
			}

			result = ((double)result / pow(10, digitLen));
			return (digitLen == maxDigitLen);
		}

		/*=================================ParseSign==================================
		**Action: Parse a positive or a negative sign.
		**Returns:      true if postive sign.  flase if negative sign.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions:   Formatstd::exception if end of string is encountered or a sign
		**              symbol is not found.
		==============================================================================*/

		static bool ParseSign(__DTString& str, bool& result)
		{
			if (!str.GetNext()) {
				// A sign symbol ('+' or '-') is expected. However, end of string is encountered.
				return false;
			}
			wchar_t ch = str.GetChar();
			if (ch == '+') {
				result = true;
				return (true);
			}
			else if (ch == '-') {
				result = false;
				return (true);
			}
			// A sign symbol ('+' or '-') is expected.
			return false;
		}

		/*=================================ParseTimeZoneOffset==================================
		**Action: Parse the string formatted using "z", "zz", "zzz" in DateTime::Format().
		**Returns: the TimeSpan for the parsed timezone offset.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**              len: the repeated number of the "z"
		**std::exceptions: Formatstd::exception if errors in parsing.
		==============================================================================*/

		static bool ParseTimeZoneOffset(__DTString& str, int len, TimeSpan& result)
		{
			bool isPositive = true;
			int hourOffset;
			int minuteOffset = 0;

			switch (len) {
			case 1:
			case 2:
				if (!ParseSign(str, isPositive)) {
					return (false);
				}
				if (!ParseDigits(str, len, hourOffset)) {
					return (false);
				}
				break;
			default:
				if (!ParseSign(str, isPositive)) {
					return (false);
				}

				// Parsing 1 digit will actually parse 1 or 2.
				if (!ParseDigits(str, 1, hourOffset)) {
					return (false);
				}
				// ':' is optional.
				if (str.Match(L":")) {
					// Found ':'
					if (!ParseDigits(str, 2, minuteOffset)) {
						return (false);
					}
				}
				else {
					// Since we can not match ':', put the char back.
					str.Index--;
					if (!ParseDigits(str, 2, minuteOffset)) {
						return (false);
					}
				}
				break;
			}
			if (minuteOffset < 0 || minuteOffset >= 60) {
				return false;
			}

			result = (TimeSpan(hourOffset, minuteOffset, 0));
			if (!isPositive) {
				result = result.Negate;
			}
			return (true);
		}

		/*=================================MatchAbbreviatedMonthName==================================
		**Action: Parse the abbreviated month name from string starting at str.Index.
		**Returns: A value from 1 to 12 for the first month to the twelveth month.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if an abbreviated month name can not be found.
		==============================================================================*/

		static bool MatchAbbreviatedMonthName(__DTString& str, const CultureInfo& dtfi, int& result)
		{
			int maxMatchStrLen = 0;
			result = -1;
			if (str.GetNext()) {
				//
				// Scan the month names (note that some calendars has 13 months) and find
				// the matching month name which has the max string length.
				// We need to do this because some cultures (e.g. "cs-CZ") which have
				// abbreviated month names with the same prefix.
				//
				int monthsInYear = (dtfi.GetBooleanFlags(CultureInfoBooleanFlags::Has13Months) ? 13 : 12);
				for (int i = 1; i <= monthsInYear; i++) {
					std::wstring searchStr = dtfi.GetConstsName((CultureInfoConstNames)(25+i));
					int matchStrLen = searchStr.length();
					if (CultureInfo_impl::GetImpl(dtfi)->_data->HasSpacesInMonthNames
						? str.MatchSpecifiedWords(searchStr, false, matchStrLen)
						: str.MatchSpecifiedWord(searchStr)) {
						if (matchStrLen > maxMatchStrLen) {
							maxMatchStrLen = matchStrLen;
							result = i;
						}
					}
				}

				// Search leap year form.
				if (((DateTimeFormatFlags)CultureInfo_impl::GetImpl(dtfi)->_data->formatFlags & DateTimeFormatFlags::UseLeapYearMonth) != DateTimeFormatFlags::None)
				{
					int tempResult = str.MatchLongestWords(CultureInfo_impl::GetImpl(dtfi)->_defaultCalData->saLeapYearMonthNames, maxMatchStrLen);
					// We found a longer match in the leap year month name.  Use this as the result.
					// The result from MatchLongestWords is 0 ~ length of word array.
					// So we increment the result by one to become the month value.
					if (tempResult >= 0) {
						result = tempResult + 1;
					}
				}


			}
			if (result > 0) {
				str.Index += (maxMatchStrLen - 1);
				return (true);
			}
			return false;
		}

		/*=================================MatchMonthName==================================
		**Action: Parse the month name from string starting at str.Index.
		**Returns: A value from 1 to 12 indicating the first month to the twelveth month.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if a month name can not be found.
		==============================================================================*/

		static bool MatchMonthName(__DTString& str, const CultureInfo& dtfi, int& result)
		{
			int maxMatchStrLen = 0;
			result = -1;
			if (str.GetNext()) {
				//
				// Scan the month names (note that some calendars has 13 months) and find
				// the matching month name which has the max string length.
				// We need to do this because some cultures (e.g. "vi-VN") which have
				// month names with the same prefix.
				//
				int monthsInYear = (dtfi.GetBooleanFlags(CultureInfoBooleanFlags::Has13Months) ? 13 : 12);
				for (int i = 1; i <= monthsInYear; i++) {
					std::wstring searchStr = dtfi.GetConstsName((CultureInfoConstNames)(13 + i));
					int matchStrLen = searchStr.length();
					if (CultureInfo_impl::GetImpl(dtfi)->_data->HasSpacesInMonthNames
						? str.MatchSpecifiedWords(searchStr, false, matchStrLen)
						: str.MatchSpecifiedWord(searchStr)) {
						if (matchStrLen > maxMatchStrLen) {
							maxMatchStrLen = matchStrLen;
							result = i;
						}
					}
				}

				// Search genitive form.
				if (((DateTimeFormatFlags)CultureInfo_impl::GetImpl(dtfi)->_data->formatFlags & DateTimeFormatFlags::UseGenitiveMonth) != DateTimeFormatFlags::None) {
					int tempResult = str.MatchLongestWords(CultureInfo_impl::GetImpl(dtfi)->_defaultCalData->saMonthGenitiveNames, maxMatchStrLen);
					// We found a longer match in the genitive month name.  Use this as the result.
					// The result from MatchLongestWords is 0 ~ length of word array.
					// So we increment the result by one to become the month value.
					if (tempResult >= 0) {
						result = tempResult + 1;
					}
				}

				// Search leap year form.
				if (((DateTimeFormatFlags)CultureInfo_impl::GetImpl(dtfi)->_data->formatFlags & DateTimeFormatFlags::UseLeapYearMonth) != DateTimeFormatFlags::None) {
					int tempResult = str.MatchLongestWords(CultureInfo_impl::GetImpl(dtfi)->_defaultCalData->saLeapYearMonthNames, maxMatchStrLen);
					// We found a longer match in the leap year month name.  Use this as the result.
					// The result from MatchLongestWords is 0 ~ length of word array.
					// So we increment the result by one to become the month value.
					if (tempResult >= 0) {
						result = tempResult + 1;
					}
				}


			}

			if (result > 0) {
				str.Index += (maxMatchStrLen - 1);
				return (true);
			}
			return false;
		}

		/*=================================MatchAbbreviatedDayName==================================
		**Action: Parse the abbreviated day of week name from string starting at str.Index.
		**Returns: A value from 0 to 6 indicating Sunday to Saturday.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if a abbreviated day of week name can not be found.
		==============================================================================*/

		static bool MatchAbbreviatedDayName(__DTString& str, const CultureInfo& dtfi, int& result)
		{
			int maxMatchStrLen = 0;
			result = -1;
			if (str.GetNext()) {
				for (int i = (int)DayOfWeek::Sunday; i <= (int)DayOfWeek::Saturday; i++) {
					std::wstring searchStr = dtfi.GetConstsName((CultureInfoConstNames)(7 + i));
					int matchStrLen = searchStr.length();
					if (CultureInfo_impl::GetImpl(dtfi)->_data->HasSpacesInDayNames
						? str.MatchSpecifiedWords(searchStr, false, matchStrLen)
						: str.MatchSpecifiedWord(searchStr)) {
						if (matchStrLen > maxMatchStrLen) {
							maxMatchStrLen = matchStrLen;
							result = (int)i;
						}
					}
				}
			}
			if (result >= 0) {
				str.Index += maxMatchStrLen - 1;
				return (true);
			}
			return false;
		}

		/*=================================MatchDayName==================================
		**Action: Parse the day of week name from string starting at str.Index.
		**Returns: A value from 0 to 6 indicating Sunday to Saturday.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if a day of week name can not be found.
		==============================================================================*/

		static bool MatchDayName(__DTString& str, const CultureInfo& dtfi, int& result)
		{
			// Turkish (tr-TR) got day names with the same prefix.
			int maxMatchStrLen = 0;
			result = -1;
			if (str.GetNext()) {
				for (int i = (int)DayOfWeek::Sunday; i <= (int)DayOfWeek::Saturday; i++) {
					std::wstring searchStr = dtfi.GetConstsName((CultureInfoConstNames)(i));
					int matchStrLen = searchStr.length();
					if (CultureInfo_impl::GetImpl(dtfi)->_data->HasSpacesInDayNames
						? str.MatchSpecifiedWords(searchStr, false, matchStrLen)
						: str.MatchSpecifiedWord(searchStr)) {
						if (matchStrLen > maxMatchStrLen) {
							maxMatchStrLen = matchStrLen;
							result = (int)i;
						}
					}
				}
			}
			if (result >= 0) {
				str.Index += maxMatchStrLen - 1;
				return (true);
			}
			return false;
		}

		/*=================================MatchEraName==================================
		**Action: Parse era name from string starting at str.Index.
		**Returns: An era value.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if an era name can not be found.
		==============================================================================*/

		static bool MatchEraName(__DTString& str, const CultureInfo& dtfi,  int& result)
		{
			if (str.GetNext()) {
				std::vector<int> eras = dtfi.Calendar->GetEras();

				if (eras.size()>0) {
					for (int i = 0; i < (int)eras.size(); i++) {
						std::wstring searchStr = dtfi.GetConstsName((CultureInfoConstNames)(CultureInfoConstNames::Era), i);
						if (str.MatchSpecifiedWord(searchStr)) {
							str.Index += (searchStr.length() - 1);
							result = eras[i];
							return (true);
						}
						searchStr = dtfi.GetConstsName((CultureInfoConstNames)(CultureInfoConstNames::Abbreviated_Era), i);
						if (str.MatchSpecifiedWord(searchStr)) {
							str.Index += (searchStr.length() - 1);
							result = eras[i];
							return (true);
						}
					}
				}
			}
			return false;
		}

		/*=================================MatchTimeMark==================================
		**Action: Parse the time mark (AM/PM) from string starting at str.Index.
		**Returns: TM_AM or TM_PM.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if a time mark can not be found.
		==============================================================================*/

		static bool MatchTimeMark(__DTString& str, const CultureInfo& dtfi, TM& result)
		{
			result = TM::NotSet;

			std::wstring am = CultureInfo_impl::GetImpl(dtfi)->_data->amDesignator;
			std::wstring pm = CultureInfo_impl::GetImpl(dtfi)->_data->pmDesignator;

			// In some cultures have empty strings in AM/PM mark. E.g. af-ZA (0x0436), the AM mark is "", and PM mark is "nm".
			if (am.length() == 0) {
				result = TM::AM;
			}
			if (pm.length() == 0) {
				result = TM::PM;
			}

			if (str.GetNext()) {
				if (am.length() > 0) {
					if (str.MatchSpecifiedWord(am)) {
						// Found an AM timemark with length > 0.
						str.Index += (am.length() - 1);
						result = TM::AM;
						return (true);
					}
				}
				if (pm.length() > 0) {
					if (str.MatchSpecifiedWord(pm)) {
						// Found a PM timemark with length > 0.
						str.Index += (pm.length() - 1);
						result = TM::PM;
						return (true);
					}
				}
				str.Index--; // Undo the GetNext call.
			}
			if (result != TM::NotSet) {
				// If one of the AM/PM marks is empty string, return the result.
				return (true);
			}
			return false;
		}

		/*=================================MatchAbbreviatedTimeMark==================================
		**Action: Parse the abbreviated time mark (AM/PM) from string starting at str.Index.
		**Returns: TM_AM or TM_PM.
		**Arguments:    str: a __DTString.  The parsing will start from the
		**              next character after str.Index.
		**std::exceptions: Formatstd::exception if a abbreviated time mark can not be found.
		==============================================================================*/

		static bool MatchAbbreviatedTimeMark(__DTString& str, const CultureInfo& dtfi, TM& result)
		{
			std::wstring am = CultureInfo_impl::GetImpl(dtfi)->_data->amDesignator;
			std::wstring pm = CultureInfo_impl::GetImpl(dtfi)->_data->pmDesignator;
			// NOTENOTE : the assumption here is that abbreviated time mark is the first
			// character of the AM/PM designator.  If this invariant changes, we have to
			// change the code below.
			if (str.GetNext())
			{
				if (str.GetChar() ==am[0]) {
					result = TM::AM;
					return (true);
				}
				if (str.GetChar() == pm[0]) {
					result = TM::PM;
					return (true);
				}
			}
			return false;
		}

		/*=================================CheckNewValue==================================
		**Action: Check if currentValue is initialized.  If not, return the newValue.
		**        If yes, check if the current value is equal to newValue.  Return false
		**        if they are not equal.  This is used to check the case like "d" and "dd" are both
		**        used to format a string.
		**Returns: the correct value for currentValue.
		**Arguments:
		**std::exceptions:
		==============================================================================*/

		static bool CheckNewValue(int& currentValue, int newValue, wchar_t patternChar, DateTimeResult& result)
		{
			if (currentValue == -1) {
				currentValue = newValue;
				return (true);
			}
			else {
				if (newValue != currentValue) {
					result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_RepeatDateTimePattern");
					return (false);
				}
			}
			return (true);
		}

		static DateTime GetDateTimeNow(DateTimeResult& result, DateTimeStyles& styles)
		{
			if ((result.flags & ParseFlags::CaptureOffset) != (ParseFlags)0)
			{
				if ((result.flags & ParseFlags::TimeZoneUsed) != (ParseFlags)0) {
					// use the supplied offset to calculate 'Now'
					return DateTime(DateTime::UtcNow().Ticks + result.timeZoneOffset.Ticks, DateTimeKind::Unspecified);
				}
				else if ((styles & DateTimeStyles::AssumeUniversal) != 0) {
					// assume the offset is Utc
					return DateTime::UtcNow();
				}
			}

			// assume the offset is Local            
			return DateTime::Now();
		}

		static bool CheckDefaultDateTime(DateTimeResult& result, const Calendar* cal, DateTimeStyles styles)
		{

			if ((result.flags & ParseFlags::CaptureOffset) != (ParseFlags)0) {
				// DateTimeOffset.Parse should allow dates without a year, but only if there is also no time zone marker;
				// e.g. "May 1 5pm" is OK, but "May 1 5pm -08:30" is not.  This is somewhat pragmatic, since we would
				// have to rearchitect parsing completely to allow this one case to correctly handle things like leap
				// years and leap months.  Is is an extremely corner case, and DateTime is basically incorrect in that
				// case today.
				//
				// values like "11:00Z" or "11:00 -3:00" are also acceptable
				//
				// if ((month or day is set) and (year is not set and time zone is set))
				//
				if (((result.Month != -1) || (result.Day != -1))
					&& ((result.Year == -1 || ((result.flags & ParseFlags::YearDefault) != (ParseFlags)0)) && (result.flags & ParseFlags::TimeZoneUsed) != (ParseFlags)0)) {
					result.SetFailure(ParseFailureKind::Format, "Format_MissingIncompleteDate");
					return false;
				}
			}


			if ((result.Year == -1) || (result.Month == -1) || (result.Day == -1)) {
				/*
				The following table describes the behaviors of getting the default value
				when a certain year/month/day values are missing.

				An "X" means that the value exists.  And "--" means that value is missing.

				Year    Month   Day =>  ResultYear  ResultMonth     ResultDay       Note

				X       X       X       Parsed year Parsed month    Parsed day
				X       X       --      Parsed Year Parsed month    First day       If we have year and month, assume the first day of that month.
				X       --      X       Parsed year First month     Parsed day      If the month is missing, assume first month of that year.
				X       --      --      Parsed year First month     First day       If we have only the year, assume the first day of that year.

				--      X       X       CurrentYear Parsed month    Parsed day      If the year is missing, assume the current year.
				--      X       --      CurrentYear Parsed month    First day       If we have only a month value, assume the current year and current day.
				--      --      X       CurrentYear First month     Parsed day      If we have only a day value, assume current year and first month.
				--      --      --      CurrentYear Current month   Current day     So this means that if the date string only contains time, you will get current date.

				*/

				DateTime now = GetDateTimeNow(result, styles);
				if (result.Month == -1 && result.Day == -1) {
					if (result.Year == -1) {
						if ((styles & DateTimeStyles::NoCurrentDateDefault) != 0) {
							// If there is no year/month/day values, and NoCurrentDateDefault flag is used,
							// set the year/month/day value to the beginning year/month/day of DateTime().
							// Note we should be using Gregorian for the year/month/day.
							cal = &CalendarHelper::GetGregorianDefaultInstance();
							result.Year = result.Month = result.Day = 1;
						}
						else {
							// Year/Month/Day are all missing.
							result.Year = cal->GetYear(now);
							result.Month = cal->GetMonth(now);
							result.Day = cal->GetDayOfMonth(now);
						}
					}
					else {
						// Month/Day are both missing.
						result.Month = 1;
						result.Day = 1;
					}
				}
				else {
					if (result.Year == -1) {
						result.Year = cal->GetYear(now);
					}
					if (result.Month == -1) {
						result.Month = 1;
					}
					if (result.Day == -1) {
						result.Day = 1;
					}
				}
			}
			// Set Hour/Minute/Second to zero if these value are not in str.
			if (result.Hour == -1) result.Hour = 0;
			if (result.Minute == -1) result.Minute = 0;
			if (result.Second == -1) result.Second = 0;
			if (result.era == -1) result.era = 0;
			return true;
		}

		// Expand a pre-defined format string (like "D" for long date) to the real format that
		// we are going to use in the date time parsing.
		// This method also set the dtfi according/parseInfo to some special pre-defined
		// formats.
		//
		static std::wstring ExpandPredefinedFormat(const std::wstring& format, CultureInfo& dtfi, ParsingInfo& parseInfo, DateTimeResult& result)
		{
			//
			// Check the format to see if we need to override the dtfi to be InvariantInfo,
			// and see if we need to set up the userUniversalTime flag.
			//
			switch (format[0]) {
			case 'o':
			case 'O':       // Round Trip Format
				parseInfo.calendar = &(CalendarHelper::GetGregorianDefaultInstance());
				dtfi = CultureInfo::Invariant();
				break;
			case 'r':
			case 'R':       // RFC 1123 Standard.  (in Universal time)
				parseInfo.calendar = &(CalendarHelper::GetGregorianDefaultInstance());
				dtfi = CultureInfo::Invariant();

				if ((result.flags & ParseFlags::CaptureOffset) != (ParseFlags)0)
				{
					result.flags |= ParseFlags::Rfc1123Pattern;
				}
				break;
			case 's':       // Sortable format (in local time)
				dtfi = CultureInfo::Invariant();
				parseInfo.calendar = &(CalendarHelper::GetGregorianDefaultInstance());
				break;
			case 'u':       // Universal time format in sortable format.
				parseInfo.calendar = &(CalendarHelper::GetGregorianDefaultInstance());
				dtfi = CultureInfo::Invariant();

				if ((result.flags & ParseFlags::CaptureOffset) != (ParseFlags)0)
				{
					result.flags |= ParseFlags::UtcSortPattern;
				}
				break;
			case 'U':       // Universal time format with culture-dependent format.
				parseInfo.calendar = &(CalendarHelper::GetGregorianDefaultInstance());
				result.flags |= ParseFlags::TimeZoneUsed;
				result.timeZoneOffset = TimeSpan(0);
				result.flags |= ParseFlags::TimeZoneUtc;
				if (!dtfi.Calendar->Is(_typeof(GregorianCalendar))) 
				{
					dtfi = dtfi.CreateByCalendar(&CalendarHelper::GetGregorianDefaultInstance());
				}
				break;
			}

			//
			// Expand the pre-defined format character to the real format from const CultureInfo&.
			//
			return (DateTimeFormat::GetRealFormat(format, dtfi));
		}





		// Given a specified format character, parse and update the parsing result.
		//
		static bool ParseByFormat(__DTString& str,__DTString& format,ParsingInfo& parseInfo,const CultureInfo& dtfi, DateTimeResult& result)
		{

			int tokenLen = 0;
			int tempYear = 0, tempMonth = 0, tempDay = 0, tempDayOfWeek = 0, tempHour = 0, tempMinute = 0, tempSecond = 0;
			double tempFraction = 0;
			TM tempTimeMark = TM::AM;

			wchar_t ch = format.GetChar();

			switch (ch) 
			{
			case 'y':
				tokenLen = format.GetRepeatCount();
				bool parseResult;
				if (dtfi.GetBooleanFlags(CultureInfoBooleanFlags::HasForceTwoDigitYears)) {
					parseResult = ParseDigits(str, 1, 4, tempYear);
				}
				else {
					if (tokenLen <= 2) {
						parseInfo.fUseTwoDigitYear = true;
					}
					parseResult = ParseDigits(str, tokenLen, tempYear);
				}
				if (!parseResult && parseInfo.fCustomNumberParser) {
					parseResult = MatchHebrewDigits(str, tokenLen,  tempYear);
				}
				if (!parseResult) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return (false);
				}
				if (!CheckNewValue(result.Year, tempYear, ch, result)) {
					return (false);
				}
				break;
			case 'M':
				tokenLen = format.GetRepeatCount();
				if (tokenLen <= 2) {
					if (!ParseDigits(str, tokenLen, tempMonth)) {
						if (!parseInfo.fCustomNumberParser ||
							!MatchHebrewDigits(str, tokenLen, tempMonth)) {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return (false);
						}
					}
				}
				else {
					if (tokenLen == 3) {
						if (!MatchAbbreviatedMonthName(str, dtfi, tempMonth)) {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return (false);
						}
					}
					else {
						if (!MatchMonthName(str, dtfi,  tempMonth)) {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return (false);
						}
					}
					result.flags |= ParseFlags::ParsedMonthName;
				}
				if (!CheckNewValue(result.Month, tempMonth, ch, result)) {
					return (false);
				}
				break;
			case 'd':
				// Day & Day of week
				tokenLen = format.GetRepeatCount();
				if (tokenLen <= 2) {
					// "d" & "dd"

					if (!ParseDigits(str, tokenLen, tempDay)) {
						if (!parseInfo.fCustomNumberParser ||
							!MatchHebrewDigits(str, tokenLen, tempDay)) {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return (false);
						}
					}
					if (!CheckNewValue(result.Day, tempDay, ch, result)) {
						return (false);
					}
				}
				else {
					if (tokenLen == 3) {
						// "ddd"
						if (!MatchAbbreviatedDayName(str, dtfi, tempDayOfWeek)) {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return (false);
						}
					}
					else {
						// "dddd*"
						if (!MatchDayName(str, dtfi,  tempDayOfWeek)) {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return (false);
						}
					}
					if (!CheckNewValue(parseInfo.dayOfWeek, tempDayOfWeek, ch, result)) {
						return (false);
					}
				}
				break;
			case 'g':
				tokenLen = format.GetRepeatCount();
				// Put the era value in result.era.
				if (!MatchEraName(str, dtfi, result.era)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return (false);
				}
				break;
			case 'h':
				parseInfo.fUseHour12 = true;
				tokenLen = format.GetRepeatCount();
				if (!ParseDigits(str, (tokenLen < 2 ? 1 : 2), tempHour)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return (false);
				}
				if (!CheckNewValue(result.Hour, tempHour, ch, result)) {
					return (false);
				}
				break;
			case 'H':
				tokenLen = format.GetRepeatCount();
				if (!ParseDigits(str, (tokenLen < 2 ? 1 : 2), tempHour)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return (false);
				}
				if (!CheckNewValue(result.Hour, tempHour, ch, result)) {
					return (false);
				}
				break;
			case 'm':
				tokenLen = format.GetRepeatCount();
				if (!ParseDigits(str, (tokenLen < 2 ? 1 : 2), tempMinute)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return (false);
				}
				if (!CheckNewValue(result.Minute, tempMinute, ch, result)) {
					return (false);
				}
				break;
			case 's':
				tokenLen = format.GetRepeatCount();
				if (!ParseDigits(str, (tokenLen < 2 ? 1 : 2), tempSecond)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return (false);
				}
				if (!CheckNewValue(result.Second, tempSecond, ch, result)) {
					return (false);
				}
				break;
			case 'f':
			case 'F':
				tokenLen = format.GetRepeatCount();
				if (tokenLen <= 8) {
					if (!ParseFractionExact(str, tokenLen, tempFraction)) {
						if (ch == 'f') {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return (false);
						}
					}
					if (result.fraction < 0) {
						result.fraction = tempFraction;
					}
					else {
						if (tempFraction != result.fraction) {
							result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_RepeatDateTimePattern");
							return (false);
						}
					}
				}
				else {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return (false);
				}
				break;
			case 't':
				// AM/PM designator
				tokenLen = format.GetRepeatCount();
				if (tokenLen == 1) {
					if (!MatchAbbreviatedTimeMark(str, dtfi, tempTimeMark)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return (false);
					}
				}
				else {
					if (!MatchTimeMark(str, dtfi, tempTimeMark)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return (false);
					}
				}

				if (parseInfo.timeMark == TM::NotSet) {
					parseInfo.timeMark = tempTimeMark;
				}
				else {
					if (parseInfo.timeMark != tempTimeMark) {
						result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_RepeatDateTimePattern");
						return (false);
					}
				}
				break;
			case 'z':
				// timezone offset
				tokenLen = format.GetRepeatCount();
				{
					TimeSpan tempTimeZoneOffset = TimeSpan(0);
					if (!ParseTimeZoneOffset(str, tokenLen, tempTimeZoneOffset)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return (false);
					}
					if ((result.flags & ParseFlags::TimeZoneUsed) != (ParseFlags)0 && tempTimeZoneOffset != result.timeZoneOffset) {
						result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_RepeatDateTimePattern");
						return (false);
					}
					result.timeZoneOffset = tempTimeZoneOffset;
					result.flags |= ParseFlags::TimeZoneUsed;
				}
				break;
			case 'Z':
				if ((result.flags & ParseFlags::TimeZoneUsed) != (ParseFlags)0 && result.timeZoneOffset.Ticks!=0LL)
				{
					result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_RepeatDateTimePattern");
					return (false);
				}

				result.flags |= ParseFlags::TimeZoneUsed;
				result.timeZoneOffset = TimeSpan();
				result.flags |= ParseFlags::TimeZoneUtc;

				// The updating of the indexes is to reflect that ParseExact MatchXXX methods assume that
				// they need to increment the index and Parse GetXXX do not. Since we are calling a Parse
				// method from inside ParseExact we need to adjust this. Long term, we should try to
				// eliminate this discrepancy.
				str.Index++;
				if (!GetTimeZoneName(str)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				str.Index--;
				break;
			case 'K':
				// This should parse either as a blank, the 'Z' character or a local offset like "-07:00"
				if (str.Match('Z')) {
					if ((result.flags & ParseFlags::TimeZoneUsed) != (ParseFlags)0 && result.timeZoneOffset.Ticks != 0LL)
					{
						result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_RepeatDateTimePattern");
						return (false);
					}

					result.flags |= ParseFlags::TimeZoneUsed;
					result.timeZoneOffset = TimeSpan(0);
					result.flags |= ParseFlags::TimeZoneUtc;
				}
				else if (str.Match('+') || str.Match('-')) {
					str.Index--; // Put the character back for the parser
					TimeSpan tempTimeZoneOffset = TimeSpan();
					if (!ParseTimeZoneOffset(str, 3, tempTimeZoneOffset)) {
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return (false);
					}
					if ((result.flags & ParseFlags::TimeZoneUsed) != (ParseFlags)0 && tempTimeZoneOffset != result.timeZoneOffset) {
						result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_RepeatDateTimePattern");
						return (false);
					}
					result.timeZoneOffset = tempTimeZoneOffset;
					result.flags |= ParseFlags::TimeZoneUsed;
				}
				// Otherwise it is unspecified and we consume no characters
				break;
			case ':':
				if (!str.Match(dtfi.GetConstsName(CultureInfoConstNames::TimeSeparator))) {
					// A time separator is expected.
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case '/':
				if (!str.Match(dtfi.GetConstsName(CultureInfoConstNames::DateSeparator))) {
					// A date separator is expected.
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			case '\"':
			case '\'':
			{
				std::wstring enquotedString;
				// Use ParseQuoteString so that we can handle escape characters within the quoted string.
				if (!TryParseQuoteString(format.Value, format.Index, enquotedString, tokenLen)) {
					result.SetFailure(ParseFailureKind::FormatWithParameter, "Format_BadQuote");
					return (false);
				}
				format.Index += tokenLen - 1;

				// Some cultures uses space in the quoted string.  E.g. Spanish has long date format as:
				// "dddd, dd' de 'MMMM' de 'yyyy".  When inner spaces flag is set, we should skip whitespaces if there is space
				// in the quoted string.
				std::wstring quotedStr = enquotedString;

				for (int i = 0; i < (int)quotedStr.length(); i++) {
					if (quotedStr[i] == ' ' && parseInfo.fAllowInnerWhite) {
						str.SkipWhiteSpaces();
					}
					else if (!str.Match(quotedStr[i])) {
						// Can not find the matching quoted string.
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
				}

				// The "r" and "u" formats incorrectly quoted 'GMT' and 'Z', respectively.  We cannot
				// correct this mistake for DateTime::ParseExact for compatibility reasons, but we can 
				// fix it for DateTimeOffset.ParseExact as DateTimeOffset has not been publically released
				// with this issue.
				if ((result.flags & ParseFlags::CaptureOffset) != (ParseFlags)0)
				{
					if ((result.flags & ParseFlags::Rfc1123Pattern) != (ParseFlags)0 && quotedStr == GMTName) {
						result.flags |= ParseFlags::TimeZoneUsed;
						result.timeZoneOffset = TimeSpan();
					}
					else if ((result.flags & ParseFlags::UtcSortPattern) != (ParseFlags)0 && quotedStr == ZuluName) {
						result.flags |= ParseFlags::TimeZoneUsed;
						result.timeZoneOffset = TimeSpan();
					}
				}
			}
				break;
			case '%':
				// Skip this so we can get to the next pattern character.
				// Used in case like "%d", "%y"

				// Make sure the next character is not a '%' again.
				if (format.Index >= (int)format.Value.length() - 1 || format.Value[format.Index + 1] == '%') {
					result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
					return false;
				}
				break;
			case '\\':
				// Escape character. For example, "\d".
				// Get the next character in format, and see if we can
				// find a match in str.
				if (format.GetNext()) {
					if (!str.Match(format.GetChar())) {
						// Can not find a match for the escaped character.
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
				}
				else {
					result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
					return false;
				}
				break;
			case '.':
				if (!str.Match(ch)) {
					if (format.GetNext()) {
						// If we encounter the pattern ".F", and the dot is not present, it is an optional
						// second fraction and we can skip this format.
						if (format.Match('F')) {
							format.GetRepeatCount();
							break;
						}
					}
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				break;
			default:
				if (ch == ' ') {
					if (parseInfo.fAllowInnerWhite) {
						// Skip whitespaces if AllowInnerWhite.
						// Do nothing here.
					}
					else {
						if (!str.Match(ch)) {
							// If the space does not match, and trailing space is allowed, we do
							// one more step to see if the next format character can lead to
							// successful parsing.
							// This is used to deal with special case that a empty string can match
							// a specific pattern.
							// The example here is af-ZA, which has a time format like "hh:mm:ss tt".  However,
							// its AM symbol is "" (empty string).  If fAllowTrailingWhite is used, and time is in
							// the AM, we will trim the whitespaces at the end, which will lead to a failure
							// when we are trying to match the space before "tt".
							if (parseInfo.fAllowTrailingWhite) {
								if (format.GetNext()) {
									if (ParseByFormat(str,  format, parseInfo, dtfi, result)) {
										return (true);
									}
								}
							}
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return false;
						}
						// Found a macth.
					}
				}
				else {
					if (format.MatchSpecifiedWord(GMTName)) {
						format.Index += (GMTName.length() - 1);
						// Found GMT string in format.  This means the DateTime string
						// is in GMT timezone.
						result.flags |= ParseFlags::TimeZoneUsed;
						result.timeZoneOffset = TimeSpan();
						if (!str.Match(GMTName)) {
							result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
							return false;
						}
					}
					else if (!str.Match(ch)) {
						// ch is expected.
						result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
						return false;
					}
				}
				break;
			} // switch
			return (true);
		}

		//
		// The pos should point to a quote character. This method will
		// get the string enclosed by the quote character.
		//
		static bool TryParseQuoteString(const std::wstring& format, int pos, std::wstring& result, int& returnValue)
		{
			//
			// NOTE : pos will be the index of the quote character in the 'format' string.
			//
			returnValue = 0;
			int formatLen = format.length();
			int beginPos = pos;
			wchar_t quoteChar = format[pos++]; // Get the character used to quote the following string.

			bool foundQuote = false;
			while (pos < formatLen) {
				wchar_t ch = format[pos++];
				if (ch == quoteChar) {
					foundQuote = true;
					break;
				}
				else if (ch == '\\') {
					// The following are used to support escaped character.
					// Escaped character is also supported in the quoted string.
					// Therefore, someone can use a format like "'minute:' mm\"" to display:
					//  minute: 45"
					// because the second double quote is escaped.
					if (pos < formatLen) {
						result.push_back(format[pos++]);
					}
					else {
						//
						// This means that '\' is at the end of the formatting string.
						//
						return false;
					}
				}
				else {
					result.push_back(ch);
				}
			}

			if (!foundQuote) {
				// Here we can't find the matching quote.
				return false;
			}

			//
			// Return the character count including the begin/end quote characters and enclosed string.
			//
			returnValue = (pos - beginPos);
			return true;
		}




		/*=================================DoStrictParse==================================
		**Action: Do DateTime parsing using the format in formatParam.
		**Returns: The parsed DateTime::
		**Arguments:
		**std::exceptions:
		**
		**Notes:
		**  When the following general formats are used, InvariantInfo is used in dtfi:
		**      'r', 'R', 's'.
		**  When the following general formats are used, the time is assumed to be in Universal time.
		**
		**Limitations:
		**  Only GregarianCalendar is supported for now.
		**  Only support GMT timezone.
		==============================================================================*/

		static bool DoStrictParse(const std::wstring& s, std::wstring formatParam, DateTimeStyles styles, CultureInfo& dtfi, DateTimeResult& result)
		{

			ParsingInfo parseInfo = ParsingInfo();
			parseInfo.Init();

			parseInfo.calendar = dtfi.Calendar;
			parseInfo.fAllowInnerWhite = ((styles & DateTimeStyles::AllowInnerWhite) != 0);
			parseInfo.fAllowTrailingWhite = ((styles & DateTimeStyles::AllowTrailingWhite) != 0);

			// We need the original values of the following two below.
			std::wstring originalFormat = formatParam;

			if (formatParam.length() == 1) {
				if (((result.flags & ParseFlags::CaptureOffset) != (ParseFlags)0) && formatParam[0] == 'U')
				{
					// The 'U' format is not allowed for DateTimeOffset
					result.SetFailure(ParseFailureKind::Format, "Format_BadFormatSpecifier");
					return false;
				}
				formatParam = ExpandPredefinedFormat(formatParam, dtfi, parseInfo, result);
			}

			bool bTimeOnly = false;
			result.calendar = parseInfo.calendar;

			if (parseInfo.calendar->GetID() == 8) {
				parseInfo.fCustomNumberParser = true;
			}

			// Reset these values to negative one so that we could throw std::exception
			// if we have parsed every item twice.
			result.Hour = result.Minute = result.Second = -1;

			__DTString format = __DTString(formatParam, CultureInfo_impl::GetImpl(dtfi), false);
			__DTString str = __DTString(s, CultureInfo_impl::GetImpl(dtfi), false);

			if (parseInfo.fAllowTrailingWhite) {
				// Trim trailing spaces if AllowTrailingWhite.
				format.TrimTail();
				format.RemoveTrailingInQuoteSpaces();
				str.TrimTail();
			}

			if ((styles & DateTimeStyles::AllowLeadingWhite) != 0) {
				format.SkipWhiteSpaces();
				format.RemoveLeadingInQuoteSpaces();
				str.SkipWhiteSpaces();
			}

			//
			// Scan every character in format and match the pattern in str.
			//
			while (format.GetNext()) {
				// We trim inner spaces here, so that we will not eat trailing spaces when
				// AllowTrailingWhite is not used.
				if (parseInfo.fAllowInnerWhite) {
					str.SkipWhiteSpaces();
				}
				if (!ParseByFormat(str, format, parseInfo, dtfi, result)) {
					return (false);
				}
			}

			if (str.Index < (int)str.Value.length() - 1) {
				// There are still remaining character in str.
				result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
				return false;
			}

			if (parseInfo.fUseTwoDigitYear && (((DateTimeFormatFlags)CultureInfo_impl::GetImpl(dtfi)->_data->formatFlags & DateTimeFormatFlags::UseHebrewRule) == DateTimeFormatFlags::None))
			{
				// A two digit year value is expected. Check if the parsed year value is valid.
				if (result.Year >= 100) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				try {
					result.Year = parseInfo.calendar->ToFourDigitYear(result.Year);
				}
				catch (...) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
			}

			if (parseInfo.fUseHour12) {
				if (parseInfo.timeMark == TM::NotSet) {
					// hh is used, but no AM/PM designator is specified.
					// Assume the time is AM.
					// Don't throw std::exceptions in here becasue it is very confusing for the caller.
					// I always got confused myself when I use "hh:mm:ss" to parse a time string,
					// and ParseExact() throws on me (because I didn't use the 24-hour clock 'HH').
					parseInfo.timeMark = TM::AM;
				}
				if (result.Hour > 12) {
					// AM/PM is used, but the value for HH is too big.
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
				if (parseInfo.timeMark == TM::AM) {
					if (result.Hour == 12) {
						result.Hour = 0;
					}
				}
				else {
					result.Hour = (result.Hour == 12) ? 12 : result.Hour + 12;
				}
			}
			else
			{
				// Military (24-hour time) mode
				//
				// AM cannot be set with a 24-hour time like 17:15.
				// PM cannot be set with a 24-hour time like 03:15.
				if ((parseInfo.timeMark == TM::AM && result.Hour >= 12)
					|| (parseInfo.timeMark == TM::PM && result.Hour <  12)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDateTime");
					return false;
				}
			}


			// Check if the parased string only contains hour/minute/second values.
			bTimeOnly = (result.Year == -1 && result.Month == -1 && result.Day == -1);
			if (!CheckDefaultDateTime(result, parseInfo.calendar, styles)) {
				return false;
			}

			if (!bTimeOnly && (((DateTimeFormatFlags)CultureInfo_impl::GetImpl(dtfi)->_data->formatFlags & DateTimeFormatFlags::UseHebrewRule) != DateTimeFormatFlags::None))
			{
				if (!CultureInfo_impl::YearMonthAdjustment(dtfi, result.Year, result.Month, ((result.flags & ParseFlags::ParsedMonthName) != (ParseFlags)0))) {
					result.SetFailure(ParseFailureKind::FormatBadDateTimeCalendar, "Format_BadDateTimeCalendar");
					return false;
				}
			}
			if (!parseInfo.calendar->TryToDateTime(result.Year, result.Month, result.Day,
				result.Hour, result.Minute, result.Second, 0, result.era, result.parsedDate)) {
				result.SetFailure(ParseFailureKind::FormatBadDateTimeCalendar, "Format_BadDateTimeCalendar");
				return false;
			}
			if (result.fraction > 0) {
				result.parsedDate = result.parsedDate.AddTicks((Int64)round(result.fraction * 10000000LL));
			}

			//
			// We have to check day of week before we adjust to the time zone.
			// It is because the value of day of week may change after adjusting
			// to the time zone.
			//
			if (parseInfo.dayOfWeek != -1) {
				//
				// Check if day of week is correct.
				//
				if (parseInfo.dayOfWeek != (int)parseInfo.calendar->GetDayOfWeek(result.parsedDate)) {
					result.SetFailure(ParseFailureKind::Format, "Format_BadDayOfWeek");
					return false;
				}
			}


			if (!DetermineTimeZoneAdjustments(result, styles, bTimeOnly)) {
				return false;
			}
			return true;
		}

	};

	const int DateTimeParse::ORDER_YMD = 0;     // The order of date is Year/Month/Day.
	const int DateTimeParse::ORDER_MDY = 1;     // The order of date is Month/Day/Year.
	const int DateTimeParse::ORDER_DMY = 2;     // The order of date is Day/Month/Year.
	const int DateTimeParse::ORDER_YDM = 3;     // The order of date is Year/Day/Month
	const int DateTimeParse::ORDER_YM = 4;     // Year/Month order.
	const int DateTimeParse::ORDER_MY = 5;     // Month/Year order.
	const int DateTimeParse::ORDER_MD = 6;     // Month/Day order.
	const int DateTimeParse::ORDER_DM = 7;     // Day/Month order.
	const std::wstring DateTimeParse::GMTName = L"GMT";
	const std::wstring DateTimeParse::ZuluName = L"Z";
	std::vector<DS> DateTimeParse::dateParsingStates[] = {
		// DS::BEGIN                                                                             // DS::BEGIN
		std::vector<DS> { DS::BEGIN, DS::Error, DS::TX_N, DS::N, DS::D_Nd, DS::T_Nt, DS::Error, DS::D_M, DS::D_M, DS::D_S, DS::T_S, DS::BEGIN, DS::D_Y, DS::D_Y, DS::Error, DS::BEGIN, DS::BEGIN, DS::Error},

		// DS::N                                                                                 // DS::N
		std::vector<DS> { DS::Error, DS::DX_NN, DS::Error, DS::NN, DS::D_NNd, DS::Error, DS::DX_NM, DS::D_NM, DS::D_MNd, DS::D_NDS, DS::Error, DS::N, DS::D_YN, DS::D_YNd, DS::DX_YN, DS::N, DS::N, DS::Error},

		// DS::NN                                                                                // DS::NN
		std::vector<DS> { DS::DX_NN, DS::DX_NNN, DS::TX_N, DS::DX_NNN, DS::Error, DS::T_Nt, DS::DX_MNN, DS::DX_MNN, DS::Error, DS::Error, DS::T_S, DS::NN, DS::DX_NNY, DS::Error, DS::DX_NNY, DS::NN, DS::NN, DS::Error},

		// DS::D_Nd                                                                              // DS::D_Nd
		std::vector<DS> { DS::Error, DS::DX_NN, DS::Error, DS::D_NN, DS::D_NNd, DS::Error, DS::DX_NM, DS::D_MN, DS::D_MNd, DS::Error, DS::Error, DS::D_Nd, DS::D_YN, DS::D_YNd, DS::DX_YN, DS::Error, DS::D_Nd, DS::Error},

		// DS::D_NN                                                                              // DS::D_NN
		std::vector<DS> { DS::DX_NN, DS::DX_NNN, DS::TX_N, DS::DX_NNN, DS::Error, DS::T_Nt, DS::DX_MNN, DS::DX_MNN, DS::Error, DS::DX_DS, DS::T_S, DS::D_NN, DS::DX_NNY, DS::Error, DS::DX_NNY, DS::Error, DS::D_NN, DS::Error},

		// DS::D_NNd                                                                             // DS::D_NNd
		std::vector<DS> { DS::Error, DS::DX_NNN, DS::DX_NNN, DS::DX_NNN, DS::Error, DS::Error, DS::DX_MNN, DS::DX_MNN, DS::Error, DS::DX_DS, DS::Error, DS::D_NNd, DS::DX_NNY, DS::Error, DS::DX_NNY, DS::Error, DS::D_NNd, DS::Error},

		// DS::D_M                                                                               // DS::D_M
		std::vector<DS> { DS::Error, DS::DX_MN, DS::Error, DS::D_MN, DS::D_MNd, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_M, DS::D_YM, DS::D_YMd, DS::DX_YM, DS::Error, DS::D_M, DS::Error},

		// DS::D_MN                                                                              // DS::D_MN
		std::vector<DS> { DS::DX_MN, DS::DX_MNN, DS::DX_MNN, DS::DX_MNN, DS::Error, DS::T_Nt, DS::Error, DS::Error, DS::Error, DS::DX_DS, DS::T_S, DS::D_MN, DS::DX_YMN, DS::Error, DS::DX_YMN, DS::Error, DS::D_MN, DS::Error},

		// DS::D_NM                                                                              // DS::D_NM
		std::vector<DS> { DS::DX_NM, DS::DX_MNN, DS::DX_MNN, DS::DX_MNN, DS::Error, DS::T_Nt, DS::Error, DS::Error, DS::Error, DS::DX_DS, DS::T_S, DS::D_NM, DS::DX_YMN, DS::Error, DS::DX_YMN, DS::Error, DS::D_NM, DS::Error},

		// DS::D_MNd                                                                             // DS::D_MNd
		std::vector<DS> { DS::Error, DS::DX_MNN, DS::Error, DS::DX_MNN, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_MNd, DS::DX_YMN, DS::Error, DS::DX_YMN, DS::Error, DS::D_MNd, DS::Error},

		// DS::D_NDS,                                                                            // DS::D_NDS,
		std::vector<DS> { DS::DX_NDS, DS::DX_NNDS, DS::DX_NNDS, DS::DX_NNDS, DS::Error, DS::T_Nt, DS::Error, DS::Error, DS::Error, DS::D_NDS, DS::T_S, DS::D_NDS, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_NDS, DS::Error},

		// DS::D_Y                                                                               // DS::D_Y
		std::vector<DS> { DS::Error, DS::DX_YN, DS::Error, DS::D_YN, DS::D_YNd, DS::Error, DS::DX_YM, DS::D_YM, DS::D_YMd, DS::D_YM, DS::Error, DS::D_Y, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_Y, DS::Error},

		// DS::D_YN                                                                              // DS::D_YN
		std::vector<DS> { DS::DX_YN, DS::DX_YNN, DS::DX_YNN, DS::DX_YNN, DS::Error, DS::Error, DS::DX_YMN, DS::DX_YMN, DS::Error, DS::Error, DS::Error, DS::D_YN, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_YN, DS::Error},

		// DS::D_YNd                                                                             // DS::D_YNd
		std::vector<DS> { DS::Error, DS::DX_YNN, DS::DX_YNN, DS::DX_YNN, DS::Error, DS::Error, DS::DX_YMN, DS::DX_YMN, DS::Error, DS::Error, DS::Error, DS::D_YN, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_YN, DS::Error},

		// DS::D_YM                                                                              // DS::D_YM
		std::vector<DS> { DS::DX_YM, DS::DX_YMN, DS::DX_YMN, DS::DX_YMN, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_YM, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_YM, DS::Error},

		// DS::D_YMd                                                                             // DS::D_YMd
		std::vector<DS> { DS::Error, DS::DX_YMN, DS::DX_YMN, DS::DX_YMN, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_YM, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_YM, DS::Error},

		// DS::D_S                                                                               // DS::D_S
		std::vector<DS> { DS::DX_DS, DS::DX_DSN, DS::TX_N, DS::T_Nt, DS::Error, DS::T_Nt, DS::Error, DS::Error, DS::Error, DS::D_S, DS::T_S, DS::D_S, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_S, DS::Error},

		// DS::T_S                                                                               // DS::T_S
		std::vector<DS> { DS::TX_TS, DS::TX_TS, DS::TX_TS, DS::T_Nt, DS::D_Nd, DS::Error, DS::Error, DS::Error, DS::Error, DS::D_S, DS::T_S, DS::T_S, DS::Error, DS::Error, DS::Error, DS::T_S, DS::T_S, DS::Error},

		// DS::T_Nt                                                                              // DS::T_Nt
		std::vector<DS> { DS::Error, DS::TX_NN, DS::TX_NN, DS::TX_NN, DS::Error, DS::T_NNt, DS::DX_NM, DS::D_NM, DS::Error, DS::Error, DS::T_S, DS::Error, DS::Error, DS::Error, DS::Error, DS::T_Nt, DS::T_Nt, DS::TX_NN},

		// DS::T_NNt                                                                             // DS::T_NNt
		std::vector<DS> { DS::Error, DS::TX_NNN, DS::TX_NNN, DS::TX_NNN, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::Error, DS::T_S, DS::T_NNt, DS::Error, DS::Error, DS::Error, DS::T_NNt, DS::T_NNt, DS::TX_NNN}

	};
}

static const  int DaysToMonth365[13] =
{
	0,
	31,
	59,
	90,
	120,
	151,
	181,
	212,
	243,
	273,
	304,
	334,
	365
};

static const int DaysToMonth366[13] =
{
	0,
	31,
	60,
	91,
	121,
	152,
	182,
	213,
	244,
	274,
	305,
	335,
	366
};

DateTime const DateTime::MinValue = DateTime(0LL, DateTimeKind::Unspecified);
DateTime const DateTime::MaxValue = DateTime(3155378975999999999LL, DateTimeKind::Unspecified);


std::wstring DateTime::CustomToString(const wchar_t* fmt, const CultureInfo& loc) const
{
	return DateTimeFormat::Format(*this, fmt==nullptr ? std::wstring() :std::wstring(fmt), loc, TimeSpan());
}

int DateTime::DaysInMonth(int year, int month)
{
	if (month < 1 || month > 12)
	{
		throw std::exception("ArgumentOutOfRange_Month");
	}
	const int* numArray = (IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365);
	return numArray[month] - numArray[month - 1];
}

int DateTime::GetDatePart(int part) const
{
	int num1 = (int)(dateData / TimeSpan::DAYS);
	int num2 = num1 / 146097;
	int num3 = num1 - num2 * 146097;
	int num4 = num3 / 36524;
	if (num4 == 4)
		num4 = 3;
	int num5 = num3 - num4 * 36524;
	int num6 = num5 / 1461;
	int num7 = num5 - num6 * 1461;
	int num8 = num7 / 365;
	if (num8 == 4)
		num8 = 3;
	if (part == 0)
		return num2 * 400 + num4 * 100 + num6 * 4 + num8 + 1;
	int num9 = num7 - num8 * 365;
	if (part == 1)
		return num9 + 1;
	const int* numArray = num8 == 3 && (num6 != 24 || num4 == 3) ? DaysToMonth366 : DaysToMonth365;
	int index = num9 >> 6;
	while (num9 >= numArray[index])
		++index;
	if (part == 2)
		return index;
	else
		return num9 - numArray[index - 1] + 1;
}

bool DateTime::IsLeapYear(int year)
{
	if (year < 1 || year > 9999)
		throw std::exception("ArgumentOutOfRange_Year", 0);
	if (year % 4 != 0)
		return false;
	if (year % 100 == 0)
		return year % 400 == 0;
	else
		return true;
}

Int64 DateTime::DateToTicks(int year, int month, int day) const
{
	if (year >= 1 && year <= 9999 && (month >= 1 && month <= 12))
	{
		const int* numArray = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
		if (day >= 1 && day <= numArray[month] - numArray[month - 1])
		{
			int num = year - 1;
			return (long long)(num * 365 + num / 4 - num / 100 + num / 400 + numArray[month - 1] + day - 1) * TimeSpan::DAYS;
		}
	}
	throw std::exception("ArgumentOutOfRange_BadYearMonthDay", 0);
}

Int64 DateTime::TimeToTicks(int hour, int minute, int second) const
{
	if (hour >= 0 && hour < 24 && (minute >= 0 && minute < 60) && (second >= 0 && second < 60))
		return Int64(hour)*TimeSpan::HOURS + Int64(minute) * TimeSpan::MINUTES + Int64(second) * TimeSpan::SECONDS;
	else
		throw std::exception("ArgumentOutOfRange_BadHourMinuteSecond", 0);
}

DateTime DateTime::Add(double value, Int64 scale)  const
{
	return AddTicks((Int64)(value * (double)scale + (value >= 0 ? 0.5 : -0.5)));
}

DateTime DateTime::AddDays(double value) const
{
	return Add(value, TimeSpan::DAYS);
}

DateTime DateTime::AddHours(double value) const
{
	return Add(value, TimeSpan::HOURS);
}

DateTime DateTime::AddMilliseconds(double value) const
{
	return Add(value, TimeSpan::MILLISECONDS);
}

DateTime DateTime::AddMinutes(double value) const
{
	return Add(value, TimeSpan::MINUTES);
}

DateTime DateTime::AddMonths(int months) const
{
	if (months < -120000 || months > 120000)
	{
		throw std::exception("ArgumentOutOfRange_DateTimeBadMonths");
	}
	int datePart = GetDatePart(0);
	int num = GetDatePart(2);
	int datePart1 = GetDatePart(3);
	int num1 = num - 1 + months;
	if (num1 < 0)
	{
		num = 12 + (num1 + 1) % 12;
		datePart = datePart + (num1 - 11) / 12;
	}
	else
	{
		num = num1 % 12 + 1;
		datePart = datePart + num1 / 12;
	}
	if (datePart < 1 || datePart > 9999)
	{
		throw std::exception("ArgumentOutOfRange_DateArithmetic");
	}
	int num2 = DaysInMonth(datePart, num);
	if (datePart1 > num2)
	{
		datePart1 = num2;
	}
	return DateTime(DateToTicks(datePart, num, datePart1) + dateData, _kind);
}

DateTime DateTime::AddSeconds(double value) const
{
	return Add(value, 1000);
}

DateTime DateTime::AddTicks(Int64 value) const
{
	return DateTime(value + dateData, _kind);
}

DateTime DateTime::AddYears(int value) const
{
	if (value < -10000 || value > 10000)
	{
		throw std::exception("ArgumentOutOfRange_DateTimeBadYears");
	}
	return AddMonths(value * 12);
}

DateTime DateTime::ToUniversalTime() const
{
	return TimeZoneInfo_impl::ConvertTimeToUtc(*this);
}

__forceinline DateTime DateTime::Now()
{
	API_SYSTEMTIME sys;
	Microsoft::winapi_GetLocalTime(sys);
	return DateTime(sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, DateTimeKind::Local);
}

__forceinline DateTime DateTime::UtcNow()
{
	API_SYSTEMTIME sys;
	Microsoft::winapi_GetSystemTime(sys); 
	return DateTime(sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, DateTimeKind::Utc);
}


System::DayOfWeek DateTime::GetDayOfWeek(const CultureInfo& loc) const
{
	return (System::DayOfWeek)((dateData / TimeSpan::DAYS + (loc.GetBooleanFlags(CultureInfoBooleanFlags::FirstDayofWeekIsMonday) ? 1LL : 0LL)) % 7LL);
}

__inline DateTime DateTime::GetDate() const
{
	return DateTime(dateData - dateData% TimeSpan::DAYS, _kind);
}



__inline int DateTime::GetDay() const
{
	return GetDatePart(3);
}

__inline int DateTime::GetDayOfYear() const
{
	return GetDatePart(1);
}
int DateTime::GetDayOfMonth() const
{
	return GetDatePart(3);
}

__inline int DateTime::GetHour() const
{
	return (int)(dateData / TimeSpan::HOURS % 24LL);
}

__inline DateTimeKind DateTime::GetKind() const
{
	return _kind;
}


__inline DateTime DateTime::SpecifyKind(const DateTime& value, DateTimeKind kind)
{
	return DateTime(value.dateData, kind);
}

__inline int  DateTime::GetMicrosecond() const
{
	return (int)(dateData % 1000LL);
}

__inline int DateTime::GetMillisecond() const
{
	return (int)(dateData / TimeSpan::MILLISECONDS % 1000LL);
}

__inline int DateTime::GetMinute() const
{
	return (int)(dateData / TimeSpan::MINUTES % 60LL);
}

__inline int DateTime::GetYear() const
{
	return GetDatePart(0);
}

__inline int DateTime::GetMonth() const
{
	return GetDatePart(2);
}

__inline int DateTime::GetSecond() const
{
	return (int)(dateData / TimeSpan::SECONDS % 60L);
}

__inline long long DateTime::GetTicks() const
{
	return dateData;
}

__inline void DateTime::SetTicks(long long ticks)
{
	dateData = ticks;
}

__inline TimeSpan DateTime::GetTimeOfDay() const
{
	return TimeSpan(dateData % TimeSpan::DAYS);
}


DateTime::DateTime(long long ticks)
{
	dateData = ticks;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
}

DateTime::DateTime()
{
	dateData = 0ULL;
	_iadst = false;
}

DateTime::DateTime(long long ticks, bool isAmbiguousDst)
{
	dateData = ticks;
	_kind = DateTimeKind::Local;
	_iadst = isAmbiguousDst;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
}

bool DateTime::GetIsAmbiguousDaylightSavingTime()const
{
	return  _iadst;
}

DateTime::DateTime(long long ticks, DateTimeKind kind)
{
	dateData = ticks;
	_kind = kind; 
	_iadst = false;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
}


DateTime::DateTime(int year, int month, int day)
{
	dateData = DateToTicks(year, month, day);
	_iadst = false;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
{
	dateData = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
	_iadst = false;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
{
	dateData = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
	_iadst = false;
	_kind = kind;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
}


DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
{
	_iadst = false;
	if (millisecond < 0 || millisecond >= 1000)
	{
		throw std::exception("ArgumentOutOfRange_Range", 7);
	}
	else
	{
		dateData = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + (long long)millisecond * TimeSpan::MILLISECONDS;
		if (dateData < 0LL || dateData > 3155378975999999999LL)
		{
			throw std::exception("OutOfDateTimeRange");
		}
	}
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
{
	_iadst = false;
	if (millisecond < 0 || millisecond >= 1000)
	{
		throw std::exception("ArgumentOutOfRange_Range", 7);
	}
	else
	{
		dateData = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + (long long)millisecond * TimeSpan::MILLISECONDS;
		_kind = kind;
		if (dateData < 0LL || dateData > 3155378975999999999LL)
		{
			throw std::exception("OutOfDateTimeRange");
		}
	}
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
{
	_iadst = false;
	if (millisecond < 0 || millisecond >= 1000)
	{
		throw std::exception("ArgumentOutOfRange_Range", 7);
	}
	if (microsecond < 0 || microsecond >= 1000)
	{
		throw std::exception("ArgumentOutOfRange_Range", 8);
	}

	dateData = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + (long long)millisecond * TimeSpan::MILLISECONDS + microsecond;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond, DateTimeKind kind)
{
	_iadst = false;
	if (millisecond < 0 || millisecond >= 1000)
	{
		throw std::exception("ArgumentOutOfRange_Range", 7);
	}	
	if (microsecond < 0 || microsecond >= 1000)
	{
		throw std::exception("ArgumentOutOfRange_Range", 8);
	}

	dateData = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + (long long)millisecond * TimeSpan::MILLISECONDS + microsecond;
	_kind = kind;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
}

bool DateTime::TryCreate(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond, DateTimeKind kind, DateTime& time)
{
	if (year >= 1 && year <= 9999 && (month >= 1 && month <= 12))
	{
		const int* numArray = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
		if (day >= 1 && day <= numArray[month] - numArray[month - 1])
		{
			int num = year - 1;
			Int64 dateData = (num * 365 + num / 4 - num / 100 + num / 400 + numArray[month - 1] + day - 1) * TimeSpan::DAYS;
			if (hour >= 0 && hour < 24 && (minute >= 0 && minute < 60) && (second >= 0 && second < 60))
			{
				dateData += Int64(hour)*TimeSpan::HOURS + Int64(minute) * TimeSpan::MINUTES + Int64(second) * TimeSpan::SECONDS;
				if (millisecond >= 0 && millisecond < 1000 && microsecond >= 0 && microsecond < 1000)
				{
					dateData += (long long)millisecond * TimeSpan::MILLISECONDS + microsecond;
					if (dateData >= 0LL && dateData <= 3155378975999999999LL)
					{					
						time=DateTime(dateData, kind);
						return true;
					}
				}
			}
		}
	}
	return false;
}


DateTime::DateTime(const DateTime& dateTime)
{
	dateData = dateTime.dateData;
	_kind = dateTime._kind;
	_iadst = dateTime._iadst;
}

DateTime & DateTime::operator=(const DateTime & dateTime)
{
	dateData = dateTime.dateData;
	_kind = dateTime._kind;
	_iadst = dateTime._iadst;
	return *this;
}

DateTime DateTime::operator + (const DateTime& dateTime) const
{
	return AddTicks(dateTime.Ticks);
}

DateTime DateTime::operator - (const DateTime& dateTime) const
{
	return AddTicks(-dateTime.Ticks);
}

__uref DateTime DateTime::operator + (const TimeSpan& time) const
{
	return AddTicks(time.Ticks);
}

__uref DateTime DateTime::operator - (const TimeSpan& time) const
{
	return AddTicks(-time.Ticks);
}


DateTime& DateTime::operator += (DateTime& dateTime)
{
	dateData += dateTime.dateData;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
	return *this;
}

DateTime& DateTime::operator -= (DateTime& dateTime)
{
	dateData -= dateTime.dateData;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
	return *this;
}

DateTime& DateTime::operator += (TimeSpan& timeSpan)
{
	dateData += timeSpan.span;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
	return *this;
}

DateTime& DateTime::operator -= (TimeSpan& timeSpan)
{
	dateData -= timeSpan.span;
	if (dateData < 0LL || dateData > 3155378975999999999LL)
	{
		throw std::exception("OutOfDateTimeRange");
	}
	return *this;
}

bool DateTime::operator == (DateTime&  dateTime) const
{
	return dateTime.dateData == dateData;
}

bool DateTime::operator == (const DateTime&  dateTime) const
{
	return  dateTime.dateData == dateData;
}

bool DateTime::operator != (DateTime&  dateTime) const
{
	return  dateTime.dateData != dateData;
}

bool DateTime::operator != (const DateTime&  dateTime) const
{
	return  dateTime.dateData != dateData;
}

bool DateTime::operator > (const DateTime&  dateTime) const
{
	return  dateData > dateTime.dateData;
}

bool DateTime::operator < (const DateTime&  dateTime) const
{
	return  dateData < dateTime.dateData;
}

bool DateTime::operator >=(const DateTime&  dateTime) const
{
	return  dateData >= dateTime.dateData;
}

bool DateTime::operator <= (const DateTime&  dateTime) const
{
	return  dateData <=dateTime.dateData ;
}

bool DateTime::TryParse(const std::wstring& input, const CultureInfo& loc, DateTime& result)
{
	return DateTimeParse::TryParse(input, loc, DateTimeStyles::None, result);
}

bool DateTime::TryParseExact(const std::wstring& input, const std::wstring& format, const CultureInfo& loc, DateTimeStyles style, DateTime& result)
{
	CultureInfo nloc = loc;
	return DateTimeParse::TryParseExact(input, format, nloc, style, result);
}

bool DateTime::TryParseExactMultiple(const std::wstring& input, const std::vector<std::wstring>& formats, const CultureInfo& loc, DateTimeStyles style, DateTime& result)
{
	CultureInfo nloc = loc;
	return DateTimeParse::TryParseExactMultiple(input, formats, nloc, style, result);
}

std::weak_ptr<DateTimeConverter> DateTimeConverter::_thisWeakPtr;
std::mutex& DateTimeConverter::getlocker()
{
	static std::mutex locker;
	return locker;
}

DateTimeConverter::DateTimeConverter() {}
DateTimeConverter::~DateTimeConverter() {}

std::shared_ptr<DateTimeConverter> DateTimeConverter::GetSharedInstance()
{
	std::shared_ptr<DateTimeConverter>  p = _thisWeakPtr.lock();
	if (p) return p;
	std::lock_guard<std::mutex> lock(getlocker());
	p = _thisWeakPtr.lock();
	if (p) return p;
	p.reset(new DateTimeConverter());
	_thisWeakPtr = p;
	return p;
}

DateTimeConverter& DateTimeConverter::GetInstance()
{
	auto sptr = GetSharedInstance();
	return *sptr.get();
}

bool DateTimeConverter::CanConvertFrom(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(DateTime))) return true;
	return type->DirectGetIsRawOrSTDString();
}

bool DateTimeConverter::CanConvertTo(_type type) const
{
	if (!type) return false;
	if (type->IsType(_typeof(DateTime))) return true;
	return type->DirectGetIsRawOrSTDString();
}

Any DateTimeConverter::ConvertFrom(const Any& obj, const CultureInfo& loc) const
{
	_type type = obj.Type;
	if (!type) return _missing;
	try
	{
		if (type->IsType(_typeof(DateTime)))
		{
			if (type->IsReference)
			{
				return DateTime(AnyVisitor<DateTime&>::Get(obj, ModifierPolicy::IgnoreAll));
			}
			DateTime* p = obj;
			if (p)
				return p->ToString();
			else
			{
				DateTime res = obj.as<DateTime>();
				return res;
			}
		}
		else
		{
			std::wstring tmp = std::wstring();
			if (obj.TryGetString(tmp, loc))
			{
				DateTime result = DateTime();
				if (DateTime::TryParse(std::wstring(tmp), loc, result))
				{
					return result;
				}
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any DateTimeConverter::ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const
{
	if (!type)return _missing;
	_type otype = obj.Type;
	if (!otype) return _missing;
	try
	{
		bool iswiden = false;
		if (type->GetIsSTDString(iswiden))
		{
			if (iswiden)
			{
				if (otype->IsReference)
				{
					return AnyVisitor<DateTime&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
				}
				DateTime* p = obj;
				if (p)
					return p->ToString();
				else
				{
					DateTime v = obj;
					return v.ToString();
				}
			}
		}
		else if (otype->IsType(_typeof(DateTime)))
		{
			if (otype->IsReference)
			{
				return AnyVisitor<DateTime&>::Get(obj, ModifierPolicy::IgnoreAll);
			}
			DateTime* p = obj;
			if (p)
				return *p;
			else
			{
				DateTime v = obj;
				return v;
			}
		}
	}
	catch (...)
	{

	}
	return _missing;
}

Any DateTimeConverter::ConvertFromString(const std::wstring& str, const CultureInfo& loc) const
{
	DateTime result = DateTime();
	if (DateTime::TryParse(str, loc, result))
		return result;
	else
		return _missing;
}

std::wstring DateTimeConverter::ConvertToString(const Any& obj, const CultureInfo& loc) const
{
	try
	{
		auto ty = obj.Type;
		if (ty->IsType(_typeof(DateTime)))
		{
			if (ty->IsReference)
			{
				return AnyVisitor<DateTime&>::Get(obj, ModifierPolicy::IgnoreAll).ToString(loc);
			}
			DateTime* p = obj;
			if (p)
				return p->ToString();
			else
			{
				DateTime v = obj;
				return v.ToString();
			}
		}
		return std::wstring();
	}
	catch (...)
	{
		return std::wstring();
	}
}

__uregister_enum(DateTimeKind)
{
	type.AddValue(L"Unspecified", DateTimeKind::Unspecified, __uhashstr(System::DateTimeKind::Unspecified));
	type.AddValue(L"Utc", DateTimeKind::Utc, __uhashstr(System::DateTimeKind::Utc));
	type.AddValue(L"Local", DateTimeKind::Local, __uhashstr(System::DateTimeKind::Local));

	BuiltInAssemblyFactory::Regisiter(&type);
}

__uregister_enum(DayOfWeek)
{
	type.AddValue(L"Sunday", DayOfWeek::Sunday, __uhashstr(System::DayOfWeek::Sunday));
	type.AddValue(L"Monday", DayOfWeek::Monday, __uhashstr(System::DayOfWeek::Monday));
	type.AddValue(L"Tuesday", DayOfWeek::Tuesday, __uhashstr(System::DayOfWeek::Tuesday));
	type.AddValue(L"Wednesday", DayOfWeek::Wednesday, __uhashstr(System::DayOfWeek::Wednesday));
	type.AddValue(L"Thursday", DayOfWeek::Thursday, __uhashstr(System::DayOfWeek::Thursday));
	type.AddValue(L"Friday", DayOfWeek::Friday, __uhashstr(System::DayOfWeek::Friday));
	type.AddValue(L"Saturday", DayOfWeek::Saturday, __uhashstr(System::DayOfWeek::Saturday));

	BuiltInAssemblyFactory::Regisiter(&type);
}

__uregister_enum(DateTimeStyles)
{
	type.AddValue(L"None", DateTimeStyles::None, __uhashstr(System::DateTimeStyles::None));
	type.AddValue(L"AllowLeadingWhite", DateTimeStyles::AllowLeadingWhite, __uhashstr(System::DateTimeStyles::AllowLeadingWhite));
	type.AddValue(L"AllowTrailingWhite", DateTimeStyles::AllowTrailingWhite, __uhashstr(System::DateTimeStyles::AllowTrailingWhite));
	type.AddValue(L"AllowInnerWhite", DateTimeStyles::AllowInnerWhite, __uhashstr(System::DateTimeStyles::AllowInnerWhite));
	type.AddValue(L"AllowWhiteSpaces", DateTimeStyles::AllowWhiteSpaces, __uhashstr(System::DateTimeStyles::AllowWhiteSpaces));
	type.AddValue(L"NoCurrentDateDefault", DateTimeStyles::NoCurrentDateDefault, __uhashstr(System::DateTimeStyles::NoCurrentDateDefault));
	type.AddValue(L"AdjustToUniversal", DateTimeStyles::AdjustToUniversal, __uhashstr(System::DateTimeStyles::AdjustToUniversal));
	type.AddValue(L"AssumeLocal", DateTimeStyles::AssumeLocal, __uhashstr(System::DateTimeStyles::AssumeLocal));
	type.AddValue(L"AssumeUniversal", DateTimeStyles::AssumeUniversal, __uhashstr(System::DateTimeStyles::AssumeUniversal));
	type.AddValue(L"RoundtripKind", DateTimeStyles::RoundtripKind, __uhashstr(System::DateTimeStyles::RoundtripKind));


	BuiltInAssemblyFactory::Regisiter(&type);
}

__inline static Object* Factory_DateTime_0() { return new DateTime(); }
__inline static Object* Factory_DateTime_1(long long ticks) { return new DateTime(ticks); }
__inline static Object* Factory_DateTime_2(long long ticks, bool isAmbiguousDst) { return new DateTime(ticks, isAmbiguousDst); }
__inline static Object* Factory_DateTime_3(long long ticks, DateTimeKind kind) { return new DateTime(ticks, kind); }
__inline static Object* Factory_DateTime_4(int year, int month, int day) { return new DateTime(year, month, day); }
__inline static Object* Factory_DateTime_5(int year, int month, int day, int hour, int minute, int second) { return new DateTime(year, month, day, hour, minute, second); }
__inline static Object* Factory_DateTime_6(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind) { return new DateTime(year, month, day, hour, minute, second, kind); }
__inline static Object* Factory_DateTime_7(int year, int month, int day, int hour, int minute, int second, int millisecond) { return new DateTime(year, month, day, hour, minute, second, millisecond); }
__inline static Object* Factory_DateTime_8(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind) { return new DateTime(year, month, day, hour, minute, second, millisecond, kind); }
__inline static Object* Factory_DateTime_9(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond) { return new DateTime(year, month, day, hour, minute, second, millisecond, microsecond); }
__inline static Object* Factory_DateTime_10(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond, DateTimeKind kind) { return new DateTime(year, month, day, hour, minute, second, millisecond, microsecond, kind); }

__uregister_class(DateTime, ClassFlags::Normal)
{
	type.AddDefaultConstructor<DateTime>(L"DateTime", ConstructorFlags::Default, &Factory_DateTime_0);
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<1, DateTime>::Create(&Factory_DateTime_1));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<2, DateTime>::Create(&Factory_DateTime_2));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<2, DateTime>::Create(&Factory_DateTime_3));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<3, DateTime>::Create(&Factory_DateTime_4));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<6, DateTime>::Create(&Factory_DateTime_5));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<7, DateTime>::Create(&Factory_DateTime_6));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<7, DateTime>::Create(&Factory_DateTime_7));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<8, DateTime>::Create(&Factory_DateTime_8));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<8, DateTime>::Create(&Factory_DateTime_9));
	type.AddConstructor(L"DateTime", ConstructorFlags::Default, ConstructorTypeAdapter<9, DateTime>::Create(&Factory_DateTime_10));


	type.AddProperty(L"Now", PropertyFlags::Static | PropertyFlags::Public, PropertyTypeOf<DateTime>(&DateTime::Now));
	type.AddProperty(L"UtcNow", PropertyFlags::Static | PropertyFlags::Public, PropertyTypeOf<DateTime>(&DateTime::UtcNow));

	type.AddMethod(L"GetDayOfWeek", MethodFlags::Default, MethodTypeAdapter<1, DateTime, DayOfWeek>::Create(&DateTime::GetDayOfWeek));
	type.AddMethod(L"DaysInMonth", MethodFlags::Default, MethodTypeAdapter<2, DateTime, int>::Create(&DateTime::DaysInMonth));

	type.AddMethod(L"SpecifyKind", MethodFlags::Public | MethodFlags::Static, MethodTypeAdapter<2, DateTime, DateTime>::Create(&DateTime::SpecifyKind));
	
	type.AddProperty(L"IsAmbiguousDaylightSavingTime", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetIsAmbiguousDaylightSavingTime));
	type.AddProperty(L"Kind", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetKind));
	type.AddProperty(L"Ticks", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetTicks, & DateTime::SetTicks));
	type.AddProperty(L"Date", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetDate));
	type.AddProperty(L"Year", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetYear));
	type.AddProperty(L"Month", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetMonth));
	type.AddProperty(L"Day", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetDay));
	type.AddProperty(L"Hour", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetHour));
	type.AddProperty(L"Minute", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetMinute));
	type.AddProperty(L"Second", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetSecond));
	type.AddProperty(L"Millisecond", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetMillisecond));
	type.AddProperty(L"Microsecond", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetMicrosecond));
	type.AddProperty(L"DayOfMonth", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetDayOfMonth));
	type.AddProperty(L"DayOfYear", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetDayOfYear));
	type.AddProperty(L"TimeOfDay", PropertyFlags::Default, PropertyTypeOf<DateTime>(&DateTime::GetTimeOfDay));

	type.AddMethod(L"TryCreate", MethodFlags::Static | MethodFlags::Public, MethodTypeAdapter<10, DateTime, bool>::Create(&DateTime::TryCreate));
	type.AddMethod(L"TryParse", MethodFlags::Static | MethodFlags::Public, MethodTypeAdapter<3, DateTime, bool>::Create(&DateTime::TryParse));
	type.AddMethod(L"TryParseExact", MethodFlags::Static | MethodFlags::Public, MethodTypeAdapter<5, DateTime, bool>::Create(&DateTime::TryParseExact));
	type.AddMethod(L"TryParseExactMultiple", MethodFlags::Static | MethodFlags::Public, MethodTypeAdapter<5, DateTime, bool>::Create(&DateTime::TryParseExactMultiple));

	type.AddAttribute(new TypeConverterAttribute(L"DateTimeConverter", DateTimeConverter::GetSharedInstance()));


	BuiltInAssemblyFactory::Regisiter(&type);
}