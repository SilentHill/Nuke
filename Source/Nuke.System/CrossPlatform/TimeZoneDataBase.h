
#pragma once
static std::string tzdbJson = R"TZDB(
[
  {
    "StandardName": "Eastern Standard Time",
    "Id": "America/New_York",
    "HasIanaId": true,
    "DisplayName": "(UTC-05:00) Eastern Time (US \u0026 Canada)",
    "DaylightName": "Eastern Daylight Time",
    "BaseUtcOffset": "-05:00:00",
    "SupportsDaylightSavingTime": true,
    "AdjustmentRules": [
      {
        "BaseUtcOffsetDelta": "00:00:00",
        "DateEnd": "2006-12-31T00:00:00",
        "DateStart": "0001-01-01T00:00:00",
        "DaylightDelta": "01:00:00",
        "DaylightTransitionEnd": {
          "Day": 1,
          "DayOfWeek": 0,
          "IsFixedDateRule": false,
          "Month": 10,
          "TimeOfDay": "0001-01-01T02:00:00",
          "Week": 5
        },
        "DaylightTransitionStart": {
          "Day": 1,
          "DayOfWeek": 0,
          "IsFixedDateRule": false,
          "Month": 4,
          "TimeOfDay": "0001-01-01T02:00:00",
          "Week": 1
        }
      },
      {
        "BaseUtcOffsetDelta": "00:00:00",
        "DateEnd": "9999-12-31T00:00:00",
        "DateStart": "2007-01-01T00:00:00",
        "DaylightDelta": "01:00:00",
        "DaylightTransitionEnd": {
          "Day": 1,
          "DayOfWeek": 0,
          "IsFixedDateRule": false,
          "Month": 11,
          "TimeOfDay": "0001-01-01T02:00:00",
          "Week": 1
        },
        "DaylightTransitionStart": {
          "Day": 1,
          "DayOfWeek": 0,
          "IsFixedDateRule": false,
          "Month": 3,
          "TimeOfDay": "0001-01-01T02:00:00",
          "Week": 2
        }
      }
    ]
  },
  {
    "StandardName": "China Standard Time",
    "Id": "Asia/Shanghai",
    "HasIanaId": true,
    "DisplayName": "(UTC\u002B08:00) Beijing, Chongqing, Hong Kong, Urumqi",
    "DaylightName": "China Daylight Time",
    "BaseUtcOffset": "08:00:00",
    "SupportsDaylightSavingTime": false,
    "AdjustmentRules": []
  }
]
)TZDB";