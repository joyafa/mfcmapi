#pragma once
#include <Interpret/SmartView/SmartViewParser.h>

namespace smartview
{
	// [MS-OXOCAL].pdf
	// PatternTypeSpecific
	// =====================
	//   This structure specifies the details of the recurrence type
	//
	union PatternTypeSpecific {
		DWORD WeekRecurrencePattern;
		DWORD MonthRecurrencePattern;
		struct
		{
			DWORD DayOfWeek;
			DWORD N;
		} MonthNthRecurrencePattern;
	};

	class RecurrencePattern : public SmartViewParser
	{
	public:
		RecurrencePattern();

		DWORD m_ModifiedInstanceCount;

	private:
		void Parse() override;
		_Check_return_ std::wstring ToStringInternal() override;

		WORD m_ReaderVersion;
		WORD m_WriterVersion;
		WORD m_RecurFrequency;
		WORD m_PatternType;
		WORD m_CalendarType;
		DWORD m_FirstDateTime;
		DWORD m_Period;
		DWORD m_SlidingFlag;
		PatternTypeSpecific m_PatternTypeSpecific;
		DWORD m_EndType;
		DWORD m_OccurrenceCount;
		DWORD m_FirstDOW;
		DWORD m_DeletedInstanceCount;
		std::vector<DWORD> m_DeletedInstanceDates;
		std::vector<DWORD> m_ModifiedInstanceDates;
		DWORD m_StartDate;
		DWORD m_EndDate;
	};
}